#ifndef calibrate_
#define calibrate_
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

class calibrate
{
    public:
        calibrate(int Ntele,int Nstrip,string file,int order =1);
        ~calibrate();
        float getEnergy(int itele,int istrip,float channel);
        int order;
        int Nstrip;  //!< number of strips
        int Ntele;   //!<number of telescopes
        vector<vector<float>> coeff;  //!< array with calibration coefficients for each strip
};
#endif
