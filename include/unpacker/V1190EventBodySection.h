#ifndef V1190_EVENT_BODY_SECTION_H
#define V1190_EVENT_BODY_SECTION_H

#include "CompositeDataChunk.h"
#include "SimpleDataChunk.h"

class V1190EventBodySection : public CompositeDataChunk
{
    public:
        V1190EventBodySection(std::string n,
                bool hasTDCHeader, bool hasTDCMeasurement,
                bool hasTDCTrailer, bool hasTDCError);

        SimpleDataChunk* TDCHeader;
        SimpleDataChunk* TDCMeasurement;
        SimpleDataChunk* TDCTrailer;
        SimpleDataChunk* TDCError;
};

#endif
