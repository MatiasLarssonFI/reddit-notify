#include "curl_handle.hxx"

#include <stdexcept>

#include <curl/curl.h>

CurlHandle::CurlHandle()
    : m_curl(curl_easy_init()) {

    if (!m_curl) {
        throw std::runtime_error("Failed to initialize libcurl.");
    }
}
