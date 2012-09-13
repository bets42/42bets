#include "bets42/deepthgt/CommandHandler.hpp"
#include "bets42/deepthgt/Exception.hpp"
#include <boost/throw_exception.hpp>
#include <glog/logging.h>
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace bets42::deepthgt;

namespace {

    const char* const TCP_SOCKET_ENTRY("Creating TCPSocket");
    const char* const TCP_SOCKET_EXIT("Destroying TCPSocket");
    const char* const COMMAND_HANDLER_ENTRY("Creating CommandHandler");
    const char* const COMMAND_HANDLER_EXIT("Destroying CommandHandler");
    
    template <std::size_t N>
    class TCPSocketReadCompleteCallback
    {
        public:
            explicit TCPSocketReadCompleteCallback(std::array<char, N>& buffer)
                : buffer_(buffer) {}

            std::size_t operator()(const boost::system::error_code& error, 
                                   const std::size_t bytes_transferred) const
            {
                return !(error || buffer_[bytes_transferred] == '\r');
            }

        private:
            std::array<char, N>& buffer_; 
    };
             
} //namespace annonymous

/**
 * TCPSocket
 */
detail::TCPSocket::TCPSocket(const unsigned short port, 
                             TCPSocketCallback& callback)
    : entryExit_(TCP_SOCKET_ENTRY, TCP_SOCKET_EXIT),
      acceptor_(service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      socket_(service_),
      thread_(&TCPSocket::listen, this),
      callback_(callback) {}

detail::TCPSocket::~TCPSocket() 
{
    boost::system::error_code error;
    acceptor_.close(error);
    if(error) {
        LOG(WARNING) << "Error whilst closing TCP socket; error=" << error.message();
    }

    thread_.join();
}

void detail::TCPSocket::listen()
{
    LOG(INFO) << "Listening on port " << acceptor_.local_endpoint().port();

    const std::size_t BUF_LEN = 1024; //look to make use of constexpr when supported
    std::array<char, BUF_LEN> buffer;

    do
    {
        boost::system::error_code error;
        acceptor_.accept(socket_, error);

        if(acceptor_.is_open())
        {
            if(!error)
            {                       
                boost::asio::read(socket_,
                                  boost::asio::mutable_buffers_1(buffer.data(), buffer.size()),
                                  TCPSocketReadCompleteCallback<BUF_LEN>(buffer),
                                  error);

                callback_.onMessage(std::string(buffer.data(), buffer.size()));
            }
            else
            {
                LOG(WARNING) << "An error occured while trying to accept a client connection; error=" 
                             << error.message();
            }
        }
    }
    while(acceptor_.is_open());

    LOG(INFO) << "Finished listening on port " << acceptor_.local_endpoint().port();
}

/**
 * CommandHandler
 */
CommandHandler::CommandHandler(const unsigned short port)
    : entryExit_(COMMAND_HANDLER_ENTRY, COMMAND_HANDLER_EXIT),
      socket_(port, *this) {}

CommandHandler::~CommandHandler() {}

bool CommandHandler::registerCommand(const Command& command)
{
    return cmdMap_.insert(CommandMap::value_type(command.command, command)).second;
}

void CommandHandler::onMessage(const std::string& msg)
{
    std::vector<std::string> tokens;
    std::istringstream stream(msg);
    std::copy(std::istream_iterator<std::string>(stream),
              std::istream_iterator<std::string>(), 
              std::back_inserter<std::vector<std::string>>(tokens));  

    const auto iter(cmdMap_.find(tokens.front()));

    if(iter != std::end(cmdMap_))
    {
        namespace prog_opts = boost::program_options;

        prog_opts::variables_map varsMap;
        try
        {
           // prog_opts::store(prog_opts::parse_command_line(tokens.size(), cmdTokens.data(), iter->second.options), varsMap);
            //prog_opts::notify(varsMap);
        }
        catch(const prog_opts::error& e)
        {
            //TODO:
        }   
    }
}

