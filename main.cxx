#include <thread>
#include <functional>
#include <chrono>
#include <memory>
#include <iostream>
#include <ctime>
#include <stdexcept>

#include "fetch_config.hxx"
#include "reddit_link_loader.hxx"
#include "notification.hxx"


int main()
{
    constexpr int notification_timeout = 10000; // ms
    bool killed = false;

    auto fetch_f = [&killed] () {
        std::vector<FetchConfig> configs {
            {"askscience", "hot", 10, (unsigned)std::time(nullptr)},
            {"funny", "hot", 5, (unsigned)std::time(nullptr)}
        };

        RedditLinkLoader loader(std::move(configs));

        while (!killed) {
            try {
                loader.update();
                std::unique_ptr<RedditLink> link = nullptr;
                while(link = loader.nextLink()) {
                    Notification notification(link->title, link->url, link->thumbnail_path);
                    notification.setTimeout(notification_timeout);
                    notification.show();
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            } catch (std::exception const & e) {
                std::cerr << "Error: " << e.what();
                exit(1);
            }
        }
    };

    std::thread fetch_t(fetch_f);
    std::string line;
    std::getline(std::cin, line);
    killed = true;
    fetch_t.join();

    return 0;
}

