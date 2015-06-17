#ifndef FETCH_CONFIG_HXX
#define FETCH_CONFIG_HXX

#include <string>
#include <sstream>


//! A reddit link fetch configuration
/*!
 *
 */
struct FetchConfig {
    std::string subreddit; //!< subreddit name
    std::string tab; //!< tab name, such as "hot" or "new"
    unsigned interval_sec; //!< Fetch interval
    unsigned last_fetch; ///!< UNIX timestamp of last fetch

    //! Returns an informative string representation
    std::string str() const {
        std::stringstream ss;
        ss << "subreddit: " << subreddit << ", tab: " << tab
            << ", interval: " << interval_sec << ", last fetch: " << last_fetch;
        return ss.str();
    }
};

#endif // FETCH_CONFIG_HXX
