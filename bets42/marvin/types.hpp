#ifndef BETS42_MARVIN_TYPES_HPP
#define BETS42_MARVIN_TYPES_HPP

#include <cstdint>
#include <string>

namespace bets42
{
	namespace marvin
	{
        enum class exch_id : uint8_t
		{
		    betdaq,
		    betfair
		};

		const std::string& to_string(const exch_id value);
	}
}

#endif  // BETS42_MARVIN_TYPES_HPP
