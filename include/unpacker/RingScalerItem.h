#ifndef RING_SCALER_ITEM_H
#define RING_SCALER_ITEM_H

#include "RingItemBody.h"
#include "RingScalerItemValues.h"
#include "SimpleDataChunk.h"
#include "CompositeDataChunk.h"

class RingScalerItem : public RingItemBody
{
    public:
        RingScalerItem(std::string n);
        ~RingScalerItem() {}

        void extractData(std::ifstream& evtfile);
        unsigned int getScalerNumber();

    private:
        SimpleDataChunk* intervalStartOffset;
        SimpleDataChunk* intervalEndOffset;
        SimpleDataChunk* timestamp;
        SimpleDataChunk* intervalDivisor;
        SimpleDataChunk* numberOfScalers;
        SimpleDataChunk* isIncremental;
        RingScalerItemValues* scalerValues; 
};

#endif
