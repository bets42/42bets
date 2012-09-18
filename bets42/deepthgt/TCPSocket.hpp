#ifndef BETS42_DEEPTHGT_TCP_SOCKET_HPP
#define BETS42_DEEPTHGT_TCP_SOCKET_HPP

#include "bets42/arthur/entry_exit.hpp"
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/utility.hpp>
#include <map>
#include <string>
#include <thread> 

namespace bets42 { namespace deepthgt {

    namespace detail 
    {
        struct TCPSocketCallback
        {
            virtual ~TCPSocketCallback() {}
            virtual std::string onMessage(const std::string& msg) = 0;
        };
    }

    class TCPSocket : private boost::noncopyable
    {
        public:
            typedef detail::TCPSocketCallback Callback;

        public:
            TCPSocket(const unsigned short port, Callback& callback);
            ~TCPSocket();

        private:
            void listen();

        private:
            bets42::arthur::entry_exit      entryExit_;

            boost::asio::io_service         service_;
            boost::asio::ip::tcp::acceptor  acceptor_;

            std::thread                     thread_;
            Callback&                       callback_;
    };

}}

#endif //BETS42_DEEPTHGT_TCP_SOCKET_HPP
