#include <bets42/deepthgt/betfair/BetfairExchange.hpp>
#include <bets42/deepthgt/betfair/Requests.hpp>
#include <boost/program_options.hpp>
#include <glog/logging.h>
#include <pugixml/pugixml.hpp>
#include <cstring>
#include <functional>
#include <ostream>
#include <string>

using namespace bets42::deepthgt::betfair;

namespace
{
    // strings
    const char* const RESPONSE_TYPE_XPATH = "//n:Result/@xsi:type";
    const char* const URL_GLOBAL = "https://api.betfair.com/global/v3/BFGlobalService";
    const char* const URL_UK = "https://api.betfair.com/exchange/v5/BFExchangeService";

    // sessionID
    std::string getSessionID(const pugi::xml_document& xml)
    {
        static const pugi::xpath_query query("//sessionToken/text()");
        return query.evaluate_string(xml);
    }

    // error codes
    struct ErrorCodes
    {
        bool isOK() const
        {
            return header == "OK" && body == "OK";
        }

        const std::string header;
        const std::string body;
    };

    std::ostream& operator<<(std::ostream& stream, const ErrorCodes& errors)
    {
        stream << "ErrorCodes: header=" << errors.header << ", body=" << errors.body;
        return stream;
    }

    ErrorCodes getErrorCodes(const pugi::xml_document& xml)
    {
        static const pugi::xpath_query headerQuery  ("//header/errorCode");
        static const pugi::xpath_query bodyQuery    ("//n:Result/errorCode");
        return ErrorCodes { headerQuery.evaluate_string(xml), bodyQuery.evaluate_string(xml) };
    }
}

// constructor
BetfairExchange::BetfairExchange(deepthgt::CommandHandler::Registrar& cmdRegistrar)
	: Exchange(marvin::exch_id::betfair)
    , session_(URL_GLOBAL, RESPONSE_TYPE_XPATH)
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
		cmdRegistrar.registerCommand(registrant_key(), "login", options, std::bind(&BetfairExchange::onRequestLogin, this, std::placeholders::_1));
	}
	{
		boost::program_options::options_description options("[logout]\nLogout from the exchange");
		cmdRegistrar.registerCommand(registrant_key(), "logout", options, std::bind(&BetfairExchange::onRequestLogout, this, std::placeholders::_1));
	}

    // register SOAP response callbacks
    session_.registerCallback("n2:LoginResp", std::bind(&BetfairExchange::onResponseLogin, this, std::placeholders::_1));
    session_.registerCallback("n2:LogoutResp", std::bind(&BetfairExchange::onResponseLogout, this, std::placeholders::_1));
}

BetfairExchange::~BetfairExchange() {}

// factory
const std::string& BetfairExchange::registrant_key()
{
	static const std::string key("betfair");
	return key;
}

// threading
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

// requests
std::string BetfairExchange::onRequestLogin(const CommandHandler::Command& command)
{
	std::stringstream response;

    if(isLoggedOut())
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

            // Synchronously call the API, we know there is no other requests in the queue
            // so we don't need to execute this request on the exchange's thread. Once we're
            // satisfied set the login status to LOGGED_IN which will then permit other command
            // requests to be accepted and processed
            if(sendRequest(request) && isLoggedIn())
            {
                response << "Successfully logged in";
            }
            else
            {
                response << "Failed to login; see logs";
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

std::string BetfairExchange::onRequestLogout(const CommandHandler::Command& command)
{
	std::stringstream response;

    if(isLoggedIn())
	{
        loginStatus_ = Exchange::LoginStatus::LOGGING_OUT;

        if(command.args().empty())
        {
            const Request request(createLogoutRequest(sessionID_));

            // Now the login status is not LOGGED_IN, no more commands will be accepted
            // meaning once the request queue is empty we can synchronously logout from the
            // exchange on this thread. Once // we are satisfied the logout is successful
            // the login status will be set to LOGGED_OUT meaning only a login can be performed.
            if(sendRequest(request) && isLoggedOut())
            {
                response << "Successfully logged out";
            }
            else
            {
                response << "Failed to logout; see logs";
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

// responses
void BetfairExchange::onResponseLogin(const pugi::xml_document& response)
{
    const ErrorCodes errors(getErrorCodes(response));

    if(errors.isOK())
    {
        sessionID_ = getSessionID(response);
        loginStatus_ = Exchange::LoginStatus::LOGGED_IN;
        LOG(INFO) << "Login succeeded; sessionToken=" << sessionID_;
    }
    else
    {
        loginStatus_ = Exchange::LoginStatus::LOGGED_OUT;
        LOG(ERROR) << "Login failed; " << errors;
    }
}

void BetfairExchange::onResponseLogout(const pugi::xml_document& response)
{
    const ErrorCodes errors(getErrorCodes(response));

    if(errors.isOK())
    {
        sessionID_.clear();
        loginStatus_ = Exchange::LoginStatus::LOGGED_OUT;
        LOG(INFO) << "Logout succeeded";
    }
    else
    {
        loginStatus_ = Exchange::LoginStatus::LOGGED_IN;
        LOG(ERROR) << "Logout failed; " << errors;
    }
}

// utils
bool BetfairExchange::isLoggedIn() const
{
    return loginStatus_ == Exchange::LoginStatus::LOGGED_IN;
}

bool BetfairExchange::isLoggedOut() const
{
    return loginStatus_ == Exchange::LoginStatus::LOGGED_OUT;
}

bool BetfairExchange::sendRequest(const Request& request)
{
    return session_.post(request.action, std::begin(request.data), request.size);
}

