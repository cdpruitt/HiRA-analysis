#ifndef det_
#define det_
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "TRandom.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "hira.h"
#include "ceasar.h"
#include "histo_sort.h"
#include "histo_read.h"
#include "correl.h"
#include "doppler.h"
#include "forest.h"
#include "event.h"

using namespace std;

/**
 *!\brief detector readout
 *
 * responsible for unpacking the data stream for physics events
 */


class det
{
 public:
  det(histo_sort * Histo_sort, histo_read * Histo_read, forest * Forest);
  ~det();
  bool unpack(unsigned short *point,int runno);
  hira *Hira;
  ceasar * Ceasar;
  histo_sort * Histo_sort;
  histo_read * Histo_read;
  forest * Forest;
  TRandom * ran;
  void analyze(int event);

  int Eventt;
  int treelevel;

  int Run;

  correl Correl;
  void corr_8B();
  void corr_15Ne();
  void corr_16Ne();
  void corr_17Ne();
  void corr_17Na();
  void corr_18Na();
  void corr_18Ne();
  void corr_19Ne();
  void corr_15F();
  void corr_16F();
  void corr_17F();
  void corr_18F();
  void corr_11C();
  void corr_12C();
  void corr_13C();
  void corr_12N();
  void corr_13N();
  void corr_14N();
  void corr_15N();
  void corr_13O();
  void corr_14O();
  void corr_15O();
  void corr_16O();
  void corr_6Li();
  void corr_5Be();
  void corr_7Be();
  void corr_9Be();
  void treeGrow();
  void loadTree(Event * event);

  int N2p6Li;
  int N2p14O;
  int N2p15O;
  int N2p14N;
  int N1p14N;

  int CsImult;

  int N_IAS;
  float xg,yg,zg;
  float xL,yL,zL;
  float thetag,phig;
  float thetaL,phiL,thetarel;
  float dot;
  float mag;
 private:
  doppler *Doppler;

};
#endif
