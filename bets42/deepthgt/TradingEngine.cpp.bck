#include "bets42/deepthgt/TradingEngine.hpp"
#include <boost/program_options.hpp>
#include <glog/logging.h>
#include <glog/log_severity.h>

using namespace bets42::deepthgt;
namespace prog_opts = boost::program_options;

namespace bets42 { namespace deepthgt { namespace detail {

    Initializer::Initializer(const std::string& dir,   
                                   const std::string& filename, 
                                   const std::string& level) 
    {
        google::InitGoogleLogging(filename.c_str());
        google::SetLogDestination(google::INFO, dir.c_str());
        google::SetLogSymlink(google::INFO, filename.c_str());
        google::LogToStderr();

        FLAGS_minloglevel = google::INFO;

        LOG(INFO) << "Creating TradingEngine";
    }

    Initializer::~Initializer() 
    {
        LOG(INFO) << "Destroying TradingEngine";
        google::ShutdownGoogleLogging();
    }
       
}}} //namespace bets42::deepthgt::detail

TradingEngine::TradingEngine(const unsigned short cmdPort,
                             const std::string& logDir,
                             const std::string& logName,
                             const std::string& logLevel)
    : initializer_(logDir, logName, logLevel),
      cmdHandler_(cmdPort) 
{
    registerCommands();
}

void TradingEngine::onCommand(const CommandHandler::Command& command)
{
    LOG(INFO) << "Received command " << command.command;
}

void TradingEngine::run()
{
    LOG(INFO) << "Running TradingEngine";   
    sleep(5);
    LOG(INFO) << "Stopping TradingEngine";   
}

void TradingEngine::registerCommands()
{
    TradingEngine& callback(*this);

    {
        const std::string name("help");
        prog_opts::options_description args(name);

        const CommandHandler::Command command = { name, args, callback };
        cmdHandler_.registerCommand(command);
    }
    {
        const std::string name("create_algo");
        prog_opts::options_description args(name);
        args.add_options()
            ("algo", prog_opts::value<std::string>(), "Create the algo that will be run by the trading engine"); 

        const CommandHandler::Command command = { name, args, callback };
        cmdHandler_.registerCommand(command);
    }
    {
        //FIXME: this probably belongs in BetfairExchange
        const std::string name("connect_betfair");
        prog_opts::options_description args(name);
        args.add_options()
            ("username", prog_opts::value<std::string>(), "The username with which to login to the API for a new session") 
            ("password", prog_opts::value<std::string>(), "The password with which to login to the API for a new session") 
            ("product_id", prog_opts::value<unsigned>(), "The API product ID with which to login to the API for a new session."
                                                         "If you want to use the Free Access API, use 82. If you are a paying API "
                                                         "subscriber, use the Id provided when you signed up.");

        const CommandHandler::Command command = { name, args, callback };
        cmdHandler_.registerCommand(command);
    }
}

