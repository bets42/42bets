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
    , socket_(service_)
    , doListen_(true)
    , thread_(&TCPSocket::listen, this)
    , callback_(callback) {}

TCPSocket::~TCPSocket()
{
    doListen_ = false;

    //TODO: this doesn't clean up properly
    boost::system::error_code error;
    socket_.close(error);
    if(error) 
    {
        LOG(WARNING) << "Error whilst closing TCP socket; error=" << error.message();
    }

    thread_.join();
}  

void TCPSocket::listen()
{  
    while(doListen_)
    {
        LOG(INFO) << "Listening on port " << acceptor_.local_endpoint().port();

        boost::system::error_code error;
        acceptor_.accept(socket_, error);

        if(error)
        {
            LOG(WARNING) 
                << "An error occured while trying to accept a client connection; error=" 
                << error.message();
        }
        else
        {
            do
            {
                boost::asio::streambuf stream;
                boost::asio::read_until(socket_, stream, '\n', error);

                const std::string msg(
                    (std::istreambuf_iterator<char>(&stream)), 
                    std::istreambuf_iterator<char>());

                LOG(INFO) << "Received message: " << msg;

                const std::string response(callback_.onMessage(msg));

                LOG(INFO) << "Sending reponse: " << response;

                boost::asio::write(socket_, boost::asio::buffer(response), error);

                if(error)
                {
                    //TODO: this recovery mechanism doesn't work
                    LOG(WARNING) << "Error whilst writing response, closing client connection: " << error.message();
                    acceptor_.close();
                }
            }
            while(acceptor_.is_open());
        }

        LOG(INFO) << "Closed client connection on port " << acceptor_.local_endpoint().port(); 
    }
}          
