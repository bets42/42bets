#ifndef BETS42_DEEPTHGT_BETFAIR_BETFAIR_EXCHANGE_HPP
#define BETS42_DEEPTHGT_BETFAIR_BETFAIR_EXCHANGE_HPP

#include <bets42/deepthgt/betfair/Requests.hpp>
#include <bets42/deepthgt/CommandHandler.hpp>
#include <bets42/deepthgt/Exchange.hpp>
#include <bets42/deepthgt/SOAP.hpp>
#include <atomic>
#include <ostream>
#include <string>

namespace pugi { class xml_document; }

namespace bets42
{
	namespace deepthgt
	{
		namespace betfair
		{
			class BetfairExchange : public Exchange
				                  , private deepthgt::ExchangeFactory::registrant<BetfairExchange>
			{
				public:
					explicit BetfairExchange(deepthgt::CommandHandler::Registrar& cmdRegistrar);
					virtual ~BetfairExchange();

					// exchange factory
					static const std::string& registrant_key();

					// threading
					void run();
					void stop();

				private:
					// command handlers
					std::string onRequestLogin(const CommandHandler::Command& command);
					std::string onRequestLogout(const CommandHandler::Command& command);
					std::string onRequestStatus(const CommandHandler::Command& command);
                    std::string onRequestGetActiveEventTypes(const CommandHandler::Command& command);

                    // response handlers
                    void onResponseLogin(const pugi::xml_document& response);
                    void onResponseLogout(const pugi::xml_document& response);
                    void onResponseGetActiveEventTypes(const pugi::xml_document& response);

                    // request rocessing
                    bool sendRequest(const SOAPClient::Request& request);
                    std::ostream& enqueueRequest(
                        SOAPClient::Request&& request,
                        const CommandHandler::Command& command,
                        std::ostream& stream);

                private:
                    typedef deepthgt::detail::LoginStatus   LoginStatus;
                    typedef deepthgt::detail::RequestQueue  RequestQueue;

                    SOAPClient                  session_;
					std::string                 sessionID_;
					LoginStatus                 loginStatus_;

                    RequestQueue                requestQueue_;
                    std::chrono::milliseconds   requestInterval_;

					std::atomic<bool>           stopped_;

                    friend class SOAPClient;        // http handlers
					friend class CommandHandler;    // command handlers
			};
		}
	}
}

#endif  // BETS42_DEEPTHGT_BETFAIR_BETFAIR_EXCHANGE_HPP
