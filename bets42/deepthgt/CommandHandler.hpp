#ifndef BETS42_DEEPTHGT_COMMAND_HANDLER_HPP
#define BETS42_DEEPTHGT_COMMAND_HANDLER_HPP

#include "bets42/arthur/entry_exit.hpp"
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/utility.hpp>
#include <map>
#include <thread>

namespace bets42 { namespace deepthgt {

    namespace detail {

        struct TCPSocketCallback
        {
            virtual ~TCPSocketCallback() {}
            virtual void onMessage(const std::string& msg) = 0;
        };

        class TCPSocket : private boost::noncopyable
        {
            public:
                TCPSocket(const unsigned short port, TCPSocketCallback& callback);
                ~TCPSocket();

            private:
                void listen();

            private:
                bets42::arthur::EntryExit       entryExit_;

                boost::asio::io_service         service_;
                boost::asio::ip::tcp::acceptor  acceptor_;
                boost::asio::ip::tcp::socket    socket_;
                
                std::thread                     thread_;
                TCPSocketCallback&              callback_;
        };
 
        struct CommandCallback;
     
        struct Command
        {
            std::string command;
            boost::program_options::options_description options;
            CommandCallback& callback;
        };
               
        struct CommandCallback
        {
            virtual ~CommandCallback() {}
            virtual void onCommand(const Command& command) = 0;
        };
           
    } //namespace detail

    class CommandHandler : public detail::TCPSocketCallback,
                           private boost::noncopyable
    {
        public:
            typedef detail::Command         Command;
            typedef detail::CommandCallback Callback;
        
        public:
            explicit CommandHandler(const unsigned short port);
            virtual ~CommandHandler();

            void onMessage(const std::string& command);
            
            bool registerCommand(const Command& command);

        private:
            typedef std::map<std::string, Command> CommandMap;
            
            bets42::arthur::EntryExit   entryExit_;
            detail::TCPSocket           socket_;
            CommandMap                  cmdMap_;
    };

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_COMMAND_HANDLER_HPP
