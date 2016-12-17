#ifndef COMPOSITE_DATA_CHUNK
#define COMPOSITE_DATA_CHUNK

#include "../include/DataChunk.h"

#include <fstream>
#include <vector>

class CompositeDataChunk : public DataChunk
{
    public:
        virtual ~CompositeDataChunk() {}

        void add(DataChunk* d)
        {
            subChunks.push_back(d);
        }

        void remove(DataChunk* d)
        {
        }

        void extractData(std::ifstream& evtfile);
        unsigned int getSize();

    protected:
        CompositeDataChunk(std::string n) : DataChunk(n) {}
        std::vector<DataChunk*> subChunks;

    private:
};

#endif
