#include <bets42/deepthgt/TCPSocket.hpp>
#include <glog/logging.h>
#include <glog/log_severity.h>
#include <string>

using namespace bets42::deepthgt;

TCPSocket::TCPSocket(const unsigned short port, TCPSocket::Callback callback)
	: entryExit_("TCPSocket")
	, acceptor_(service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	, callback_(callback) {}

void TCPSocket::listen()
{
	const auto port(acceptor_.local_endpoint().port());

	while(acceptor_.is_open())
	{
		boost::system::error_code error;

		LOG(INFO) << "Listening on port " << port;

		boost::asio::ip::tcp::socket socket(service_);
		acceptor_.accept(socket, error);

		if(error)
		{
			if(error != boost::asio::error::bad_descriptor)
			{
				LOG(ERROR)
				        << "An error occured while trying to accept a client connection; error="
				        << error.message();

				sleep(1);  // don't want to flood logs
			}
		}
		else
		{
			while(socket.is_open())
			{
				boost::asio::streambuf stream;
				boost::asio::read_until(socket, stream, "\n", error);

				const std::string msg(
				    (std::istreambuf_iterator<char>(&stream)),
				    std::istreambuf_iterator<char>());

				LOG(INFO) << "Received message: " << msg;

				boost::asio::write(
				    socket,
				    boost::asio::buffer(callback_(msg)),
				    error);

				if(error)
				{
					if(error != boost::asio::error::broken_pipe)
					{
						LOG(ERROR)
						        << "Error whilst writing response, closing client connection: "
						        << error.message();
					}

					socket.close();

					sleep(1);  // don't want to flood logs
				}
			}
		}

		LOG(INFO) << "Closed client connection on port " << port;
	}
}

void TCPSocket::stop()
{
	boost::system::error_code error;
	acceptor_.close(error);

	if(error)
	{
		LOG(ERROR) << "Error whilst stopping TCP socket; error=" << error.message();
	}
}

