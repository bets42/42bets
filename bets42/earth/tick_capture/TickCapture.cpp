#include <bets42/earth/tick_capture/TickCapture.hpp>
#include <glog/logging.h>
#include <string>

using namespace bets42::earth::tick_capture;

TickCapture::TickCapture(deepthgt::ExchangeSet& exchanges,
                         deepthgt::CommandHandler::Registrar& cmdRegistrar)
	: Algo("TickCapture")
	, exchanges_(exchanges)
	, stopped_(false) {}

TickCapture::~TickCapture() {}

const std::string& TickCapture::registrant_key()
{
	static const std::string key("tick_capture");
	return key;
}

void TickCapture::run()
{
	LOG(INFO) << "Running '" << name() << "' algo";

	while(! stopped_)
	{
		sleep(1);
	}

	LOG(INFO) << "Stopped '" << name() << "' algo";
}

void TickCapture::stop()
{
	LOG(INFO) << "Proceeding to stop '" << name() << "' algo";
	stopped_ = true;
}
