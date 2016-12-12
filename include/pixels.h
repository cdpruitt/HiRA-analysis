#ifndef pixels_
#define pixels_

#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

struct location
{
  double x,y,z;
  double theta,phi;
};

struct teleP
{
  location Location[32][32];
};

class pixels
{
 public:
  pixels();
  teleP TeleP[14];
  location getCenter(int telescopeNumber);
  float getCsiCenter(int telescopeNumber, int CsINumber);
  float phi;
  float getAngle(int telescopeNumber,int ifront, int iback);

  
};
#endif
