#ifndef BETS42_DEEPTHGT_BETFAIR_REQUESTS_HPP
#define BETS42_DEEPTHGT_BETFAIR_REQUESTS_HPP

#include <ostream>
#include <string>

namespace bets42
{
	namespace deepthgt
	{
		namespace betfair
		{
			// session management
			std::ostream& createLoginRequest(
			    std::ostream& stream,
			    const std::string& username,
			    const std::string& password,
			    const unsigned productID);

			std::ostream& createLogoutRequest(
			    std::ostream& stream,
			    const std::string& sessionID);

			std::ostream& createKeepAliveRequest(
			    std::ostream& stream,
			    const std::string& sessionID);
		}
	}
}

#endif  // BETS42_DEEPTHGT_BETFAIR_REQUESTS_HPP
