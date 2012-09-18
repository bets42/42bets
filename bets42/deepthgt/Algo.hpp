#ifndef BETS42_DEEPTHGT_ALGO_HPP
#define BETS42_DEEPTHGT_ALGO_HPP

#include "bets42/deepthgt/TradingEngineImpl.hpp"

namespace bets42 { namespace deepthgt {

    template <typename TDerived>
    struct Algo : private TradingEngineFactory_AlgoRegistrant<TDerived>
    {
        virtual ~Algo() {}
    };

}}

#endif //BETS42_DEEPTHGT_ALGO_HPP
