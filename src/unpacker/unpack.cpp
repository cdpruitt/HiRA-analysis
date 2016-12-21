/******************************************************************************
* sort.cpp:
*   - reads raw run data, event-by-event
*   - initializes HiRA classes for data analysis
*
* There are two ways to run this program:
*
*   list runs to analyze as arguments
*   (e.g., ./sort [run number 1] [run number 2] ... [run number n])
*
*                       - OR -
*
*   list no arguments - runs to analyze will be parsed from "numbers.beam", a
*   text file in the same directory as sort.cpp
*   (e.g., ./sort)
*
*
* The input .evt file is expected to consist of a series of events back-to-back,
* each with the following structure:
*
* Event header
* ------------------------------------------------------------------------------
* Size of file   | uint16; number of 2-byte words in the event (self-inclusive)
* Event type     | (unknown at the moment - see NSCL DAQ documentation)
* ------------------------------------------------------------------------------
*
* Event body
* ------------------------------------------------------------------------------
* For each motherboard:
*   Motherboard flag       | uint16; indicates motherboard # of following events
*   Ringbuffer word size   | uint32; indicates size of motherboard words
*   XLM word size          | uint16; indicates size of XLM words
*   XLM time               | uint64; time data from the XLM
*
*   For each channel:
*       channel ID      | ringbuffer word size; indicates channel # of event
*       high-gain ADC   | ringbuffer word size; high-gain ADC value for event
*       low-gain ADC    | ringbuffer word size; low-gain ADC value for event
*       time            | ringbuffer word size; time value for event
*       dummy words     | ringbuffer word size; keep event structure ordered
* ---------------------------------------------------------------------------
*
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

#include "../../include/singletons/FileOpener.h"
#include "../../include/unpacker/RingItem.h"
#include "../../include/Counter.h"

#include "TTree.h"

using namespace std;

const string EVENT_FILE_STEM = "/events/e14002/complete/run-";
const string TEXT_OUTPUT_NAME = "evtfilePrinted.txt";

int main(int argc, char* argv[])
{
    // check for flag indicating we should produce text output of run data
    bool produceText;
    if(argc>1)
    {
        produceText = argv[1];
    }

    FileOpener* fileOpener = FileOpener::Instance();

    /**************************************************************************
    * Determine which runs to sort
    **************************************************************************/

    vector<unsigned int> runNumbers; // for holding the list of the run numbers to read

    ifstream runFile;
    fileOpener->openFile("numbers.beam", runFile);

    string line;
    while(getline(runFile,line))
    {
        runNumbers.push_back(stoi(line));
    }

    /**************************************************************************
    * Prepare for run sorting
    **************************************************************************/

    // initialize counters for event types
    Counter physicsEventCounter("Physics events");
    Counter physicsEventGoodCounter("Good physics events");
    Counter scalerCounter("Scalers");
    Counter pauseCounter("Pauses");
    Counter resumeCounter("Resumes");

    /**************************************************************************
    * Sort each run
    **************************************************************************/

    for(unsigned int runNumber : runNumbers)
    {
        // form the run file name
        stringstream runNumberFormatted;
        runNumberFormatted.str("");
        runNumberFormatted << setfill('0') << setw(4) << runNumber;

        stringstream fileNameStream;
        fileNameStream.str("");
        fileNameStream << EVENT_FILE_STEM << runNumberFormatted;
        string eventFileName = fileNameStream.str();

        // create a generic ring item for reading NSCLDAQ data
        RingItem* ringItem = new RingItem("Ring Item");

        // create a ROOT tree to hold this run's data
        TTree* runTree = new TTree();
        ringItem->branch(runTree); // link tree to ringItem variables

        // open the event file
        ifstream evtfile;
        fileOpener->openFile(eventFileName, evtfile, "b");

        cout << endl << "Processing " << eventFileName << "..." << endl;

        ofstream textOutput;
        if(produceText)
        {
            textOutput.open(TEXT_OUTPUT_NAME);
        }

        // parse the run file into events
        while(!evtfile.eof() && !evtfile.bad())
        {
            ringItem->extractData(evtfile);

            if(produceText)
            {
                ringItem->print(textOutput);
            }
        }
    }

    /**************************************************************************
     * Print event counters
     **************************************************************************/

    physicsEventCounter.print();
    physicsEventGoodCounter.print();

    if (physicsEventCounter.getCount() > 0)
    {
        double percentGood =
            ((double)physicsEventGoodCounter.getCount()/physicsEventCounter.getCount())*100;
        cout << "Bad physics events = " << 100-percentGood << "\% of total." << endl;
    }

    scalerCounter.print();
    pauseCounter.print();
    resumeCounter.print();
}
