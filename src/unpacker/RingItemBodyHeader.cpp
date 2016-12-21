#include "../../include/unpacker/RingItemBodyHeader.h"
#include "../../include/unpacker/RingItem.h"

using namespace std;

RingItemBodyHeader::RingItemBodyHeader(string n) : CompositeDataChunk(n)
{
    bodyHeaderSize = new SimpleDataChunk("Body Header Inclusive Size Word", 4);
    bodyHeaderSize->add(Datum("Body Header Inclusive Size", 0xFFFFFFFF, 0));
    add(bodyHeaderSize);

    // Optional body header components
    if(BODY_HEADER_TIMESTAMP)
    {
        bodyHeaderTimestamp = new SimpleDataChunk("Body Header Timestamp Word", 8);
        bodyHeaderTimestamp->add(Datum("Body Header Timestamp", 0xFFFFFFFFFFFFFFFF, 0));
        add(bodyHeaderTimestamp);
    }

    if(BODY_HEADER_SOURCE_ID)
    {
        bodyHeaderSourceID = new SimpleDataChunk("Body Header Source ID Word", 4);
        bodyHeaderSourceID->add(Datum("Body Header Source ID", 0xFFFFFFFF, 0));
        add(bodyHeaderSourceID);
    }

    if(BODY_HEADER_BARRIER_TYPE)
    {
        bodyHeaderBarrierType = new SimpleDataChunk("Body Header Barrier Type Word", 4);
        bodyHeaderBarrierType->add(Datum("Body Header Barrier Type", 0xFFFFFFFF, 0));
        add(bodyHeaderBarrierType);
    }
}
