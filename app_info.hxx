#ifndef APPINFO_HXX
#define APPINFO_HXX

#include <string>

class AppInfo
{
    public:
        static std::string userAgent();
        static std::string appID();
        static std::string appVersion();
};

#endif // APPINFO_HXX
