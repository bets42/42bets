#include <bets42/deepthgt/betfair/BetfairExchange.hpp>
#include <bets42/deepthgt/betfair/Requests.hpp>
#include <bets42/arthur/entry_exit.hpp>
#include <boost/program_options.hpp>
#include <glog/logging.h>
#include <pugixml/pugixml.hpp>
#include <chrono>
#include <cstring>
#include <functional>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>

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
    , loginStatus_(LoginStatus::LOGGED_OUT)
    , requestInterval_(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(1)))
	, stopped_(false)
{
    // register commands
    {
        boost::program_options::options_description options("[login]\nLogin to the exchange");
        options.add_options()("username", boost::program_options::value<std::string>(), "Account username");
        options.add_options()("password", boost::program_options::value<std::string>(), "Account password");
        options.add_options()("product_id", boost::program_options::value<unsigned>()->default_value(82),
                "Product ID for the specific API you wish to use, defaults to 82 (free API)");
        options.add_options()("request_limit", boost::program_options::value<unsigned>(),
                "The maximum number of requests a second that should be sent");
        cmdRegistrar.registerCommand(registrant_key(), "login", options, std::bind(&BetfairExchange::onRequestLogin, this, std::placeholders::_1));
    }
    {
        boost::program_options::options_description options("[logout]\nLogout from the exchange");
        cmdRegistrar.registerCommand(registrant_key(), "logout", options, std::bind(&BetfairExchange::onRequestLogout, this, std::placeholders::_1));
    }
    {
        boost::program_options::options_description options("[status]\nSelection of exchange states, e.g. login status, request limit threshold...");
        cmdRegistrar.registerCommand(registrant_key(), "status", options, std::bind(&BetfairExchange::onRequestStatus, this, std::placeholders::_1));
    }
    {
        boost::program_options::options_description options("[get_sports]\nGet details of all sports supported");
        cmdRegistrar.registerCommand(registrant_key(), "get_sports", options, std::bind(&BetfairExchange::onRequestGetActiveEventTypes, this, std::placeholders::_1));
    }

    // register SOAP response callbacks
    session_.registerCallback("n2:LoginResp", std::bind(&BetfairExchange::onResponseLogin, this, std::placeholders::_1));
    session_.registerCallback("n2:LogoutResp", std::bind(&BetfairExchange::onResponseLogout, this, std::placeholders::_1));
    session_.registerCallback("n2:GetEventTypesResp", std::bind(&BetfairExchange::onResponseGetActiveEventTypes, this, std::placeholders::_1));
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
    const arthur::entry_exit entryExit("BetfairExchange thread");

    bool isEmpty(true);

	while(! stopped_)
	{
        auto start(std::chrono::high_resolution_clock::now());

        {
            // We only acquire the lock to check if the queue is empty (this isn't
            // an atomic operation). If it isn't then its safe to hold an iterator
            // to the data without the lock given (a) writes - possible on another
            // thread - don't invalidate the read iterator and (b) deletes only
            // invalidate iterators to the deleted item and this thread is the
            // only one which performs deletes.
            std::lock_guard<RequestQueue> lock(requestQueue_);
            isEmpty = requestQueue_.empty();
        }

        if( ! isEmpty)
        {
            RequestQueue::const_iterator requestIter(requestQueue_.begin());

            if(sendRequest(*requestIter))
            {
                LOG(INFO) << "Processed request: " << *requestIter;
            }
            else
            {
                LOG(ERROR) << "Failed to process request: " << *requestIter;
            }

            requestQueue_.erase(requestIter);
        }

        // throttle requests to avoid data usage charges
        std::this_thread::sleep_until(start + requestInterval_);
	}
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

    std::lock_guard<LoginStatus> lock(loginStatus_);

    if(loginStatus_.isLoggedOut())
	{
		if(command.args().count("username")
		    && command.args().count("password")
		    && command.args().count("product_id")
		    && command.args().count("request_limit"))
		{
            loginStatus_ = LoginStatus::LOGGING_IN;

            const auto oneSec(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(1)));
            requestInterval_ =  oneSec / command.args()["request_limit"].as<unsigned>();

            const SOAPClient::Request request(
                createLoginRequest(
                    command.args()["username"].as<std::string>(),
                    command.args()["password"].as<std::string>(),
                    command.args()["product_id"].as<unsigned>()));

            // Synchronously call the API, we know there is no other requests in the queue
            // so we don't need to execute this request on the exchange's thread. Once we're
            // satisfied set the login status to LOGGED_IN which will then permit other command
            // requests to be accepted and processed
            if(sendRequest(request) && loginStatus_.isLoggedIn())
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
		response << "Must be logged out in order to connect, current login status '" << loginStatus_ << "'";
	}

	return response.str();
}

std::string BetfairExchange::onRequestLogout(const CommandHandler::Command& command)
{
	std::stringstream response;

    std::lock_guard<LoginStatus> lock(loginStatus_);

    if(loginStatus_.isLoggedIn())
	{
        loginStatus_ = LoginStatus::LOGGING_OUT;

        if(command.args().empty())
        {
            const SOAPClient::Request request(createLogoutRequest(sessionID_));

            // Now the login status is not LOGGED_IN, no more commands will be accepted
            // meaning once the request queue is empty we can synchronously logout from the
            // exchange on this thread. Once // we are satisfied the logout is successful
            // the login status will be set to LOGGED_OUT meaning only a login can be performed.
            if(sendRequest(request) && loginStatus_.isLoggedOut())
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
		response << "Must be logged in order to disconnect, current login status '" << loginStatus_ << "'";
	}

	return response.str();
}

std::string BetfairExchange::onRequestStatus(const CommandHandler::Command& command)
{
	std::stringstream response;

    if(command.args().empty())
    {
        response
            << "Login Status: " << loginStatus_ << "\n"
            << "Request Limit: " << requestInterval_.count() << "ms";
    }
    else
    {
        response << command.options();
    }

	return response.str();
}

std::string BetfairExchange::onRequestGetActiveEventTypes(const CommandHandler::Command& command)
{
	std::stringstream response;

    if(command.args().empty())
    {
        enqueueRequest(
            createGetActiveEventTypesRequest(sessionID_),
            command,
            response);
    }
    else
    {
        response << command.options();
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
        loginStatus_ = LoginStatus::LOGGED_IN;
        LOG(INFO) << "Login succeeded; sessionToken=" << sessionID_;
    }
    else
    {
        loginStatus_ = LoginStatus::LOGGED_OUT;
        LOG(ERROR) << "Login failed; " << errors;
    }
}

void BetfairExchange::onResponseLogout(const pugi::xml_document& response)
{
    const ErrorCodes errors(getErrorCodes(response));

    if(errors.isOK())
    {
        sessionID_.clear();
        loginStatus_ = LoginStatus::LOGGED_OUT;
        LOG(INFO) << "Logout succeeded";
    }
    else
    {
        loginStatus_ = LoginStatus::LOGGED_IN;
        LOG(ERROR) << "Logout failed; " << errors;
    }
}

void BetfairExchange::onResponseGetActiveEventTypes(const pugi::xml_document& response)
{
    const ErrorCodes errors(getErrorCodes(response));
    sessionID_ = getSessionID(response);

    if(errors.isOK())
    {
        LOG(INFO) << "GetActiveEventTypes request succeeded";
    }
    else
    {
        LOG(ERROR) << "GetActiveEventTypes request failed; " << errors;
    }
}

// utils
bool BetfairExchange::sendRequest(const SOAPClient::Request& request)
{
    return session_.post(request);
}

std::ostream& BetfairExchange::enqueueRequest(SOAPClient::Request&& request, const CommandHandler::Command& command, std::ostream& stream)
{
    std::lock_guard<LoginStatus> lock1(loginStatus_);

    if(loginStatus_.isLoggedIn())
	{
        {
            // We lock whilst pushing to ensure consumer thread
            // doesn't try to read until the item is fully constructed
            std::lock_guard<RequestQueue> lock2(requestQueue_);
            requestQueue_.push_back(std::move(request));
        }

        stream << "Successfully enqueued request for command '" << command.name() << "'";
    }
	else
	{
		stream
            << "Must be logged in order to enqueue request for command: '" << command.name()
            << "', current login status '" << loginStatus_ << "'";
	}

	return stream;
}

