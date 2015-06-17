#include <atomic>
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
    std::atomic<bool> killed(false);

    auto fetch_f = [&killed] () {
        std::vector<FetchConfig> configs {
            {"askscience", "hot", 10, (unsigned)std::time(nullptr)},
            {"funny", "hot", 5, (unsigned)std::time(nullptr)}
        };

        RedditLinkLoader loader(std::move(configs));
        // TODO: consider condvar here
        while (!killed.load()) {
            try {
                loader.update();
                std::unique_ptr<RedditLink> link = nullptr;
                while(link = loader.nextLink()) {
                    Notification notification(link->title, link->url, link->thumbnail_path);
                    notification.setTimeout(notification_timeout);
                    notification.show();
                }
            } catch (std::exception const & e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cerr << "fetch_f killed" << std::endl;
    };

    std::thread fetch_t(fetch_f);
    std::string line;
    do {
        std::getline(std::cin, line);
    } while(line != "q");

    std::cerr << "Quit requested" << std::endl;
    killed.store(true);
    fetch_t.join();
    std::cerr << "Joined fetch_f, exiting" << std::endl;
    return 0;
}

