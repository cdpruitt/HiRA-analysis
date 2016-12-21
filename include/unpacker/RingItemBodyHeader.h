#ifndef RING_ITEM_BODY_HEADER_H
#define RING_ITEM_BODY_HEADER_H

#include "CompositeDataChunk.h"
#include "SimpleDataChunk.h"

class RingItemBodyHeader : public CompositeDataChunk
{
    public:
        RingItemBodyHeader(std::string n);

    private:
        SimpleDataChunk* bodyHeaderSize;
        SimpleDataChunk* bodyHeaderTimestamp;
        SimpleDataChunk* bodyHeaderSourceID;
        SimpleDataChunk* bodyHeaderBarrierType;
};

#endif
