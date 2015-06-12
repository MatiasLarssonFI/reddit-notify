#include "curl_init.hxx"

#include <stdexcept>

#include <curl/curl.h>

CurlInit::CurlInit()
    : m_curl(curl_easy_init()) {

    if (!m_curl) {
        throw std::runtime_error("Failed to initialize libcurl.");
    }
}


CurlInit::~CurlInit() {
    curl_easy_cleanup(m_curl);
}


CurlInit& CurlInit::getInit() {
    static CurlInit init;
    return init;
}
