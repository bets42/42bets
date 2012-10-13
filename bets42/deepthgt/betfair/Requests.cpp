#include <bets42/deepthgt/betfair/Requests.hpp>
#include <bets42/arthur/memory.hpp>
#include <bets42/deepthgt/Exception.hpp>
#include <pugixml/pugixml.hpp>
#include <functional>
#include <iterator>
#include <memory>
#include <string>

//TODO:
// 1. Check Request max size when copying XML into it

namespace
{
    // generic request sections
    const std::string GLOBAL_SOAP_HEADER =
        "<?xml version='1.0' encoding='UTF-8'?>"
        "<SOAP-ENV:Envelope xmlns:SOAP-ENV='http://schemas.xmlsoap.org/soap/envelope/' "
            "xmlns:SOAP-ENC='http://schemas.xmlsoap.org/soap/encoding/' "
            "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
            "xmlns:xsd='http://www.w3.org/2001/XMLSchema' "
            "xmlns:ns2='http://www.betfair.com/publicapi/types/global/v3/' "
            "xmlns:ns1='http://www.betfair.com/publicapi/v3/BFGlobalService/'>"
        "<SOAP-ENV:Body>";

    const std::string SOAP_FOOTER =
        "</SOAP-ENV:Body>"
        "</SOAP-ENV:Envelope>";

    const std::string REQUEST_HEADER =
        "<header>"
            "<clientStamp>0</clientStamp>"
            "<sessionToken>SESSION_ID</sessionToken>"
        "</header>";

    // build request XML document
	std::unique_ptr<pugi::xml_document> createRequest(const std::string& xml)
	{
		auto req(bets42::arthur::make_unique<pugi::xml_document>());

		const pugi::xml_parse_result result(req->load_buffer(xml.data(), xml.size()));

		if(! result)
		{
			throw bets42::deepthgt::Exception(__FILE__, __LINE__)
			        << "Failed to create request, error=" << result.description();
		}

		return req;
	}

    // write XML to HTTP request
    struct XMLWriter : public pugi::xml_writer
    {
        explicit XMLWriter(const char* const action)
            : request(action) {}

        void write(const void* data, const std::size_t size)
        {
            request.size = size;
            std::memcpy(std::begin(request.data), data, size);
        }

        bets42::deepthgt::SOAPClient::Request request;
    };
}

bets42::deepthgt::SOAPClient::Request bets42::deepthgt::betfair::createLoginRequest(
    const std::string& username,
    const std::string& password,
    const unsigned productID)
{
    static const std::string XML =
        GLOBAL_SOAP_HEADER
        + "<ns1:login>"
        + "<ns1:request>"
        + "<ipAddress></ipAddress>"
        + "<locationId>0</locationId>"
        + "<password>PASSWORD</password>"
        + "<productId>PRODUCT_ID</productId>"
        + "<username>USERNAME</username>"
        + "<vendorSoftwareId>0</vendorSoftwareId>"
        + "</ns1:request>"
        + "</ns1:login>"
        + SOAP_FOOTER;

	static auto xml = createRequest(XML);
	static auto fields = std::make_tuple
	                     (
	                         xml->select_single_node("//username").node(),
	                         xml->select_single_node("//password").node(),
	                         xml->select_single_node("//productId").node()
	                     );

	std::get<0>(fields).text().set(username.c_str());
	std::get<1>(fields).text().set(password.c_str());
	std::get<2>(fields).text().set(productID);

    static const char* const action("login");

    XMLWriter writer(action);
    xml->save(writer);

    return std::move(writer.request);
}

bets42::deepthgt::SOAPClient::Request bets42::deepthgt::betfair::createLogoutRequest(
    const std::string& sessionID)
{
    static const std::string XML =
        GLOBAL_SOAP_HEADER
        + "<ns1:logout>"
        + "<ns1:request>"
        + REQUEST_HEADER
        + "</ns1:request>"
        + "</ns1:logout>"
        + SOAP_FOOTER;

	static auto request = createRequest(XML);
	static auto fields = std::make_tuple
	                     (
	                         request->select_single_node("//sessionToken").node()
	                     );

	std::get<0>(fields).text().set(sessionID.c_str());

    static const char* const action("logout");

    XMLWriter writer(action);
	request->save(writer);

    return std::move(writer.request);
}

bets42::deepthgt::SOAPClient::Request bets42::deepthgt::betfair::createKeepAliveRequest(
    const std::string& sessionID)
{
    static const std::string XML =
        GLOBAL_SOAP_HEADER
        + "<ns1:keepAlive>"
        + "<ns1:request>"
        + REQUEST_HEADER
        + "</ns1:request>"
        + "</ns1:keepAlive>"
        + SOAP_FOOTER;

	static auto request = createRequest(XML);
	static auto fields = std::make_tuple
	                     (
	                         request->select_single_node("//sessionToken").node()
	                     );

	std::get<0>(fields).text().set(sessionID.c_str());

    static const char* const action("keepAlive");

    XMLWriter writer(action);
	request->save(writer);

    return std::move(writer.request);
}

bets42::deepthgt::SOAPClient::Request bets42::deepthgt::betfair::createGetActiveEventTypesRequest(
    const std::string& sessionID)
{
    static const std::string XML =
        GLOBAL_SOAP_HEADER
        + "<ns1:getActiveEventTypes>"
        + "<ns1:request>"
        + REQUEST_HEADER
        + "</ns1:request>"
        + "</ns1:getActiveEventTypes>"
        + SOAP_FOOTER;

	static auto request = createRequest(XML);
	static auto fields = std::make_tuple
	                     (
	                         request->select_single_node("//sessionToken").node()
	                     );

	std::get<0>(fields).text().set(sessionID.c_str());

    static const char* const action("getActiveEventTypes");

    XMLWriter writer(action);
	request->save(writer);

    return std::move(writer.request);
}

