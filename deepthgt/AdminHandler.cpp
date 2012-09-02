#include "bets42/deepthgt/AdminHandler.hpp"
#include "bets42/deepthgt/Exception.hpp"
#include <boost/asio.hpp>
#include <boost/throw_exception.hpp>
#include <functional>

using namespace bets42::deepthgt;

AdminHandler::AdminHandler(const unsigned short port)
    : doHandle_(false),
      port_(port) {}

AdminHandler::~AdminHandler()
{
    stop();
}

void AdminHandler::start()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if( ! thread_.joinable())
    {
        doHandle_ = true;
        thread_ = std::move(std::thread(&AdminHandler::handle, this));
    } 
}

void AdminHandler::stop()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(thread_.joinable())
    {
        doHandle_ = false;
        thread_.join();
    }
}

void AdminHandler::handle()
{
    try
    {
        using boost::asio::ip::tcp;

        boost::asio::io_service service;
        tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), port_));

        do
        {
            tcp::socket socket(service);
            acceptor.accept(socket);

            boost::system::error_code error;
            boost::asio::write(socket, boost::asio::buffer("hello world!"), error);
        } 
        while(doHandle_); 
    }
    catch(const std::exception& e)
    {
        boost::throw_exception(
                Exception(__FILE__, __LINE__) 
                << "Failed to start or continue to listen on admin port " << port_
                << "; exception: " << e.what()
                );
    } 
}
