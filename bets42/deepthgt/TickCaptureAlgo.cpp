#include "bets42/deepthgt/TickCaptureAlgo.hpp"

using namespace bets42::deepthgt;

TickCaptureAlgo::TickCaptureAlgo(CommandHandler::Registrar& cmdRegistrar) {}
TickCaptureAlgo::~TickCaptureAlgo() {}

const std::string& TickCaptureAlgo::name()
{
    static const std::string key("tick_capture");
    return key;
}
