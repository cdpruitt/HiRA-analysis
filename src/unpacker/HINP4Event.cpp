#include "../../include/unpacker/HINP4Event.h"
#include "../../include/unpacker/HINP4EventHeader.h"
#include "../../include/unpacker/HINP4EventBody.h"
#include "../../include/unpacker/HINP4EventBodySection.h"
#include "../../include/unpacker/readData.h"

#include "../../include/unpacker/SimpleDataChunk.h"
#include "../../include/unpacker/Datum.h"

#include <iostream>
#include <fstream>

using namespace std;

HINP4Event::HINP4Event(string n) : CompositeDataChunk(n)
{
    // add header to event
    header = new HINP4EventHeader("HINP4 Header");
    add(header);

    // For each channel hit on this MB, add a BodySection to the event Body
    body = new HINP4EventBody("HINP4 Body");
    add(body);
}

void HINP4Event::extractData(ifstream& evtfile)
{
    header->extractData(evtfile);

    for(unsigned int i=0; i<getChannelsHit(); i++)
    {
        body->add(new HINP4EventBodySection("HINP4 Event Body Section"));
    }

    body->extractData(evtfile);

    treeVariables.XLMMarker =
         dynamic_cast<SimpleDataChunk*>(header->getSubChunks()[2])->getDataValue(0);

    std::vector<unsigned int> channelID;
    std::vector<unsigned int> energyHG;
    std::vector<unsigned int> energyLG;
    std::vector<unsigned int> time;

    for(DataChunk* channel : body->getSubChunks())
    {
        channelID.push_back(dynamic_cast<SimpleDataChunk*>(channel)->getDataValue(0));
        energyHG.push_back(dynamic_cast<SimpleDataChunk*>(channel)->getDataValue(1));
        energyLG.push_back(dynamic_cast<SimpleDataChunk*>(channel)->getDataValue(2));
        time.push_back(dynamic_cast<SimpleDataChunk*>(channel)->getDataValue(3));
    }

    treeVariables.channelID = channelID;
    treeVariables.energyHG = energyHG;
    treeVariables.energyLG = energyLG;
    treeVariables.time = time;
}

unsigned int HINP4Event::getChannelsHit()
{
    return dynamic_cast<SimpleDataChunk*>(header->getSubChunks()[2])->getDataValue(2);
}

void HINP4Event::branch(TTree*& tree)
{
    tree->Branch("HINP4EventBranch", &treeVariables, "XLMMarker/i:channelID:energyHG:energyLG:time");
}

void HINP4Event::reset()
{
    for(DataChunk* bodyPiece : body->getSubChunks())
    {
        delete bodyPiece;
    }
}
