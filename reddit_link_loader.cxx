#include "reddit_link_loader.hxx"

#include <memory>
#include <stdexcept>
#include <utility>

#include "reddit_link.hxx"
#include "fetch_config.hxx"
#include "unique_reddit_link_loader.hxx"


RedditLinkLoader::RedditLinkLoader(std::vector<FetchConfig> configs, TempImageManager& img_man) {
    for (auto & config : configs) {
        m_loaders.emplace_back(std::move(config), img_man);
    }
}


void RedditLinkLoader::update() {
    for (auto & loader : m_loaders) {
        loader.update();
    }
}


std::unique_ptr<RedditLink> RedditLinkLoader::nextLink() {
    for (auto & loader : m_loaders) {
        std::unique_ptr<RedditLink> link = loader.nextLink();
        if (link) {
            return link;
        }
    }

    return nullptr;
}
