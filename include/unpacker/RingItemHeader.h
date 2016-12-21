#ifndef RING_ITEM_HEADER
#define RING_ITEM_HEADER

#include "CompositeDataChunk.h"
#include "SimpleDataChunk.h"

class RingItemHeader : public CompositeDataChunk
{
    public:
        RingItemHeader(std::string n);

    private:
        SimpleDataChunk* inclusiveSize;
        SimpleDataChunk* eventType;
};

#endif
