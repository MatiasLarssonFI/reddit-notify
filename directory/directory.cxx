#include "directory.hxx"

#include <stdexcept>
#include <utility>
#include <mutex>

#include <dirent.h>
#include <errno.h>
#include <string.h>

Directory::Directory(std::string path)
    : m_path(std::move(path))
    , m_handle(::opendir(m_path.c_str())) {

    if (!m_handle) {
        throw std::invalid_argument(std::string("Cannot open directory ") + path);
    }
}


Directory::~Directory() {
    ::closedir(m_handle);
}


std::string Directory::nextEntry() {
    const int old_errno = errno;
    m_mutex.lock();
    dirent* entry = readdir(m_handle); // not a thread-safe call (dirent may be overwritten)
    if (entry != nullptr) {
        std::string entry_str = entry->d_name;
        m_mutex.unlock();
        if (entry_str != ".." && entry_str != ".") {
            return m_path + "/" + entry_str;
        } else {
            return nextEntry();
        }
    } else {
        m_mutex.unlock();
        if (errno == old_errno) { // no error
            return "";
        } else {
            throw std::runtime_error(std::string("readdir: ") + strerror(errno));
        }
    }
}


void Directory::reset() {
    ::rewinddir(m_handle);
}
