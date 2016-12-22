#ifndef RING_FORMAT_ITEM_H
#define RING_FORMAT_ITEM_H

#include "RingItemBody.h"
#include "SimpleDataChunk.h"

class RingFormatItem : public RingItemBody
{
    public:
        RingFormatItem(std::string n);

    private:
        SimpleDataChunk* majorVersion;
        SimpleDataChunk* minorVersion;
};

#endif
