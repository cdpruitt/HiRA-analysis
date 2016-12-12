#include "histo_sort.h"
#include "histo_read.h"
#include "TRandom.h"
#include "caen.h"
#include "TDC1190.h"
#include "calibrate.h"
#include <cmath>
#include "doppler.h"
#include <vector>

using namespace std;

struct dataEC
{
  int ienergy;
  int id;
  int itime;
  int iRing;
  int iLoc;
  float theta;
  float energy;
  float Total;
  float dop_energy;
  float phi;
  float time;
};

struct dataTC
{
  int itime;
  int id;
  float time;
};

struct mapC
{
  int iRing;
  int iLoc;
};

class ceasar
{
 private:
  TRandom * ran;
  histo_sort * Histo;
  doppler *Doppler;
  caen Caen;
  TDC1190 ** tdc;

  mapC MapC[12][16];
  calibrate * calCeasar;
  calibrate * calCeasarT;

 public:
  ceasar(TRandom* ran0, histo_sort * Histo0);
  ~ceasar();
  bool unpack(unsigned short *point);

  dataEC DataEC[192];
  dataTC DataTC[192];
  
  dataEC select[192];
  dataEC added[192];
  int Nselect;
  int Nadded;

  float angle[7][24];
  float angle2[7][24];
  float Txfp[3];
  int Nxfp;
  float TRF;

  int NE;
  int NT;


  void reset();

};
