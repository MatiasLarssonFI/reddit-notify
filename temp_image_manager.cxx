#include "temp_image_manager.hxx"

#include <string>
#include <fstream>
#include <mutex>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>

#include "app_info.hxx"
#include "http/http_request.hxx"
#include "directory/directory.hxx"

TempImageManager::TempImageManager()
    : m_dir(std::string("/tmp/") + AppInfo::appID() + ".tmp")
    , m_img_count(0) {

    // create directory if it doesn't exist
    if (::access(m_dir.c_str(), F_OK) != 0) {
        if (::mkdir(m_dir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
            throw std::runtime_error("Failed to create temporary image directory.");
        }
    }
}


TempImageManager::~TempImageManager() {
    _truncateDir();
    ::rmdir(m_dir.c_str());
}


TempImageManager& TempImageManager::getManager() {
    static TempImageManager manager;
    return manager;
}


void TempImageManager::_truncateDir() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (::access(m_dir.c_str(), W_OK) == 0) {
        Directory dir(m_dir);
        std::string fname;
        while (!(fname = dir.nextEntry()).empty()) {
            if (::remove(fname.c_str()) != 0) {
                throw std::runtime_error(std::string("Failed to remove temporary image file ") + fname);
            }
        }
    }
}


void TempImageManager::_checkDir() {
    // TODO: currently we might delete images that have not been
    // displayed yet. Maybe we should make a TempImage class, with
    // a field like "allow_delete"?
    if (m_img_count.load() > 20) {
        _truncateDir();
        m_img_count.store(0);
    }
}


std::string TempImageManager::download(std::string uri) {
    _checkDir();
    m_img_count++;
    HTTPRequest request(uri);
    request.addHeader(std::string("User-Agent: ") + AppInfo::userAgent());
    HTTPResponse resp = request.perform();
    std::string img_name = m_dir + "/" + std::to_string(m_img_count);
    std::ofstream img(img_name, std::ios::binary | std::ios::out);
    img << resp.body() << std::flush;
    return img_name;
}
