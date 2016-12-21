#ifndef COMPOSITE_DATA_CHUNK
#define COMPOSITE_DATA_CHUNK

#include "DataChunk.h"

#include <fstream>
#include <vector>

#include "TTree.h"

class CompositeDataChunk : public DataChunk
{
    public:
        CompositeDataChunk(std::string n);

        void add(DataChunk* d);
        virtual void extractData(std::ifstream& evtfile);
        void print(std::ofstream& outputFile);
        void branch(TTree*& tree);

        unsigned int getSize();

    protected:
        std::vector<DataChunk*> subChunks;

    private:
};

#endif
