#ifndef CURLHANDLE_HXX
#define CURLHANDLE_HXX

#include <memory>

#include <curl/curl.h>

//! Handles libcurl initialization and cleanup
class CurlHandle
{
    public:
        CurlHandle();
        CurlHandle(CurlHandle&& rhs) = default;


        //! Returns a pointer to the CURL handle
        /*!
         * Using this function is potentially dangerous because it
         * returns the internal pointer. The returned pointer must
         * not be invalidated and its address must remain unchanged.
         */
        CURL* curl() { return m_curl.get(); }
        CurlHandle& operator = (CurlHandle&& rhs) = default;

        CurlHandle(CurlHandle const & rhs) = delete;
        CurlHandle& operator = (CurlHandle const & rhs) = delete;
    private:
        struct CurlDelete {
            void operator()(CURL* curl) {
                curl_easy_cleanup(curl);
            }
        };

        std::unique_ptr<CURL, CurlDelete> m_curl;
};

#endif // CURLHANDLE_HXX
