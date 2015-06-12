#include "http_response.hxx"

HTTPResponse::HTTPResponse(CURLcode curl_code, unsigned http_code, std::string body)
    : m_curl_code(curl_code)
    , m_http_code(http_code)
    , m_body(body)
{}
