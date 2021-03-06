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
        ~NotifyInit();
        bool good() const { return m_good; }
        explicit operator bool() const { return m_good; }

        NotifyInit(NotifyInit const & rhs) = delete;
        NotifyInit& operator = (NotifyInit const & rhs) = delete;
    private:
        NotifyInit(std::string const & app_name);
        bool m_good;
};

#endif // NOTIFYINIT_HXX
