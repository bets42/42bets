#ifndef BETS42_DEEPTHGT_BETFAIR_REQUESTS_HPP
#define BETS42_DEEPTHGT_BETFAIR_REQUESTS_HPP

#include <bets42/deepthgt/SOAP.hpp>
#include <string>

namespace bets42
{
	namespace deepthgt
	{
		namespace betfair
		{
			// session management
            SOAPClient::Request createLoginRequest(
			    const std::string& username,
			    const std::string& password,
			    const unsigned productID);

			SOAPClient::Request createLogoutRequest(
			    const std::string& sessionID);

			SOAPClient::Request createKeepAliveRequest(
			    const std::string& sessionID);

            // reference data
			SOAPClient::Request createGetActiveEventTypesRequest(
			    const std::string& sessionID);
		}
	}
}

#endif  // BETS42_DEEPTHGT_BETFAIR_REQUESTS_HPP
