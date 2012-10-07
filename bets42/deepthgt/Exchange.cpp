#include <bets42/deepthgt/Exchange.hpp>

using namespace bets42::deepthgt;

Exchange::Exchange(const marvin::exch_id id)
	: id_(id)
	, name_(marvin::to_string(id)) {}

Exchange::~Exchange() {}

bool bets42::deepthgt::operator<(const Exchange& lhs, const Exchange& rhs)
{
	return lhs.id() < rhs.id();
}

