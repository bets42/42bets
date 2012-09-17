#ifndef BET42_ARTHUR_ENTRY_EXIT_HPP
#define BET42_ARTHUR_ENTRY_EXIT_HPP

#include <glog/logging.h>
#include <string>

namespace bets42 { namespace arthur {

    class entry_exit
    {
        public:
            entry_exit(const std::string& entry_msg, const std::string& exit_msg) 
                : exit_msg_(exit_msg)
            { 
                LOG(INFO) << entry_msg; 
            }

            ~entry_exit() 
            { 
                LOG(INFO) << exit_msg_; 
            }
        
        private:
            const std::string exit_msg_;
    };

}}

#endif //BET42_ARTHUR_ENTRY_EXIT_HPP
