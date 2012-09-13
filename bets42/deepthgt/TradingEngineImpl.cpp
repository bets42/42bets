#include "bets42/deepthgt/TradingEngineImpl.hpp"

namespace {

    const char* const TRADING_ENGINE_IMPL_ENTRY("Creating TradingEngineImpl");
    const char* const TRADING_ENGINE_IMPL_EXIT("Destroying TradingEngineImpl");

} //annonymous namespace

using namespace bets42::deepthgt;
namespace prog_opts = boost::program_options;

template <typename TAlgo>
TradingEngineImpl<TAlgo>::TradingEngineImpl(const std::vector<std::string>& exchanges,
                                            const unsigned short port)
    : entryExit_(TRADING_ENGINE_IMPL_ENTRY, TRADING_ENGINE_IMPL_EXIT),
      cmdHandler_(port)
{
    registerCommands();
}

template <typename TAlgo>
TradingEngineImpl<TAlgo>::~TradingEngineImpl()
{
}

template <typename TAlgo>
void TradingEngineImpl<TAlgo>::onCommand(const CommandHandler::Command& command)
{
}

template <typename TAlgo>
void TradingEngineImpl<TAlgo>::run()
{
}

template <typename TAlgo>
void TradingEngineImpl<TAlgo>::registerCommands()
{
    TradingEngineImpl<TAlgo>& callback(*this);

    {
        const std::string name("help");
        prog_opts::options_description args(name);

        const CommandHandler::Command command = { name, args, callback };
        cmdHandler_.registerCommand(command);
    }
    /*
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
    */
}


