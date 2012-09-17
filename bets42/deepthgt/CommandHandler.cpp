#include "bets42/deepthgt/CommandHandler.hpp"
#include <glog/logging.h>
#include <algorithm>
#include <iterator>

using namespace bets42::deepthgt;

namespace 
{
    const char* const COMMAND_HANDLER_ENTRY("Creating CommandHandler");
    const char* const COMMAND_HANDLER_EXIT("Destroying CommandHandler");
}

/* -- Command -- */

detail::Command::Command(const std::string& name, 
                         const boost::program_options::options_description& options, 
                         const std::vector<std::string>& args)
    : name_(name)
{
    boost::program_options::store(boost::program_options::command_line_parser(args).options(options).run(), args_);
    boost::program_options::notify(args_);
}


/* -- CommandRegistrar -- */

detail::CommandRegistrar::CommandRegistrar(CommandHandler& handler)
    : handler_(handler) {}

bool detail::CommandRegistrar::registerCommand(const std::string& component,
                                               const std::string& command,
                                               const boost::program_options::options_description& options,
                                               CommandCallback& callback)
{
    std::lock_guard<std::mutex> lock(handler_.registryMutex_);

    auto componentIter(handler_.registry_.find(component));
    if(componentIter == std::end(handler_.registry_))
    {
        componentIter = handler_.registry_.emplace(component, typename CommandHandler::CommandRegistry::mapped_type()).first;
    }

    const CommandHandler::RegistryValue value { options, callback };
    const auto commandIter(componentIter->second.emplace(component, value));

    if(commandIter.second)
    {
        LOG(INFO) 
            << "Inserted component '" 
            << component << "' and command '" << command << "'";
    }
    else
    {
        LOG(WARNING) 
            << "Can't insert duplicate, component '" 
            << component << "' and command '" << command << "'";
    }

    return commandIter.second;
}


/* -- CommandRegistrar -- */

CommandHandler::CommandHandler(const unsigned short port)
    : entryExit_(COMMAND_HANDLER_ENTRY, COMMAND_HANDLER_EXIT)
    , socket_(port, *this)
    , registrar_(*this) {}

std::string CommandHandler::onMessage(const std::string& msg)
{
    std::string response;

    //split msg into tokens
    std::vector<std::string> tokens;
    std::istringstream stream(msg);
    std::copy(
            std::istream_iterator<std::string>(stream),
            std::istream_iterator<std::string>(), 
            std::back_inserter<std::vector<std::string>>(tokens));

    //component and command are minimum requirement for a command msg
    if(tokens.size() < 2)
    {
        response = "Invalid command, must provide both component and command: " + msg;
    }
    else
    {
        //extract component and command then remove from token set
        const std::string component(tokens[0]);
        const std::string command(tokens[1]);
        tokens.erase(tokens.begin(), tokens.begin()+1);

        std::lock_guard<std::mutex> lock(registryMutex_);

        //lookup set of commands for module
        const auto componentIter(registry_.find(component));
        if(componentIter == std::end(registry_))
        {
            response = "Unrecognised component: " + msg;
        }
        else
        {
            //find command options (for parsing args) and callback
            const auto commandIter(componentIter->second.find(command));
            if(commandIter == std::end(componentIter->second))
            {
                response = "Unrecognised command: " + msg;
            }
            else
            {
                //callback with cmd then return response to socket
                const Command cmd(command, commandIter->second.options, tokens);
                response = commandIter->second.callback.onCommand(cmd);
            }
        }
    }

    return response;
}

