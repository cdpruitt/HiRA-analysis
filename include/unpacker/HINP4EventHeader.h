#ifndef HINP4_EVENT_HEADER_H
#define HINP4_EVENT_HEADER_H

#include "CompositeDataChunk.h"
#include "SimpleDataChunk.h"

class HINP4EventHeader : public CompositeDataChunk
{
    public:
        HINP4EventHeader(std::string n);

    private:
        SimpleDataChunk* XLMMarker;
        SimpleDataChunk* wordsInEvent;
        SimpleDataChunk* channelsHit;
        SimpleDataChunk* XLMTimestamp;
};

#endif
