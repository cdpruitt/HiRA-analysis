#include "../include/branch.h"

using namespace std;

void branchADC(TTree*& tree, ADCVariables& ADC)
{
    tree->Branch("ADCEvent", &ADC, "crate/I:geographicalAddress:channelID:ADCValue");
}

void branchTDC(TTree*& tree, TDCVariables& TDC)
{
    tree->Branch("TDCEvent", &TDC, "geographicalAddress/I:channelID:TDCValue");
}

void branchHINP4(TTree*& tree, HINP4Variables& HINP4)
{
    tree->Branch("HINP4Event", &HINP4, "XLMMarker/I:channelsHit:channelID:energyHG:energyLG:time");
}

void branch(TTree*& tree, TreeVariables& treeVariables)
{
    branchADC(tree, treeVariables.adc);
    branchTDC(tree, treeVariables.tdc);
    branchHINP4(tree, treeVariables.hinp4);
}
