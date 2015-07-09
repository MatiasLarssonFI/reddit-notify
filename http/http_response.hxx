#ifndef HTTPRESPONSE_HXX
#define HTTPRESPONSE_HXX

#include <string>

#include <curl/curl.h>


class HTTPResponse {
    public:
        HTTPResponse(CURLcode curl_code, unsigned http_code, std::string body);
        CURLcode curlCode() const { return m_curl_code; }
        unsigned HTTPCode() const { return m_http_code; }
        std::string const & body() const { return m_body; }
    private:
        CURLcode m_curl_code;
        unsigned m_http_code;
        std::string m_body;
};

#endif // HTTPRESPONSE_HXX
