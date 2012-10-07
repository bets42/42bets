#ifndef BET42_ARTHUR_ENTRY_EXIT_HPP
#define BET42_ARTHUR_ENTRY_EXIT_HPP

#include <glog/logging.h>
#include <string>

namespace bets42
{
	namespace arthur
	{
		class entry_exit
		{
			public:
				explicit entry_exit(const std::string& name)
					: name_(name)
				{
					LOG(INFO) << "Entering " << name_;
				}

				~entry_exit()
				{
					LOG(INFO) << "Exiting " << name_;
				}

			private:
				const std::string name_;
		};
	}
}

#endif  // BET42_ARTHUR_ENTRY_EXIT_HPP
