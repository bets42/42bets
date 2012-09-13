#ifndef BETS42_DEEPTHGT_TRADAING_ENGINE_HPP
#define BETS42_DEEPTHGT_TRADAING_ENGINE_HPP

#include "bets42/arthur/abstract_factory.hpp"
#include <string>
#include <vector>

namespace bets42 { namespace deepthgt {

    struct TradingEngine
    {
        TradingEngine() {}
        virtual ~TradingEngine() {}

        virtual void run() = 0;
    };

   typedef arthur::static_abstract_factory<TradingEngine, std::vector<std::string>, unsigned short> TradingEngineFactory;

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_TRADAING_ENGINE_HPP
