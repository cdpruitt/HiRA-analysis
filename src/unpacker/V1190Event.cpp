#include "../../include/unpacker/DataChunk.h"
#include "../../include/unpacker/CompositeDataChunk.h"
#include "../../include/unpacker/V1190Event.h"
#include "../../include/unpacker/V1190EventBodySection.h"
#include "../../include/unpacker/V1190Configuration.h"
#include "../../include/unpacker/readData.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

V1190Event::V1190Event(string n) : CompositeDataChunk(n)
{
    // Create global header for event
    header = new SimpleDataChunk("V1190 Global Header",4);
    header->add(Datum("Geographic Address", 0x1F, 0));
    header->add(Datum("Event Count", 0x3FFFFF, 5));
    header->add(Identifier("Header Identifier", 0x1F, 27, 0x01000));
    add(header);

    // Create global body for event
    body = new V1190EventBody("V1190 Global Body");
    add(body);

    // Optionally create 'extended time' section
    if(HAS_EXTENDED_TIME)
    {
        extendedTime = new SimpleDataChunk("V1190 Extended Time",4);
        extendedTime->add(Datum("Extended trigger timetag", 0x7FFFFFF, 0));
        extendedTime->add(Identifier("Extended time identifier", 0x1F, 27, 0x10001));

        add(extendedTime);
    }

    // Create global trailer
    trailer = new SimpleDataChunk("V1190  Trailer",4);
    trailer->add(Datum("geographic address", 0x1F, 0));
    trailer->add(Datum("word count", 0xFFFF, 5));
    trailer->add(Datum("trigger status", 0x7, 24));
    trailer->add(Identifier("trailer identifier", 0x1F, 27, 0x10000));
    add(trailer);
}

void V1190Event::extractData(ifstream& evtfile)
{
    header->extractData(evtfile);

    // populate body with appropriate number of channels hit
    for(unsigned int i=0; i<getChannelsHit(); i++)
    {
        body->add(new V1190EventBodySection("V1190 Event Body Section",
                    HAS_TDC_HEADER, HAS_TDC_MEASUREMENT,
                    HAS_TDC_TRAILER, HAS_TDC_ERROR));
    }

    body->extractData(evtfile);

    if(HAS_EXTENDED_TIME)
    {
        extendedTime->extractData(evtfile);
    }

    trailer->extractData(evtfile);

    // store data in tree variables for extraction by tree
    treeVariables.geographicalAddress = header->getDataValue(0);

    vector<unsigned int> channelID;
    vector<unsigned int> TDCValue;

    for(DataChunk* channel : body->getSubChunks())
    {
        SimpleDataChunk* TDCMeasurement =
            dynamic_cast<SimpleDataChunk*>(
            dynamic_cast<CompositeDataChunk*>(channel)->getSubChunks()[1]);
        channelID.push_back(TDCMeasurement->getDataValue(1));
        TDCValue.push_back(TDCMeasurement->getDataValue(0));
    }

    treeVariables.channelID = channelID;
    treeVariables.TDCValue = TDCValue;
}

unsigned int V1190Event::getChannelsHit()
{
    return header->getDataValue(1);
}

void V1190Event::branch(TTree*& tree)
{
    tree->Branch("V1190Event",&treeVariables,"geographicalAddress/I:channelID:TDCValue");
}

void V1190Event::reset()
{
    for(DataChunk* bodyPiece : body->getSubChunks())
    {
        delete bodyPiece;
    }
}
