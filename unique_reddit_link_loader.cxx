#include "unique_reddit_link_loader.hxx"

#include <ctime>
#include <utility>
#include <stdexcept>
#include <json/json.h>
#include <iostream> // for cerr

#include "app_info.hxx"
#include "http/http_request.hxx"


UniqueRedditLinkLoader::UniqueRedditLinkLoader(FetchConfig config, TempImageManager& img_man)
    : m_config(std::move(config))
    , m_img_man(img_man)
    , m_fetch_count(0)
{}


UniqueRedditLinkLoader::UniqueRedditLinkLoader(UniqueRedditLinkLoader&& rhs)
    : m_config(std::move(rhs.m_config))
    , m_img_man(rhs.m_img_man)
    , m_fetch_count(rhs.m_fetch_count)
{}


void UniqueRedditLinkLoader::update() {
    if (std::time(nullptr) >= m_config.last_fetch + m_config.interval_sec) {
        this->_fetch();
        m_config.last_fetch = std::time(nullptr);
    }
}


std::unique_ptr<RedditLink> UniqueRedditLinkLoader::nextLink() {
    if (!m_buffer.empty()) {
        std::unique_ptr<RedditLink> ret(new RedditLink(std::move(m_buffer.back())));
        m_buffer.pop_back();
        return ret;
    } else {
        return nullptr;
    }
}


bool UniqueRedditLinkLoader::operator == (UniqueRedditLinkLoader const & rhs) const {
    return m_config.subreddit == rhs.m_config.subreddit &&
            m_config.tab == rhs.m_config.tab;
}


void UniqueRedditLinkLoader::_fetch() {
    std::string uri = "http://www.reddit.com/r/";
    uri += m_config.subreddit;
    uri += '/';
    uri += m_config.tab;
    uri += ".json?";
    uri += "limit=1";
    if (m_fetch_count > 0) {
        uri += "&count=";
        uri += std::to_string(m_fetch_count);
    }

    if (!m_last_fullname.empty()) {
        uri += "&after=";
        uri += m_last_fullname;
    }

    HTTPRequest request(uri);
    request.addHeader(std::string("User-Agent: ") + AppInfo::userAgent());
    HTTPResponse response = request.perform();
    if (response.HTTPCode() == 200) {
        Json::Value root;
        Json::Reader reader;

        if (reader.parse(response.body(), root)) {
            Json::Value children = root.get("data", Json::Value())
                                        .get("children", Json::Value());

            if (!children.empty()) {
                Json::Value json = children.get(Json::ArrayIndex(0), Json::Value())
                                            .get("data", Json::Value());
                if (!json.empty()) {
                    Json::Value title = json.get("title", Json::Value());
                    Json::Value thumb = json.get("thumbnail", Json::Value());
                    Json::Value url = json.get("permalink", Json::Value());
                    m_last_fullname = std::string("t3_") + json.get("id", Json::Value()).asString();

                    if (!title.empty() && !thumb.empty() && !url.empty()) {
                        const std::string thumb_path = m_img_man.download(thumb.asString());
                        m_fetch_count++;
                        m_buffer.push_back( {m_config.subreddit, title.asString(), url.asString(), thumb_path} );
                        std::cerr << "JSON: " << response.body() << "\n"
                                    "fullname: " << m_last_fullname << "\n"
                                    "url: " << uri << "\n\n" << std::endl;
                    } else {
                        throw std::invalid_argument(std::string("Failed to parse urls, config: ") + m_config.str());
                    }
                } else {
                    throw std::invalid_argument(std::string("Failed to parse json, config: ") + m_config.str() + ", json: " + response.body());
                }
            } else {
                std::cerr << "No children at " << uri << std::endl;
            }
        } else {
            throw std::invalid_argument(std::string("Failed to parse response, config: ") + m_config.str() + "response: " + response.body());
        }
    } else {
        throw std::runtime_error(std::string("HTTP ") + std::to_string(response.HTTPCode()) + ", body: " + response.body());
    }
}
