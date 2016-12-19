#ifndef FILE_OPENER_H
#define FILE_OPENER_H

#include <fstream>

class FileOpener
{
    public:
        static FileOpener* Instance();
        void openFile(std::string fileName, std::ifstream& file);
        void openFile(std::string fileName, std::ifstream& file, std::string);

    protected:
        FileOpener() {}

    private:
        static FileOpener* instance;
};

#endif
