#ifndef BET42_ARTHUR_ENTRY_EXIT_HPP
#define BET42_ARTHUR_ENTRY_EXIT_HPP

#include <glog/logging.h>
#include <string>

namespace bets42 { namespace arthur {

    class EntryExit
    {
        public:
            EntryExit(const std::string& entryMsg, const std::string& exitMsg) 
                : exitMsg_(exitMsg)
            { 
                LOG(INFO) << entryMsg; 
            }

            ~EntryExit() 
            { 
                LOG(INFO) << exitMsg_; 
            }
        
        private:
            const std::string exitMsg_;
    };

}} //namespace bets::arthur

#endif //BET42_ARTHUR_ENTRY_EXIT_HPP
