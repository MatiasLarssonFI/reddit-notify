#ifndef DIRECTORY_HXX
#define DIRECTORY_HXX

#include <string>

#include <sys/types.h>
#include <dirent.h>

class Directory {
    public:
        // TBD: doc, path without trailing slash
        Directory(std::string path);
        ~Directory();


        //! Returns the path of next file in the directory.
        /*!
         * If there are no more files in the directory, empty
         * string is returned.
         *
         * @return std::string Name of next file or empty string.
         */
        std::string nextFile();


        //! Resets the internal pointer used by nextFile
        void reset();
    private:
        DIR* m_handle;
        std::string m_path;
};

#endif // DIRECTORY_HXX
