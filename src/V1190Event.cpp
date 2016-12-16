#include "../include/V1190Configuration.h"
#include "../include/V1190Event.h"
#include "../include/readData.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

bool V1190Event::readEvent(ifstream& evtfile)
{
    // read header data from input file into buffer
    buffer.resize(globalHeader.size);
    readWord(evtfile, buffer);

    if(globalHeader.checkID(buffer[0]))
    {
        // matched header indicator - store header data from the buffer
        globalHeader.extractData(buffer[0]);
    }

    else
    {
        cerr << "Error: missing global header to V1190 Event." << endl;
        return false;
    }

    // read one or more global bodies from filestream
    buffer.resize(globalBody.size);
    readWord(evtfile, buffer);

    // if this word is of the global body type, read into a global body event
    // each TDC has its own global body
    for(int i=0; i<NUMBER_OF_TDCS; i++)
    {
        globalBody.extractData(buffer);
    }

    // read footer
    buffer.resize(globalTrailer.size);
    readWord(evtfile, buffer);

    if(globalTrailer.checkID(buffer[0]))
    {
        globalTrailer.extractData(buffer[0]);
    }

    else
    {
        cerr << "Error: missing global footer to V1190 Event." << endl;
        return false;
    }

    return true;
}
