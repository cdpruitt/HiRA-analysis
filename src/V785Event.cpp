#include "../include/V785Event.h"
#include "../include/readData.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

bool V785Event::readEvent(ifstream& evtfile)
{
    // read header
    unsigned int word;
    readNextWord(evtfile, &word);

    while(!headerIndicator.check(word))
    {
        // failed to find header indicator
        cout << "Error: missing header while reading ADC event. Skipping to next header..." << endl;
        readNextWord(evtfile, &word);
    }

    channelsHit.read(word);
    crateNumber.read(word);
    headerIndicator.read(word);

    // read body
    while(readNextWord(evtfile, &word) && bodyIndicator.check(word))
    {
        allChannelsData.push_back(singleChannelData(word));
    }

    // reached footer
    if(!footerIndicator.check(word))
    {
        // failed to find footer indicator
        cout << "Error: missing footer while reading ADC event. Continuing..." << endl;
        return false;
    }

    return true;
}
