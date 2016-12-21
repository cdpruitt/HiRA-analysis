/******************************************************************************
 * Represents event structure from a CAEN V1190 TDC module, which is as follows:
 *
 * ------------------------------------
 * Event header | 32-bit word
 *      geographic address | bits 0-4
 *      converted channels | bits 5-26
 *      word identifier    | bit 27-31
 * ------------------------------------
 * Event body (one per converted channel)
 *
 *      TDC Header | 32-bit word
 *           Bunch ID   | bits 0-11
 *           Event ID   | bits 12-23
 *           TDC        | bits 24-25
 *           identifier | bits 27-31
 *
 *      TDC Measurements | 32-bit word
 *           Measurement | bits 0-18
 *           Channel     | bits 19-25
 *           Trail/lead  | bit 26
 *           identifier  | bits 27-31
 *
 *      TDC Trailer | 32-bit word
 *           Word Count  | bits 0-11
 *           Event ID    | bits 12-23
 *           TDC         | bits 24-25
 *           identifier  | bits 27-31
 *           
 *      TDC Error | 32-bit word
 *           Error flags | bits 0-14
 *           TDC         | bits 24-25
 *           identifier  | bits 27-31
 * ------------------------------------
 * Extended trigger timetag | 32-bit word
 *      Extended trigger timetag | bits 0-26
 *      identifier    | bits 27-31
 * ------------------------------------
 * Event Trailer | 32-bit word
 *      geographical address | bits 0-4
 *      word count           | bits 5-20
 *      trigger status       | bits 24-26
 *      identifier    | bits 27-31
 *
 *****************************************************************************/

#ifndef V1190_EVENT_H
#define V1190_EVENT_H

#include "SimpleDataChunk.h"
#include "CompositeDataChunk.h"
#include "Identifier.h"
#include "Datum.h"
#include "V1190Configuration.h"
#include "V1190EventBody.h"

#include <vector>

class V1190Event : public CompositeDataChunk
{
    public:
        V1190Event(std::string n);
        void extractData(ifstream& evtfile);
        unsigned int getChannelsHit();
        void branch(TTree*& tree);
        void reset();

    private:
        SimpleDataChunk* header;
        V1190EventBody* body;
        SimpleDataChunk* extendedTime;
        SimpleDataChunk* trailer;

        struct TreeVariables
        {
            unsigned int geographicalAddress;
            std::vector<unsigned int> channelID;
            std::vector<unsigned int> TDCValue;
        };

        TreeVariables treeVariables;
};

#endif
