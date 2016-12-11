#ifndef forest_
#define forest_
#include "TTree.h"
#include "TFile.h"
#include "event.h"

class forest
{
public:

  TFile * file; //!< output root file

  TTree * tree;
  Event * event;
  forest();
  ~forest();
  void reset();
  void newTree(int);
  void writeTree();


};
#endif
