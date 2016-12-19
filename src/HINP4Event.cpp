#include "../include/HINP4Event.h"
#include "../include/readData.h"

#include <iostream>
#include <fstream>

using namespace std;

void HINP4Event::HINP4Event(ifstream evtfile)
{
    SimpleDataChunk* header = new SimpleDataChunk("HINP4 header");
    header->add(Datum("XLM Marker", 0xFFFF, 0));
    header->add(Datum("Words in Event", 0xFFFFFFFF, 0));
    header->add(Datum("Channels Hit", 0xFFFF, 0));
    header->add(Datum("XLM Timestamp", 0xFFFFFFFFFFFFFFFF,0));

    add(header);

    // For each channel hit on this MB
    for(int j=0; j<getChannelsHit(); j++)
    {
        // add a body section for this hit channel
        SimpleDataChunk* body = new SimpleDataChunk("HINP4 Body");
        body->add(Datum("Channel ID", 0xFFFF, 0));
        body->add(Datum("High-gain Energy", 0xFFFF, 0));
        body->add(Datum("Low-gain Energy", 0xFFFF, 0));
        body->add(Datum("Time", 0xFFFF, 0));

        add(body);
    }
}
