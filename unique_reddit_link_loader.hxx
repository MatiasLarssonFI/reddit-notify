#ifndef UNIQUEREDDITLINKLOADER_HXX
#define UNIQUEREDDITLINKLOADER_HXX

#include "fetch_config.hxx"

class UniqueRedditLinkLoader
{
    public:
        UniqueRedditLinkLoader(FetchConfig config);

        UniqueRedditLinkLoader(UniqueRedditLinkLoader const & rhs) = delete;
        CurlInit& operator = (CurlInit const & rhs) = delete;
        bool operator == (UniqueRedditLinkLoader const & rhs) const;
        FetchConfig const & config() const;
    private:
        FetchConfig m_config;
};

#endif // UNIQUEREDDITLINKLOADER_HXX
