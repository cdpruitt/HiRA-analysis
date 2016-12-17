// This file contains utilities to read data from an input stream

#ifndef READ_DATA_H
#define READ_DATA_H

#include <fstream>
#include <vector>

template <typename T>
bool readWord(std::ifstream& evtfile, std::vector<T>& variable)
{
    return evtfile.read((char*)&variable[0], sizeof(T)*sizeof(variable));
}

#endif
