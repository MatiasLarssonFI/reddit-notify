#ifndef TEMPIMAGEMANAGER_HXX
#define TEMPIMAGEMANAGER_HXX

#include <atomic>
#include <string>
#include <mutex>


//! Manages temporary images
/**
 * Thread-safe singleton.
 */
class TempImageManager
{
    public:
        //! Returns the instance
        static TempImageManager& getManager();

        //! Downloads an image and returns local file name
        std::string download(std::string uri);

        ~TempImageManager();

        TempImageManager(TempImageManager const & rhs) = delete;
        TempImageManager& operator = (TempImageManager const & rhs) = delete;
    private:
        void _truncateDir();
        void _checkDir();

        TempImageManager();
        std::string m_dir;
        std::atomic<unsigned> m_img_count;
        std::mutex m_mutex;
};

#endif // TEMPIMAGEMANAGER_HXX
