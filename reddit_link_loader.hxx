#ifndef REDDITLINKLOADER_HXX
#define REDDITLINKLOADER_HXX

#include <string>
#include <vector>
#include <memory>

#include "reddit_link.hxx"
#include "fetch_config.hxx"
#include "unique_reddit_link_loader.hxx"


//! Manages the fetching of Reddit links according to FetchConfiguration objects
class RedditLinkLoader
{
    public:
        //! Constructor
        /*!
         * \param configs Vector of fetch configurations
         */
        RedditLinkLoader(std::vector<FetchConfig> configs);


        //! Loads links to buffer
        /*!
         * If a configuration (timestamp) indicates that
         * a link should be loaded, a RedditLink will be loaded
         * into the internal buffer according to the configuration.
         */
        void update();


        //! Returns next link from the buffer or nullptr if buffer empty
        /*!
         * Returns the next link from the internal buffer, or nullptr
         * if internal buffer is empty. The next link is always at the
         * back of the buffer, so it's really a lifo (last in, first out).
         *
         * \return pointer to next RedditLink or nullptr
         */
        std::unique_ptr<RedditLink> nextLink();
    private:
        // One UniqueRedditLinkLoader is constructed for each FetchConfiguration
        std::vector<UniqueRedditLinkLoader> m_loaders;
};

#endif // REDDITLINKLOADER_HXX
