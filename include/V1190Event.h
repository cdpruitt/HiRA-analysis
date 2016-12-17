/******************************************************************************
 * Represents event structure from a CAEN V1190 TDC module, which is as follows:
 *
 * ------------------------------------
 * Global header | 32-bit word
 *      geographic address | bits 0-4
 *      converted channels | bits 5-26
 *      word identifier    | bit 27-31
 * ------------------------------------
 * Event body:
 *
 *      TDC Header | 32-bit word
 *           Bunch ID   | bits 0-11
 *           Event ID   | bits 12-23
 *           TDC        | bits 24-25
 *           identifier    | bits 27-31
 *
 *      TDC Measurements | 32-bit word
 *           Measurement | bits 0-18
 *           Channel     | bits 19-25
 *           Trail/lead  | bit 26
 *           identifier    | bits 27-31
 *
 *      TDC Trailer | 32-bit word
 *           Word Count  | bits 0-11
 *           Event ID    | bits 12-23
 *           TDC         | bits 24-25
 *           identifier    | bits 27-31
 *           
 *      TDC Error | 32-bit word
 *           Error flags | bits 0-14
 *           TDC         | bits 24-25
 *           identifier    | bits 27-31
 * ------------------------------------
 * Extended trigger timetag | 32-bit word
 *      Extended trigger timetag | bits 0-26
 *      identifier    | bits 27-31
 * ------------------------------------
 * Trailer | 32-bit word
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

#include <vector>

class V1190Event : public CompositeDataChunk
{
    public:
        V1190Event(std::string n, unsigned int numberTDCs, bool hasExtendedTime);

    private:
        class GlobalBody : public CompositeDataChunk
        {
            public:
                GlobalBody(bool hasTDCHeader,
                           bool hasTDCMeasurement,
                           bool hasTDCTrailer,
                           bool hasTDCError) : CompositeDataChunk("V1190 Global Body")
                {
                    if(hasTDCHeader)
                    {
                        SimpleDataChunk* TDCHeader = new SimpleDataChunk("TDC Header", 4);

                        TDCHeader->add(Datum("Bunch ID", 0xFFF, 0));
                        TDCHeader->add(Datum("Event ID", 0xFFF, 12));
                        TDCHeader->add(Datum("TDC ID", 0x3, 24));
                        TDCHeader->add(Identifier("TDC Header identifier", 0x7, 24, 0x00001));

                        add(TDCHeader);
                    }

                    if(hasTDCMeasurement)
                    {
                        SimpleDataChunk* TDCMeasurement = new SimpleDataChunk("TDC Measurement", 4);

                        TDCMeasurement->add(Datum("Measurement", 0x7FFFF, 0));
                        TDCMeasurement->add(Datum("Channel number", 0x1FFF, 19));
                        TDCMeasurement->add(Datum("Trailing or leading", 0x1, 26));
                        TDCMeasurement->add(Identifier("TDC Measurement identifier", 0x7, 24, 0x00000));

                        add(TDCMeasurement);
                    }

                    if(hasTDCTrailer)
                    {
                        SimpleDataChunk* TDCTrailer = new SimpleDataChunk("TDC Trailer", 4);

                        TDCTrailer->add(Datum("Word count", 0xFFF, 0));
                        TDCTrailer->add(Datum("Event ID", 0xFFF, 12));
                        TDCTrailer->add(Datum("TDC ID", 0x3, 24));
                        TDCTrailer->add(Identifier("TDC Trailer identifier", 0x7, 24, 0x00011));

                        add(TDCTrailer);
                    }

                    if(hasTDCError)
                    {
                        SimpleDataChunk* TDCError = new SimpleDataChunk("TDC Error", 4);

                        TDCError->add(Datum("Error flags", 0x3FFF, 0));
                        TDCError->add(Datum("TDC ID", 0x3, 24));
                        TDCError->add(Identifier("TDC Error identifier", 0x7, 24, 0x00100));

                        add(TDCError);
                    }
                }

                void add(DataChunk* d)
                {
                    subChunks.push_back(d);
                }

                void extractData(std::ifstream& evtfile)
                {
                }
        };
};

#endif
