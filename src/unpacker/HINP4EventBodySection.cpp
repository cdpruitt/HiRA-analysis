#include "../../include/unpacker/HINP4EventBodySection.h"

using namespace std;

HINP4EventBodySection::HINP4EventBodySection(std::string n) : CompositeDataChunk(n)
{
    channelID = new SimpleDataChunk("Channel ID Word", 2);
    channelID->add(Datum("Channel ID", 0xFFFF, 0));
    add(channelID);

    energyHG = new SimpleDataChunk("High-gain Energy Word", 2);
    energyHG->add(Datum("High-gain Energy", 0xFFFF, 0));
    add(energyHG);

    energyLG = new SimpleDataChunk("Low-gain Energy Word", 2);
    energyLG->add(Datum("Low-gain Energy", 0xFFFF, 0));
    add(energyLG);

    time = new SimpleDataChunk("Time Word", 2);
    time->add(Datum("Time", 0xFFFF, 0));
    add(time);
}
