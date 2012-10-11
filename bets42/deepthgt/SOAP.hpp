#ifndef BETS42_DEEPTHGT_SOAP_HPP
#define BETS42_DEEPTHGT_SOAP_HPP

#include <bets42/arthur/http.hpp>
#include <pugixml/pugixml.hpp>
#include <functional>
#include <string>
#include <unordered_map>

namespace bets42 { namespace deepthgt {

    class SOAPClient
    {
        public:
            typedef std::function<void(const pugi::xml_document&)>  ResponseCallback;

        public:
            SOAPClient(
                const char* const url,
                const char* const responseTypeXPath);

            bool post(
                const char* const action,
                const char* const request,
                const std::size_t request_size);

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
