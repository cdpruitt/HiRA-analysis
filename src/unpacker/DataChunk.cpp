#include "../../include/unpacker/DataChunk.h"

using namespace std;

DataChunk::DataChunk(string n) : name(n) {}
DataChunk::DataChunk(string n, unsigned int s) : name(n), size(s) {}

string DataChunk::getName()
{
    return name;
}

unsigned int DataChunk::getSize()
{
    return size;
}
