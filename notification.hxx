#ifndef NOTIFICATION_HXX
#define NOTIFICATION_HXX

#include <libnotify/notify.h>
#include <string>

class Notification {
    public:
        Notification(std::string const & summary, std::string const & body, std::string const & icon = "");
        ~Notification();
        void setTimeout(gint milliseconds);
        void show();
    private:
        NotifyNotification* m_handle;
};

#endif // NOTIFICATION_HXX
