#include "app_info.hxx"

std::string AppInfo::userAgent() {
    return std::string ("linux:") + appID() + ":" + appVersion();
}


std::string AppInfo::appID() {
    return "reddit-notify";
}


std::string AppInfo::appVersion() {
    return "v0.1";
}
