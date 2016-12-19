#include "../../include/unpacker/CompositeDataChunk.h"
#include "../../include/unpacker/DataChunk.h"

#include <fstream>

using namespace std;

void CompositeDataChunk::extractData(ifstream& evtfile)
{
    for(DataChunk* chunk : subChunks)
    {
        chunk->extractData(evtfile);
    }
}

unsigned int CompositeDataChunk::getSize()
{
    size = 0;

    for(DataChunk* chunk : subChunks)
    {
        size += chunk->getSize();
    }

    return size;
}
