#ifndef SIMPLE_DATA_CHUNK_H
#define SIMPLE_DATA_CHUNK_H

#include "DataChunk.h"
#include "Datum.h"

#include <vector>

class SimpleDataChunk : public DataChunk
{
    public:
        SimpleDataChunk(std::string n, unsigned int s) : DataChunk(n)
        {
            size = s;
        }

        void add(DataChunk*)
        {}

        void remove(DataChunk*)
        {}

        void add(Datum d)
        {
            data.push_back(d);
        }

        void extractData(std::ifstream& evtfile)
        {
            unsigned int word;
            evtfile.read((char*)&word,size);

            for(Datum d : data)
            {
                d.readValue(word);
            }
        }

        unsigned int getSize()
        {
            return size;
        }

    private:
        std::string name;
        unsigned int size; // in bytes

        std::vector<Datum> data;
};

#endif
