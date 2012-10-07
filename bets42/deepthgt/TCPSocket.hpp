#ifndef BETS42_DEEPTHGT_TCP_SOCKET_HPP
#define BETS42_DEEPTHGT_TCP_SOCKET_HPP

#include <bets42/arthur/entry_exit.hpp>
#include <boost/asio.hpp>
#include <boost/utility.hpp>
#include <functional>
#include <map>
#include <string>

namespace bets42
{
	namespace deepthgt
	{
		class TCPSocket : private boost::noncopyable
		{
			public:
				typedef std::function<std::string(const std::string&)> Callback;

			public:
				TCPSocket(const unsigned short port, Callback callback);

				void listen();
				void stop();

			private:
				bets42::arthur::entry_exit      entryExit_;
				boost::asio::io_service         service_;
				boost::asio::ip::tcp::acceptor  acceptor_;
				Callback                        callback_;
		};
	}
}

#endif  // BETS42_DEEPTHGT_TCP_SOCKET_HPP
