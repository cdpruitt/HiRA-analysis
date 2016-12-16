/*******************************************************************************
 * Represents XLM-mediated event structure for the HINP4 chip system:
 *
 * Event header
 *     XLM Marker | 32-bit word
 *     Number of chip words | 32-bit word
 *     Number of channels | 32-bit word
 *     (10 bytes with unknown function)
 *
 * Event body
 * For each MB triggered:
 *     For each channel triggered:
 *         channelID | 32-bit word
 *         high-gain energy | 32-bit word
 *         low-gain energy | 32-bit word
 *         time | 32-bit word
 *
*******************************************************************************/

#ifndef HINP4_EVENT_H
#define HINP4_EVENT_H

#include "../include/event.h"

struct HINP4Event : public Event
{
    Quantity channelID("channel ID", 0xFFFFFFFF, 0);
    Quantity energyHG("energy, high gain", 0xFFFFFFFF, 0);
    Quantity energyLG("energy, low gain", 0xFFFFFFFF, 0);
    Quantity time("time", 0xFFFFFFFF, 0);

    vector<Quantity> eventQuantities = {channelID, energyHG, energyLG, time};

    size = eventQuantities.size();
}



#endif
