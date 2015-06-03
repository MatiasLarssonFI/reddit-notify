#include "notify_init.hxx"
#include <libnotify/notify.h>

NotifyInit::NotifyInit(std::string const & app_name)
    : m_good(notify_init(app_name.c_str()))
{}


NotifyInit::~NotifyInit() {
    notify_uninit();
}


NotifyInit& NotifyInit::getInit(std::string const & app_name) {
    static NotifyInit init(app_name);
    return init;
}
