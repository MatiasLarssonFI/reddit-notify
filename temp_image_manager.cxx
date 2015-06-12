#include "temp_image_manager.hxx"

#include <string>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>

#include "app_info.hxx"
#include "http/http_request.hxx"
#include "directory/directory.hxx"

TempImageManager::TempImageManager()
    : m_dir(std::string("/tmp/") + AppInfo::appID() + ".tmp")
    , m_img_count(0) {

    _makeDir();
}


TempImageManager::~TempImageManager() {
    _removeDir();
}


void TempImageManager::_makeDir() const {
    // create directory if it doesn't exist
    if (::access(m_dir.c_str(), F_OK) != 0) {
        if (::mkdir(m_dir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
            throw std::runtime_error("Failed to create temporary image directory.");
        }
    }
}


void TempImageManager::_removeDir() const {
    if (::access(m_dir.c_str(), W_OK) == 0) {
        Directory dir(m_dir);
        std::string fname;
        while (!(fname = dir.nextFile()).empty()) {
            if (::remove(fname.c_str()) != 0) {
                throw std::runtime_error("Failed to remove temporary image file.");
            }
        }
        if (::rmdir(m_dir.c_str()) != 0) {
            throw std::runtime_error("Failed to remove temporary image directory.");
        }
    }
}


void TempImageManager::_resetDir() const {
    _removeDir();
    _makeDir();
}


void TempImageManager::_checkDir() {
    if (m_img_count > 20) {
        _resetDir();
        m_img_count = 0;
    }
}


TempImageManager& TempImageManager::getManager() {
    static TempImageManager manager;
    return manager;
}



std::string TempImageManager::download(std::string uri) {
    _checkDir();
    ++m_img_count;
    HTTPRequest request(uri);
    request.addHeader(std::string("User-Agent: ") + AppInfo::userAgent());
    HTTPResponse resp = request.perform();
    std::string img_name = m_dir + "/" + std::to_string(m_img_count);
    std::ofstream img(img_name, std::ios::binary | std::ios::out);
    img << resp.body() << std::flush;
    return img_name;
}
