#include "unique_reddit_link_loader.hxx"

UniqueRedditLinkLoader::UniqueRedditLinkLoader(FetchConfig config)
    : m_config(std::move(config))
{}


bool UniqueRedditLinkLoader::operator == (UniqueRedditLinkLoader const & rhs) const {
    return m_config.subreddit == rhs.config().subreddit &&
            m_config.tab == rhs.config().tab
}
