#include "../../include/unpacker/SimpleDataChunk.h"

using namespace std;

SimpleDataChunk::SimpleDataChunk(std::string n, unsigned int s) : DataChunk(n, s) {}

void SimpleDataChunk::add(DataChunk*) {}

void SimpleDataChunk::extractData(std::ifstream& evtfile)
{
    unsigned int word;
    evtfile.read((char*)&word,size);

    for(Datum d : data)
    {
        d.readValue(word);
    }
}

void SimpleDataChunk::print(ofstream& outputFile)
{
    outputFile << getName() << endl << endl;
    for(Datum d : data)
    {
        d.print(outputFile);
    }
}

void SimpleDataChunk::branch(TTree*& tree)
{
    for(Datum d : data)
    {
        d.branch(tree);
    }
}

void SimpleDataChunk::add(Datum d)
{
    data.push_back(d);
}

unsigned int SimpleDataChunk::getDataValue(unsigned int i)
{
    if(data.size() > i)
    {
        return data[i].value;
    }

    cerr << "Error: tried to retrieve non-existent data value " << i << " from " << name << endl;
    return 0;
}
