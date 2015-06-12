#ifndef EASYHANDLE_HXX
#define EASYHANDLE_HXX

#include <string>
#include <cstdlib>
#include <functional>

#include <curl/curl.h>

#include "http_response.hxx"

class HTTPRequest
{
    public:
        HTTPRequest(std::string uri);
        ~HTTPRequest();
        HTTPResponse perform();
        void addHeader(std::string header);

        HTTPRequest(HTTPRequest const & rhs) = delete;
        HTTPRequest& operator = (HTTPRequest const & rhs) = delete;
    private:
        CURL* m_curl;
        curl_slist * m_headers;
        std::string m_uri;
        std::string m_response_body;
};

#endif // EASYHANDLE_HXX
