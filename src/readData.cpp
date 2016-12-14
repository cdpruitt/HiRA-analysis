// This file contains utilities to read data from an input stream

#include "../include/readData.h"

#include <vector>

using namespace std;

// read an 8-bit word
bool readWord(ifstream& evtfile, std::vector<char> variable)
{
    return evtfile.read((char*)&variable[0], sizeof(variable));
}

// read a 16-bit word
bool readWord(ifstream& evtfile, std::vector<unsigned short> variable)
{
    return evtfile.read((char*)&variable[0], sizeof(variable));
}

// read a 32-bit word
bool readWord(ifstream& evtfile, std::vector<unsigned int> variable)
{
    return evtfile.read((char*)&variable[0], sizeof(variable));
}

// read a 64-bit word
bool readWord(ifstream& evtfile, std::vector<double> variable)
{
    return evtfile.read((char*)&variable[0], sizeof(variable));
}
