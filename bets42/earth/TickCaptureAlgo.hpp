#ifndef BETS42_EARTH_TICK_CAPTURE_ALGO_HPP
#define BETS42_EARTH_TICK_CAPTURE_ALGO_HPP

#include "bets42/deepthgt/Algo.hpp"
#include "bets42/deepthgt/CommandHandler.hpp"

namespace bets42 { namespace earth {

    struct TickCaptureAlgo : public deepthgt::Algo<TickCaptureAlgo>
    {
        explicit TickCaptureAlgo(deepthgt::CommandHandler::Registrar& cmdRegistrar);
        virtual ~TickCaptureAlgo();

        static const std::string& name();
    };

}} //namespace bets42::earth

#endif //BETS42_EARTH_TICK_CAPTURE_ALGO_HPP   
