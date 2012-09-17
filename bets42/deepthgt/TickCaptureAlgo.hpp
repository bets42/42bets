#ifndef BETS42_DEEPTHGT_TICK_CAPTURE_ALGO_HPP
#define BETS42_DEEPTHGT_TICK_CAPTURE_ALGO_HPP

#include "bets42/deepthgt/Algo.hpp"
#include "bets42/deepthgt/CommandHandler.hpp"

namespace bets42 { namespace deepthgt {

    struct TickCaptureAlgo : public Algo<TickCaptureAlgo>
    {
        explicit TickCaptureAlgo(CommandHandler::Registrar& cmdRegistrar);
        virtual ~TickCaptureAlgo();

        static const std::string& name();
    };

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_TICK_CAPTURE_ALGO_HPP   
