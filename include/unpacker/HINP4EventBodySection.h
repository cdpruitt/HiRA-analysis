#ifndef HINP4_EVENT_BODY_SECTION_H
#define HINP4_EVENT_BODY_SECTION_H

#include "CompositeDataChunk.h"
#include "SimpleDataChunk.h"

class HINP4EventBodySection : public CompositeDataChunk
{
    public:
        HINP4EventBodySection(std::string n);

    private:
        SimpleDataChunk* channelID;
        SimpleDataChunk* energyHG;
        SimpleDataChunk* energyLG;
        SimpleDataChunk* time;
};

#endif
