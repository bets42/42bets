#include <functional>

namespace 
{
    constexpr char* TRADING_ENGINE_IMPL_ENTRY("Creating TradingEngineImpl");
    constexpr char* TRADING_ENGINE_IMPL_EXIT("Destroying TradingEngineImpl");

    constexpr char* COMPONENT("engine");
}

namespace bets42 { namespace deepthgt {

    template <typename TAlgo>
    TradingEngineImpl<TAlgo>::TradingEngineImpl(const std::vector<std::string>& exchanges,
                                                const unsigned short port)
        : entryExit_(TRADING_ENGINE_IMPL_ENTRY, TRADING_ENGINE_IMPL_EXIT)
        , cmdHandler_(port)
        , algo_(cmdHandler_.registrar()) 
    {
        typedef TradingEngineImpl<TAlgo> SelfType;

        using namespace std::placeholders;

        {
            boost::program_options::options_description options("[help]\nGet help information");
            cmdHandler_.registrar().registerCommand(COMPONENT, "help", options, std::bind(&SelfType::onHelp, this, _1));
        }    
        {
            boost::program_options::options_description options("[get_log_level]\nGet log level threshold");
            cmdHandler_.registrar().registerCommand(COMPONENT, "get_log_level", options, std::bind(&SelfType::onGetLogLevel, this, _1));
        }
        {
            boost::program_options::options_description options("[set_log_level]\nSet log level threshold");
            options.add_options()("level", boost::program_options::value<std::string>(), "Log level threshold");
            cmdHandler_.registrar().registerCommand(COMPONENT, "set_log_level", options, std::bind(&SelfType::onSetLogLevel, this, _1));
        }      
    }

    template <typename TAlgo>
    TradingEngineImpl<TAlgo>::~TradingEngineImpl() {}

    template <typename TAlgo>
    std::string TradingEngineImpl<TAlgo>::onGetLogLevel(const CommandHandler::Command& command) 
    {
        return "get_log_level notimplemented";
    }

    template <typename TAlgo>
    std::string TradingEngineImpl<TAlgo>::onSetLogLevel(const CommandHandler::Command& command) 
    {
        return "set_log_level notimplemented";
    }   
 
    template <typename TAlgo>
    std::string TradingEngineImpl<TAlgo>::onHelp(const CommandHandler::Command& command) 
    {
        return cmdHandler_.usage(COMPONENT);
    }
      
    template <typename TAlgo>
    void TradingEngineImpl<TAlgo>::run() 
    {
        LOG(INFO) << "Running!";
        sleep(30);
        LOG(INFO) << "!Running!";
    }

}}
