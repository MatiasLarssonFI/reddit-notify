#ifndef DIRECTORY_HXX
#define DIRECTORY_HXX

#include <string>
#include <mutex>

#include <sys/types.h>
#include <dirent.h>


/**
 * Directory stream wrapper.
 *
 * Used to loop through entry names inside a directory.
 * Thread-safe. Excludes . (dot) and .. (dot-dot) entries.
 */
class Directory {
    public:
        //! Constructor.
        /**
         * \param path The directory path, without trailing slash (/)
         */
        Directory(std::string path);
        ~Directory();


        //! Returns the relative path of next entry in the directory.
        /*!
         * Returned path is relative to the path passed to constructor.
         * If there are no more entries in the directory, an empty
         * string is returned.
         *
         * \return std::string Name of next entry or empty string.
         */
        std::string nextEntry();


        //! Resets the internal pointer used by nextEntry
        void reset();
    private:
        std::mutex m_mutex;
        std::string m_path;
        DIR* m_handle;
};

#endif // DIRECTORY_HXX
