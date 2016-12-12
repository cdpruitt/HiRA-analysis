#ifndef telescope_
#define telescope_

#include "elist.h"
#include "histo_sort.h"
#include "histo_read.h"
#include "pixels.h"
#include "pid.h"
#include <string>
#include <sstream>
#include "solution.h"
#include "calibrate.h"
#include "loss.h"
#include "TRandom.h"
class telescope
{
 public:


  static bool const relativity;
  telescope(TRandom* ran,int id0, histo_read * Histo0);
  ~telescope();
  elist Front;
  elist Back;
  elist Csi;

  void analyze(int);
  void reset();
  int multiHitCsi();
  int multiHit();
  void load(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int);
  void Addfake();
  void Addfake2();
  void Addfake3();
  void Addfake4();
  void getMomentum();

  int id;
  float fenergy;
  float benergy;
  float Sienergy;
  float CsIenergy;
  float flow;
  float blow;
  float xhit;
  float yhit;
  float theta;
  float phi;

  int fhit;
  int bhit;
  int CsIhit;

  int Np;
  int N6;

  int multFront;
  int multBack;

  int Event;

 
 solution Solution[10];
  int Nsolution;


 private:
  TRandom * ran;
  histo_read * Histo;
  pixels Pixels;

  int NestDim;
  void loop(int);
  int NestArray[50];
  int arrayD[50];
  int arrayB[50];
  float deMin;
  int dstripMin;

  int FrontLow[4];
  int FrontHigh[4];
  int BackLow[4];
  int BackHigh[4];

  pid  * Pid[4];


  CLoss * Loss[25];

  calibrate * calCsi;
  calibrate * calCsid;
  calibrate * calCsit;

  calibrate * calCsiHe3;
  calibrate * calCsiA;

  calibrate * calCsiLi6;

  calibrate * calCsiBe7;

  calibrate * calCsiB10;
  calibrate * calCsiB11;

  calibrate * calCsiC11;
  calibrate * calCsiC12;
  calibrate * calCsiC13;

  calibrate * calCsiN12;
  calibrate * calCsiN13;
  calibrate * calCsiN14;
  calibrate * calCsiN15;

  calibrate * calCsiO13;
  calibrate * calCsiO14;
  calibrate * calCsiO15;
  calibrate * calCsiO16;
  calibrate * calCsiF17;

  calibrate * calFB;

};

#endif
