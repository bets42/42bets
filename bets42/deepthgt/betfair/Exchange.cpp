#include <bets42/deepthgt/betfair/Exchange.hpp>
#include <glog/logging.h>
#include <boost/program_options.hpp>
#include <functional>

using namespace bets42::deepthgt::betfair;

namespace
{
	const char*
}

Exchange::Exchange(deepthgt::CommandHandler::Registrar& cmdRegistrar)
	: deepthgt::Exchange(marvin::exch_id::betfair)
	, stopped_(false)
{
	using namespace std::placeholders;
	{
		boost::program_options::options_description options("[connect]\nConnect to the exchange");
		options.add_options()("username", boost::program_options::value<std::string>(), "Account username");
		options.add_options()("password", boost::program_options::value<std::string>(), "Account password");
		options.add_options()("product_id", boost::program_options::value<unsigned>()->default_value(82),
		                      "Product ID for the specific API you wish to use, defaults to 82 (free API)");
		cmdRegistrar.registerCommand(name(), "connect", options, std::bind(&Exchange::onConnect, this, _1));
	}
	{
		boost::program_options::options_description options("[disconnect]\nDisconnect from the exchange");
		cmdRegistrar.registerCommand(name(), "disconnect", options, std::bind(&Exchange::onDisconnect, this, _1));
	}
}

Exchange::~Exchange() {}

const std::string& Exchange::registrant_key()
{
	static const std::string key("betfair");
	return key;
}

void Exchange::run()
{
	LOG(INFO) << "Running '" << name() << "' exchange";

	while(! stopped_)
	{
		sleep(1);
	}

	LOG(INFO) << "Stopped '" << name() << "' exchange";
}

void Exchange::stop()
{
	LOG(INFO) << "Proceeding to stop '" << name() << "'exchange";
	stopped_ = true;
}


bool cas(T& val, T& new_val, T& compare)
{
	const bool result(val == compare);

	if(result)
	{
		val == new_val;
	}

	return val;
}

bool Exchange::loginStatusCAS(LoginStatus required, LoginStatus newValue)
{
	return std::atomic_compare_exchange_strong(&loginStatus_, &required, &newStatus);
}

std::string Exchange::onConnect(const CommandHandler::Command& command)
{
	std::stringstream response;

	LoginStaus expected(LoginStatus::LOGGED_OUT);

	if(std::atomic_compare_exchange_strong(&loginStatus_, &expected, LoginStatus::LOGGING_IN))
	{
		if(command.args().count("username")
		   && commands.args().count("password")
		   && commands.args().count("productID"))
		{
			createLoginRequest(
			    commands.args()["username"].as<std::string>(),
			    commands.args()["password"].as<std::string>(),
			    commands.args()["productID"].as<unsigned>());
		}
		else
		{
			response << command.options();
		}
	}
	else
	{
		response << "Must be logged out in order to connect, current status " << expected;
	}

	return response;
}

std::string Exchange::onDisconnect(const CommandHandler::Command& command)
{
	std::string response;

	LoginStaus expected(LoginStatus::LOGGED_IN);

	if(std::atomic_compare_exchange_strong(&loginStatus_, &expected, LoginStatus::LOGGING_OUT))
	{
		createLogoutRequest(sessionID_);
	}
	else
	{
		response = "Must be logged in order to disconnect";
	}

	return response;
}
