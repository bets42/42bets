#include "bets42/deepthgt/TickCaptureAlgo.hpp"

using namespace bets42::deepthgt;

const std::string& TickCaptureAlgo::registrar_key()
{
    static const std::string key("tick_capture");
    return key;
}
