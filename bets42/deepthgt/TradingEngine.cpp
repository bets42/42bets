#include <bets42/deepthgt/TradingEngine.hpp>
#include <bets42/deepthgt/Exception.hpp>
#include <glog/logging.h>
#include <boost/program_options.hpp>
#include <functional>
#include <future>
#include <sstream>
#include <thread>
#include <vector>

using namespace bets42::deepthgt;

namespace 
{
    const char* const COMPONENT("engine");
}

TradingEngine::TradingEngine(const std::string& algo, 
                             const std::vector<std::string>& exchanges, 
                             const unsigned short port)
    : entryExit_("TradingEngine")
    , cmdHandler_(port)
{
    //register engine commands
    using namespace std::placeholders;
    {
        boost::program_options::options_description options("[get_log_level]\nGet log level threshold");
        cmdHandler_.registrar().registerCommand(COMPONENT, "get_log_level", options, std::bind(&TradingEngine::onGetLogLevel, this, _1));
    }
    {
        boost::program_options::options_description options("[set_log_level]\nSet log level threshold");
        options.add_options()("level", boost::program_options::value<std::string>(), "Log level threshold, choose from [INFO|WARN|ERROR]");
        cmdHandler_.registrar().registerCommand(COMPONENT, "set_log_level", options, std::bind(&TradingEngine::onSetLogLevel, this, _1));
    }
    {
        boost::program_options::options_description options("[shutdown]\nTerminte the process");
        cmdHandler_.registrar().registerCommand(COMPONENT, "shutdown", options, std::bind(&TradingEngine::onShutdown, this, _1));
    }   
    {
        boost::program_options::options_description options("[help]\nGet help information");
        cmdHandler_.registrar().registerCommand(COMPONENT, "help", options, std::bind(&TradingEngine::onHelp, this, _1));
    }    

    //create exchanges
    for(const std::string& exchange : exchanges)
    {
        try
        {
            LOG(INFO) << "Creating exchange '" << exchange << "'";

            if( ! exchanges_.emplace(ExchangeFactory::create(exchange, cmdHandler_.registrar())).second)
            {
                LOG(WARNING) << "Can't add duplicate exchange '" << exchange << "'";
            }
        }
        catch(const std::exception& e)
        {
            throw Exception(__FILE__, __LINE__) 
                << "Error whilst creating exchange '" << exchange << "', error= " << e.what();
        }
    }

    //create algo
    try
    {
        LOG(INFO) << "Creating algo '" << algo << "'";
        algo_ = AlgoFactory::create(algo, exchanges_, cmdHandler_.registrar());
    }
    catch(const std::exception& e)
    {
        throw Exception(__FILE__, __LINE__) 
            << "Error whilst creating algo '" << algo << "', error=" << e.what();
    }
}

TradingEngine::~TradingEngine() {}

std::string TradingEngine::onGetLogLevel(const CommandHandler::Command&) 
{
    std::lock_guard<std::mutex> lock(cmdMutex_);

    std::stringstream stream;
    stream << "Log level threshold is " << google::GetLogSeverityName(FLAGS_minloglevel);
    return stream.str();
}

std::string TradingEngine::onSetLogLevel(const CommandHandler::Command& command) 
{
    std::lock_guard<std::mutex> lock(cmdMutex_);

    static const std::map<std::string, decltype(FLAGS_minloglevel)> logLevels 
    {
        { "INFO", google::INFO },
        { "WARN", google::WARNING },
        { "ERROR", google::ERROR }
    };

    std::stringstream response;

    if(command.args().count("level"))
    {
        const auto level(command.args()["level"].as<std::string>());
        const auto iter(logLevels.find(level));

        if(iter != std::end(logLevels))
        {
            const auto prevLogLevel(FLAGS_minloglevel);
            FLAGS_minloglevel = iter->second;

            response 
                << "Changed log level from " 
                << google::GetLogSeverityName(prevLogLevel) << " to "
                << google::GetLogSeverityName(FLAGS_minloglevel);

            LOG(INFO) << response.str(); 
        }
    }

    if(response.str().empty())
    {
        response << command.options();
    }

    return response.str();
}   

std::string TradingEngine::onShutdown(const CommandHandler::Command&) 
{
    std::lock_guard<std::mutex> lock1(cmdMutex_);
    std::lock_guard<std::mutex> lock2(shutdownConditionMutex_);

    const std::string response("Proceeding to shutdown engine");
    LOG(INFO) << response;

    shutdownCondition_.notify_one();

    return response;
}

std::string TradingEngine::onHelp(const CommandHandler::Command&) 
{
    std::lock_guard<std::mutex> lock(cmdMutex_);
    return cmdHandler_.usage(COMPONENT);
}
  
void TradingEngine::run() 
{
    //run threads
    auto algoFuture(std::async(
        std::launch::async, 
        &Algo::run, 
        algo_.get()));
               
    std::vector<std::future<void>> exchangeFutures;
    for(auto& exchange : exchanges_)
    {
        exchangeFutures.emplace_back(std::async(
            std::launch::async,
            &Exchange::run,
            exchange.get()));
    }

    auto cmdHandlerFuture(std::async(
        std::launch::async, 
        &CommandHandler::run, 
        std::addressof(cmdHandler_)));
                                 
    //wait for shutdown signal
    {
        std::unique_lock<std::mutex> lock(shutdownConditionMutex_);
        shutdownCondition_.wait(lock);
    }

    //join threads
    cmdHandler_.stop();
    cmdHandlerFuture.wait();

    std::size_t offset(0);
    for(auto& exchange : exchanges_)
    {
        exchange->stop();
        exchangeFutures.at(offset++).wait();
    }

    algo_->stop();
    algoFuture.wait();
}
