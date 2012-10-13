#include <bets42/deepthgt/SOAP.hpp>
#include <glog/logging.h>
#include <cstring>
#include <iterator>

using namespace bets42::deepthgt;

// Request
detail::Request::Request(const char* const a)
{
    std::strcpy(std::begin(action), "SOAPAction:");
    std::strncat(std::begin(action) + 11, a, action.size() - 11);
}

std::ostream& bets42::deepthgt::detail::operator<<(std::ostream& stream, const Request& request)
{
    stream << std::begin(request.data);
    return stream;
}


// SOAPClient
SOAPClient::SOAPClient(const char* const url, const char* const responseTypeXPath)
    : http_(url, std::bind(&SOAPClient::onResponse, this, std::placeholders::_1, std::placeholders::_2))
    , responseTypeQuery_(responseTypeXPath) {}

bool SOAPClient::post(const Request& request)
{
    return http_.post(std::begin(request.action), std::begin(request.data), request.size);
}

bool SOAPClient::registerCallback(const char* const responseType, ResponseCallback callback)
{
    const auto result(registry_.emplace(responseType, callback));

    if(result.second)
	{
		LOG(INFO) << "Registered callback for '" << responseType;
	}
	else
	{
		LOG(WARNING) << "Can't register callback for '" << responseType;
	}

    return result.second;
}

void SOAPClient::onResponse(const char* response, const std::size_t size)
{
    // build xml
    pugi::xml_document xml;
    const pugi::xml_parse_result result(xml.load_buffer(response, size));

    if(! result)
    {
        LOG(ERROR) << "Failed to create SOAP response, error=" << result.description();
    }

    // get msgtype
    const std::string responseType(responseTypeQuery_.evaluate_string(xml));

    // dispatch to registered callback
    const auto iter(registry_.find(responseType));
    if(iter != std::end(registry_))
    {
        LOG(INFO) << "Dispatching to callback for " << responseType;
        iter->second(xml);
    }
    else
    {
        LOG(WARNING) << "Ignoring SOAP response '" << responseType << "': " << response;
    }
}

