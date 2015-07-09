#include "notification.hxx"

#include <stdexcept>

#include <libnotify/notify.h>
#include <glib.h>

// constructor: m_handle becomes NotifyNotification* or nullptr, depending on m_init
//              notify_notification_new's third parameter becomes const char* or nullptr, depending on its size
Notification::Notification(std::string const & summary, std::string const & body, std::string const & icon)
    : m_init(NotifyInit::getInit("RedditNotify"))
    , m_handle((m_init ? notify_notification_new(summary.c_str(), body.c_str(), (!icon.empty() ? icon.c_str() : nullptr)) : nullptr))
{
    if (!m_init) {
        throw std::runtime_error("Failed to initialize notifications library.");
    }
}

Notification::~Notification() {
    g_object_unref(G_OBJECT(m_handle));
}


void Notification::setTimeout(gint milliseconds) {
    notify_notification_set_timeout(m_handle, milliseconds);
}


void Notification::show() {
    GError** error = {nullptr};
    if (!notify_notification_show(m_handle, error)) {
        throw std::runtime_error(std::string("Failed to show notification: ") + (*error)->message);
    }
}
