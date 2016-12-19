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

#include "../include/event.h"

class HINP4Event : public CompositeDataChunk
{
    public:
        HINP4Event(std::string n);

    private:
        void extractData(std::ifstream& evtfile);
}

#endif
