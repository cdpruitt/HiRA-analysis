#ifndef DATA_CHUNK_H
#define DATA_CHUNK_H

#include <fstream>

/******************************************************************************/
/* DataChunk structure */

class DataChunk
{
    public:
        virtual ~DataChunk() {}
        virtual void extractData(std::ifstream& evtfile) = 0;

        virtual void add(DataChunk*) = 0;

        unsigned int getSize();
        std::string getName();

    protected:
        DataChunk(std::string n);
        unsigned int size; // in bytes

    private:
        std::string name;
};

#endif
