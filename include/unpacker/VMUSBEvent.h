/******************************************************************************
 * Represents event structure produced by VMUSBReadout, which is as follows: 
 *
 * ------------------------------------
 * Event header | uint16
 *      Number of 16-bit words to follow | bits 0-11
 * Event body | (variable number of uint16)
 *      Raw data as produced by VME event stack 
 *
 *****************************************************************************/

#ifndef VMUSB_EVENT_H
#define VMUSB_EVENT_H

#include "RingItemBody.h"
#include "SimpleDataChunk.h"
#include "HINP4Event.h"
#include "V785Event.h"
#include "V1190Event.h"

class VMUSBEvent : public RingItemBody
{
    public:
        VMUSBEvent(std::string n);
        ~VMUSBEvent() {}

        SimpleDataChunk* eventHeader;
        HINP4Event* siliconEvent;
        V785Event* CsIADCEvent;
        V1190Event* CsITDCEvent;
};

#endif
