#ifndef RING_STATE_CHANGE_ITEM_H
#define RING_STATE_CHANGE_ITEM_H

#include "RingBodyItem.h"
#include "SimpleDataChunk.h"

class RingStateChangeItem : public RingBodyItem
{
    public:
        RingStateChangeItem(std::string n);
        ~RingStateChangeItem() {}

    private:
        SimpleDataChunk* runNumber;
        SimpleDataChunk* timeOffset;
        SimpleDataChunk* timeStamp;
        SimpleDataChunk* offsetDivisor;
        SimpleDataChunk* runTitle;
};

#endif
