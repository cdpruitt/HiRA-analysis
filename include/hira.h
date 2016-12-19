#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "TRandom.h"
#include "histo_sort.h"
#include "histo_read.h"
#include "telescope.h"
#include "unpacker/V785Event.h"
#include "unpacker/CompositeDataChunk.h"
#include "unpacker/DataChunk.h"
#include "pixels.h"
#include "unpacker/TDC1190.h"
#include "detector/geometry.h"

using namespace std;

struct dataE
{
    int uncalEnergy;
    int id;
    int uncalTime;
    float energy;
};

struct dataT
{
    int uncalTime;
    int id;
};

struct telescopeIsFront
{
    int telescopeNumber;
    bool isFrontCB;
};

struct telescopeCsIMap
{
    int telescopeNumber;
    int CsINumber;
};

class hira
{
    public:
        hira(histo_read * Histo1);
        ~hira();

        void init();
        bool unpack(ifstream& evtfile);
        bool unpackSi_sis(ifstream& evtfile);
        bool unpackSi_HINP4(ifstream& evtfile);
        bool unpackSi_adc(ifstream& evtfile);
        bool unpackCsi(ifstream& evtfile);
        void analyze();
        telescope **Telescope;
        void reset();

    private:
        TRandom * ran;
        unsigned short chanXLM[3][400];
        unsigned short nXLM[3];

        telescopeIsFront Map[3][15];
        telescopeCsIMap CsIMap[TOTAL_CSIS];

        histo_sort * Histo;
        histo_read * Histo_read;

        V785Event* ADC;
        TDC1190* TDC;

        dataE DataE[TOTAL_CSIS];
        dataT DataT[TOTAL_CSIS];

        //calibrate * calBack;
        //calibrate * calFront;
        //calibrate * calLBack;
        //calibrate * calLFront;
        //calibrate * calCsi;
        //calibrate * calBDual;
        //calibrate * calFrontT;
        //calibrate * calBackT;
        //calibrate * calFrontTLG;
        //calibrate * calBackTLG;
        //calibrate * calrecal;

        int NE;
        int NT;

        int BlockerESum;
        bool haveBlocker;

        //high-low correlation
        double fsumN[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];
        double fsumx[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];
        double fsumxx[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];
        double fsumy[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];
        double fsumyx[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];

        double bsumN[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];
        double bsumx[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];
        double bsumxx[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];
        double bsumy[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];
        double bsumyx[NUMBER_OF_TELESCOPES][NUMBER_OF_STRIPS];

        float timeRFScintillator;
        float timeA1900;
        float timeRFCyclotron;

};
