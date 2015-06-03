#include "notification.hxx"

#include <stdexcept>

#include <libnotify/notify.h>
#include <glib.h>

#include "notify_init.hxx"

Notification::Notification(std::string const & summary, std::string const & body, std::string const & icon)
    : m_handle(notify_notification_new(summary.c_str(), body.c_str(), (!icon.empty() ? icon.c_str() : nullptr))) {

    if (!NotifyInit::getInit("RedditNotify")) {
        throw std::runtime_error("Failed to init libnotify");
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
