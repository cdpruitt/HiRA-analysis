#include "../../include/unpacker/VMUSBEvent.h"

using namespace std;

VMUSBEvent::VMUSBEvent(string n) : RingBodyItem(n)
{
    eventHeader = new SimpleDataChunk("VMUSB Event Header", 4);
    eventHeader->add(Datum("Number of Words in VMUSBEvent body", 0xFFF, 0));
    add(eventHeader);

    siliconEvent = new HINP4Event("HINP4 Event");
    add(siliconEvent);

    CsIADCEvent = new V785Event("V785 Event");
    add(CsIADCEvent);

    CsITDCEvent = new V1190Event("V1190 Event", NUMBER_OF_TDCS, HAS_EXTENDED_TIME);
    add(CsITDCEvent);
}
