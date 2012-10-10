#ifndef BETS42_DEEPTHGT_BETFAIR_BETFAIR_EXCHANGE_HPP
#define BETS42_DEEPTHGT_BETFAIR_BETFAIR_EXCHANGE_HPP

#include <bets42/arthur/http.hpp>
#include <bets42/deepthgt/CommandHandler.hpp>
#include <bets42/deepthgt/Exchange.hpp>
#include <string>
#include <unordered_map>

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

					// factory
					static const std::string& registrant_key();

					// threading
					void run();
					void stop();

				private:
					// command handlers
					std::string onLogin(const CommandHandler::Command& command);
					std::string onLogout(const CommandHandler::Command& command);

                    // response handlers
                    void onResponse(const char* response, const std::size_t size);
                    void onResponseLogin(const pugi::xml_document& response);
                    void onResponseLogout(const pugi::xml_document& response);

                private:
                    typedef std::function<void(const pugi::xml_document&)>      ResponseCallback;
                    typedef std::unordered_map<std::string, ResponseCallback>   ResponseCallbackMap;

                    arthur::http_client     client_;
					Exchange::LoginStatus   loginStatus_;
                    ResponseCallbackMap     responseCallbacks_;
					std::string             sessionID_;
					bool                    stopped_;

                    friend class arthur::http_client;   // http handlers
					friend class CommandHandler;        // command handlers
			};
		}
	}
}

#endif  // BETS42_DEEPTHGT_BETFAIR_BETFAIR_EXCHANGE_HPP
