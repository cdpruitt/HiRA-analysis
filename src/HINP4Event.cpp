#include "../include/HINP4Event.h"
#include "../include/readData.h"

#include <iostream>
#include <fstream>

using namespace std;

bool HINP4Event::readEvent(ifstream evtfile)
{
    // read event data from input file into buffer
    buffer.resize(size);
    readWord(evtFile, buffer);

    // store event data from each word in the buffer into the appropriate
    // quantity
    for(int i=0; i<buffer.size(); i++)
    {
        eventQuantities[i].read(buffer[i]);
    }

    return true;
}
