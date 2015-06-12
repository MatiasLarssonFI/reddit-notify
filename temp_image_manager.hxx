#ifndef TEMPIMAGEMANAGER_HXX
#define TEMPIMAGEMANAGER_HXX

#include <string>

class TempImageManager
{
    public:
        static TempImageManager& getManager();
        std::string download(std::string uri);
        ~TempImageManager();
    private:
        void _makeDir() const;
        void _removeDir() const;
        void _resetDir() const;
        void _checkDir();

        TempImageManager();
        std::string m_dir;
        unsigned m_img_count;
};

#endif // TEMPIMAGEMANAGER_HXX
