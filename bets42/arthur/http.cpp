#include <bets42/arthur/http.hpp>
#include <glog/logging.h>

using namespace bets42::arthur;

// TODO:
// 1. externalise certificates path

// curl_initializer
detail::curl_initializer::curl_initializer()
{
    curl_global_init(CURL_GLOBAL_SSL);
}

detail::curl_initializer::~curl_initializer()
{
    curl_global_cleanup();
}

const detail::curl_initializer http_client::curl_init_;


// http response handler -
// libcurl is a C implementation and so we can't pass a pointer to
// an http_client memeber function.
std::size_t detail::on_response(
    const char* response,
    const std::size_t size,
    const std::size_t nmemb,
    http_client* client)
{
    LOG(INFO) << "Callback";
    return client->on_response_impl(response, size*nmemb);
}


// http_client
http_client::http_client(const char* const url, callback_type callback)
    : curl_(curl_easy_init())
    , callback_(callback)
{
    curl_easy_setopt(curl_, CURLOPT_URL, url);
    curl_easy_setopt(curl_, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl_, CURLOPT_NOPROGRESS  ,1L);
    curl_easy_setopt(curl_, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_easy_setopt(curl_, CURLOPT_ERRORBUFFER, error_);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, detail::on_response);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl_, CURLOPT_CAINFO, "data/cacert.pem");
    curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 1L);
}

http_client::~http_client()
{
    curl_easy_cleanup(curl_);
}

bool http_client::post_soap(
    const char* const action,
    const char* const request,
    const std::size_t request_size)
{
    char header_action[64] = "SOAPAction:";
    std::strncat(header_action + 11, action, 64 - 11);

    struct curl_slist* header(nullptr);
    header = curl_slist_append(header, "Content-Type:text/xml");     //;charset=UTF-8
    header = curl_slist_append(header, header_action);

    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, header);
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, request);
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, request_size);

    const CURLcode result(curl_easy_perform(curl_));
    const bool success(result == CURLE_OK);

    if(!success)
    {
        LOG(WARNING)
            << "Error whilst sending HTTP request;"
            << " code=" << result
            << " code_str=" << curl_easy_strerror(result)
            << " error=" << error_;
    }

    curl_slist_free_all(header);

    return success;
}

std::size_t http_client::on_response_impl(const char* response, const std::size_t response_size)
{
    callback_(response, response_size);
    return response_size;
}
