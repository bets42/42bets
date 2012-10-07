#ifndef BETS42_DEEPTHGT_EXCHANGE_HPP
#define BETS42_DEEPTHGT_EXCHANGE_HPP

#include "bets42/deepthgt/CommandHandler.hpp"
#include <bets42/arthur/abstract_factory.hpp>
#include <bets42/marvin/types.hpp>
#include <memory>
#include <set>
#include <string>

namespace bets42
{
	namespace deepthgt
	{
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
