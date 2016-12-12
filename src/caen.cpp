/******************************************************************************
 * caen.cpp reads an event from the CAEN ADC buffer
 *
 * The event structure for the CAEN V785 is as follows:
 *
 * ------------------------------------
 * Event header | uint32
 *      converted channels | bits 8-13
 *      crate              | bits 16-23
 *      indicator bit (0)  | bit 24
 *      indicator bit (1)  | bit 25
 *      indicator bit (0)  | bit 26
 *      geographic address | bits 27-31
 * ------------------------------------
 * Event data   | uint32
 *      ADC value          | bits 0-11
 *      overflow bit       | bit 12
 *      underflow bit      | bit 13
 *      channel #          | bits 16-20
 *      indicator bit (0)  | bit 24
 *      indicator bit (0)  | bit 25
 *      indicator bit (0)  | bit 26
 *      geographic address | bits 27-31
 * .
 * .
 * .
 * (event data block repeats, one for each converted channel)
 * ------------------------------------
 * End of block | uint 32
 *      Event counter      | bits 0-23
 *      indicator bit (0)  | bit 24
 *      indicator bit (0)  | bit 25
 *      indicator bit (1)  | bit 26
 *      geographic address | bits 27-31
 *
 *****************************************************************************/

#include "../include/caen.h"
#include <iostream>
#include <iomanip>

using namespace std;

//*********************************************************************
unsigned long* caen::read(unsigned long* point)
{
    // read header
    unsigned long header = *point++; // read bits 0-31 in header
    if((header & 0x07000000) != 0x02000000) // check for header indicator bits
    {
        // failed to find indicator bits - this is not an event header
        cout << "Error: missing header while reading ADC event. Skipping to next event..." << endl;
        return point;
    }

    channelsHit = header >> 8; // read total number of channels triggered for this event
    geographic = header >> 27; // read geographic address of digitizer
    crate = (header >> 19) & 0x7F; // read crate address of digitizer (legacy - should be corrected?)

    // read body
    for (int i=0;i<channelsHit;i++)
    {
        unsigned long body = *point++;
        if ((body & 0x07000000) != 0x0)
        {
            // failed to find indicator bits - this is not an event body
            cout << "Error: missing body while reading ADC event. Skipping to next event..." << endl;
            return point;
        }

        // extract body data for this channel
        ADCchannel c;
        c.ADCvalue = body & 0xFFF;
        c.overflow = (body >> 12) & 0x1;
        c.underflow = (body >> 13) & 0x1;
        c.channelNumber = (body >> 16) & 0x1F;

        // add channel data to event
        events.push_back(c);
    }

    // read footer
    unsigned long footer = *point++;

    if ((footer & 0x07000000) != 0x04000000)
    {
        // failed to find indicator bits - this is not an event footer
        cout << "Error: missing footer while reading ADC event. Skipping to next event..." << endl;
        return point;
    }

    return point;
}
