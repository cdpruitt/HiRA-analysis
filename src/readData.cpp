// This file contains utilities to read data from an input stream

#include "../include/readData.h"

using namespace std;

// read an 8-bit word
bool readNextWord(ifstream& evtfile, char* variable)
{
    return evtfile.read((char*)variable, sizeof(*variable));
}

// read a 16-bit word
bool readNextWord(ifstream& evtfile, unsigned short* variable)
{
    return evtfile.read((char*)variable, sizeof(*variable));
}

// read a 32-bit word
bool readNextWord(ifstream& evtfile, unsigned int* variable)
{
    return evtfile.read((char*)variable, sizeof(*variable));
}

// read a 64-bit word
bool readNextWord(ifstream& evtfile, double* variable)
{
    return evtfile.read((char*)variable, sizeof(*variable));
}
