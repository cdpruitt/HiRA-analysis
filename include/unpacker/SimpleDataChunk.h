#ifndef SIMPLE_DATA_CHUNK_H
#define SIMPLE_DATA_CHUNK_H

#include "DataChunk.h"
#include "Datum.h"

#include <iostream>
#include <vector>

class SimpleDataChunk : public DataChunk
{
    public:
        SimpleDataChunk(std::string n, unsigned int s);

        void add(DataChunk*);
        void extractData(std::ifstream& evtfile);
        void print(std::ofstream& evtfile);
        virtual void branch(TTree*& tree);

        void add(Datum d);
        unsigned int getDataValue(unsigned int i);

        std::vector<Datum> data;

    private:
        std::string name;
};

#endif
