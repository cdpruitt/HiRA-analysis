#include "../include/forest.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

forest::forest()
{
  event = new Event();
  reset();
  file = new TFile ("forest.root", "RECREATE");
}

forest::~forest()
{
  file->Write();
  cout << " " << endl;
  cout << "Tree written" << endl;
  delete event;
}

void forest::newTree(int runNumber)
{
  file->cd();
  int run = runNumber;
  ostringstream outstring;

  outstring.str("");

  outstring << "tree_" << run;
  string name = outstring.str();

  tree = new TTree(name.c_str(), "if a tree falls in the forest and noone is there");
  
  tree->Branch("nfront",&event->nfront,"nfront/I");
  tree->Branch("frontE",event->frontE,"frontE[nfront]/F");
  tree->Branch("frontT",event->frontT,"frontT[nfront]/I");
  tree->Branch("frontID",event->frontID,"frontT_id[nfront]/I");
  tree->Branch("nback",&event->nback,"nback/I");
  tree->Branch("backE",event->backE,"backE[nback]/F");
  tree->Branch("backT",event->backT,"backT[nback]/I");
  tree->Branch("backID",event->backID,"backID[nback]/I");
  tree->Branch("ncsi",&event->ncsi,"ncsi/I");
  tree->Branch("csiE",event->csiE,"csiE[ncsi]/F");
  tree->Branch("csiER",event->csiER,"csiER[ncsi]/I");
  tree->Branch("csiT",event->csiT,"csiT[ncsi]/I");
  tree->Branch("csiID",event->csiID,"csid[ncsi]/I");
}

void forest::writeTree()
{
  tree->Write();
  delete tree;
}

//************************************
void forest::reset()
{
  event->nfront = 0;
  event->nback = 0;
  event->ncsi = 0;
}
