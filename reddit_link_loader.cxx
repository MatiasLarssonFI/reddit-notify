#include "reddit_link_loader.hxx"

#include <string>
#include <vector>
#include <ctime>
#include <stdexcept>

#include <json/json.h>

#include "reddit_link.hxx"
#include "fetch_config.hxx"
#include "http/http_request.hxx"
#include "app_info.hxx"

#include <iostream> // debug

RedditLinkLoader::RedditLinkLoader(std::vector<FetchConfig> configs)
    : m_configs(std::move(configs))
    , m_img_man(TempImageManager::getManager())
    , m_fetch_count(0)
{}


void RedditLinkLoader::update() {
    for (auto & config : m_configs) {
        if (std::time(nullptr) >= config.last_fetch + config.interval_sec) {
            m_buffer.push_back(this->_fetch(config));
            config.last_fetch = std::time(nullptr); // time called again because an HTTP request takes an undefined time
        }
    }
}


std::unique_ptr<RedditLink> RedditLinkLoader::nextLink() {
    if (!m_buffer.empty()) {
        std::unique_ptr<RedditLink> ret(new RedditLink(std::move(m_buffer.back())));
        m_buffer.pop_back();
        return ret;
    } else {
        return nullptr;
    }
}


RedditLink RedditLinkLoader::_fetch(FetchConfig const & config) {
    std::string uri = "http://www.reddit.com/r/";
    uri += config.subreddit;
    uri += "/";
    uri += config.tab;
    uri += ".json?limit=";
    uri += std::to_string(m_fetch_count + 2);

    HTTPRequest request(uri);
    request.addHeader(std::string("User-Agent: ") + AppInfo::userAgent());
    HTTPResponse response = request.perform();
    if (response.HTTPCode() == 200) {
        Json::Value root;
        Json::Reader reader;

        if (reader.parse(response.body(), root)) {
            Json::Value json = root.get("data", Json::Value())
                                    .get("children", Json::Value())
                                    .get(m_fetch_count + 1, Json::Value())
                                    .get("data", Json::Value());
            if (!json.empty()) {
                Json::Value title = json.get("title", Json::Value());
                Json::Value thumb = json.get("thumbnail", Json::Value());
                Json::Value url = json.get("permalink", Json::Value());

                if (!title.empty() && !thumb.empty() && !url.empty()) {
                    const std::string thumb_path = m_img_man.download(thumb.asString());
                    m_fetch_count++;
                    return {config.subreddit, title.asString(), url.asString(), thumb_path};
                } else {
                    throw std::invalid_argument(std::string("Failed to parse urls, config: ") + config.str());
                }
            } else {
                throw std::invalid_argument(std::string("Failed to parse json, config: ") + config.str() + ", json: " + response.body());
            }
        } else {
            throw std::invalid_argument(std::string("Failed to parse response, config: ") + config.str() + "response: " + response.body());
        }
    } else {
        throw std::runtime_error(std::string("HTTP ") + std::to_string(response.HTTPCode()) + ", body: " + response.body());
    }
}
