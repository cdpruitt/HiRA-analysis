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
    eventHeader = new RingItemHeader("Ring Item Event Header");
    add(eventHeader);

    // Create body header for event
    bodyHeader = new RingItemBodyHeader("Ring Item Body Header");
    add(bodyHeader);

    // create event data for event
    eventData = new RingItemBody("Ring Item Body");
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
