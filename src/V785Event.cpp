#include "../include/V785Event.h"
#include "../include/SimpleDataChunk.h"
#include "../include/CompositeDataChunk.h"
#include "../include/readData.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

V785Event::V785Event(string n) : CompositeDataChunk(n)
{
    // Create header for event
    header = new SimpleDataChunk("V785 Header", 4);
    header->add(Datum("Channels Hit", 0x3f, 8));
    header->add(Datum("Crate Number", 0xFF, 16));
    header->add(Datum("Geographic Address", 0x1F, 27));
    header->add(Identifier("Header Identifier", 0x7, 24, 0x010));

    add(header);

    // Create body for event
    body = new CompositeDataChunk("V785 Body");
    for(unsigned int i=0; i<getChannelsHit(); i++)
    {
        SimpleDataChunk* channelData = new SimpleDataChunk("V785 Single Channel", 4);
        channelData->add(Datum("ADC Value", 0xFFF, 0));
        channelData->add(Datum("Overflow Bit", 0x1, 12));
        channelData->add(Datum("Underflow Bit", 0x1, 13));
        channelData->add(Datum("Channel ID", 0x1F, 16));
        channelData->add(Datum("Address of Module", 0x1F, 27));
        channelData->add(Identifier("Body Identifier", 0x7, 24, 0x0));

        body->add(channelData);
    }

    // Create trailer for event
    trailer = new SimpleDataChunk("V785 Trailer", 4);
    trailer->add(Datum("Event Counter", 0xFFFFFF, 0));
    trailer->add(Identifier("Trailer Identifier", 0x7, 24, 0x100));

    add(trailer);
}

unsigned int V785Event::getChannelsHit()
{
    for(Datum d : header->data)
    {
        if(d.name == "Channels Hit")
        {
            return d.value;
        }
    }

    cerr << "Error: couldn't find \"Channels Hit\" in event header." << endl;
    return 0;
}
