#ifndef BETS42_DEEPTHGT_BETFAIR_BETFAIR_EXCHANGE_HPP
#define BETS42_DEEPTHGT_BETFAIR_BETFAIR_EXCHANGE_HPP

#include <bets42/deepthgt/betfair/Requests.hpp>
#include <bets42/deepthgt/CommandHandler.hpp>
#include <bets42/deepthgt/Exchange.hpp>
#include <bets42/deepthgt/SOAP.hpp>
#include <queue>
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

                    // response handlers
                    void onResponseLogin(const pugi::xml_document& response);
                    void onResponseLogout(const pugi::xml_document& response);

                    // utils
                    bool isLoggedIn() const;
                    bool isLoggedOut() const;
                    bool sendRequest(const Request& request);

                private:
                    typedef std::queue<Request> RequestQueue;

                    SOAPClient              session_;
					std::string             sessionID_;
					Exchange::LoginStatus   loginStatus_;
                    RequestQueue            requestQueue_;
					bool                    stopped_;

                    friend class SOAPClient;        // http handlers
					friend class CommandHandler;    // command handlers
			};
		}
	}
}

#endif  // BETS42_DEEPTHGT_BETFAIR_BETFAIR_EXCHANGE_HPP
