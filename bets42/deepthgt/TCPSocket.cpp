#include "bets42/deepthgt/TCPSocket.hpp"
#include <glog/logging.h>
#include <array>

using namespace bets42::deepthgt;

namespace 
{
    const char* const TCP_SOCKET_ENTRY("Creating TCPSocket");
    const char* const TCP_SOCKET_EXIT("Destroying TCPSocket");     
}

TCPSocket::TCPSocket(const unsigned short port, TCPSocket::Callback& callback)
    : entryExit_(TCP_SOCKET_ENTRY, TCP_SOCKET_EXIT)
    , acceptor_(service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    , thread_(&TCPSocket::listen, this)
    , callback_(callback) {}

TCPSocket::~TCPSocket()
{
    boost::system::error_code error;
    acceptor_.close(error);
    if(error) 
    {
        LOG(WARNING) << "Error whilst closing TCP socket; error=" << error.message();
    }

    thread_.join();
}  

void TCPSocket::listen()
{  
    const auto port(acceptor_.local_endpoint().port());

    for(;;)
    {
        boost::system::error_code error;

        LOG(INFO) << "Listening on port " << port;

        boost::asio::ip::tcp::socket socket_(service_);
        acceptor_.accept(socket_, error);

        if(error)
        {
            LOG(WARNING) 
                << "An error occured while trying to accept a client connection; error=" 
                << error.message();
        }
        else
        {
            while(acceptor_.is_open())
            {
                boost::asio::streambuf stream;
                boost::asio::read_until(socket_, stream, '\n', error);

                const std::string msg(
                    (std::istreambuf_iterator<char>(&stream)), 
                    std::istreambuf_iterator<char>());

                LOG(INFO) << "Received message: " << msg;

                const std::string response("hello)back");//callback_.onMessage(msg));

                LOG(INFO) << "Sending reponse: " << response;

                boost::asio::write(socket_, boost::asio::buffer(response), error);

                //TODO: this recovery mechanism isn't complete
                if(error)
                {
                    if(error == boost::asio::error::broken_pipe)
                    {
                        LOG(INFO) << "Client disconnected";
                    }
                    else if(error)
                    {
                        LOG(WARNING) << "Error whilst writing response, closing client connection: " << error.message();
                    }

                    acceptor_.close();
                }
            }
        }

        LOG(INFO) << "Closed client connection on port " << port; 
    }
}          
