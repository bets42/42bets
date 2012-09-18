namespace 
{
    const char* const TRADING_ENGINE_IMPL_ENTRY("Creating TradingEngineImpl");
    const char* const TRADING_ENGINE_IMPL_EXIT("Destroying TradingEngineImpl");

    const char* const COMPONENT_NAME("engine");
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
          //  boost::program_options::options_description options("help");
          //  cmdHandler_.registrar().registerCommand("engine", "help", options, *this);
            //cmdHandler_.registrar().registerCommand("engine", "help", options, *this);
        }   
    }

    template <typename TAlgo>
    TradingEngineImpl<TAlgo>::~TradingEngineImpl() {}

    template <typename TAlgo>
    std::string TradingEngineImpl<TAlgo>::onCommand(const CommandHandler::Command& command) 
    {
        return "not_implemented";
    }

    template <typename TAlgo>
    void TradingEngineImpl<TAlgo>::run() 
    {
        LOG(INFO) << "Running!";
        sleep(30);
        LOG(INFO) << "!Running!";
    }

}}
