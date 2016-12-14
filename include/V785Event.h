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

class V785Event : public Event
{
    public:
        /*********************************************************************
        // Header data
         *********************************************************************/

        // number of channels triggered on this event
        Quantity channelsHit{"channels hit", 0x3F, 8};

        // crate number of module
        Quantity crateNumber{"crate number", 0xFF, 16};

        // unique identifier for header words
        Quantity headerIndicator{"header indicator", 0x7, 24, 0x010};

        // geographic address of module
        Quantity moduleAddress = {"address of module", 0x1F, 27};

        /*********************************************************************
        // Body data
         *********************************************************************/

        // unique indicator for body words
        Quantity bodyIndicator = {"body indicator", 0x7, 24, 0x0};

        class singleChannelData
        {
            public:
                // value of ADC for this channel
                Quantity ADCValue = {"ADC value", 0xFFF, 0};

                // indicates that value exceeded ADC range
                Quantity overflow = {"overflow bit", 0x1, 12};

                // indicates that value was below ADC range
                Quantity underflow = {"underflow bit", 0x1, 13};

                // this channel's number, starting from 0
                Quantity channelID = {"channel ID", 0x1F, 16};

                // geographic address of module
                Quantity moduleAddress = {"address of module", 0x1F, 27};

                // read quantities for a single channel from a word
                singleChannelData(unsigned int word)
                {
                    ADCValue.read(word);
                    overflow.read(word);
                    underflow.read(word);
                    channelID.read(word);
                }
        };

        // keep track of each channel's data, in order read out by the ADC
        std::vector<singleChannelData> allChannelsData; 

        /*********************************************************************
        // Footer data for an ADC event
         *********************************************************************/

        // Keep track of the total number of events outputted by the ADC
        Quantity eventCounter = {"Event counter", 0xFFFFFF, 0};

        // unique indicator for footer words
        Quantity footerIndicator = {"footer indicator", 0x7, 24, 0x100};

        bool readEvent(std::ifstream& evtfile);
};

#endif
