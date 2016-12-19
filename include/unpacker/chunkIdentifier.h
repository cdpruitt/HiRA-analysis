#ifndef CHUNK_IDENTIFIER_H
#define CHUNK_IDENTIFIER_H

class ChunkIdentifier : public DataChunk
{
    bool checkID(ifstream& evtfile)
    {
        unsigned int* word;
        evtfile.read(word,size);
        return ((word >> bitShift) & bitMask) == ID;
    }

    const unsigned int bitShift;
    const unsigned int bitMask;
    const unsigned int ID;
}

#endif
