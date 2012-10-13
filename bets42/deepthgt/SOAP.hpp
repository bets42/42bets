#ifndef BETS42_DEEPTHGT_SOAP_HPP
#define BETS42_DEEPTHGT_SOAP_HPP

#include <bets42/arthur/http.hpp>
#include <pugixml/pugixml.hpp>
#include <array>
#include <functional>
#include <ostream>
#include <string>
#include <unordered_map>

namespace bets42 { namespace deepthgt {

    namespace detail
    {
        struct Request
        {
            explicit Request(const char* const a);

            std::array<char, 64>    action;
            std::array<char, 4096>  data;
            std::size_t             size;

            Request() {}  // for use by STL containers
        };

        std::ostream& operator<<(std::ostream& stream, const Request& request);
    }

    class SOAPClient
    {
        public:
            typedef detail::Request                                 Request;
            typedef std::function<void(const pugi::xml_document&)>  ResponseCallback;

        public:
            SOAPClient(
                const char* const url,
                const char* const responseTypeXPath);

            bool post(const Request& request);

            bool registerCallback(
                const char* responseType,
                ResponseCallback callback);

        private:
            void onResponse(const char* response, const std::size_t size);

        private:
            typedef std::unordered_map<std::string, ResponseCallback>   ResponseCallbackRegistry;

            arthur::http_client         http_;
            const pugi::xpath_query     responseTypeQuery_;
            ResponseCallbackRegistry    registry_;

            friend class arthur::http_client;  // response callback
    };

}}

#endif  // BETS42_DEEPTHGT_SOAP_HPP
