#include "../../include/unpacker/RingItemBody.h"
#include "../../include/unpacker/RingStateChangeItem.h"

#include "../../include/unpacker/CompositeDataChunk.h"
#include "../../include/unpacker/SimpleDataChunk.h"
#include "../../include/unpacker/Datum.h"

using namespace std;

RingStateChangeItem::RingStateChangeItem(string n) : RingItemBody(n)
{
    // Create word structure for a State Change Item
    runNumber = new SimpleDataChunk("Run Number Word", 4);
    runNumber->add(Datum("Run Number", 0xFFFF, 0));
    add(runNumber);

    timeOffset = new SimpleDataChunk("Time Offset Word", 4);
    timeOffset->add(Datum("Time OFfset", 0xFFFF, 0));
    add(timeOffset);
    
    timeStamp = new SimpleDataChunk("Timestamp Word", 4);
    timeStamp->add(Datum("Timestamp", 0xFFFF, 0));
    add(timeStamp);
    
    offsetDivisor = new SimpleDataChunk("Offset Divisor Word", 4);
    offsetDivisor->add(Datum("Offset Divisor", 0xFFFF, 0));
    add(offsetDivisor);

    runTitle = new SimpleDataChunk("Run Title Word", 0);
    runTitle->add(Datum("Run Title", 0xFFFF, 80));
    add(runTitle);

}

