#include "../../include/unpacker/RingItem.h"
#include "../../include/unpacker/SimpleDataChunk.h"
#include "../../include/unpacker/CompositeDataChunk.h"
#include "../../include/unpacker/RingStateChangeItem.h"
#include "../../include/unpacker/RingScalerItem.h"
#include "../../include/unpacker/VMUSBEvent.h"
#include "../../include/unpacker/RingEventCountItem.h"

#include <iostream>
#include <fstream>

using namespace std;

RingItem::RingItem(string n) : CompositeDataChunk(n)
{
    // Create event header for event
    eventHeader = new CompositeDataChunk("Ring Item Event Header");

    SimpleDataChunk* inclusiveSize = new SimpleDataChunk("Inclusive Size Word", 4);
    inclusiveSize->add(Datum("Inclusive Size", 0xFFFFFFFF, 0));
    eventHeader->add(inclusiveSize);

    SimpleDataChunk* eventType = new SimpleDataChunk("Event Type Word", 4);
    eventType->add(Datum("Event Type", 0xFFFFFFFF, 0));
    eventHeader->add(eventType);

    add(eventHeader);

    // Create body header for event
    bodyHeader = new CompositeDataChunk("Ring Item Body Header");

    SimpleDataChunk* bodyHeaderSize = new SimpleDataChunk("Body Header Inclusive Size Word", 4);
    bodyHeaderSize->add(Datum("Body Header Inclusive Size", 0xFFFFFFFF, 0));
    bodyHeader->add(bodyHeaderSize);

    // Optional body header components
    if(BODY_HEADER_TIMESTAMP)
    {
        SimpleDataChunk* bodyHeaderTimestamp = new SimpleDataChunk("Body Header Timestamp Word", 8);
        bodyHeaderTimestamp->add(Datum("Body Header Timestamp", 0xFFFFFFFFFFFFFFFF, 0));
        bodyHeader->add(bodyHeaderTimestamp);
    }

    if(BODY_HEADER_SOURCE_ID)
    {
        SimpleDataChunk* bodyHeaderSourceID = new SimpleDataChunk("Body Header Source ID Word", 4);
        bodyHeaderSourceID->add(Datum("Body Header Source ID", 0xFFFFFFFF, 0));
        bodyHeader->add(bodyHeaderSourceID);
    }

    if(BODY_HEADER_BARRIER_TYPE)
    {
        SimpleDataChunk* bodyHeaderBarrierType = new SimpleDataChunk("Body Header Barrier Type Word", 4);
        bodyHeaderBarrierType->add(Datum("Body Header Barrier Type", 0xFFFFFFFF, 0));
        bodyHeader->add(bodyHeaderBarrierType);
    }

    add(bodyHeader);

    // create event data for event
    eventData = new RingBodyItem("Ring Item Body");
    add(eventData);
}

void RingItem::extractData(ifstream& evtfile)
{
    eventHeader->extractData(evtfile);

    string name; // for naming the event body type

    switch(type)
    {
        case 1: 
            name = "Begin Run Item";
        case 2:
            name = "End Run Item";
        case 3:
            name = "Pause Run Item";
        case 4:
            name = "Resume Run Item";

            eventData = new RingStateChangeItem(name);
            break;

        case 20:
            name = "Scaler Item";
            eventData = new RingScalerItem(name);
            break;

        case 30:
            name = "Physics Event Item";
            eventData = new VMUSBEvent(name);
            break;

        case 31:
            name = "Event Counter Item";
            eventData = new RingEventCountItem(name);
            break;

        default:
            cout << "Error: encountered unknown evtType " << type << ". Exiting..." << endl;
            return;
    }

    eventData->extractData(evtfile);
}

unsigned int RingItem::getType()
{
    return type;
}
