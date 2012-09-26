#ifndef BETS42_DEEPTHGT_BETFAIR_EXCHANGE_HPP
#define BETS42_DEEPTHGT_BETFAIR_EXCHANGE_HPP

#include <bets42/deepthgt/Exchange.hpp>
#include <bets42/deepthgt/CommandHandler.hpp>
#include <string>

namespace bets42 { namespace deepthgt { namespace betfair {

    class Exchange : public deepthgt::Exchange
                   , private deepthgt::ExchangeFactory::registrant<Exchange>
    {
        public:
            explicit Exchange(deepthgt::CommandHandler::Registrar& cmdRegistrar);
            virtual ~Exchange();

            //factory
            static const std::string& registrant_key();

            //threading
            void run();
            void stop();

        private:
            //command handlers
            std::string onConnect(const CommandHandler::Command& command);
            std::string onDisconnect(const CommandHandler::Command& command);

        private:
            bool stopped_;
            friend class CommandHandler; //command handlers
    };                                                   

}}}

#endif //BETS42_DEEPTHGT_BETFAIR_EXCHANGE_HPP
