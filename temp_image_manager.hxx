#ifndef TEMPIMAGEMANAGER_HXX
#define TEMPIMAGEMANAGER_HXX

#include <atomic>
#include <string>
#include <mutex>


//! Manages temporary images
/**
 * Thread-safe.
 */
class TempImageManager
{
    public:
        //! Constructor.
        /*!
         * \param dir_path Directory path without trailing slash
         */
        TempImageManager(std::string&& dir_path);
        ~TempImageManager();

        TempImageManager(TempImageManager&& rhs);
        TempImageManager& operator = (TempImageManager&& rhs);

        //! Downloads an image and returns local file name
        std::string download(std::string&& uri);

        TempImageManager(TempImageManager const & rhs) = delete;
        TempImageManager& operator = (TempImageManager const & rhs) = delete;
    private:
        void _truncateDir();
        void _checkDir();

        std::string m_dir;
        std::atomic<unsigned> m_img_count;
        std::mutex m_mutex;
};

#endif // TEMPIMAGEMANAGER_HXX
