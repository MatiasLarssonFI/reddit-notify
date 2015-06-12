#ifndef REDDITLINKLOADER_HXX
#define REDDITLINKLOADER_HXX

#include <string>
#include <vector>
#include <memory>

#include "reddit_link.hxx"
#include "fetch_config.hxx"
#include "temp_image_manager.hxx"


//! Loads a reddit link
/*!
 *
 */
class RedditLinkLoader
{
    public:
        //! Constructor
        /*!
         * \param configs Array of configurations
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

        bool hasLinks() const { return !m_buffer.empty(); }

    private:
        RedditLink _fetch(FetchConfig const & config);

        std::vector<FetchConfig> m_configs;
        std::vector<RedditLink> m_buffer;
        TempImageManager m_img_man;
};

#endif // REDDITLINKLOADER_HXX
