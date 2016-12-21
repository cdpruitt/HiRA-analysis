#include "../../include/unpacker/CompositeDataChunk.h"
#include "../../include/unpacker/DataChunk.h"

#include <fstream>
#include <iostream>

#include "TTree.h"

using namespace std;

CompositeDataChunk::CompositeDataChunk(string n) : DataChunk(n) {}

void CompositeDataChunk::add(DataChunk* d)
{
    subChunks.push_back(d);
    size += d->getSize();
}

void CompositeDataChunk::extractData(ifstream& evtfile)
{
    for(DataChunk* chunk : subChunks)
    {
        chunk->extractData(evtfile);
    }
}

unsigned int CompositeDataChunk::getSize()
{
    return size;
}

void CompositeDataChunk::print(ofstream& outputFile)
{
    outputFile << getName() << endl << endl;
    for(DataChunk* chunk : subChunks)
    {
        chunk->print(outputFile);
    }
}

void CompositeDataChunk::branch(TTree*& tree)
{
    for(DataChunk* chunk : subChunks)
    {
        chunk->branch(tree);
    }
}

std::vector<DataChunk*> CompositeDataChunk::getSubChunks()
{
    return subChunks;
}

void CompositeDataChunk::reset()
{
    for(DataChunk* subChunk : subChunks)
    {
        subChunk->reset();
    }
}
