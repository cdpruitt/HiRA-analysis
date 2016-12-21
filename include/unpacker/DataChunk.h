#ifndef DATA_CHUNK_H
#define DATA_CHUNK_H

#include <fstream>
#include "TTree.h"

/******************************************************************************/
/* DataChunk structure */

class DataChunk
{
    public:
        virtual ~DataChunk() {}

        virtual void add(DataChunk*) = 0;
        virtual void extractData(std::ifstream& evtfile) = 0;
        virtual void print(std::ofstream& evtfile) = 0;
        virtual void branch(TTree*& tree) = 0;

        std::string getName();
        unsigned int getSize();
        virtual void reset() = 0;

    protected:
        DataChunk(std::string n);
        DataChunk(std::string n, unsigned int s);
        unsigned int size; // in bytes

    private:
        std::string name;
};

#endif
