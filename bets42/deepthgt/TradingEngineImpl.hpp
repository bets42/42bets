#ifndef BETS42_DEEPTHGT_TRADING_ENGINE_IMPL_HPP
#define BETS42_DEEPTHGT_TRADING_ENGINE_IMPL_HPP

#include "bets42/arthur/entry_exit.hpp"
#include "bets42/deepthgt/TradingEngine.hpp"
#include "bets42/deepthgt/CommandHandler.hpp"
#include <string>
#include <vector>

namespace bets42 { namespace deepthgt {

    template <typename TAlgo>
    class TradingEngineImpl : public TradingEngine
    {
        public:
            TradingEngineImpl(const std::vector<std::string>& exchanges,
                              const unsigned short port);

            virtual ~TradingEngineImpl();

            static const std::string& registrant_key() { return TAlgo::name(); }

            /* commands */
            std::string onGetLogLevel(const CommandHandler::Command& command);
            std::string onSetLogLevel(const CommandHandler::Command& command);
            std::string onHelp(const CommandHandler::Command& command);

            /* engine thread */
            void run();

        private:
            arthur::entry_exit  entryExit_;
            CommandHandler      cmdHandler_;
            TAlgo               algo_;
    };
    
    template <typename TAlgo>
    using TradingEngineFactory_AlgoRegistrant = TradingEngineFactory::registrant<TradingEngineImpl<TAlgo>>;
}}

#include "bets42/deepthgt/TradingEngineImpl.inl"

#endif //BETS42_DEEPTHGT_TRADING_ENGINE_IMPL_HPP
