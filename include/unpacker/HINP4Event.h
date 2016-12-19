/*******************************************************************************
 * Represents XLM-mediated event structure for the HINP4 chip system:
 *
 * Event header
 *     XLM Marker | 16-bit word
 *     Number of chip words | 32-bit word
 *     Number of channels | 16-bit word
 *     XLM Timestemp | 64-bit word
 *
 * Event body
 * For each MB triggered:
 *     For each channel triggered:
 *         channelID | 16-bit word
 *         high-gain energy | 16-bit word
 *         low-gain energy | 16-bit word
 *         time | 16-bit word
 *
*******************************************************************************/

#ifndef HINP4_EVENT_H
#define HINP4_EVENT_H

#include "CompositeDataChunk.h"
#include "SimpleDataChunk.h"

class HINP4Event : public CompositeDataChunk
{
    public:
        HINP4Event(std::string n);
        unsigned int getChannelsHit();

    private:
        class Header : public CompositeDataChunk
        {
            public:
                Header(std::string n) : CompositeDataChunk(n)
                {
                    XLMMarker = new SimpleDataChunk("XLM Marker Word", 2);
                    XLMMarker->add(Datum("XLM Marker", 0xFFFF, 0));
                    add(XLMMarker);

                    wordsInEvent = new SimpleDataChunk("Words In Event Word", 4);
                    wordsInEvent->add(Datum("Words in Event", 0xFFFFFFFF, 0));
                    add(wordsInEvent);

                    channelsHit = new SimpleDataChunk("Channels Hit Word", 2);
                    channelsHit->add(Datum("Channels Hit", 0xFFFF, 0));
                    add(channelsHit);

                    XLMTimestamp = new SimpleDataChunk("XLM Timestamp Word", 2);
                    XLMTimestamp->add(Datum("XLM Timestamp", 0xFFFF, 0));
                    add(XLMTimestamp);
                }

                SimpleDataChunk* XLMMarker;
                SimpleDataChunk* wordsInEvent;
                SimpleDataChunk* channelsHit;
                SimpleDataChunk* XLMTimestamp;
        };

        Header* header;
        CompositeDataChunk* body;
        unsigned int channelsHit;
};

#endif
