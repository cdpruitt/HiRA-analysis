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

#include "../include/event.h"
#include "../include/V1190Configuration.h"

#include <vector>

struct V1190Event : public Event
{
    V1190Event() : Event(globalHeader.size + NUMBER_OF_TDCS*globalBody.size +
           extendedTimeSection.size + globalTrailer.size) {}

    /*********************************************************************/
    struct GlobalHeader : public Section
    {
        GlobalHeader() : Section(Quantity("header identifier", 0x1F, 27, 0x01000),1),
                         geographicAddress("geographic address", 0x1F, 0),
                         eventCount("event count", 0x3FFFFF, 5)
                         {}

        Quantity geographicAddress;
        Quantity eventCount;

        void extractData(unsigned int word)
        {
            geographicAddress.read(word);
            eventCount.read(word);
        }
    };
    /*********************************************************************/

    struct GlobalBodySetup
    {
        GlobalBodySetup(int s) : size(s)
        {
            if(HAS_TDC_HEADER)
            {
                subsections.push_back(TDCHeader());
            }

            if(HAS_TDC_MEASUREMENT)
            {
                subsections.push_back(TDCMeasurement());
            }

            if(HAS_TDC_TRAILER)
            {
                subsections.push_back(TDCTrailer());
            }

            if(HAS_TDC_ERROR)
            {
                subsections.push_back(TDCError());
            }

            // detemine total size of body section
            for(Section subsection : subsections)
            {
                size += subsection.size;
            }
        }

        // Global Body sections
        struct TDCHeader : public Section
        {
            TDCHeader() : Section(Quantity("TDC Header identifier", 0x7, 24, 0x00001),1),
                          bunchID("Bunch ID", 0xFFF, 0),
                          eventID("Event ID", 0xFFF, 12),
                          TDCID("TDC ID", 0x3, 24)
                          {}

            Quantity bunchID;
            Quantity eventID;
            Quantity TDCID;

            void extractData(unsigned int word)
            {
                bunchID.read(word);
                eventID.read(word);
                TDCID.read(word);
            }
        };

        struct TDCMeasurement : public Section
        {
            TDCMeasurement() : Section(Quantity("TDC Measurement identifier", 0x7, 24, 0x00000),1),
                               measurement ("Measurement", 0x7FFFF, 0),
                               channel ("Channel number", 0x1FFF, 19),
                               trailOrLead("Trailing or leading", 0x1, 26)
            {}

            Quantity measurement;
            Quantity channel;
            Quantity trailOrLead;

            void extractData(unsigned int word)
            {
                measurement.read(word);
                channel.read(word);
                trailOrLead.read(word);
            }
        };

        struct TDCTrailer : public Section
        {
            TDCTrailer() : Section(Quantity("TDC Trailer identifier", 0x7, 24, 0x00011),1),
                                   wordCount("Word count", 0xFFF, 0),
                                   eventID("Event ID", 0xFFF, 12),
                                   TDCID("TDC ID", 0x3, 24)
                                   {}

            Quantity wordCount;
            Quantity eventID;
            Quantity TDCID;

            void extractData(unsigned int word)
            {
                wordCount.read(word);
                eventID.read(word);
                TDCID.read(word);
            }
        };

        struct TDCError : public Section
        {
            TDCError() : Section(Quantity("TDC Error identifier", 0x7, 24, 0x00100),1),
                                 errorFlags("Error flags", 0x3FFF, 0),
                                 TDCID("TDC ID", 0x3, 24)
            {}

            Quantity errorFlags;
            Quantity TDCID;

            void extractData(unsigned int word)
            {
                errorFlags.read(word);
                TDCID.read(word);
            }
        };

        std::vector<Section> subsections;

        unsigned int size;
    };

    /*********************************************************************/
    struct GlobalBody : public GlobalBodySetup
    {
        GlobalBody() : GlobalBodySetup(subsections.size())
        {}

        void extractData(std::vector<unsigned int> buffer)
        {
            for(int i=0; i<(int)buffer.size(); i++)
            {
                subsections[i].extractData(buffer[i]);
            }
        }
    };

    // store each GlobalBody in order (one GlobalBody per TDC)
    std::vector<GlobalBody> TDCData; 
    /*********************************************************************/

    /*********************************************************************/
    struct ExtendedTime : public Section
    {
        ExtendedTime() : Section(Quantity("Extended time identifier", 0x1F, 27, 0x10001),1),
                extendedTriggerTimetag("Extended trigger timetag", 0x7FFFFFF, 0)
        {}

        Quantity extendedTriggerTimetag;

        void extractData(unsigned int word)
        {
            extendedTriggerTimetag.read(word);
        }
    };
    /*********************************************************************/

    /*********************************************************************/
    struct GlobalTrailer : public Section
    {
        GlobalTrailer() : Section(Quantity("trailer identifier", 0x1F, 27, 0x10000),1),
                          geographicAddress("geographic address", 0x1F, 0),
                          wordCount("word count", 0xFFFF, 5),
                          triggerStatus("trigger status", 0x7, 24)
        {}

        Quantity geographicAddress;
        Quantity wordCount;
        Quantity triggerStatus;

        void extractData(unsigned int word)
        {
            geographicAddress.read(word);
            wordCount.read(word);
            triggerStatus.read(word);
        }

    };
    /*********************************************************************/

    GlobalHeader globalHeader;
    GlobalBody globalBody;
    GlobalTrailer globalTrailer;
    ExtendedTime extendedTimeSection; // optional?

    std::vector<unsigned int> buffer;

    virtual bool readEvent(std::ifstream& evtfile);
};

#endif
