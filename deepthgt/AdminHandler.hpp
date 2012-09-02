#ifndef BETS42_DEEPTHGT_ADMIN_HANDLER_HPP
#define BETS42_DEEPTHGT_ADMIN_HANDLER_HPP

#include <boost/utility.hpp>
#include <mutex>
#include <thread>

namespace bets42 { namespace deepthgt {

    class AdminHandler : private boost::noncopyable
    {
         public:
            explicit AdminHandler(const unsigned short port);
            ~AdminHandler();

            void start();
            void stop();

        private:
            void handle();

        private:
            std::thread             thread_;
            mutable std::mutex      mutex_;
            bool                    doHandle_;
            const unsigned short    port_;
    };

}} //namespace bets42::deepthgt

#endif //BETS42_DEEPTHGT_ADMIN_HANDLER_HPP
