#include "../include/pixels.h"


pixels::pixels()
{
    /*
       ifstream file("pixels.dat");

       double a,b,c,x,y,z;
       double theta1 = -2.82725; 
       double theta2 = 1.57277;
       int e,f;
       for (int telescopeNumber=0;telescopeNumber<14;telescopeNumber++)
       for (int ifront=0;ifront<32;ifront++)
       for  (int iback=0;iback<32;iback++)
       {
       file >> x >> y >> z >> e >> f;


    //x = -18.3083;
    //y = -29.1029;
    //z = 8.5688;

    //take away "target position" in inches
    x -= 15.2146;
    y -= -18.2034;
    z -= 8.63831;


    x *= 2.54;
    y *= 2.54;
    z *= 2.54;


    double xx = x*cos(theta1) + y*sin(theta1);
    double yy = y*cos(theta1) - x*sin(theta1);


    //cout << xx << " " << yy << " " << z << endl;

    double zz = z*cos(theta2) + xx*sin(theta2);
    double xxx = xx*cos(theta2) - z*sin(theta2);

    //cout << xxx << " " << yy << " " << zz << endl;

    TeleP[telescopeNumber].Location[ifront][iback].x = -yy;
    TeleP[telescopeNumber].Location[ifront][iback].y = -xxx;
    TeleP[telescopeNumber].Location[ifront][iback].z = zz-.79-.05;

    }
    file.close();
    file.clear();
    */

    ifstream file2("hirapixels.dat");

    int tele[14] = {1,0,4,3,2,8,7,6,5,11,10,9,13,12}; //Jack -> real mapping

    int B,F,G,H;
    double x,y,z;
    for (int telescopeNumber=0;telescopeNumber<14;telescopeNumber++)
    {
        file2 >> G >> H;
        for (int ifront=0;ifront<32;ifront++)
            for  (int iback=0;iback<32;iback++)
            {
                file2 >> F >> B >> x >> y >> z;

                TeleP[tele[telescopeNumber]].Location[ifront][31-iback].x = -x;
                TeleP[tele[telescopeNumber]].Location[ifront][31-iback].y = -y;
                TeleP[tele[telescopeNumber]].Location[ifront][31-iback].z = z;

            }
    }
    file2.close();
    file2.clear();

    location center1 = getCenter(6);
    location center2 = getCenter(7);

    for (int telescopeNumber=0;telescopeNumber<14;telescopeNumber++)
        for (int ifront=0;ifront<32;ifront++)
            for  (int iback=0;iback<32;iback++)
            {

                float r = pow(TeleP[telescopeNumber].Location[ifront][iback].x,2)
                    + pow(TeleP[telescopeNumber].Location[ifront][iback].y,2)
                    + pow(TeleP[telescopeNumber].Location[ifront][iback].z,2);
                r = sqrt(r);
                TeleP[telescopeNumber].Location[ifront][iback].theta =
                    acos(TeleP[telescopeNumber].Location[ifront][iback].z/r);
                TeleP[telescopeNumber].Location[ifront][iback].phi =
                    atan2(TeleP[telescopeNumber].Location[ifront][iback].y,
                            TeleP[telescopeNumber].Location[ifront][iback].x);
            }
}
//*********************************
location pixels::getCenter(int telescopeNumber)
{
    float x = (TeleP[telescopeNumber].Location[15][15].x 
            + TeleP[telescopeNumber].Location[15][16].x
            + TeleP[telescopeNumber].Location[16][15].x
            + TeleP[telescopeNumber].Location[16][16].x)/4.;

    float y = (TeleP[telescopeNumber].Location[15][15].y
            + TeleP[telescopeNumber].Location[15][16].y
            + TeleP[telescopeNumber].Location[16][15].y
            + TeleP[telescopeNumber].Location[16][16].y)/4.;

    float z = (TeleP[telescopeNumber].Location[15][15].z 
            + TeleP[telescopeNumber].Location[15][16].z
            + TeleP[telescopeNumber].Location[16][15].z
            + TeleP[telescopeNumber].Location[16][16].z)/4.;

    location out;
    out.x = x;
    out.y = y;
    out.z = z;

    return out;
}
//***************************************
float pixels::getAngle(int telescopeNumber, int ifront, int iback)
{
    phi = TeleP[telescopeNumber].Location[ifront][iback].phi;
    return TeleP[telescopeNumber].Location[ifront][iback].theta;

}
//*********************************
float pixels::getCsiCenter(int telescopeNumber, int CsINumber)
{
    float x,y,z;
    if (CsINumber == 0)
    {
        x = (TeleP[telescopeNumber].Location[7][7].x 
                + TeleP[telescopeNumber].Location[7][8].x
                + TeleP[telescopeNumber].Location[8][7].x
                + TeleP[telescopeNumber].Location[8][8].x)/4.;

        y = (TeleP[telescopeNumber].Location[7][7].y
                + TeleP[telescopeNumber].Location[7][8].y
                + TeleP[telescopeNumber].Location[8][7].y
                + TeleP[telescopeNumber].Location[8][8].y)/4.;

        z = (TeleP[telescopeNumber].Location[7][7].z 
                + TeleP[telescopeNumber].Location[7][8].z
                + TeleP[telescopeNumber].Location[8][7].z
                + TeleP[telescopeNumber].Location[8][8].z)/4.;
    }
    if (CsINumber == 1)
    {
        x = (TeleP[telescopeNumber].Location[7][23].x 
                + TeleP[telescopeNumber].Location[7][24].x
                + TeleP[telescopeNumber].Location[8][23].x
                + TeleP[telescopeNumber].Location[8][24].x)/4.;

        y = (TeleP[telescopeNumber].Location[7][23].y
                + TeleP[telescopeNumber].Location[7][24].y
                + TeleP[telescopeNumber].Location[8][23].y
                + TeleP[telescopeNumber].Location[8][24].y)/4.;

        z = (TeleP[telescopeNumber].Location[7][23].z 
                + TeleP[telescopeNumber].Location[7][24].z
                + TeleP[telescopeNumber].Location[8][23].z
                + TeleP[telescopeNumber].Location[8][24].z)/4.;
    }
    if (CsINumber == 2)
    {
        x = (TeleP[telescopeNumber].Location[23][23].x 
                + TeleP[telescopeNumber].Location[23][24].x
                + TeleP[telescopeNumber].Location[24][23].x
                + TeleP[telescopeNumber].Location[24][24].x)/4.;

        y = (TeleP[telescopeNumber].Location[23][23].y
                + TeleP[telescopeNumber].Location[23][24].y
                + TeleP[telescopeNumber].Location[24][23].y
                + TeleP[telescopeNumber].Location[24][24].y)/4.;

        z = (TeleP[telescopeNumber].Location[23][23].z 
                + TeleP[telescopeNumber].Location[23][24].z
                + TeleP[telescopeNumber].Location[24][23].z
                + TeleP[telescopeNumber].Location[24][24].z)/4.;
    }
    if (CsINumber == 3)
    {
        x = (TeleP[telescopeNumber].Location[23][7].x 
                + TeleP[telescopeNumber].Location[23][8].x
                + TeleP[telescopeNumber].Location[24][7].x
                + TeleP[telescopeNumber].Location[24][8].x)/4.;

        y = (TeleP[telescopeNumber].Location[23][7].y
                + TeleP[telescopeNumber].Location[23][8].y
                + TeleP[telescopeNumber].Location[24][7].y
                + TeleP[telescopeNumber].Location[24][8].y)/4.;

        z = (TeleP[telescopeNumber].Location[23][7].z 
                + TeleP[telescopeNumber].Location[23][8].z
                + TeleP[telescopeNumber].Location[24][7].z
                + TeleP[telescopeNumber].Location[24][8].z)/4.;
    }

    float r = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    return acos(z/r)*180./3.14159;
}
