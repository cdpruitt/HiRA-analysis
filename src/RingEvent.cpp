#include "../include/RingEvent.h"
#include "../include/readData.h"

#include <iostream>
#include <fstream>

using namespace std;

bool RingEvent::readEvent(ifstream evtfile)
{
    // read event header data from input file input buffer
    buffer.resize(eh.size);
    readWord(evtFile.buffer);

    eh.extractData(buffer[0]);

    // read body header data from input file input buffer
    buffer.resize(bh.size);
    readWord(evtFile.buffer);

    bh.extractData(buffer[0]);

    // read event data from input file input buffer
    buffer.resize(ed.size);
    readWord(evtFile.buffer);

    ed.extractData(buffer[0]);

    // process each section of the event
    return true;
}
