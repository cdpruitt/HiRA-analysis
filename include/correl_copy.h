#include "solution.h"
#include <iostream>


/**
 * !\brief multi-particle correlations
 *
 * This class histograms the kinetic energy of multi-particle
 * events in their center-of-mass frame and looks for the 
 * correlations between the fragments
 */


class correl
{
  public:
#ifdef rel
  CEinstein Kinematics;
#else
  CNewton Kinematics;
#endif
  solution *proton[7];
  solution *alpha[5];
  solution *H2[5];
  solution *He3[5];
  solution *Be7[5];
  solution *He6[5];
  solution *He8[5];
  solution *H3[5];
  solution *Li6[5];
  solution *Li7[5];
  solution *Li8[5];
  solution *Be9[5];
  solution *B10[5];
  solution *B11[5];
  solution *C11[5];
  solution *C12[5];
  solution *C13[5];
  solution *N12[5];
  solution *N13[5];
  solution *N14[5];
  solution *N15[5];
  solution *O13[5];
  solution *O14[5];
  solution *fakeO13[5];
  solution *fakeO132[5];
  solution *fakeN13[5];
  solution *fakeO14[5];
  solution *O15[5];
  solution *O16[5];
  solution *F17[5];
  solution *frag[7];
  solution *gamma[5];
  int N;
  float energyTot;

  void load(solution*);
  void Reset();
  float findErel();
  void setMask();
  void zeroMask();
  void zeroProton();
  void zeroAlpha();
  void zero3He(); 
  void zeroH2();
  void printP();
  void makeArray(bool flagMask = 0);

  int multProton;
  int multAlpha;
  int mult3He;
  int mult7Be; 
  int mult6He;
  int mult8He;
  int multH2;
  int mult6Li;
  int mult7Li;
  int mult8Li;
  int mult9Be;
  int mult10B;
  int mult11B;
  int multH3;
  int multGamma;
  int mult11C;
  int mult12C;
  int mult13C;
  int mult12N;
  int mult13N;
  int mult14N;
  int mult15N;
  int mult13O;
  int mult14O;
  int multfake13O;
  int multfake13O2;
  int multfake13N;
  int multfake14O;
  int mult15O;
  int mult16O;
  int mult17F;
  float Vcmframevector[3];
  float Vcmframe;
  float VcmframePerp;
  float VcmframePara;
  float TKEL;
  float Erel;
  float KEToT;
  float thetaCM;
  float phiCM;
  float thetaInReactCM;
  int maskProton[5];
  int maskAlpha[5];
  int mask3He[5];
  int mask2H[5];
  int mask3H[5];
  int mask6He[5];
  int mask6Li[5];
  int mask7Li[5];
  int mask8Li[5];
  int mask7Be[5];
  int mask9Be[5];
  int mask10B[5];
  int mask11B[5];
  int mask13O[5];
  int mask14O[5];
  int maskfake13O[5];
  int maskfake13O2[5];
  int maskfake13N[5];
  int maskfake14O[5];
  int mask15O[5];
  int mask16O[5];
  int mask17F[5];
  int mask11C[5];
  int mask12C[5];
  int mask13C[5];
  int mask12N[5];
  int mask13N[5];
  int mask14N[5];
  int mask15N[5];
  int ifront;
  int istrip;
  int telescopeNumber;
  // int Norder;
  float Vlab;

  int multAlp[14];
  int multPro[14];
  int mult3he[14];
  int multh2[14];
  int mult7be[14];
  int mult6he[14];
  int mult8he[14];
  int mult6li[14];
  int mult7li[14];
  int mult8li[14];
  int mult9be[14];
  int mult10b[14];
  int mult11b[14];
  int mult11c[14];
  int mult12c[14];
  int mult13c[14];
  int mult12n[14];
  int mult13n[14];
  int mult14n[14];
  int mult15n[14];
  int mult13o[14];
  int mult14o[14];
  int multfake13o[14];
  int multfake13o2[14];
  int multfake13n[14];
  int multfake14o[14];
  int mult15o[14];
  int mult16o[14];
  int mult17f[14];
  int multh3[14];
  int mult_Gamma[14];

  float thetaAlpha;
  float phiAlpha;
  float Thetaopen;
  float ppThetaRel;
  float cosThetapp;
  // float protvel1; 
  // float protvel2; 
 float pTheta0;
  float pTheta2;
  float pTheta3;
  float pTheta1;
  float aTheta0;
  float pThetaMax;
  float aTheta1;
  float thetadip;
  float Epp;
  float CosAngle;

  float CosAngleY1;
  float CosAngleY2;
  float Ey1;
  float Ey2;


  float Ey1_10;
  float Ey2_10;
  float CosAngleAlphaY1_10;
  float CosAngleAlphaY2_10;

  float Epp_10;
  float CosAngle_10;

  float CosAngleAlpha;
  float CosAngleAlphaY1;
  float CosAngleAlphaY2;
  float Eaa;
  float ThetaP;
  float ThetaA1;
  float ThetaA2;
  float PhiP;
  float PhiA1;
  float PhiA2;
  float cosAA;
 
  float aarelbeamangle;
  float prelbeamangle;

  float Eap_min, Eap_max;

  float ThetaA1_P, ThetaA2_P;

  float cosAlpha[2];
  float cosProton0[2];
  float cosProton1[2];
  float energy3;

  float velocityCM;
  float momentumCM;


  //jacobi
  void getJacobi();
  float cosThetaT;
  float cosThetaY[2];
  float cosThetaV;
  float getAlphaMom();
  float x_T;
  float x_Y[2];
  void rotate();

  void find6Be(int,int,int,int);


  float momC[3];
  float Mtot[3];
  float cosThetaC;
  float PperpC;
  float PparaC;
  float PtotC;
  float cosAlphaQ;

};
