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

#ifndef RING_EVENT_H
#define RING_EVENT_H

#include "../include/event.h"

struct RingEvent : public Event
{
    struct EventHeader
    {
        Quantity sizeOfEvent;
        Quantity eventType;
    }

    struct BodyHeader
    {
    }

    struct EventData
    {
    }

    EventHeader eh;
    BodyHeader bh;
    EventData ed;

    size = eh.size + bh.size + ed.size;
}

#endif
