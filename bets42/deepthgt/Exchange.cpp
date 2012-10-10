#include <bets42/deepthgt/Exchange.hpp>

using namespace bets42::deepthgt;

Exchange::Exchange(const marvin::exch_id id)
	: id_(id)
	, name_(marvin::to_string(id)) {}

Exchange::~Exchange() {}

const std::string& Exchange::asString(const LoginStatus loginStatus) const
{
    switch(loginStatus)
    {
        case LoginStatus::LOGGED_OUT:
        {
            static const std::string str("LOGGED_OUT");
            return str;
        }
        case LoginStatus::LOGGING_IN:
        {
            static const std::string str("LOGGING_OUT");
            return str;
        }
        case LoginStatus::LOGGED_IN:
        {
            static const std::string str("LOGGED_IN");
            return str;
        }
        case LoginStatus::LOGGING_OUT:
        {
            static const std::string str("LOGGING_OUT");
            return str;
        }
    }
}

bool bets42::deepthgt::operator<(const Exchange& lhs, const Exchange& rhs)
{
	return lhs.id() < rhs.id();
}

