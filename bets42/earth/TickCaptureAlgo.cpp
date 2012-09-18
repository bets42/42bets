#include "bets42/earth/TickCaptureAlgo.hpp"

using namespace bets42::earth;

TickCaptureAlgo::TickCaptureAlgo(deepthgt::CommandHandler::Registrar& cmdRegistrar) {}
TickCaptureAlgo::~TickCaptureAlgo() {}

const std::string& TickCaptureAlgo::name()
{
    static const std::string key("tick_capture");
    return key;
}
