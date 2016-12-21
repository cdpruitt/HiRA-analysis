#include "../../include/unpacker/RingItemHeader.h"

using namespace std;

RingItemHeader::RingItemHeader(string n) : CompositeDataChunk(n)
{
    inclusiveSize = new SimpleDataChunk("Inclusive Size Word", 4);
    inclusiveSize->add(Datum("Inclusive Size", 0xFFFFFFFF, 0));
    add(inclusiveSize);

    eventType = new SimpleDataChunk("Event Type Word", 4);
    eventType->add(Datum("Event Type", 0xFFFFFFFF, 0));
    add(eventType);
}
