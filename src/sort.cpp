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

#include "../include/det.h"
#include "../include/histo_sort.h"
#include "../include/histo_read.h"
#include "../include/forest.h"
#include "../include/singletons/FileOpener.h"

#include "../include/unpacker/RingItem.h"

using namespace std;

const string EVENT_FILE_STEM = "/events/e14002/complete/run-";

int main(int argc, char* argv[])
{
    /**************************************************************************
    * Determine which runs to sort
    **************************************************************************/

    vector<int> runNumbers; // list of the run numbers we'd like to read

    FileOpener* fileOpener = FileOpener::Instance();

    if(argc==1) // no run numbers provided as arguments to './sort';
                // read run numbers from runfile
    {
        ifstream runFile;
        fileOpener->openFile("numbers.beam", runFile);

        string line;
        while(getline(runFile,line))
        {
            runNumbers.push_back(stoi(line));
        }
    }

    else // run numbers provided as arguments to './sort'
    {
        for(int i=1; i<argc; i++)
        {
            runNumbers.push_back(atoi(argv[i]));
        }
    }

    /**************************************************************************
    * Prepare for run sorting
    **************************************************************************/

    // initialize counters for event types
    long physicsEvent = 0;
    long physicsEventGood = 0;
    long physicsEventCounter = 0;
    long scalerBufferCounter = 0;
    long Npauses = 0;
    long Nresumes = 0;
    short runno = 0;

    // initialize the classes for processing sorted events
    ///histo_sort * Histo_sort = new histo_sort();
    //histo_read * Histo_read = new histo_read();
    ///forest * Forest = new forest();

    ///det Det(Histo_sort);

    for(int runNumber : runNumbers)
    {
        ///Forest->newTree(runNumber);

        // form the run file name
        stringstream runNumberFormatted;
        runNumberFormatted.str("");
        runNumberFormatted << setfill('0') << setw(4) << runNumber;

        stringstream fileNameStream;
        fileNameStream.str("");
        fileNameStream << EVENT_FILE_STEM << runNumberFormatted;
        string eventFileName = fileNameStream.str();

        // open the event file
        ifstream evtfile;
        fileOpener->openFile(eventFileName, evtfile, "b");

        cout << endl << "Processing " << eventFileName << "..." << endl;

        /***********************************************************************
         * Parse an event file word-by-word
         **********************************************************************/
        bool endOfRun = false; // keep track of end-of-run indicator events

        while(!evtfile.eof() && !evtfile.bad() && !endOfRun)
        {
            /*******************************************************************
             * Read event header
             ******************************************************************/

            RingItem* ringItem = new RingItem("Ring Item");
            ringItem->extractData(evtfile);

            /**int dataBytes = totalBytesInEvent - HEADER_BYTES;
            int dataWords = dataBytes/2;

            unsigned short dataBuffer[dataWords];
            evtfile.read((char*)dataBuffer,dataBytes);

            switch(evtType)
            {
            }**/

        } //end loop over evtfile

        ///Forest->writeTree();

    } //end loop of run file numbers

    if (physicsEvent > 0)
    {
        double percentGood = ((double)physicsEventGood/physicsEvent)*100;
        cout << "Bad physics events = " << 100-percentGood << "\% of total." << endl;
    }

    cout << "physics Event Counters = " << physicsEventCounter << endl;
    cout << "scaler buffers = " << scalerBufferCounter << endl;
    cout << "Numbers of pauses = " << Npauses << endl;
    cout << "Number of resumes = " << Nresumes << endl;

    ///Histo_sort->write(); // this forces the histrograms to be read out to file
}
