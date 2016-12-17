#include "../include/V785Event.h"
#include "../include/readData.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

bool V785Event::readEvent(ifstream& evtfile)
{
    // read header
    buffer.resize(header.size);
    readWord(evtfile, buffer);

    if(header.checkID(buffer[0]))
    {
        // matched header indicator - store header data from the buffer
        header.extractData(buffer[0]);
    }

    else
    {
        cerr << "Error: missing header for V785 Event." << endl;
        return false;
    }

    // read body
    buffer.resize(body.size);
    readWord(evtfile, buffer);

    if(body.checkID(buffer[0]))
    {
        // matched header indicator - store header data from the buffer
        body.extractData(buffer[0]);
    }

    else
    {
        cerr << "Error: missing body for V785 Event." << endl;
        return false;
    }
    
    // read trailer
    buffer.resize(trailer.size);
    readWord(evtfile, buffer);

    if(trailer.checkID(buffer[0]))
    {
        // matched header indicator - store header data from the buffer
        trailer.extractData(buffer[0]);
    }

    else
    {
        cerr << "Error: missing trailer for V785 Event." << endl;
        return false;
    }

    return true;
}
