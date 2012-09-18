#ifndef BETS42_DEEPTHGT_EXCHANGE_IMPL_HPP
#define BETS42_DEEPTHGT_EXCHANGE_IMPL_HPP

namespace bets42 { namespace deepthgt {{

    template <typename TClient, typename TProtocol>
    class ExchangeImpl : public Exchange
    {
        public:
            ExchangeImpl(TClient& client, CommandHandler::Registrar& cmdRegistrar);
            virtual ~ExchangeImpl();

        private:
            
    };

}} 

#endif //BETS42_DEEPTHGT_EXCHANGE_IMPL_HPP
