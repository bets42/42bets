#ifndef BETS42_DEEPTHGT_ALGO_HPP
#define BETS42_DEEPTHGT_ALGO_HPP

#include "bets42/deepthgt/CommandHandler.hpp"
#include "bets42/deepthgt/Exchange.hpp"
#include <bets42/arthur/abstract_factory.hpp>
#include <string>

namespace bets42
{
	namespace deepthgt
	{

		class Algo
		{
			public:
				explicit Algo(const std::string& name);
				virtual ~Algo();

				//general
				const std::string& name() const
				{
					return name_;
				}

				//threading
				virtual void run() = 0;
				virtual void stop() = 0;

			private:
				const std::string name_;
		};

		typedef arthur::static_abstract_factory<Algo, ExchangeSet&, CommandHandler::Registrar&> AlgoFactory;

	}
}

#endif //BETS42_DEEPTHGT_ALGO_HPP
