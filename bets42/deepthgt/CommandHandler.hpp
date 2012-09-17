#ifndef BETS42_DEEPTHGT_COMMAND_HANDLER_HPP
#define BETS42_DEEPTHGT_COMMAND_HANDLER_HPP

#include "bets42/arthur/entry_exit.hpp"
#include "bets42/deepthgt/TCPSocket.hpp"
#include <boost/program_options.hpp>
#include <boost/utility.hpp>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace bets42 { namespace deepthgt {

    class CommandHandler;

    namespace detail 
    {
        class Command
        {
            public:
                Command(const std::string& name, 
                        const boost::program_options::options_description& options, 
                        const std::vector<std::string>& args);

                const std::string& name() const { return name_; }
                const boost::program_options::variables_map args() const { return args_; }

            private:
                const std::string                       name_;
                boost::program_options::variables_map   args_;
        };

        struct CommandCallback
        {
            virtual ~CommandCallback() {}
            virtual std::string onCommand(const Command& command) = 0;
        };

        class CommandRegistrar
        {
            public:
                explicit CommandRegistrar(CommandHandler& handler);

                bool registerCommand(
                    const std::string& component,
                    const std::string& command,
                    const boost::program_options::options_description& options,
                    CommandCallback& callback);

            private:
                CommandHandler& handler_;
        };
    }


    class CommandHandler : public TCPSocket::Callback,
                           private boost::noncopyable
    {
        public:
            typedef detail::Command             Command;
            typedef detail::CommandCallback     Callback;
            typedef detail::CommandRegistrar    Registrar;

        public:
            explicit CommandHandler(const unsigned short port);

            std::string onMessage(const std::string& msg);

            Registrar& registrar() { return registrar_; }

        private:
            struct RegistryValue { boost::program_options::options_description options; Callback& callback; };
            typedef std::map<std::string, std::map<std::string, RegistryValue>> CommandRegistry;

            arthur::entry_exit          entryExit_;
            TCPSocket                   socket_;
            CommandRegistry             registry_;
            detail::CommandRegistrar    registrar_;

            mutable std::mutex  registryMutex_;

            friend class detail::CommandRegistrar;
    };

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_COMMAND_HANDLER_HPP
