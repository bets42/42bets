#include "bets42/deepthgt/CommandHandler.hpp"
#include <boost/algorithm/string/case_conv.hpp>
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
                                               CommandCallback callback)
{
    std::lock_guard<std::mutex> lock(handler_.registryMutex_);

    const CommandHandler::RegistryValue value { options, callback };
    const auto result(handler_.registry_[component].emplace(command, value));

    if(result.second)
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

    return result.second;
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

    //process request - only send usage() when asked, otherwise it's an error
    if(tokens.size() == 1 && tokens[0] == "help")
    {
        response = usage();
    }
    else if(tokens.size() > 1)
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
            response = "Unrecognised component, see help command";
        }
        else
        {
            //find command options (for parsing args) and callback
            const auto commandIter(componentIter->second.find(command));
            if(commandIter == std::end(componentIter->second))
            {
                response = "Unrecognised command, see help command";
            }
            else
            {
                //callback with cmd then return response to socket
                const Command cmd(command, commandIter->second.options, tokens);
                response = commandIter->second.callback(cmd);
            }
        }   
    }
    else
    {
        response = "Invalid request, see help command";
    }

    return response;
}

std::string CommandHandler::usage() const
{
    std::stringstream stream;
    stream << usageImpl() << '\n';
    return stream.str();
}

std::string CommandHandler::usage(const std::string& component) const
{
    std::stringstream stream;
    stream << usageImpl(component) << '\n';
    return stream.str();
}

boost::program_options::options_description CommandHandler::usageImpl() const
{
    boost::program_options::options_description usage(
        "The DeepThought framework is structured as a set of modules, each of\n"
        "which can be independently administared via the set of commands it makes\n"
        "available. To invoke a command the following form should be used:\n\n"
        "   <component> <command> [options...]\n\n"
        "Following is a list of the available commands, split by component");

    for(const auto& component : registry_)
    {
        usage.add(usageImpl(component.first));
    }

    return usage;
}

boost::program_options::options_description CommandHandler::usageImpl(const std::string& component) const
{
    const std::string usageStr("\n" + component + " COMMANDS");
    boost::program_options::options_description usage(boost::to_upper_copy(usageStr));

    const auto iter(registry_.find(component));
    if(iter != std::end(registry_))
    {
        for(const auto& command : iter->second)
        {
            usage.add(command.second.options);   
        }
    }

    return usage;
}

