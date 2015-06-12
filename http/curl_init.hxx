#ifndef NOTIFYINIT_HXX
#define NOTIFYINIT_HXX

#include <memory>

#include <curl/curl.h>

/** Singleton. Inits libcurl at construction.
 *
 */
class CurlInit
{
    public:
        static CurlInit& getInit();
        ~CurlInit();
        CURL* handle() const { return m_curl; }

        CurlInit(CurlInit const & rhs) = delete;
        CurlInit& operator = (CurlInit const & rhs) = delete;
    private:
        CurlInit();
        CURL* m_curl;
};

#endif // NOTIFYINIT_HXX
