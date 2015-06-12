#include "notify_init.hxx"

#include <stdexcept>

#include <libnotify/notify.h>

NotifyInit::NotifyInit(std::string const & app_name)
    : m_good(notify_init(app_name.c_str())) {

    if (!m_good) {
        throw std::runtime_error("Failed to initialize libnotify.");
    }
}


NotifyInit::~NotifyInit() {
    notify_uninit();
}


NotifyInit& NotifyInit::getInit(std::string const & app_name) {
    static NotifyInit init(app_name);
    return init;
}
