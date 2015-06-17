#ifndef UNIQUEREDDITLINKLOADER_HXX
#define UNIQUEREDDITLINKLOADER_HXX

#include <string>
#include <memory>
#include <vector>

#include "fetch_config.hxx"
#include "temp_image_manager.hxx"
#include "reddit_link.hxx"

//! Manages fetching of Reddit links according to a FetchConfig configuration
/*!
 * This class handles the link downloading for a single configuration.
 * Also handles the 'pagination' so that the same link is not loaded twice.
 */
class UniqueRedditLinkLoader
{
    public:
        //! Constructor.
        /*!
         * \param config Configuration for fetching links
         */
        UniqueRedditLinkLoader(FetchConfig config);


        //! Loads links to buffer
        /*!
         * If the configuration (timestamp) indicates that
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

        bool operator == (UniqueRedditLinkLoader const & rhs) const;
    private:
        void _fetch();

        FetchConfig m_config;
        TempImageManager & m_img_man;
        unsigned m_fetch_count;
        std::string m_last_fullname;
        std::vector<RedditLink> m_buffer;
};

#endif // UNIQUEREDDITLINKLOADER_HXX
