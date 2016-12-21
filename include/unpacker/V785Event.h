/******************************************************************************
 * Represents event structure from a CAEN V785 ADC module, which is as follows:
 *
 * ------------------------------------
 * Event header | uint32
 *      converted channels | bits 8-13
 *      crate              | bits 16-23
 *      header identifier  | bits 24-26
 *      geographic address | bits 27-31
 * ------------------------------------
 * Event data   | uint32
 *      ADC value          | bits 0-11
 *      overflow bit       | bit 12
 *      underflow bit      | bit 13
 *      channel #          | bits 16-20
 *      body identifier    | bits 24-26
 *      geographic address | bits 27-31
 * .
 * .
 * .
 * (event data block repeats, one for each converted channel)
 * ------------------------------------
 * End of block | uint 32
 *      Event counter      | bits 0-23
 *      trailer identifier | bits 24-26
 *      geographic address | bits 27-31
 *
 *****************************************************************************/

#ifndef ADC_EVENT_H
#define ADC_EVENT_H

#include "event.h"
#include "SimpleDataChunk.h"
#include "CompositeDataChunk.h"
#include "Identifier.h"
#include "Datum.h"
#include "V785Configuration.h"
#include "V785EventBody.h"

#include <vector>

class V785Event : public CompositeDataChunk
{
    public:
        V785Event(std::string n);
        unsigned int getChannelsHit();
        void extractData(ifstream& evtfile);
        void branch(TTree*& tree);
        void reset();

    private:
        SimpleDataChunk* header;
        V785EventBody* body;
        SimpleDataChunk* trailer;

        struct TreeVariables
        {
            unsigned int crateNumber;
            unsigned int geographicalAddress;
            std::vector<unsigned int> channelID;
            std::vector<unsigned int> ADCValue;
        };

        TreeVariables treeVariables;
};

#endif
