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
  TDirectory * dir15Ne; //!< directory for 15Ne correlations
  TDirectory * dir16Ne; //!< directory for 16Ne correlations
  TDirectory * dir17Ne; //!< directory for 17Ne correlations 
  TDirectory * dir15F; //!< directory for 15F correlations
  TDirectory * dir16F; //!< directory for 16F correlations
  TDirectory * dir17F; //!< directory for 17F correlations
  TDirectory * dir18F; //!< directory for 17F correlations
  TDirectory * dir13O; //!< direcotry for 13O correlations (12N +p)
  TDirectory * dir14O; //!< direcotry for 14O correlations (13N +p)
  TDirectory * dir15O; //!< direcotry for 15O correlations (14N +p)
  TDirectory * dir16O; 
  TDirectory * dir17Na; //!< direcotry for 17Na correlations 
  TDirectory * dir18Na; //!< direcotry for 18Na correlations 
  TDirectory * dir18Ne; //!< direcotry for 18Ne correlations 
  TDirectory * dir19Ne; //!< direcotry for 19Ne correlations 
  TDirectory * dir6Li; //!< direcotry for 6Li correlations 
  TDirectory * dir5Be; //!< direcotry for 5Be correlations 
  TDirectory * dir7Be; //!< direcotry for 7Be correlations 
  TDirectory * dir9Be; //!< direcotry for 7Be correlations 
  TDirectory * dir12N; 
  TDirectory * dir13N; 
  TDirectory * dir14N; 
  TDirectory * dir15N; 
  TDirectory * dir11C; 
  TDirectory * dir12C; 
  TDirectory * dir13C; 




  
public:
    histo_read();                  //!< constructor
    ~histo_read(){};
    void write(); //!< write the root spectra to file


    int Ntele;
    int Nstrip;
    int Nceasar;
    int NCsI;
    int Nring;

    TH1I * CeasarT_2p14N;
    TH1I * CeasarT_2p14N_IAS;

    TH2I * ETOF_Ceasar;
  
  
    //correlations
    //15Ne
    TH1I* Erel_15Ne_2p13O;
    TH1I* Erel15Ne_2pfake13O;
    TH1I* Erel15Ne_2pfake13O2;
    TH1I* vel_15Ne_2p13O;
    TH1I * Erel15Ne_3p12N;
    TH2I * JacobiY_15Ne_2p13O;
    TH2I * JacobiT_15Ne_2p13O;

    TH2I * JacobiY_15Ne_2pfake13O;
    TH2I * JacobiT_15Ne_2pfake13O;
    TH2I * JacobiY_15Ne_2pfake13O2;
    TH2I * JacobiT_15Ne_2pfake13O2;

    //16Ne
    TH2I* ErelEkp_16Ne2p14O;
    TH2I* ErelExT_16Ne2p14O;
    TH2I* ExExT_16Ne2p14O;
    TH2I* ErelDiAngle_16Ne2p14O;

    TH1I* Ex16Ne_2p14O;
    TH1I* Ex16Ne_2p14O_m2;
    TH1I* Ex16Ne_2p14O_m3;
    TH1I* Ex16Ne_2p14O_25;
    TH1I* Ex16Ne_2p14O_26;
    TH1I* Ex16Ne_2p14O_28;
    TH1I* Ex16Ne_2p14O_31;

    TH1I* ET16Ne_2p14O;
    TH1I* Ex16Ne_2pfake14O;
    TH1I* ET16Ne_2pfake14O;
    TH2I* ErelExT_16Ne2pfake14O;
    TH2I* ExExT_16Ne2pfake14O;


    TH1I* Ex16Ne_2pfake14O_25;
    TH1I* Ex16Ne_2pfake14O_26;
    TH1I* Ex16Ne_2pfake14O_28;
    TH1I* Ex16Ne_2pfake14O_31;

    TH1I* vel_16Ne_2p14O;
    TH1I* Jacobi_x_T_16Ne_gs;
    TH1I* Jacobi_x_Y_16Ne_gs;
    TH1I* Jacobi_x_T_16Ne_1st;
    TH1I* Jacobi_x_Y_16Ne_1st;
    TH2I* Jacobi_T_16Ne_back;
    TH2I* Jacobi_Y_16Ne_back;
    TH2I* Jacobi_T_16Ne_gs;
    TH2I* Jacobi_Y_16Ne_gs;
    TH2I* Jacobi_T_16Ne_1st;
    TH2I* Jacobi_Y_16Ne_1st;
    TH2I * ETvTEC_16Ne2p14O;
    TH1I* Ne16_gs_VelCM;
    TH1I* Ne16_gs_ThetaCM;
    TH1I* Ne16_1st_VelCM;
    TH1I* Ne16_1st_ThetaCM;
    TH1I* Jacobi_Y_16Ne_1st_Back1;
    TH1I* Jacobi_Y_16Ne_1st_Back2;
    TH1I* Jacobi_T_16Ne_1st_Back1;
    TH1I* Jacobi_T_16Ne_1st_Back2;
    TH1I* Jacobi_T_16Ne_1st_Left;
    TH1I* Jacobi_Y_16Ne_1st_Left;
    TH1I* Jacobi_T_16Ne_1st_Right;
    TH1I* Jacobi_Y_16Ne_1st_Right;
    TH2I * Jacobi_T_16Ne_45;
    TH2I * Jacobi_Y_16Ne_45;
    TH2I * Jacobi_T_16Ne_56;
    TH2I * Jacobi_Y_16Ne_56;
    TH2I * Jacobi_Y_16Ne_69;
    TH1I * Ppara16Negs;
    TH1I * Ppara16Ne1st;
    TH1I * Ppara16Ne2nd;
    TH1I * Ppara16Ne3rd;

    TH2I * Erel_thetaCM_Ne16;

    TH1I * Ex16Ne_4p12C;
    TH1I * vel_16Ne_4p12C;
    TH1I * Ex16Ne_4p3a;
    TH1I * vel_16Ne_4p3a;
    TH2I * ExvET_16Ne2p14O;
    TH2I * AngvET_16Ne2p14O;
    TH1I * Ex16Ne_2p14O_For;
    TH1I * Ex16Ne_2p14O_Back;
    TH1I * Ex16Ne_2p14O_Long;
    TH1I * Ex16Ne_2p14O_Trans;
    TH1I * Ex16Ne_2p14O_ForBack;

    TH2I * Jacobi_Y_16Ne_fake_1st;
    TH2I * Jacobi_Y_16Ne_fake_1st_Back1;
    TH2I * Jacobi_Y_16Ne_fake_1st_Back2;

    TH2I * Jacobi_T_16Ne_fake_1st;
    TH2I * Jacobi_T_16Ne_fake_1st_Back1;
    TH2I * Jacobi_T_16Ne_fake_1st_Back2;

    TH1I * Ex16Ne_3p13N;
    TH1I * Et16Ne_3p13N;
    TH1I * Ex16Ne_p3He12N;
    TH2I * Ex14O_16Ne_3p13N;
    TH2I * Ex15F_16Ne_3p13N;
    TH1I * Ex16Ne_3pfake13N;
    TH1I * Et16Ne_3pfake13N;



    //17Ne
    TH1I* Ex17Ne_2p15O; 
    TH1I* Ex17Ne_2p15O_m2; 
    TH1I* Ex17Ne_2p15O_m3; 
    TH1I* Ex17Ne_2p15O_25;
    TH1I* Ex17Ne_2p15O_26;
    TH1I* Ex17Ne_2p15O_28;
    TH1I* Ex17Ne_2p15O_31;


    TH2I* ExExT_17Ne2p15O;


    TH1I* Ex17Ne_pd14O;
    TH1I* vel17Ne_pd14O;

    TH2I* Ex17Ne_vs_fake16Ne;
    TH2I* Ex17Ne_vs_fake16Ne_25;
    TH2I* Ex17Ne_vs_fake16Ne_26;
    TH2I* Ex17Ne_vs_fake16Ne_28;
    TH2I* Ex17Ne_vs_fake16Ne_31;

    TH1I* vel_17Ne_2p15O;
    TH1I * ExTarget_17Ne_2p15O;
    TH1I * ExTarget_17Ne_2p15O_2nd;
    TH1I* Jacobi_x_T_17Ne_2nd;
    TH1I* Jacobi_x_Y_17Ne_2nd;
    TH2I* Jacobi_T_17Ne_2nd;
    TH2I* Jacobi_Y_17Ne_2nd;
    TH2I* Jacobi_T_17Ne_2nd_right;
    TH2I* Jacobi_Y_17Ne_2nd_right;
    TH2I* Jacobi_T_17Ne_2nd_left;
    TH2I* Jacobi_Y_17Ne_2nd_left;
    TH1I* Txfp_17Ne;
    TH1I* VelCM_17Ne_2p15O_2nd;
    TH1I* ThetaCM_17Ne_2p15O_2nd;
    TH1I * Ne17_CsImult;
    TH2I * JacobiY_mult2;
    TH2I * JacobiY_mult3;
    TH2I * protonxy25;
    TH2I * protonxy26;
    TH2I * protonxy28;
    TH2I * protonxy31;
    TH2I * protonxy_out;
    TH2I * protonxy_in;
    TH2I * ExvET_17Ne2p15O;
    TH1I * Ex17Ne_2p15O_For;
    TH1I * Ex17Ne_2p15O_Back;
    TH1I * Ex17Ne_2p15O_Long;
    TH1I * Ex17Ne_2p15O_Trans;
    TH1I * Ex17Ne_2p15O_ForBack;

    TH1I * ExNe17_2p15O_onep;

    TH1I * Ex16Ne_2pfake14O_For;
    TH1I * Ex16Ne_2pfake14O_Back;
    TH1I * Ex16Ne_2pfake14O_Long;
    TH1I * Ex16Ne_2pfake14O_Trans;

    TH2I * Erel_thetaCM_Ne17;

    TH1I * Cospsi_17Ne2nd;
    TH1I * Cospsi_17Ne2nd_elas;
    TH1I * Cospsi_17Ne2nd_elas_p1;
    TH1I * Cospsi_17Ne2nd_elas_p2;
    TH1I * Cospsi_17Ne2nd_inelas;
    TH1I * Cospsi_17Ne2nd_inelas_p1;
    TH1I * Cospsi_17Ne2nd_inelas_p2;

    TH1I * Ex17Ne_3He14O;
    TH1I * Ex17Ne_pa12N;
    TH1I * Ex17Ne_3p14N;
    TH1I * Ex17Ne_2pa11C;




    //F16
    TH1I * Ex16F_2p14N;
    TH1I * Ex16F_2p14N_17Ne; 
    TH1I * Ex16F_2p14N_15O;
    TH1I * vel_16F_2p14N;
    TH1I * TEC_Dop_2p14N;
    TH1I * TEC_Dop_2p14N_noadd;
    TH1I * Ex16F_2p14N_Gamma;
    TH1I * Txfp_16F;
    TH1I * Ex16F_2p14N_noGamma;
    TH1I * Ex16F_2p14N_noGoodGamma;
    TH1I * Ex16F_2p14N_gammaBack;

    TH1I * Ex16F_2p14N_CsIMult;
    TH2I * Ex16F_2p_v_ceasar;

    TH2I * Ex16Fv15O;
    TH3I * Ex16Fv15Ovgamma;
    TH1I * Ex16F_peak1;
    TH1I * Ex16F_peak2;
    TH1I * Ex16F_peak3;

    TH1I * thetaCM_16F_peak2;
    TH1I * velCM_16F_peak2;

    TH1I * Ex16F_backlo;
    TH1I * Ex16F_backhi;

    TH1I * TEC_16F_15Ogated;
    TH2I * Ex16F_p15O_v_ceasar;
    TH1I * TEC_16F_p15O_0;
    TH1I * TEC_16F_p15O_1;
    TH1I * TEC_16F_p15O_2;
    TH1I * TEC_16F_p15O_3;
    TH1I * TEC_16F_p15O_4;
    TH1I * TEC_16F_p15O_5;

    TH1I * TEC_16F_p15O_0_O15;
    TH1I * TEC_16F_p15O_1_O15;
    TH1I * TEC_16F_p15O_2_O15;
    TH1I * TEC_16F_p15O_3_O15;
    TH1I * TEC_16F_p15O_4_O15;
    TH1I * TEC_16F_p15O_5_O15;

    TH1I * TEC_16F_p15O_1_times;
    TH1I * TEC_16F_p15O_others;
    TH1I * TEC_16F_p15O_1_theta;

    TH1I * TEC_16F_p15O_5_nodopp;
    TH1I * TEC_17Ne_2p15O;
    TH1I * TEC_15O_p14N;

    TH1I * Ex16F_p15O_gamma;
    TH1I * Ex16F_p15O_gammaHi;
    TH1I * Ex16F_p15O_gamma_O15;

    TH1I * TEC_16F_p15O_0_16F;
    TH1I * TEC_16F_p15O_1_16F;
    TH1I * TEC_16F_p15O_2_16F;
    TH1I * TEC_16F_p15O_3_16F;
    TH1I * TEC_16F_p15O_4_16F;
    TH1I * TEC_16F_p15O_5_16F;

    TH1I * TEC_16F_p15O_0_16F_O15;
    TH1I * TEC_16F_p15O_1_16F_O15;
    TH1I * TEC_16F_p15O_2_16F_O15;
    TH1I * TEC_16F_p15O_3_16F_O15;
    TH1I * TEC_16F_p15O_4_16F_O15;
    TH1I * TEC_16F_p15O_5_16F_O15;

    TH1I * TEC_15O_added;

    TH1I * Ex16F_3He13N;
    TH1I * vel_16F_3He13N;

    TH1I * Ex16F_p15O_15O;
    TH1I * Ex16F_p15O_14N;
    TH1I * Ex16F_p15O_17Ne;
    TH1I * Vel14N;
    TH1I* Ex16F_p15O;
    TH1I* vel_16F_p15O;
    TH1I* Ex16F_a12N;
    TH1I* vel_16F_a12N;

    TH1I * Ex16F_d14O;
    TH1I * Ex16F_3p13C;
    TH1I * Ex16F_pa11C;
    TH1I * Ex16F_p3He12C;
    TH1I * Ex16F_pt12N;
    TH1I * Ex16F_2pt11C;
    TH1I * Ex16F_4pa8Li;


    //15F
    TH1I* Ex15F_p14O;
    TH1I* Ex15F_p14O_15O;
    TH1I* Ex15F_p14O_17Ne;
    TH1I* ET15F_p14O_15O;
    TH1I* vel_15F_p14O;
    TH1I* Ex15F_2p14O_onep;
    TH1I * Ex15F_16Ne1st;
    TH1I * Ex15F_2p13N;
    TH1I * costhetap15F;
    TH2I * ETvTEC_15Fp14O;
    TH1I * Ex15F_3p12C;
    TH1I * Ex15F_3He12N;
    TH1I * Ex15F_p3He11C;
    TH1I * Ex15F_3p3a;


    //17F
    TH1I* Ex17F_p16O;
    TH1I* vel_17F_p16O;
    TH1I* Ex17F_a13N;
    TH1I* vel_17F_a13N;
    TH1I* TEC_17F_p16O;
    TH1I * Ex17F_3He14N;
    TH1I * Ex17F_t14O;
    TH1I * Ex17F_d15O;
    TH1I * Ex17F_2p15N;
    TH1I * Ex17F_pa12C;
    TH2I * Ex17F_pa12C_vs_16O;
    TH2I * Ex17F_pa12C_vs_13N;
    TH1I * Ex17F_pa12C_Fast;
    TH2I * Ex17F_pa12C_vs_16O_Fast;
    TH2I * Ex17F_pa12C_vs_13N_Fast;
    TH1I * Ex17F_pa12C_Slow;
    TH2I * Ex17F_pa12C_vs_16O_Slow;
    TH2I * Ex17F_pa12C_vs_13N_Slow;
    TH1I * vel17F_pa12C;


    //18F
    TH1I * Ex18F_3He15N;
    TH1I * Ex18F_d16O;
    TH1I * Ex18F_a14N;
    TH1I * Ex18F_t15O;
    TH1I * Ex18F_6Li12C;


    //13O
    TH1I* Ex13O_1p12N;
    TH1I* vel_13O_1p12N;
    TH1I * TEC_Dop_13O;
    TH1I * Ex13O_2p11C;

    //14O
    TH1I* Ex14O_1p13N;
    TH1I* Ex14O_1p13N_17Ne;
    TH1I* Ex14O_1p13N_15O;
    TH1I* vel_14O_1p13N;
    TH1I * TEC_Dop_14O;
    TH1I * Ex14O_For;
    TH1I * Ex14O_Back;
    TH1I * Ex14O_Trans;
    TH1I * Ex14O_Long;
    TH1I * Ex14O_2p12C;
    TH1I * Ex14O_3He11C;
    TH1I * TEC_14O_2p12C;

    //15O
    TH1I* Ex15O_1p14N;
    TH1I* Ex15O_1p14N_17Ne;
    TH1I* vel_15O_1p14N;
    TH1I * TEC_Dop_15O;
    TH1I * TEC_15O_p14N_17Ne;
    TH1I * Ex15O_p14N_17Ne_IAS;
    TH1I * Ex15O_p14N_17Ne_Back;
    TH1I * Ex15O_1p14N_inner;
    TH1I * Ex15O_p14N_15O_gamma;
    TH2I * Ex15O_vs_caesar;
    TH1I * Ex15O_For;
    TH1I * Ex15O_Back;
    TH1I * Ex15O_Trans;
    TH1I * Ex15O_Long;
    TH1I * Ex15O_For_17Ne;
    TH1I * Ex15O_Back_17Ne;
    TH1I * Ex15O_Trans_17Ne;
    TH1I * Ex15O_Long_17Ne;
    TH1I * VelCM_15O_7MeV;
    TH1I * ThetaCM_15O_7MeV;
    TH1I * VelCM_15O_8MeV;
    TH1I * ThetaCM_15O_8MeV;
    TH1I * costheta_15O;

    TH1I * Ex15O_2p13C;
    TH1I * TEC15O_2p13C;
    TH1I * Ex15O_a11C;
    TH1I * TEC15O_a11C;
    TH1I * Ex15O_3He12C;
    TH1I * TEC15O_3He12C;
    TH1I * Ex15O_t12N;
    TH1I * Ex15O_d13N;
    TH1I * Ex15O_p2a6Li;

    //O16
    TH1I * Ex16O_p15N;
    TH1I * Ex16O_a12C;
    TH1I * Ex16O_4a;
    TH1I * Ex16O_d14N;
    TH1I * Ex16O_3He13C;
    TH1I * Ex16O_t13N;
    TH1I * Ex16O_6Li10B;
    TH1I * Ex16O_pa11B;

    //18Na
    TH1I * Ex18Na_3p15O;
    TH1I * vel_18Na_3p15O;
    TH1I * Ex18Na_2pd14O;
    TH1I * Ex18Na_p3He14O;

    //18Ne
    TH1I * Ex18Ne_2p16O;
    TH1I * ET18Ne_2p16O;

    TH1I * Ex18Ne_a14O;
    TH1I * Ex18Ne_3He15O;
    TH1I * Ex18Ne_pd15O;
    TH1I * Ex18Ne_pt14O;
    TH1I * Ex18Ne_6Li12N;
    TH1I * Ex18Ne_p17F;
    TH1I * ET18Ne_p17F;

    TH1I * thetaCM_18Ne_p17F;
    TH1I * velCM_18Ne_p17F;

    TH1I * thetaCM_18Ne_2p16O;
    TH1I * velCM_18Ne_2p16O;

    TH2I *Jacobi_Y_18Ne_2p16O;
    TH2I *Jacobi_T_18Ne_2p16O;

    TH1I * TEC_18Ne_2p16O;
    TH1I * TEC_18Ne_p17F;

    TH2I * Ex_vs_gamma_18Ne_p17F;

    TH1I * Ex18Ne_p17F_Back;
    TH1I * Ex18Ne_p17F_For;
    TH1I * Ex18Ne_p17F_Long;
    TH1I * Ex18Ne_p17F_Trans;
    TH1I * ET18Ne_p17F_Trans;

    TH2I * Ne18_protonXY;
    TH2I * Ne18_coreXY;

    //Blocker CsI
    TH1D* Blocker_E;
    TH2D* Blocker_ETOF;

    //6Li
    TH1I * Erel_6Li;

    TH1I * Erel_Back_Diff;
    TH1I * Erel_For_Diff;
    TH1I * Erel_Trans_Diff;
    TH1I * Erel_Back_Same;
    TH1I * Erel_For_Same;
    TH1I * Erel_Trans_Same;
    TH1I * Costheta;
    TH1I * vel_6Li;


    //5Be

    TH1I * ET5Be_2p3He;
    TH1I * vel5Be_2p3He;
    TH1I * Jacobi_x_T_5Be;
    TH1I * Jacobi_x_Y_5Be;
    TH1I * Jacobi_y_T_5Be;
    TH1I * Jacobi_y_Y_5Be;

    TH1I * ET5Be_2p3He_17Ne;
    TH1I * Jacobi_x_T_5Be_17Ne;
    TH1I * Jacobi_x_Y_5Be_17Ne;
    TH1I * Jacobi_y_T_5Be_17Ne;
    TH1I * Jacobi_y_Y_5Be_17Ne;

    TH1I * ET5Be_2p3He_15O;
    TH1I * Jacobi_x_T_5Be_15O;
    TH1I * Jacobi_x_Y_5Be_15O;
    TH1I * Jacobi_y_T_5Be_15O;
    TH1I * Jacobi_y_Y_5Be_15O;

    TH1I * ET5Be_2p3He_12C;
    TH1I * Jacobi_x_T_5Be_12C;
    TH1I * Jacobi_x_Y_5Be_12C;
    TH1I * Jacobi_y_T_5Be_12C;
    TH1I * Jacobi_y_Y_5Be_12C;

    TH1I * ET5Be_2p3He_no12C;
    TH1I * Jacobi_x_T_5Be_no12C;
    TH1I * Jacobi_x_Y_5Be_no12C;
    TH1I * Jacobi_y_T_5Be_no12C;
    TH1I * Jacobi_y_Y_5Be_no12C;

    TH1I * Ex12C_3A_5Be;
    TH1I * ET5Be_2p3He_3A;

    TH1I * TEC12C_5Be;

    TH2I * Epp_ET_5Be;
    TH2I * Costheta_ET_5Be;

    TH2I * Epp_ET_5Be_15O;
    TH2I * Costheta_ET_5Be_15O;

    TH2I * Epp_ET_5Be_17Ne;
    TH2I * Costheta_ET_5Be_17Ne;

    TH1I * ET_17Ne_2p3He12C;
    TH1I * ET_14O_2p12C;
    TH1I * ET_15O_3He12C;
    TH1I * ET_13N_p12C;

    //Be7
    TH1I * Ex7Be_p6Li;
    TH1I * vel_7Be_p6Li;
    TH1I * vel_7Be_p6Li_left;
    TH1I * vel_7Be_p6Li_right;
    TH1I * TEC_7Be_p6Li;
    TH1I * Ex7Be_p6Li_gamma;
    TH1I * Ex7Be_p6Li_gammaback1;
    TH1I * Ex7Be_p6Li_gammaback2;
    TH1I * Ex7Be_p6Li_gammaback3;
    TH2I * Caesarmult;

    //Be9
    TH1I * Ex9Be_p8Li;
    TH1I * vel_9Be_p8Li;

    //N12
    TH1I * Ex12N_p11C;
    TH1I * Ex12N_2p10B;
    TH1I * Ex12N_p2a3He;
    TH1I * Ex12N_2pa6Li;

    //N13
    TH1I * Ex13N_p12C;
    TH1I * Ex13N_2p11B;
    TH1I * Ex13N_d11C;
    TH1I * Ex13N_p3a;
    TH1I * Ex13N_3He10B;

    //N14
    TH1I * Ex14N_p13C;
    TH1I * vel14N_p13C;
    TH1I * Ex14N_d12C;
    TH1I * Ex14N_t11C;
    TH1I * Ex14N_3He11B;
    TH1I * Ex14N_a10B;
    TH1I * Ex14N_2a6Li;

    //N15
    TH1I * Ex15N_d13C;
    TH1I * Ex15N_t12C;
    TH1I * Ex15N_a11B;

    //C11
    TH1I * Ex11C_p10B;
    TH1I * vel11C_p10B;
    TH1I * TEC11C_p10B;
    TH1I * TEC11C_p10B_noadd;
    TH1I * TEC11C_p10B_neighbors;
    TH2I * GammaGamma_11C_p10B;
    TH1I * Ex11C_p10B_gamma;
    TH1I * Ex11C_p10B_gammagamma;
    TH1I * CeasarT_p10B;
    TH1I * Ex11C_pd2a;
    TH1I * Ex11C_a7Be;
    TH1I * Ex11C_pa6Li;
    TH1I * Ex11C_3He2a;

    //C12
    TH1I * Ex12C_p11B;
    TH1I * Ex12C_d10B;
    TH1I * Ex12C_tp2a;
    TH1I * Ex12C_3a;
    TH1I * Ex12C_6Li6Li;
    TH1I * Ex12C_pa7Li;
    TH1I * Ex12C_da6Li;
    TH1I * Erms12C_3a;

    //C13
    TH1I * Ex13C_d11B;
    TH1I * Ex13C_t10B;

    //Ne19
    TH1I * Ex19Ne_a15O;
    TH1I * Ex19Ne_3He16O;
    TH1I * Ex19Ne_pt15O;

    //Na17
    TH1I * Erel17Na_3p14O;
    TH1I * Erel17Na_3pfake14O;
    TH2I * Erel17Na_Erel16Ne;
    TH1I * vel_17Na;
    TH2I * ErelEx_17Na;
};
#endif
