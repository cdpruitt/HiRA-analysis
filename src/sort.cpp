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

using namespace std;

const string EVENT_FILE_STEM = "/events/e14002/complete/run-";
const int HEADER_WORDS = 4;
const int HEADER_BYTES = HEADER_WORDS*2;

int main(int argc, char* argv[])
{
    /**************************************************************************
    * Determine which runs to sort
    **************************************************************************/

    vector<int> runNumbers; // list of the run numbers we'd like to read

    if(argc==1) // no run numbers provided as arguments to './sort';
                // read run numbers from runfile
    {
        ifstream runFile;
        runFile.open("numbers.beam");
        if (!runFile.is_open())
        {
            cout << "Error: could not open runfile. Exiting..." << endl;
            return 1;
        }

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
    long runno = 0;

    // initialize the classes for processing sorted events
    histo_sort * Histo_sort = new histo_sort();
    //histo_read * Histo_read = new histo_read();
    forest * Forest = new forest();

    det Det(Histo_sort, Forest);

    for(int runNumber : runNumbers)
    {
        Forest->newTree(runNumber);

        // form the run file name
        stringstream runNumberFormatted;
        runNumberFormatted.str("");
        runNumberFormatted << setfill('0') << setw(4) << runNumber;

        stringstream fileNameStream;
        fileNameStream.str("");
        fileNameStream << EVENT_FILE_STEM << runNumberFormatted;
        string eventFileName = fileNameStream.str();

        //open evt file
        ifstream evtfile;
        evtfile.open(eventFileName.c_str(),ios::binary);
        if (!evtfile.is_open())
        {
            cout << "Error: could not open event file" << eventFileName << ". Exiting..." << endl;
            return 1;
        }

        if (evtfile.bad() || evtfile.fail())
        {
            cout << "Error: " << eventFileName << " bad." << endl;
        }

        cout << endl << "Processing " << eventFileName << "..." << endl;

        unsigned short headerBuffer[HEADER_WORDS];

        bool endOfRun = false; // keep track of end-of-run indicator events

        /***********************************************************************
         * Parse an event file word-by-word
         **********************************************************************/

        while(!evtfile.eof() && !evtfile.bad() && !endOfRun)
        {
            /*******************************************************************
             * Read event header
             ******************************************************************/

            evtfile.read((char*)headerBuffer,HEADER_BYTES);

            unsigned short *point;
            point = headerBuffer;
            int totalBytesInEvent = *point++;
            int evtType = *point++;

            /*******************************************************************
             * Read event body
             ******************************************************************/

            int dataBytes = totalBytesInEvent - HEADER_BYTES;
            int dataWords = dataBytes/2;

            unsigned short dataBuffer[dataWords];
            evtfile.read((char*)dataBuffer,dataBytes);

            switch(evtType)
            {
                case 1: 
                    runno = dataBuffer;
                    cout << "run number = " << runno << endl; 
                    break;

                case 2:
                    endOfRun = true;
                    break;

                case 3:
                    Npauses++;
                    break;

                case 4:
                    Nresumes++;
                    break;

                case 20:
                    scalerBufferCounter++;
                    break;

                case 30:
                    physicsEvent++;
                    if (physicsEvent%10000 == 0) 
                    {
                        cout << '\xd'<< physicsEvent << flush;
                    }

                    // attempt to unpack event
                    if (Det.unpack(evtfile,runno))
                    {
                        physicsEventGood++;
                        Det.treeGrow();
                    }
                    break;

                case 31:
                    physicsEventCounter++;
                    break;

                default:
                    cout << "Error: encountered unknown evtType " << evtType << ". Exiting..." << endl;
                    return 1;
            }

        } //end loop over evtfile

        Forest->writeTree();

    } //end loop of run file numbers

    cout << endl << "physics Events = " << physicsEvent << endl;
    cout << "Good physics Events = " << physicsEventGood << endl;

    if (physicsEvent > 0)
    {
        double percentGood = ((double)physicsEventGood/physicsEvent)*100;
        cout << "Bad physics events = " << 100-percentGood << "\% of total." << endl;
    }

    cout << "physics Event Counters = " << physicsEventCounter << endl;
    cout << "scaler buffers = " << scalerBufferCounter << endl;
    cout << "Numbers of pauses = " << Npauses << endl;
    cout << "Number of resumes = " << Nresumes << endl;

    Histo_sort->write(); // this forces the histrograms to be read out to file
}
