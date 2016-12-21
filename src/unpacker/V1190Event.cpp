#include "../../include/unpacker/DataChunk.h"
#include "../../include/unpacker/CompositeDataChunk.h"
#include "../../include/unpacker/V1190Event.h"
#include "../../include/unpacker/V1190Configuration.h"
#include "../../include/unpacker/readData.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

V1190Event::V1190Event(string n) : CompositeDataChunk(n)
{
    // Create global header for event
    header = new SimpleDataChunk("V1190 Global Header",4);
    header->add(Datum("Geographic Address", 0x1F, 0));
    header->add(Datum("Event Count", 0x3FFFFF, 5));
    header->add(Identifier("Header Identifier", 0x1F, 27, 0x01000));
    add(header);

    // Create global body for event
    body = new V1190EventBody("V1190 Global Body");
    add(body);

    // Optionally create 'extended time' section
    if(HAS_EXTENDED_TIME)
    {
        extendedTime = new SimpleDataChunk("V1190 Extended Time",4);
        extendedTime->add(Datum("Extended trigger timetag", 0x7FFFFFF, 0));
        extendedTime->add(Identifier("Extended time identifier", 0x1F, 27, 0x10001));

        add(extendedTime);
    }

    // Create global trailer
    trailer = new SimpleDataChunk("V1190  Trailer",4);
    trailer->add(Datum("geographic address", 0x1F, 0));
    trailer->add(Datum("word count", 0xFFFF, 5));
    trailer->add(Datum("trigger status", 0x7, 24));
    trailer->add(Identifier("trailer identifier", 0x1F, 27, 0x10000));
    add(trailer);
}
