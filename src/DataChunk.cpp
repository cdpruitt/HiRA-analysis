#include "../include/DataChunk.h"

using namespace std;

unsigned int DataChunk::getSize()
{
    return size;
}

string DataChunk::getName()
{
    return name;
}

DataChunk::DataChunk(std::string n) : name(n)
{
}
