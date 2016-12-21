#ifndef TREE_VARIABLES_H
#define TREE_VARIABLES_H

// define ADC representation in ROOT tree
struct TreeVariables
{
    TreeVariables(RingItem ri) : ringItem(ri) {}

    struct ADCVariables
    {
        unsigned int crate;
        unsigned int geographicalAddress;
        vector<unsigned int> channelID;
        vector<unsigned int> ADCValue;

        void fillEvent()
        {
            crate = ringItem->eventData->CsIADCEvent->header->crateNumber.value;
            geographicalAddress = ;
            channelID = ;
            ADCValue = ;
        }
    };

    // define TDC representation in ROOT tree
    struct TDCVariables
    {
        unsigned int geographicalAddress;
        vector<unsigned int> channelID;
        vector<unsigned int> TDCValue;

        void fillEvent()
        {
            geographicalAddress = ;
            channelID = ;
            TDCValue = ;
        }
    };

    // define HINP4 representation in ROOT tree
    struct HINP4Variables
    {
        unsigned int XLMMarker;
        unsigned int channelsHit;
        vector<unsigned int> channelID;
        vector<unsigned int> energyHG;
        vector<unsigned int> energyLG;
        vector<unsigned int> time;

        void fillEvent()
        {
            XLMMarker = ;
            channelsHit = ;
            channelID = ;
            energyHG = ;
            energyLG = ;
            time = ;
        }
    };

    RingItem ringItem;

    ADCVariables adc;
    TDCVariables tdc;
    HINP4Variables hinp4;

    void fillEvent()
    {
        adc.fillEvent();
        tdc.fillEvent();
        hinp4.fillEvent();
    }
};

#endif
