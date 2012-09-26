#ifndef BETS42_DEEPTHGT_TRADAING_ENGINE_HPP
#define BETS42_DEEPTHGT_TRADAING_ENGINE_HPP

#include "bets42/deepthgt/Algo.hpp"
#include "bets42/deepthgt/CommandHandler.hpp"
#include "bets42/deepthgt/Exchange.hpp"
#include <bets42/arthur/entry_exit.hpp>
#include <bets42/marvin/types.hpp>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace bets42 { namespace deepthgt {

    class TradingEngine
    {
        public:
            TradingEngine(
                const std::string& algo,
                const std::vector<std::string>& exchanges,
                const unsigned short port);

            ~TradingEngine();

            void run();

        private:
            //command handlers
            std::string onGetLogLevel(const CommandHandler::Command& command);
            std::string onSetLogLevel(const CommandHandler::Command& command);
            std::string onShutdown(const CommandHandler::Command& command);
            std::string onHelp(const CommandHandler::Command& command);  

        private:
            arthur::entry_exit      entryExit_;
            CommandHandler          cmdHandler_;
            ExchangeSet             exchanges_;
            std::unique_ptr<Algo>   algo_;

            std::condition_variable shutdownCondition_;

            mutable std::mutex      cmdMutex_;
            mutable std::mutex      shutdownConditionMutex_;

            friend class CommandHandler; //command callbacks
    };

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_TRADAING_ENGINE_HPP
