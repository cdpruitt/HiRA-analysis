/******************************************************************************
 * Represents event structure from a CAEN V785 ADC module, which is as follows:
 *
 * ------------------------------------
 * Event header | uint32
 *      converted channels | bits 8-13
 *      crate              | bits 16-23
 *      word indicator     | bits 24-26
 *      geographic address | bits 27-31
 * ------------------------------------
 * Event data   | uint32
 *      ADC value          | bits 0-11
 *      overflow bit       | bit 12
 *      underflow bit      | bit 13
 *      channel #          | bits 16-20
 *      word indicator     | bits 24-26
 *      geographic address | bits 27-31
 * .
 * .
 * .
 * (event data block repeats, one for each converted channel)
 * ------------------------------------
 * End of block | uint 32
 *      Event counter      | bits 0-23
 *      word indicator     | bits 24-26
 *      geographic address | bits 27-31
 *
 *****************************************************************************/

#ifndef ADC_EVENT_H
#define ADC_EVENT_H

#include "../include/event.h"

#include <vector>

struct V785Event : public Event
{
    V785Event() : Event(1/*header.size + body.size + footer.size*/) {}

    /*********************************************************************
    // Header data
     *********************************************************************/

    struct Header : public Section
    {
        Header() : Section(Quantity("header identifier", 0x7, 24, 0x010),1),
                   channelsHit("channels hit this event", 0x3f, 8),
                   crateNumber("crate number of module", 0xFF, 16),
                   geographicAddress("address of module", 0x1F, 27)
        {}


        Quantity channelsHit;
        Quantity crateNumber;
        Quantity geographicAddress;

        void extractData(unsigned int word)
        {
            channelsHit.read(word);
            crateNumber.read(word);
            geographicAddress.read(word);
        }
    };

    /*********************************************************************
    // Body data: one per channel triggered during this event
     *********************************************************************/

    struct Body : public Section
    {
        Body() : Section(Quantity("body identifier", 0x7, 24, 0x0),1),
                 ADCValue("ADC value", 0xFFF, 0),
                 overflow("overflow bit", 0x1, 12),
                 underflow("underflow bit", 0x1, 13),
                 channelID("channel ID", 0x1F, 16),
                 geographicAddress("address of module", 0x1F, 27)
        {}

        Quantity ADCValue;
        Quantity overflow;
        Quantity underflow;
        Quantity channelID;
        Quantity geographicAddress;

        // read quantities for a single channel from a word
        void extractData(unsigned int word)
        {
            ADCValue.read(word);
            overflow.read(word);
            underflow.read(word);
            channelID.read(word);
            geographicAddress.read(word);
        }
    };

    // keep track of each body section in the order read out by the ADC
    std::vector<Body> bodyData; 

    /*********************************************************************
    // Trailer
     *********************************************************************/

    struct Trailer : public Section
    {
        Trailer() : Section(Quantity("trailer identifier", 0x7, 24, 0x100),1),
                    eventCounter("Event counter", 0xFFFFFF, 0)
        {}

        Quantity eventCounter;

        void extractData(unsigned int word)
        {
            eventCounter.read(word);
        }
    };

    Header header;
    Body body;
    Trailer trailer;

    std::vector<unsigned int> buffer;

    virtual bool readEvent(std::ifstream& evtfile);
};

#endif
