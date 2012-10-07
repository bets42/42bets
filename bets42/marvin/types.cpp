#include <bets42/marvin/types.hpp>
#include <string>

#define TO_STRING_CASE(value, value_str) case value: { static const std::string str(value_str); return str; }

const std::string& bets42::marvin::to_string(const bets42::marvin::exch_id value)
{
	switch(value)
	{
			TO_STRING_CASE(exch_id::betdaq,     "Betdaq")
			TO_STRING_CASE(exch_id::betfair,    "Betfair")
	}
}
