#ifndef BETS42_DEEPTHGT_TRADING_ENGINE_IMPL_HPP
#define BETS42_DEEPTHGT_TRADING_ENGINE_IMPL_HPP

#include "bets42/arthur/entry_exit.hpp"
#include "bets42/deepthgt/TradingEngine.hpp"
#include "bets42/deepthgt/CommandHandler.hpp"
#include <boost/program_options.hpp>
#include <string>
#include <vector>

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

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_TRADING_ENGINE_IMPL_HPP
