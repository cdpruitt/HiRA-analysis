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

//unsigned int HINP4Event::getChannelsHit()
//{
//    return header->channelsHit->data[0].value;
//}

//void HINP4Event::branch(TTree*& tree)
//{
//    tree->Branch("HINP4EventBranch", treeVariables, "XLMMarker/i:channelsHit:channelID:energyHG:energyLG:time");
//}
