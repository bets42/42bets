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
        {
            boost::program_options::options_description options("[help]\nGet help information");
            cmdHandler_.registrar().registerCommand(COMPONENT, "help", options, *this);
        }
        {
            boost::program_options::options_description options("[get_log_level]\nGet log level threshold");
            cmdHandler_.registrar().registerCommand(COMPONENT, "get_log_level", options, *this);
        }
        {
            boost::program_options::options_description options("[set_log_level]\nSet log level threshold");
            options.add_options()("level", boost::program_options::value<std::string>(), "Log level threshold");
            cmdHandler_.registrar().registerCommand(COMPONENT, "set_log_level", options, *this);
        }
    }

    template <typename TAlgo>
    TradingEngineImpl<TAlgo>::~TradingEngineImpl() {}

    template <typename TAlgo>
    std::string TradingEngineImpl<TAlgo>::onCommand(const CommandHandler::Command& command) 
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
