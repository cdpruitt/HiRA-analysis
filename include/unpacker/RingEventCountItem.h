#ifndef RING_EVENT_COUNT_ITEMS
#define RING_EVENT_COUNT_ITEMS

#include "RingItemBody.h"
#include "SimpleDataChunk.h"

class RingEventCountItem : public RingItemBody
{
    public:
        RingEventCountItem(std::string n);
        ~RingEventCountItem() {}

    private:
        SimpleDataChunk* timeOffset;
        SimpleDataChunk* offsetDivisor;
        SimpleDataChunk* timestamp;
        SimpleDataChunk* eventCount;
};

#endif
