#include <iostream>
#include <fstream>

#include "../../include/singletons/FileOpener.h"

using namespace std;

FileOpener* FileOpener::instance = 0;

FileOpener* FileOpener::Instance() {
    if(instance == 0)
    {
        instance = new FileOpener;
    }

    return instance;
}

void FileOpener::openFile(string fileName, ifstream& file)
{
    file.open(fileName);

    if(!file.is_open())
    {
        cerr << "Error: couldn't open " << fileName << "." << endl;
    }

    else
    {
        cout << "Successfully opened " << fileName << "." << endl;
    }
}

void FileOpener::openFile(string fileName, ifstream& file, string mode)
{
    if(mode == "b")
    {
        file.open(fileName, ios::binary);
    }

    else
    {
        cerr << "Error: couldn't understand file opening mode " << mode
             << " when trying to open " << fileName << endl;
        return;
    }

    if(!file.is_open())
    {
        cerr << "Error: couldn't open " << fileName << "." << endl;
        return;
    }

    cout << "Successfully opened " << fileName << "." << endl;
}
