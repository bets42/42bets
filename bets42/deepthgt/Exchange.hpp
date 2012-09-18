#ifndef BETS42_DEEPTHGT_EXCHANGE_HPP
#define BETS42_DEEPTHGT_EXCHANGE_HPP

#include "bets42/arthur/abstract_factory.hpp"
#include <string>
#include <vector>   

namespace bets42 { namespace deepthgt {

    struct Exchange
    {
        virtual ~Exchange() {}
    };

    typedef arthur::static_abstract_factory<Exchange> ExchangeFactory;

}}

#endif //BETS42_DEEPTHGT_EXCHANGE_HPP
