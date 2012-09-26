#ifndef BETS42_DEEPTHGT_EXCHANGE_IMPL_HPP
#define BETS42_DEEPTHGT_EXCHANGE_IMPL_HPP

namespace bets42 { namespace deepthgt {{

    template <typename TAdapter>
    class ExchangeImpl : public Exchange
    {
        public:
            ExchangeImpl(CommandHandler::Registrar& cmdRegistrar);
            virtual ~ExchangeImpl();

        private:
            
    };

}} 

#endif //BETS42_DEEPTHGT_EXCHANGE_IMPL_HPP
