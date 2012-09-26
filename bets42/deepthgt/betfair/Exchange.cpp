#include <bets42/deepthgt/betfair/Exchange.hpp>
#include <glog/logging.h>
#include <boost/program_options.hpp>
#include <functional>

using namespace bets42::deepthgt::betfair;

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
    while( ! stopped_) sleep(1);
    LOG(INFO) << "Stopped '" << name() << "' exchange";
}

void Exchange::stop()
{
    LOG(INFO) << "Proceeding to stop '" << name() << "'exchange";
    stopped_ = true;
} 

std::string Exchange::onConnect(const CommandHandler::Command& command)
{
    return "kk";
}

std::string Exchange::onDisconnect(const CommandHandler::Command& command)
{
    return "kk";
} 
