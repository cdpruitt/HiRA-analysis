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
#include "histo_sort.h"
#include "histo_read.h"
#include "correl.h"
#include "doppler.h"
#include "forest.h"
#include "unpacker/event.h"
#include "detector/geometry.h"

const double REL_ENERGY_LOWER_BOUND = 2.25; // in MeV
const double REL_ENERGY_UPPER_BOUND = 2.75; // in MeV

const double Q6Li = 1.4738;
const double Q7Li = 2.46579; //needs to be changed
const double Q7Be = 0; //will need to be changed of course

using namespace std;

/**
 *!\brief detector readout
 *
 * responsible for unpacking the data stream for physics events
 */

class det
{
 public:
  det(histo_sort * Histo_sort);///, forest * Forest);
  det(histo_read * Histo_read);
  ~det();
  bool unpack(ifstream& evtfile);
  hira *Hira;
  histo_sort * Histo_sort;
  histo_read * Histo_read;
  forest * Forest;
  TRandom * ran;
  void analyze(int event);

  int Eventt;
  int treelevel;
  int ngood = 0;

  int Run;

  correl Correl;
  void corr_6Li();
  void corr_7Li();
  void corr_7Be();
  void corr_11O();
  void treeGrow();
  void loadTree(Event * event);

  int CsImult;

 private:
  ///doppler *Doppler;

};
#endif
