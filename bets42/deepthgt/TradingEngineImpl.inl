namespace 
{
    const char* const TRADING_ENGINE_IMPL_ENTRY("Creating TradingEngineImpl");
    const char* const TRADING_ENGINE_IMPL_EXIT("Destroying TradingEngineImpl");

    const char* const COMPONENT("engine");
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
            const std::string cmd("help");
            boost::program_options::options_description options(cmd);
            cmdHandler_.registrar().registerCommand(COMPONENT, cmd, options, *this);
        }
        {
            const std::string cmd("get_log_level");
            boost::program_options::options_description options(cmd);
            cmdHandler_.registrar().registerCommand(COMPONENT, cmd, options, *this);
        }
        {
            const std::string cmd("set_log_level");
            boost::program_options::options_description options(cmd);
            options.add_options()("level", boost::program_options::value<std::string>(), "Log level threshold");
            cmdHandler_.registrar().registerCommand(COMPONENT, cmd, options, *this);
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
