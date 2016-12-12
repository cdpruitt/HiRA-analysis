#include <fstream>
#include "../include/TDC1190.h"
#include "../include/buffer.h"

/**
 * constructor
 \param depth0 - number of mulihits stored
 \param referenceChannel0 - channels in which trigger is sent
 \param Nchannels0 - number of channels for which data is saved
 */

TDC1190::TDC1190(int depth0, int referenceChannel0, int Nchannels0)
{
    depth = depth0;
    referenceChannel = referenceChannel0;
    Nchannels = Nchannels0;
    order = new int [Nchannels];
    dataOut = new tdcStuff  [Nchannels*depth];
}
//********************************************************************
/**
 * destructor
 */
TDC1190::~TDC1190()
{
    delete [] order;
    delete [] dataOut;
}
//*******************************************************************
/**
 * read information from grobal header. As this is 32 bits it is 
 * contained in two wordas of the data stream. 
 \param header2 first word
 \param header1 second word 
 */
bool TDC1190::readGlobalHeader(ifstream& evtfile)
{
    //extract information

    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short header1 = buffer[0];

    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short header2 = buffer[0];

    unsigned short bit =  (header1>>14) & 0x1;
    if (bit == 0) 
    {
        cout << "TDC1190 global header not found" << endl;
        abort();
    }
    geographic = header2 & 0x1F;
    eventCount = (header1 & 0x7FF) << 10;
    eventCount += (header2>>5);

    Ndata = 0;
    for (int i=0;i<Nchannels;i++)
    {
        order[i] = 0;
    }
    return true;
}


//*********************************************************************
/**
 * reads a TDC header. As this is 32 bits it is contained in two words
 \param header2 - first word
 \param header1 - second word
 */
bool TDC1190::readTDCheader(ifstream& evtfile)
{
    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short header2;

    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short header1;

    //extract information
    unsigned short bit =  (header1>>11) & 0x1;
    if (bit == 0) 
    {
        cout << "TDC1190 TDC header not found" << endl;
        //abort();
    }
    bunchID = header2 & 0x7FF;
    eventID = (header1 & 0xFF) << 4;
    eventID += (header2 >> 12) & 0xF;
    TDC = (header1 >> 8) & 0x3;
    return true;
}

//**************************************************************
/**
 * reads measured values from a TDC channel. As data is 32 bits, it is 
 * contained in two words
 \param data2 - first word
 \param data1 - second word
 */
bool TDC1190::readTDCmeasurement(ifstream& evtfile)
{
    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short data1  = buffer[0];

    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short data2  = buffer[0];

    /*if (front != 0) 
    {
        cout << " not a TDC1190 measurment" <<endl;
        //abort();
    }*/
    channel = (data1 >> 3) & 0x7F;
    data = data2 + ((data1 & 0x7)<<16);
    leading = (data1 >> 10) & 0x1;

    if (channel >= Nchannels)
    {
        //cout << "channel > Nchannels" << endl;
        return false;
    }

    if (channel == referenceChannel && order[channel] == 0)
    {
        referenceTime = data;
        ///    cout << "reference time is here" << endl;
    }

    if (order[channel] < depth)
    {
        dataOut[Ndata].time = data;
        dataOut[Ndata].channel = channel;
        dataOut[Ndata].order = order[channel];
        Ndata++;

    }
    order[channel]++;
    return true;
}

//****************************************************
/**
 * reads a TDC trailer from the event. As the trailer is 32 bits, it is 
 * contained in two words
 \param trailer2 - first word
 \param trailer1 - second word
 */

bool TDC1190::readTDCtrailer(ifstream& evtfile)
{
    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short trailer1  = buffer[0];

    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short trailer2  = buffer[0];

    unsigned short bit = (trailer1>>11);
    if (bit != 0x3)
    {
        cout << " not a TDC trailer " << endl;
        //abort();
    }
    wordCount = trailer2 & 0xFFF;
    eventID = trailer2 >>12;
    eventID += (trailer1 & 0xFF)<<8;

    return true;
}

//*****************************************************
/**
 * reads TDC error information. As this is 32 bits, it is contained in 
 * two words.
 \param error2 - first word
 \param error1 - second word
 */

bool TDC1190::readTDCerror(ifstream& evtfile)
{
    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short error2  = buffer[0];

    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short error1  = buffer[0];

    unsigned short bit = (error2>>14);
    if (bit != 0x1)
    {
        cout << " not a TDC error " << endl;
        //abort();
    }
    errorFlag = error1;
    return true;
}

//***********************************************
/**
 * reads the trailer from the data stream. As the trailer is 32 bits,
 * it is contained in two words
 \param trailer2 - first word
 \param trailer1 - second word
 */
bool TDC1190::readTrailer(ifstream& evtfile)
{
    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short trailer2  = buffer[0];

    evtfile.read((char*)buffer,BUFFER_WORDS);
    unsigned short trailer1  = buffer[0];
    
    int geo = trailer2 & 0x1F;
    if (geo != geographic)
    {
        cout << "geographic address in trailer doesn't match one in header" 
            << endl;
        //abort();
    }
    unsigned short bit = trailer1 >> 15;

    if (!bit)
    {
        cout << "not a trailer buffer for TDC1190 " << endl;
        //abort();
    }
    status = (trailer1 >> 8) & 0x7;

    for (int i=0;i<Ndata;i++)
    {
        dataOut[i].time -= referenceTime;
    }

    return true;
}

//************************************************
/**
 * reads the infomation generated from a single trigger
 * to the TDC
 \param point is a pointer to the data stream
 */

bool TDC1190::read(ifstream& evtfile)
{
    readGlobalHeader(evtfile);

    unsigned short word1;// = *point++;
    unsigned short word2;// = *point++;
    for (;;)
    {
        unsigned short word3 = word2 >> 11;
        if (word3 ==0x10)  //if trailer then leave
        {
            readTrailer(evtfile);
            break;
        }
        else if (word3 == 0)  readTDCmeasurement(evtfile);
        else if (word3 == 0x1) readTDCheader(evtfile);
        else if (word3 == 0x3) readTDCtrailer(evtfile);
        else if (word3 == 0x4) readTDCerror(evtfile);
    }
    return true;
}
