#ifndef BETS42_DEEPTHGT_TRADING_ENGINE_IMPL_HPP
#define BETS42_DEEPTHGT_TRADING_ENGINE_IMPL_HPP

#include "bets42/arthur/entry_exit.hpp"
#include "bets42/deepthgt/TradingEngine.hpp"
#include "bets42/deepthgt/CommandHandler.hpp"
#include <boost/program_options.hpp>
#include <string>
#include <vector>

namespace {

    const char* const TRADING_ENGINE_IMPL_ENTRY("Creating TradingEngineImpl");
    const char* const TRADING_ENGINE_IMPL_EXIT("Destroying TradingEngineImpl");

} //annonymous namespace  

namespace bets42 { namespace deepthgt {

    template <typename TAlgo>
    class TradingEngineImpl : public TradingEngine,
                              public CommandHandler::Callback
    {
        public:
            TradingEngineImpl(const std::vector<std::string>& exchanges,
                              const unsigned short port);

            virtual ~TradingEngineImpl();

            static const std::string& registrar_key() { return TAlgo::registrar_key(); }

            void onCommand(const CommandHandler::Command& command);

            void run();

        private:
            void registerCommands();

        private:
            arthur::EntryExit   entryExit_;
            CommandHandler      cmdHandler_;
            TAlgo               algo_;
    };

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
        namespace prog_opts = boost::program_options;

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

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_TRADING_ENGINE_IMPL_HPP
