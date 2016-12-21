#include "../../include/unpacker/V1190EventBodySection.h"
#include "../../include/unpacker/Datum.h"
#include "../../include/unpacker/Identifier.h"

using namespace std;

V1190EventBodySection::V1190EventBodySection(std::string n,
        bool hasTDCHeader, bool hasTDCMeasurement,
        bool hasTDCTrailer, bool hasTDCError) : CompositeDataChunk(n)
{
    if(hasTDCHeader)
    {
        TDCHeader = new SimpleDataChunk("TDC Header", 4);

        TDCHeader->add(Datum("Bunch ID", 0xFFF, 0));
        TDCHeader->add(Datum("Event ID", 0xFFF, 12));
        TDCHeader->add(Datum("TDC ID", 0x3, 24));
        TDCHeader->add(Identifier("TDC Header identifier", 0x7, 24, 0x00001));

        add(TDCHeader);
    }

    if(hasTDCMeasurement)
    {
        TDCMeasurement = new SimpleDataChunk("TDC Measurement", 4);

        TDCMeasurement->add(Datum("Measurement", 0x7FFFF, 0));
        TDCMeasurement->add(Datum("Channel number", 0x1FFF, 19));
        TDCMeasurement->add(Datum("Trailing or leading", 0x1, 26));
        TDCMeasurement->add(Identifier("TDC Measurement identifier", 0x7, 24, 0x00000));

        add(TDCMeasurement);
    }

    if(hasTDCTrailer)
    {
        TDCTrailer = new SimpleDataChunk("TDC Trailer", 4);

        TDCTrailer->add(Datum("Word count", 0xFFF, 0));
        TDCTrailer->add(Datum("Event ID", 0xFFF, 12));
        TDCTrailer->add(Datum("TDC ID", 0x3, 24));
        TDCTrailer->add(Identifier("TDC Trailer identifier", 0x7, 24, 0x00011));

        add(TDCTrailer);
    }

    if(hasTDCError)
    {
        TDCError = new SimpleDataChunk("TDC Error", 4);

        TDCError->add(Datum("Error flags", 0x3FFF, 0));
        TDCError->add(Datum("TDC ID", 0x3, 24));
        TDCError->add(Identifier("TDC Error identifier", 0x7, 24, 0x00100));

        add(TDCError);
    }
}
