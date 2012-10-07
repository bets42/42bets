#ifndef BETS42_EARTH_TICK_CAPTURE_TICK_CAPTURE_HPP
#define BETS42_EARTH_TICK_CAPTURE_TICK_CAPTURE_HPP

#include <bets42/deepthgt/Algo.hpp>
#include <bets42/deepthgt/CommandHandler.hpp>
#include <bets42/deepthgt/Exchange.hpp>
#include <string>

namespace bets42
{
	namespace earth
	{
		namespace tick_capture
		{

			class TickCapture : public deepthgt::Algo
				, private deepthgt::AlgoFactory::registrant<TickCapture>
			{
				public:
					TickCapture(
					    deepthgt::ExchangeSet& exchanges,
					    deepthgt::CommandHandler::Registrar& cmdRegistrar);

					virtual ~TickCapture();

					//factory
					static const std::string& registrant_key();

					//threading
					void run();
					void stop();

				private:
					deepthgt::ExchangeSet&  exchanges_;
					bool                    stopped_;
			};

		}
	}
}

#endif //BETS42_EARTH_TICK_CAPTURE_TICK_CAPTURE_HPP   
