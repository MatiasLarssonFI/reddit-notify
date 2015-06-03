#ifndef NOTIFYINIT_HXX
#define NOTIFYINIT_HXX

#include <string>

/** Singleton. Inits libnotify at construction.
 *
 */
class NotifyInit
{
    public:
        static NotifyInit& getInit(std::string const & app_name);
        bool good() const { return m_good; }
        operator bool() const { return m_good; }
        ~NotifyInit();
    private:
        NotifyInit(std::string const & app_name);
        bool m_good;
};

#endif // NOTIFYINIT_HXX
