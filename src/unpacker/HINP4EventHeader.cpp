#include "../../include/unpacker/HINP4EventHeader.h"

using namespace std;

HINP4EventHeader::HINP4EventHeader(std::string n) : CompositeDataChunk(n)
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
