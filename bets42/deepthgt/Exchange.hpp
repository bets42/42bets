#ifndef BETS42_DEEPTHGT_EXCHANGE_HPP
#define BETS42_DEEPTHGT_EXCHANGE_HPP

#include "bets42/deepthgt/CommandHandler.hpp"
#include "bets42/deepthgt/SOAP.hpp"
#include <bets42/arthur/abstract_factory.hpp>
#include <bets42/arthur/basic_lockable.hpp>
#include <bets42/marvin/types.hpp>
#include <memory>
#include <mutex>
#include <ostream>
#include <list>
#include <set>
#include <string>

namespace bets42
{
	namespace deepthgt
	{
        namespace detail
        {
            class LoginStatus : public arthur::basic_lockable<std::mutex>
            {
                public:
                    enum Status { LOGGED_OUT, LOGGING_IN, LOGGED_IN, LOGGING_OUT };

                public:
                    explicit LoginStatus(const Status status);
                    LoginStatus& operator=(Status copy);

                    bool isLoggedIn() const;
                    bool isLoggedOut() const;

                    Status status() const;

                private:
                    Status status_;
            };

            bool operator==(const LoginStatus& lhs, const LoginStatus& rhs);
            std::ostream& operator<<(std::ostream& stream, const LoginStatus& status);

            typedef arthur::basic_lockable_type<std::list<SOAPClient::Request>, boost::detail::spinlock> RequestQueue;
        }

		class Exchange
		{
			public:
				explicit Exchange(const marvin::exch_id exchID);
				virtual ~Exchange();

				// general
				marvin::exch_id id() const
				{
					return id_;
				}
				const std::string name() const
				{
					return name_;
				}

				// threading
				virtual void run() = 0;
				virtual void stop() = 0;

			private:
				const marvin::exch_id   id_;
				const std::string       name_;
		};

		bool operator<(const Exchange& lhs, const Exchange& rhs);

		typedef std::set<std::unique_ptr<Exchange>> ExchangeSet;
		typedef arthur::static_abstract_factory<Exchange, CommandHandler::Registrar&> ExchangeFactory;
	}
}

#endif  // BETS42_DEEPTHGT_EXCHANGE_HPP
