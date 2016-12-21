/*******************************************************************************
 * Represents a generic ringbuffer event
 *
 * Event header
 *    Size of event (inclusive, in bytes) | 32-bit word
 *    Event type | 32-bit word
 *    (16 bits of 'event type' are 0, defining the event's word-byte ordering)
 *
 * Body header (introduced in NSCLDAQ 11.0 - not present in earlier versions)
 *   Size | 32-bit word (inclusive size of body header, in bytes)
 *   Timestamp | 64-bit word (time of ring buffer initial formation)
 *   Source ID | 32-bit word (unique ID of ring source)
 *   Barrier Type | (barrier type - see NSCLDAQ docs for details)
 *   (Note: all items after 'size' in the body header are optional. If they're
 *   not present, size will be '0x0')
 *
 * Event data (defined by user configuration)
 *   VMUSB, CCUSB, SBS, or Eventbuilt data | variable size
 *   (see NSCLDAQ docs for details on each of these data-generating components)
 *
*******************************************************************************/

#ifndef RING_ITEM_H
#define RING_ITEM_H

#include "CompositeDataChunk.h"
#include "SimpleDataChunk.h"
#include "RingItemHeader.h"
#include "RingItemBodyHeader.h"
#include "RingItemBody.h"

const bool BODY_HEADER_TIMESTAMP = false;
const bool BODY_HEADER_SOURCE_ID = false;
const bool BODY_HEADER_BARRIER_TYPE = false;

class RingItem : public CompositeDataChunk
{
    public:
        RingItem(std::string n);
        unsigned int getType();
        void extractData(std::ifstream& evtfile);

        RingItemHeader* eventHeader;
        RingItemBodyHeader* bodyHeader;
        RingItemBody* eventData;
        unsigned int type;
};

#endif
