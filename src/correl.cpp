#include "../include/correl.h"
#include "../include/constants/physicalConstants.h"
#include <cmath>

using namespace std;
void correl::load(solution* particle)
{

  if (particle->iZ == 1 && particle->iA == 1)
    {
      proton[multProton] = particle;
      if (multProton < 7) multProton++;
      multPro[particle->telescopeNumber]++;
      //proton[multProton]->A = 1;

    }
  else if (particle->iZ == 1 && particle->iA == 2)
    {
      H2[multH2] = particle;
      if (multH2 < 5) multH2++;
      telescopeNumber = particle->telescopeNumber;
      //H2[multH2]->A = 2;
      multh2[telescopeNumber]++;
    }
  else if (particle->iZ == 1 && particle->iA == 3)
    {

      H3[multH3] = particle;
      if (multH3 < 5) multH3++;
      telescopeNumber = particle->telescopeNumber;
      //H3[multH3]->A = 3;
      multh3[telescopeNumber]++;
    }
 else if (particle->iZ == 2 && particle->iA == 3)
    {
      He3[mult3He] = particle;
      if (mult3He < 5) mult3He++;
      telescopeNumber = particle->telescopeNumber;
      //He3[mult3He]->A = 3;
      mult3he[telescopeNumber]++;
    }
  else if (particle->iZ == 2 && particle->iA ==4)
    {
      alpha[multAlpha] = particle;
      if (multAlpha < 5) multAlpha++;
      telescopeNumber = particle->telescopeNumber;
      //alpha[multAlpha]->A = 4;
      multAlp[telescopeNumber]++;
    }
  else if (particle->iZ == 3 && particle->iA == 6)
    {
      Li6[mult6Li] = particle;
      if (mult6Li < 5) mult6Li++;
      telescopeNumber = particle->telescopeNumber;
     
      mult6li[telescopeNumber]++;
    }
  else if (particle->iZ == 6 && particle->iA == 9)
    {
      C9[mult9C] = particle;
      if (mult9C < 5) mult9C++;
      telescopeNumber = particle->telescopeNumber;
     
      mult9c[telescopeNumber]++;
    }
  else if (particle->iZ == 6 && particle->iA == 10)
    {
      C10[mult10C] = particle;
      if (mult10C < 5) mult10C++;
      telescopeNumber = particle->telescopeNumber;
     
      mult10c[telescopeNumber]++;
    }
  else if (particle->iZ == 8 && particle->iA == 13)
    {
      O13[mult13O] = particle;
      if (mult13O < 5) mult13O++;
      telescopeNumber = particle->telescopeNumber;
     
      mult13o[telescopeNumber]++;
    }
}
//**********************************************************
void correl::setMask()
{
  for (int i=0;i<5;i++)
    {
      maskProton[i] = 1;
      maskAlpha[i] = 1;
      mask2H[i] = 1;
      mask3He[i] = 1;
      mask6Li[i] = 1;
      mask3H[i] = 1;
    }
}
//************************************************************
void correl::zeroMask()
{
  for (int i=0;i<5;i++)
    {
      maskProton[i] =0;
      maskAlpha[i] = 0;
      mask3He[i] = 0;
      mask2H[i] = 0;
      mask3H[i] = 0;
      mask6Li[i] = 0;
      mask9C[i] = 0;
      mask10C[i] = 0;
      mask13O[i] = 0;
    }
}
//**********************************************************
void correl::zeroH2()
{
  for (int i=0;i<5;i++) mask2H[i] = 0;
}
//**********************************************************
void correl::zero3He()
{
  for (int i=0;i<5;i++) mask3He[i] = 0;
}
//*********************************************************
void correl::zeroProton()
{
  for (int i=0;i<5;i++) maskProton[i] = 0;
}
//************************************************
void correl::zeroAlpha()
{
  for (int i=0;i<5;i++) maskAlpha[i] = 0;
}
//*******************************************************
  /**
   * make an array of fragments for later analyisis
   */
void correl::makeArray(bool flagMask/*=0*/)
{

 N = 0;

 for (int j=0;j<multProton;j++) 
    {
    if (!flagMask || maskProton[j])
      {
	frag[N] = proton[j];
        N++;
      }
    }
  for (int j=0;j<multAlpha;j++)
    {
    if (!flagMask || maskAlpha[j]) 
      {
	frag[N] = alpha[j];
        N++;
      }
    }
  for (int j=0;j<multH2;j++)
    {
    if (!flagMask || mask2H[j]) 
      {
	frag[N] = H2[j];
	N++;
      }
    }
 for (int j=0;j<multH3;j++)
    {
    if (!flagMask || mask3H[j]) 
      {
	frag[N] = H3[j];
	N++;
      }
    }
 for (int j=0;j<mult3He;j++)
    {
    if (!flagMask || mask3He[j]) 
      {
	frag[N] = He3[j];
	N++;
      }
    } 
 for (int j=0;j<mult6Li;j++)
    {
    if (!flagMask || mask6Li[j]) 
      {
	frag[N] = Li6[j];
	N++;
      }
    } 
 
 for (int j=0;j<mult9C;j++)
    {
    if (!flagMask || mask9C[j]) 
      {
	frag[N] = C9[j];
	N++;
      }
    } 
 for (int j=0;j<mult10C;j++)
    {
    if (!flagMask || mask10C[j]) 
      {
	frag[N] = C10[j];
	N++;
      }
    }  
 for (int j=0;j<mult13O;j++)
    {
    if (!flagMask || mask13O[j]) 
      {
	frag[N] = O13[j];
	N++;
      }
    } 
}

//***************************************
void correl::Reset()
{
  multProton = 0;
  multAlpha = 0;
  multH2 = 0;
  mult3He = 0;
  mult6Li = 0;
  multH3 = 0;
  mult9C =0;
  mult10C=0;
  mult13O =0;
  multGamma = 0;
  for (int telescopeNumber=0;telescopeNumber<14;telescopeNumber++) 
    {
      multAlp[telescopeNumber] = 0;
      multPro[telescopeNumber] = 0;
      multh2[telescopeNumber] = 0;
      mult3he[telescopeNumber] = 0;
      mult6li[telescopeNumber] = 0;
      multh3[telescopeNumber] = 0;
      mult9c[telescopeNumber]=0;
      mult10c[telescopeNumber]=0;
      mult13o[telescopeNumber]=0;
      mult_Gamma[telescopeNumber] = 0;
    }
}

//**************************************************************
  /**
   * Finds the total kinetic energy of the fragments
   * in there center-of-mass frame.
   */
float correl::findErel()
{
  
  //first find total momentum
  for (int i=0;i<3;i++) Mtot[i] = 0.;
  float energyTot = 0.;   // total energy for relativity, total mass for newton



  for (int i=0;i<N;i++) 
    {
      energyTot += frag[i]->energyTot;
      for (int j=0;j<3;j++)
        Mtot[j] += frag[i]->Mvect[j];
    }

  momentumCM = 0.;
  for (int j=0;j<3;j++) momentumCM += pow(Mtot[j],2);
  momentumCM = sqrt(momentumCM);

  //transform to average
  
  //float velC[3]={0.,0.,10.8699}; // 7Be beam
  //float velC[3]={0.,0.,10.7685};// 9C
  float velC[3]={0.,0.,10.8381};// 9C

  Kinematics.transformMomentum(Mtot,velC,
        energyTot,momC);
  float mmc = 0.;
  for (int i=0;i<3;i++) mmc += pow(momC[i],2);
  mmc = sqrt(mmc);
  cosThetaC = momC[2]/mmc;
  PperpC = sqrt(pow(momC[0],2)+pow(momC[1],2));
  PparaC = momC[2];
  PtotC = sqrt(pow(momC[0],2)+pow(momC[1],2)+pow(momC[2],2));

  //velocity of center of mass
  velocityCM = momentumCM*SPEED_OF_LIGHT/energyTot;


  float velCM[3]={0.};
  for (int j=0;j<3;j++) velCM[j] = velocityCM/momentumCM*Mtot[j];
  thetaCM = acos(velCM[2]/velocityCM);
  phiCM = atan2(velCM[1],velCM[0]);

  float totalKE = 0.;
  for (int i=0;i<N;i++)
    {
      float eKinNew = 
      Kinematics.transformMomentum(frag[i]->Mvect,velCM,
        frag[i]->energyTot,frag[i]->MomCM);
      frag[i]->energyCM = eKinNew - Kinematics.scale*frag[i]->mass;
      totalKE += eKinNew - Kinematics.scale*frag[i]->mass;
    }


  if (N == 3)
    {
      float dot = 0.;
      float mm = 0.;
      for (int j=0;j<3;j++)
	{
         dot += frag[2]->MomCM[j]*momC[j];
         mm += pow(frag[2]->MomCM[j],2);
	}
      mm = sqrt(mm);
      cosAlphaQ = dot/mm/PtotC;
    }


  return totalKE;
}
//***********************************************************
void correl::getJacobi()
{

  for (int i=0;i<3;i++)
    {
      frag[i]->momentumCM = 0.;
      for (int k=0;k<3;k++) frag[i]->momentumCM +=
	pow(frag[i]->MomCM[k],2);
      frag[i]->momentumCM = sqrt(frag[i]->momentumCM);
    }


  //alpha is the  third fragment
  //first JacobiT
  float dot = 0.;
  float pp[3] = {0.};
  float PP = 0.;
  for (int k=0;k<3;k++)
    {
      pp[k] = frag[0]->MomCM[k] - frag[1]->MomCM[k];
      PP += pow(pp[k],2);
      dot += pp[k]*frag[2]->MomCM[k];
    }
  PP = sqrt(PP);
  cosThetaT = dot/PP/frag[2]->momentumCM;


  dot = 0;
  double PP1 = 0;
  double pp1[3]={0.};
  for (int k=0;k<3;k++)
    {
      pp1[k] = frag[0]->MomCM[k]/frag[0]->mass 
            - frag[2]->MomCM[k]/frag[2]->mass;
      PP1 += pow(pp1[k],2);
      dot += pp1[k]*frag[1]->MomCM[k];
    }  
  PP1 = sqrt(PP1);
  cosThetaY[0] = -dot/PP1/frag[1]->momentumCM;


  dot = 0;
  double PP2 = 0;
  double pp2[3]={0.};
  for (int k=0;k<3;k++)
    {
      pp2[k] = frag[1]->MomCM[k]/frag[1]->mass 
            - frag[2]->MomCM[k]/frag[2]->mass;
      PP2 += pow(pp2[k],2);
      dot += pp2[k]*frag[0]->MomCM[k];
    }  
  PP2 = sqrt(PP2);
  cosThetaY[1] = -dot/PP2/frag[0]->momentumCM;


  cosThetaV = (pp1[0]*pp2[0] + pp1[1]*pp2[1] + pp1[2]*pp2[2])/PP1/PP2;
}
//*****************************************************
void correl::printP()
{

  for (int i=0;i<5;i++)
    {
      frag[i]->momentumCM = 0.;
      for (int k=0;k<3;k++) frag[i]->momentumCM +=
	pow(frag[i]->MomCM[k],2);
      frag[i]->momentumCM = sqrt(frag[i]->momentumCM);
    }

  cout << frag[0]->MomCM[0] << " " << frag[0]->MomCM[1] << " " << 
          frag[0]->MomCM[2] <<endl;

  cout << frag[1]->MomCM[0] << " " << frag[1]->MomCM[1] << " " << 
          frag[1]->MomCM[2] <<endl;


  cout << frag[2]->MomCM[0] << " " << frag[2]->MomCM[1] << " " << 
          frag[2]->MomCM[2] <<endl;

  cout << frag[3]->MomCM[0] << " " << frag[3]->MomCM[1] << " " << 
          frag[3]->MomCM[2] <<endl;


  cout << frag[4]->MomCM[0] << " " << frag[4]->MomCM[1] << " " << 
    frag[4]->MomCM[2] << " " << frag[4]->momentumCM << " " <<
    Kinematics.getKE(frag[4]->momentumCM,frag[4]->mass) <<  endl;




  cout << endl;

    
}
//****************************************************************
float correl::getAlphaMom()
{
     frag[4]->momentumCM = 0.;
      for (int k=0;k<3;k++) frag[4]->momentumCM +=
	pow(frag[4]->MomCM[k],2);
      frag[4]->momentumCM = sqrt(frag[4]->momentumCM);
      return frag[4]->momentumCM;
}
//********************************************************************
void correl::rotate()
{
  float phi = atan2(frag[4]->MomCM[1],frag[4]->MomCM[0]);

  //rotate in x-y plane
  for (int j=0;j<5;j++)
    {
      frag[j]->MomRot[0] = frag[j]->MomCM[0]*cos(phi) + frag[j]->MomCM[1]*sin(phi);
      frag[j]->MomRot[1] = frag[j]->MomCM[1]*cos(phi) - frag[j]->MomCM[0]*sin(phi);
      frag[j]->MomRot[2] = frag[j]->MomCM[2];
    }

  float theta = acos(frag[4]->MomCM[2]/frag[4]->momentumCM);


  //rotate in x-y plane
  for (int j=0;j<5;j++)
    {
      frag[j]->MomRot2[2] = frag[j]->MomRot[2]*cos(theta) + frag[j]->MomRot[0]*sin(theta);
      frag[j]->MomRot2[0] = frag[j]->MomRot[0]*cos(theta) - frag[j]->MomRot[2]*sin(theta);
      frag[j]->MomRot2[1] = frag[j]->MomRot[1];
    }
}
