#include "../../include/unpacker/RingItemBody.h"

using namespace std;

RingItemBody::RingItemBody(string n) : CompositeDataChunk(n) {}

void RingItemBody::print(ofstream& outputFile)
{
    outputFile << endl << getName() << endl;
    for(DataChunk* chunk : subChunks)
    {
        chunk->print(outputFile);
    }
}
