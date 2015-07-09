#ifndef NOTIFICATION_HXX
#define NOTIFICATION_HXX

#include <libnotify/notify.h>
#include <string>

#include "notify_init.hxx"

class Notification {
    public:
        Notification(std::string const & summary, std::string const & body, std::string const & icon = "");
        ~Notification();
        void setTimeout(gint milliseconds);
        void show();

        Notification(Notification const & rhs) = delete;
        Notification& operator = (Notification const & rhs) = delete;
    private:
        NotifyInit& m_init;
        NotifyNotification* m_handle;
};

#endif // NOTIFICATION_HXX
