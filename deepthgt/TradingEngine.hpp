#ifndef BETS42_DEEPTHGT_TRADAING_ENGINE_HPP
#define BETS42_DEEPTHGT_TRADAING_ENGINE_HPP

#include "bets42/deepthgt/CommandHandler.hpp"
#include <string>

namespace bets42 { namespace deepthgt {

    namespace detail {
                        
        struct Initializer
        {
            Initializer(const std::string& dir, 
                        const std::string& filename, 
                        const std::string& level);

            ~Initializer(); 
        };    

    } //namespace detail

    class TradingEngine : public CommandHandler::Callback
    {
        public:
            TradingEngine(const unsigned short cmdPort,
                          const std::string& logDir,
                          const std::string& logName,
                          const std::string& logLevel);

            void onCommand(const CommandHandler::Command& command);

            void run();

        private:
            void registerCommands();

        private:
            detail::Initializer initializer_;
            CommandHandler      cmdHandler_;
    };

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_TRADAING_ENGINE_HPP
