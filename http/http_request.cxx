#include "http_request.hxx"

#include <string>
#include <utility>

#include <curl/curl.h>

#include "http_response.hxx"

static std::size_t writeResp(char* resp_data, std::size_t size, std::size_t nmemb, std::string* buf) {
    const std::size_t length = size * nmemb;
    buf->append(resp_data, length);
    return length;
}

HTTPRequest::HTTPRequest(std::string uri)
    : m_headers(nullptr)
    , m_uri(std::move(uri)) {

    curl_easy_setopt(m_curl.curl(), CURLOPT_WRITEFUNCTION, writeResp);
    curl_easy_setopt(m_curl.curl(), CURLOPT_WRITEDATA, &m_response_body);
    curl_easy_setopt(m_curl.curl(), CURLOPT_FOLLOWLOCATION, 1);
}


HTTPRequest::~HTTPRequest() {
    curl_slist_free_all(m_headers);
}


void HTTPRequest::addHeader(std::string const & header) {
    m_headers = curl_slist_append(m_headers, header.c_str());
}


void HTTPRequest::clearHeaders() {
    curl_slist_free_all(m_headers);
    m_headers = nullptr;
}


HTTPResponse HTTPRequest::perform() {
    long http_code = 0;
    m_response_body.clear();

    curl_easy_setopt(m_curl.curl(), CURLOPT_URL, m_uri.c_str());
    if (m_headers) {
        curl_easy_setopt(m_curl.curl(), CURLOPT_HTTPHEADER, m_headers);
    }

    CURLcode curl_code = curl_easy_perform(m_curl.curl());
    curl_easy_getinfo(m_curl.curl(), CURLINFO_RESPONSE_CODE, &http_code);
    return HTTPResponse(curl_code, http_code, std::move(m_response_body));
}
