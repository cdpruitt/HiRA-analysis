#ifndef COMPOSITE_DATA_CHUNK
#define COMPOSITE_DATA_CHUNK

#include "DataChunk.h"

#include <fstream>
#include <vector>

class CompositeDataChunk : public DataChunk
{
    public:
        CompositeDataChunk(std::string n) : DataChunk(n) {}
        ~CompositeDataChunk() {}

        void add(DataChunk* d)
        {
            subChunks.push_back(d);
        }

        void remove(DataChunk*)
        {
        }

        unsigned int getSize();
        void extractData(std::ifstream& evtfile);

    protected:
        std::vector<DataChunk*> subChunks;

    private:
};

#endif
