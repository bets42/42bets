#ifndef BETS42_ARTHUR_HTTP_HPP
#define BETS42_ARTHUR_HTTP_HPP

#include <curl/curl.h>
#include <functional>

namespace bets42 { namespace arthur {

    class http_client;

    namespace detail
    {
        struct curl_initializer
        {
            curl_initializer();
            ~curl_initializer();
        };

        typedef std::function<void(const char*, const std::size_t)>  callback_type;
    }

    class http_client
    {
        public:
            typedef detail::callback_type callback_type;

        public:
            explicit http_client(const char* const url, callback_type callback);
            ~http_client();

            bool post(
                const char* const header,
                const char* const request,
                const std::size_t request_size);

        private:
            CURL*           curl_;
            char            error_[CURL_ERROR_SIZE];
            callback_type   callback_;

            static const detail::curl_initializer curl_init_;
    };

}}

#endif  // BETS42_ARTHUR_HTTP_HPP
