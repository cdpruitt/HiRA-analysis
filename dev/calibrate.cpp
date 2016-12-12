#include "../include/calibrate.h"
#include "../include/math.h"

#include <vector>

/**
 * Constructor
 \param Nstrip0 is number of strips or csi in a telescope
 \param name is string contain the file of coefficients
 \param order is order of polynomial

*/

calibrate::calibrate(int Ntele0, int Nstrip0, string name, int order0)
{
    Nstrip = Nstrip0;
    Ntele = Ntele0;
    order = order0;

    vector<vector<float>> polyCoefficients;

    for (int i=0;i<Ntele;i++)
    {
        polyCoefficients.push_back(vector<float>)
    }

    ifstream file(name.c_str());
    string title;

    int itele,istrip;
    vector<float> polyCoefficients(4,0);

    for(;;)
    {
        file >> itele >> istrip >> polyCoefficients[1] >> polyCoefficients[0];

        if (order >=2)
        {
            file >> polyCoefficients[2];
        }

        if (order == 3)
        {
            file >> polyCoefficients[3];
        }

        if (file.eof() || file.bad())
        {
            break;
        }

        Coeff[itele][istrip].intercept = polyCoefficients[0];
        Coeff[itele][istrip].slope = polyCoefficients[1];
        Coeff[itele][istrip].a2 = polyCoefficients[2];
        Coeff[itele][istrip].a3 = polyCoefficients[3];
    }

    file.close();
    file.clear();  
}

//*****************************************************
/**
 * destructor
 */
calibrate::~calibrate()
{
    for (int i=0;i<Ntele;i++)
    {
        delete Coeff[i];
    }

    delete [] Coeff;
}
//*****************************************

/**
 * returns the calibrated energy
 \param istrip - number of the strip or detector
 \param channel - raw channels from the ADC, etc
 */
float calibrate::getEnergy(int itele,int istrip,float channel)
{
    float fact = Coeff[itele][istrip].intercept;

    if(order>3)
    {
        cout << "Error: calibration polynomial more than 3rd degree." << endl;
        abort();
    }

    for(int i=0; i<order; i++)
    {
        fact += pow(channel,i)*Coeff[itele][istrip].polyCoefficients[i];
    }

    return fact;
}
