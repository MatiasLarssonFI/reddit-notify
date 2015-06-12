#include "directory.hxx"

#include <stdexcept>
#include <utility>

#include <dirent.h>
#include <errno.h>
#include <string.h>

Directory::Directory(std::string path)
    : m_handle(::opendir(path.c_str()))
    , m_path(std::move(path)) {

    if (!m_handle) {
        throw std::invalid_argument(std::string("Directory ") + path + " does not exist.");
    }
}

Directory::~Directory() {
    ::closedir(m_handle);
}


std::string Directory::nextFile() {
    int old_errno = errno;
    dirent* entry = readdir(m_handle);
    if (entry != nullptr) {
        std::string entry_str = entry->d_name;
        if (entry_str != ".." && entry_str != ".") {
            return m_path + "/" + entry_str;
        } else {
            return nextFile();
        }
    } else {
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
