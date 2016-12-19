#include "../../include/unpacker/HINP4Event.h"
#include "../../include/unpacker/readData.h"

#include "../../include/unpacker/SimpleDataChunk.h"
#include "../../include/unpacker/Datum.h"

#include <iostream>
#include <fstream>

using namespace std;

HINP4Event::HINP4Event(string n) : CompositeDataChunk(n)
{
    // add header to event
    header = new Header("HINP4 Header");
    add(header);

    // For each channel hit on this MB
    body = new CompositeDataChunk("HINP4 body");

    for(int j=0; j<getChannelsHit(); j++)
    {
        // add a body section for this hit channel
        SimpleDataChunk* channelID = new SimpleDataChunk("Channel ID Word", 2);
        channelID->add(Datum("Channel ID", 0xFFFF, 0));
        body->add(channelID);

        SimpleDataChunk* energyHG = new SimpleDataChunk("High-gain Energy Word", 2);
        energyHG->add(Datum("High-gain Energy", 0xFFFF, 0));
        body->add(energyHG);

        SimpleDataChunk* energyLG = new SimpleDataChunk("Low-gain Energy Word", 2);
        energyLG->add(Datum("Low-gain Energy", 0xFFFF, 0));
        body->add(energyLG);

        SimpleDataChunk* time = new SimpleDataChunk("Time Word", 2);
        time->add(Datum("Time", 0xFFFF, 0));
        body->add(time);

        add(body);
    }
}

unsigned int HINP4Event::getChannelsHit()
{
    return header->channelsHit->data[0].value;
}
