#ifndef BETS42_DEEPTHGT_BETFAIR_REQUESTS_HPP
#define BETS42_DEEPTHGT_BETFAIR_REQUESTS_HPP

#include <array>
#include <string>

namespace bets42
{
	namespace deepthgt
	{
		namespace betfair
		{
            struct Request
            {
                explicit Request(const char* const a)
                    : action(a) {}

                const char* const       action;
                std::array<char, 4096>  data;
                std::size_t             size;
            };

			// session management
            Request createLoginRequest(
			    const std::string& username,
			    const std::string& password,
			    const unsigned productID);

			Request createLogoutRequest(
			    const std::string& sessionID);

			Request createKeepAliveRequest(
			    const std::string& sessionID);
		}
	}
}

#endif  // BETS42_DEEPTHGT_BETFAIR_REQUESTS_HPP
