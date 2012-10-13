#include <bets42/deepthgt/Exchange.hpp>

using namespace bets42::deepthgt;

// LoginStatus
detail::LoginStatus::LoginStatus(const Status status)
    : status_(status) {}

detail::LoginStatus& detail::LoginStatus::operator=(Status status)
{
    status_ = status;
    return *this;
}

bool detail::LoginStatus::isLoggedIn() const
{
    return status_ == LOGGED_IN;
}

bool detail::LoginStatus::isLoggedOut() const
{
    return status_ == LOGGED_OUT;
}

detail::LoginStatus::Status detail::LoginStatus::status() const
{
    return status_;
}

bool bets42::deepthgt::detail::operator==(const LoginStatus& lhs, const LoginStatus& rhs)
{
    return lhs.status() == rhs.status();
}

std::ostream& bets42::deepthgt::detail::operator<<(std::ostream& stream, const LoginStatus& status)
{
    switch(status.status())
    {
        case LoginStatus::LOGGED_OUT:
        {
            stream << "LOGGED_OUT";
            break;
        }
        case LoginStatus::LOGGING_IN:
        {
            stream << "LOGGING_IN";
            break;
        }
        case LoginStatus::LOGGED_IN:
        {
            stream << "LOGGED_IN";
            break;
        }
        case LoginStatus::LOGGING_OUT:
        {
            stream << "LOGGING_OUT";
            break;
        }
    }

    return stream;
}

// Exchange
Exchange::Exchange(const marvin::exch_id id)
	: id_(id)
	, name_(marvin::to_string(id)) {}

Exchange::~Exchange() {}

bool bets42::deepthgt::operator<(const Exchange& lhs, const Exchange& rhs)
{
	return lhs.id() < rhs.id();
}



