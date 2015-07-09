#include <atomic>
#include <thread>
#include <chrono>
#include <memory>
#include <utility>
#include <iostream>
#include <ctime>
#include <stdexcept>

#include "fetch_config.hxx"
#include "reddit_link_loader.hxx"
#include "notification.hxx"
#include "temp_image_manager.hxx"
#include "app_info.hxx"


int main()
{
    constexpr int notification_timeout = 10000; // ms
    TempImageManager img_man(std::string("/tmp/") + AppInfo::appID());
    std::atomic<bool> killed(false);

    auto fetch_f = [&killed, &img_man] () {
        const unsigned timenow = std::time(nullptr);
        // hard-coded example configurations,
        // these could be loaded e.g. from a JSON file
        std::vector<FetchConfig> configs {
            {"askscience", "hot", 21, timenow},
            {"funny", "hot", 12, timenow}
        };

        RedditLinkLoader loader(std::move(configs), img_man);

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

