#include <bets42/deepthgt/betfair/BetfairExchange.hpp>
#include <bets42/deepthgt/betfair/Requests.hpp>
#include <boost/program_options.hpp>
#include <glog/logging.h>
#include <pugixml/pugixml.hpp>
#include <cstring>
#include <functional>
#include <string>

using namespace bets42::deepthgt::betfair;

namespace
{
    const char* const GLOBAL_URL = "https://api.betfair.com/global/v3/BFGlobalService";
    const char* const UK_URL = "https://api.betfair.com/exchange/v5/BFExchangeService";
}

BetfairExchange::BetfairExchange(deepthgt::CommandHandler::Registrar& cmdRegistrar)
	: Exchange(marvin::exch_id::betfair)
    , client_(GLOBAL_URL, std::bind(&BetfairExchange::onResponse, this, std::placeholders::_1, std::placeholders::_2))
    , loginStatus_(Exchange::LoginStatus::LOGGED_OUT)
	, stopped_(false)
{
    // register commands
	{
		boost::program_options::options_description options("[login]\nLogin to the exchange");
		options.add_options()("username", boost::program_options::value<std::string>(), "Account username");
		options.add_options()("password", boost::program_options::value<std::string>(), "Account password");
		options.add_options()("product_id", boost::program_options::value<unsigned>()->default_value(82),
		                      "Product ID for the specific API you wish to use, defaults to 82 (free API)");
		cmdRegistrar.registerCommand(registrant_key(), "login", options, std::bind(&BetfairExchange::onLogin, this, std::placeholders::_1));
	}
	{
		boost::program_options::options_description options("[logout]\nLogout from the exchange");
		cmdRegistrar.registerCommand(registrant_key(), "logout", options, std::bind(&BetfairExchange::onLogout, this, std::placeholders::_1));
	}

    // register response callbacks
    responseCallbacks_.emplace("n2:LoginResp", std::bind(&BetfairExchange::onResponseLogin, this, std::placeholders::_1));
    responseCallbacks_.emplace("n2:LogoutResp", std::bind(&BetfairExchange::onResponseLogout, this, std::placeholders::_1));
}

BetfairExchange::~BetfairExchange() {}

const std::string& BetfairExchange::registrant_key()
{
	static const std::string key("betfair");
	return key;
}

void BetfairExchange::run()
{
	LOG(INFO) << "Running '" << name() << "' exchange";

	while(! stopped_)
	{
		sleep(1);
	}

	LOG(INFO) << "Stopped '" << name() << "' exchange";
}

void BetfairExchange::stop()
{
	LOG(INFO) << "Proceeding to stop '" << name() << "'exchange";
	stopped_ = true;
}

std::string BetfairExchange::onLogin(const CommandHandler::Command& command)
{
	std::stringstream response;

    if(loginStatus_ == Exchange::LoginStatus::LOGGED_OUT)
	{
        loginStatus_ = Exchange::LoginStatus::LOGGING_IN;

		if(command.args().count("username")
		    && command.args().count("password")
		    && command.args().count("product_id"))
		{
            const Request request(
                createLoginRequest(
                    command.args()["username"].as<std::string>(),
                    command.args()["password"].as<std::string>(),
                    command.args()["product_id"].as<unsigned>()));

            if(client_.post_soap("login", std::begin(request.data), request.size))
            {
                response << "Successfully sent login request";
            }
            else
            {
                response << "Failed to send login request; see logs";
            }
		}
		else
		{
			response << command.options();
		}
	}
	else
	{
		response << "Must be logged out in order to connect, current login status '" << Exchange::asString(loginStatus_) << "'";
	}

	return response.str();
}

std::string BetfairExchange::onLogout(const CommandHandler::Command& command)
{
	std::stringstream response;

    if(loginStatus_ == Exchange::LoginStatus::LOGGED_IN)
	{
        loginStatus_ = Exchange::LoginStatus::LOGGING_OUT;

        if(command.args().empty())
        {
            const Request request(createLogoutRequest(sessionID_));

            if(client_.post_soap("logout", std::begin(request.data), request.size))
            {
                response << "Successfully sent logout request";
            }
            else
            {
                response << "Failed to send logout request; see logs";
            }
        }
        else
        {
            response << command.options();
        }
	}
	else
	{
		response << "Must be logged in order to disconnect, current login status '" << Exchange::asString(loginStatus_) << "'";
	}

	return response.str();
}

void BetfairExchange::onResponse(const char* response, const std::size_t size)
{
    static const pugi::xpath_query msgTypeQuery("//n:Result/@xsi:type");

    LOG(INFO) << std::string(response, size);  // TODO: make debug print

    // build xml
    pugi::xml_document xml;
    const pugi::xml_parse_result result(xml.load_buffer(response, size));

    if(! result)
    {
        LOG(ERROR) << "Failed to create response, error=" << result.description();
    }

    // get msgtype and dispatch to registered callback
    const std::string msgType(msgTypeQuery.evaluate_string(xml));
    const auto iter(responseCallbacks_.find(msgType));

    if(iter != std::end(responseCallbacks_))
    {
        iter->second(xml);
    }
    else
    {
        LOG(ERROR) << "Unknown msgType '" << msgType << "', can't handle";
    }
}

void BetfairExchange::onResponseLogin(const pugi::xml_document& response)
{
    static const pugi::xpath_query sessionIDQuery       ("//sessionToken/text()");
    static const pugi::xpath_query headerErrorCodeQuery ("//header/errorCode");
    static const pugi::xpath_query bodyErrorCodeQuery   ("//n:Result/errorCode");

    const std::string headerErrorCode(headerErrorCodeQuery.evaluate_string(response));
    const std::string bodyErrorCode(bodyErrorCodeQuery.evaluate_string(response));

    if(headerErrorCode == "OK" && bodyErrorCode == "OK")
    {
        loginStatus_ = Exchange::LoginStatus::LOGGED_IN;
        sessionID_ = sessionIDQuery.evaluate_string(response);

        LOG(INFO)
            << "Login succeeded, "
            << " sessionToken=" << sessionID_;
    }
    else
    {
        loginStatus_ = Exchange::LoginStatus::LOGGED_OUT;

        LOG(ERROR)
            << "Login failed,"
            << " headerErrorCode=" << headerErrorCode
            << " bodyErrorCode=" << bodyErrorCode;
    }
}

void BetfairExchange::onResponseLogout(const pugi::xml_document& response)
{
    static const pugi::xpath_query headerErrorCodeQuery ("//header/errorCode");
    static const pugi::xpath_query bodyErrorCodeQuery   ("//n:Result/errorCode");

    const std::string headerErrorCode(headerErrorCodeQuery.evaluate_string(response));
    const std::string bodyErrorCode(bodyErrorCodeQuery.evaluate_string(response));

    if(headerErrorCode == "OK" && bodyErrorCode == "OK")
    {
        loginStatus_ = Exchange::LoginStatus::LOGGED_OUT;
        sessionID_.clear();

        LOG(INFO) << "Logout succeeded";
    }
    else
    {
        loginStatus_ = Exchange::LoginStatus::LOGGED_IN;

        LOG(ERROR)
            << "Logout failed,"
            << " headerErrorCode=" << headerErrorCode
            << " bodyErrorCode=" << bodyErrorCode;
    }
}
