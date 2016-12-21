#include "../../include/unpacker/RingEventCountItem.h"
#include "../../include/unpacker/SimpleDataChunk.h"
#include "../../include/unpacker/Datum.h"

using namespace std;

RingEventCountItem::RingEventCountItem(string n) : RingItemBody(n)
{
    // Create word structure for an Event Count item
    timeOffset = new SimpleDataChunk("Time Offset Word", 4);
    timeOffset->add(Datum("Time Offset", 0xFFFF, 0));
    add(timeOffset);

    offsetDivisor = new SimpleDataChunk("Offset Divisor Word", 4);
    offsetDivisor->add(Datum("Offset Divisor", 0xFFFF, 0));
    add(offsetDivisor);

    timestamp = new SimpleDataChunk("Timestamp Word", 4);
    timestamp->add(Datum("Timestamp", 0xFFFF, 0));
    add(timestamp);

    eventCount = new SimpleDataChunk("Event Count Word", 8);
    eventCount->add(Datum("Event Count", 0xFFFF, 0));
    add(eventCount);
}
