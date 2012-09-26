#ifndef BETS42_DEEPTHGT_COMMAND_HANDLER_HPP
#define BETS42_DEEPTHGT_COMMAND_HANDLER_HPP

#include "bets42/deepthgt/TCPSocket.hpp"
#include <bets42/arthur/entry_exit.hpp>
#include <boost/program_options.hpp>
#include <boost/utility.hpp>
#include <functional>
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
                const boost::program_options::options_description options() const { return options_; }
                const boost::program_options::variables_map args() const { return args_; }

            private:
                const std::string                                   name_;
                const boost::program_options::options_description   options_;
                boost::program_options::variables_map               args_;
        };

        typedef std::function<std::string(const Command&)>  CommandCallback;

        class CommandRegistrar
        {
            public:
                explicit CommandRegistrar(CommandHandler& handler);

                bool registerCommand(
                    const std::string& component,
                    const std::string& command,
                    const boost::program_options::options_description& options,
                    CommandCallback callback);

            private:
                CommandHandler& handler_;
        };
    }

    class CommandHandler : private boost::noncopyable
    {
        public:
            typedef detail::Command             Command;
            typedef detail::CommandCallback     Callback;
            typedef detail::CommandRegistrar    Registrar;

        public:
            explicit CommandHandler(const unsigned short port);
            ~CommandHandler();

            Registrar& registrar() { return registrar_; }

            std::string usage() const;
            std::string usage(const std::string& component) const;

            //threading
            void run();
            void stop();

        private:
            //socket handler
            std::string onSocketMessage(const std::string& msg);

            boost::program_options::options_description usageImpl() const;
            boost::program_options::options_description usageImpl(const std::string& component) const;

        private:
            struct RegistryValue { boost::program_options::options_description options; Callback callback; };
            typedef std::map<std::string, std::map<std::string, RegistryValue>> CommandRegistry;

            arthur::entry_exit          entryExit_;
            TCPSocket                   socket_;
            CommandRegistry             registry_;
            detail::CommandRegistrar    registrar_;

            mutable std::mutex          registryMutex_;

            friend class TCPSocket; //socket callbacks
            friend class detail::CommandRegistrar; //access to registry
    };

}}

#endif //BETS42_DEEPTHGT_COMMAND_HANDLER_HPP
