#ifndef BETS42_DEEPTHGT_TICK_CAPTURE_ALGO_HPP
#define BETS42_DEEPTHGT_TICK_CAPTURE_ALGO_HPP

#include "bets42/deepthgt/Algo.hpp"
#include "bets42/deepthgt/TradingEngine.hpp"
#include "bets42/deepthgt/TradingEngineImpl.hpp"

namespace bets42 { namespace deepthgt {

    struct TickCaptureAlgo : public Algo,
                             public TradingEngineFactory::registrar<TradingEngineImpl<TickCaptureAlgo>>
    {
        virtual ~TickCaptureAlgo() {}

        static const std::string& registrar_key();
    };

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_TICK_CAPTURE_ALGO_HPP   
