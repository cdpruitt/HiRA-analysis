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

#include "event.h"
#include "SimpleDataChunk.h"
#include "CompositeDataChunk.h"
#include "Identifier.h"
#include "Datum.h"
#include "V785Configuration.h"

#include <vector>

class V785Event : public CompositeDataChunk
{
    public:
        V785Event(std::string n);
        unsigned int getChannelsHit();

    private:
        SimpleDataChunk* header;
        CompositeDataChunk* body;
        SimpleDataChunk* trailer;
};

#endif
