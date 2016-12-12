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
  location getCenter(int itele);
  float getCsiCenter(int itele, int iCsi);
  float phi;
  float getAngle(int itele,int ifront, int iback);

  
};
#endif
