#ifndef HTTPREQUEST_HXX
#define HTTPREQUEST_HXX

#include <string>
#include <cstdlib>
#include <utility>

#include <curl/curl.h>

#include "http_response.hxx"
#include "curl_handle.hxx"

class HTTPRequest
{
    public:
        HTTPRequest(std::string uri);
        ~HTTPRequest();

        HTTPResponse perform();
        void addHeader(std::string const & header);
        inline void changeURI(std::string new_uri) {m_uri = std::move(new_uri);}
        void clearHeaders();

        HTTPRequest(HTTPRequest const & rhs) = delete;
        HTTPRequest& operator = (HTTPRequest const & rhs) = delete;
    private:
        CurlHandle m_curl;
        curl_slist * m_headers;
        std::string m_uri;
        std::string m_response_body;
};

#endif // HTTPREQUEST_HXX
