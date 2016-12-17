#include "../include/DataChunk.h"
#include "../include/CompositeDataChunk.h"
#include "../include/V1190Event.h"
#include "../include/V1190Configuration.h"
#include "../include/readData.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

V1190Event::V1190Event(string n, unsigned int numberOfTDCs, bool hasExtendedTime) :
    CompositeDataChunk(n)
{
    // Create global header for event
    SimpleDataChunk* globalHeader = new SimpleDataChunk("V1190 Global Header",4);
    globalHeader->add(Datum("Geographic Address", 0x1F, 0));
    globalHeader->add(Datum("Event Count", 0x3FFFFF, 5));
    globalHeader->add(Identifier("Header Identifier", 0x1F, 27, 0x01000));

    add(globalHeader);

    // Create global body for event
    for(unsigned int i=0; i<numberOfTDCs; i++)
    {
        add(new GlobalBody(HAS_TDC_HEADER, HAS_TDC_MEASUREMENT,
                    HAS_TDC_TRAILER, HAS_TDC_ERROR));
    }

    // Create optional 'extended time' section
    if(hasExtendedTime)
    {
        SimpleDataChunk* extendedTime = new SimpleDataChunk("V1190 Extended Time",4);
        extendedTime->add(Datum("Extended trigger timetag", 0x7FFFFFF, 0));
        extendedTime->add(Identifier("Extended time identifier", 0x1F, 27, 0x10001));

        add(extendedTime);
    }

    // Create global trailer
    SimpleDataChunk* globalTrailer = new SimpleDataChunk("V1190 Global Trailer",4);
    globalTrailer->add(Datum("geographic address", 0x1F, 0));
    globalTrailer->add(Datum("word count", 0xFFFF, 5));
    globalTrailer->add(Datum("trigger status", 0x7, 24));
    globalTrailer->add(Identifier("trailer identifier", 0x1F, 27, 0x10000));

    add(globalTrailer);
}
