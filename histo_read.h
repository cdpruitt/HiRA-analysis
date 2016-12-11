#ifndef histo_read_
#define histo_read_
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "TH1I.h"
#include "TH2I.h"
#include "TH3I.h"
#include "TFile.h"
#include "TDirectory.h"

using namespace std;

class histo_read
{
protected:

  TFile * file; //!< output root file

  //correlations
  TDirectoryFile * dirCorr; //!< directory for correlations
  TDirectory * dir11O; //!< directory for 11O correlations
  TDirectory * dir12O; //!< directory for 12O correlations
  TDirectory * dir6Li; //!< directory for 6Li correlations
  TDirectory * dir7Li; //!< direcotry for 7Li correlations 
  TDirectory * dir7Be; //!< direcotry for 7Be correlations 
  TDirectory * dir12C;

  TDirectoryFile *dirdEE; //!< directory for the dEE maps

    
public:
    histo_read();                  //!< constructor
    ~histo_read(){};
    void write(); //!< write the root spectra to file


    int Ntele;
    int Nstrip;
    int Nceasar;
    int NCsI;
    int Nring;

    TH2I ** dEE;
    TH2I * HitMap;

    
    TH1I* TEC_13O_added;
    
    //correlations
    //11O
    TH1I* Erel_11O_2p9C;
    TH1I* vel_11O_2p9C;
    TH2I * JacobiY_11O_2p9C;
    TH2I * JacobiT_11O_2p9C;

    //12O
    TH1I* Erel_12O_2p10C;
    TH1I* vel_12O_2p10C;
    TH2I * JacobiY_12O_2p10C;
    TH2I * JacobiT_12O_2p10C;

     //Blocker CsI
    TH2D* Blocker_ETOF;

    //7Li
    TH1I * Erel_7Li;

    TH1I * Erel_Back_Diff;
    TH1I * Erel_For_Diff;
    TH1I * Erel_Trans_Diff;
    TH1I * Erel_Back_Same;
    TH1I * Erel_For_Same;
    TH1I * Erel_Trans_Same;
    TH1I * Costheta;
    TH1I * vel_7Li;

    //6Li

    TH1I * Erel_6Li;
    
    //Be7
    TH1I * Ex7Be_alpha3He;
    TH1I * vel_7Be_alpha3He;
    TH1I * vel_7Be_alpha3He_left;
    TH1I * vel_7Be_alpha3He_right;
    TH1I * TEC_7Be_alpha3He;
    TH1I * Ex7Be_alpha3He_gamma;
    TH1I * Ex7Be_alpha3He_gammaback1;
    TH1I * Ex7Be_alpha3He_gammaback2;
    TH1I * Ex7Be_alpha3He_gammaback3;
    TH2I * Caesarmult;

    //C12
    TH1I * Ex12C_da6Li;
};
#endif
