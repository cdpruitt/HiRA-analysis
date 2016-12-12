#include "../include/det.h"

#include <fstream>

#include <iostream>





//*********************************************************
  /**
   * Constructor
   */
det::det(histo_sort * Histo0, histo_read * Histo1, forest * Forest0)
{
  
  Histo_sort = Histo0;
  Histo_read = Histo1;
  Forest = Forest0;
  ran = new TRandom;
  Hira = new hira(ran,Histo_sort);
  Ceasar = new ceasar(ran,Histo_sort);
  //  Doppler = new doppler(0.349); // beta for 65 Mev/A
  Doppler = new doppler(0.326477); // beta for 54 MeV/A Ne-17

}
//************************************************
  /**
   * destructor
   */
det::~det()
{
  delete Ceasar;
  delete Hira;
  delete ran;
  delete Doppler;
  delete Forest;
  cout << "You made it!" << endl;
}
//*************************************************************
  /**
   * unpacks a physics event from the data stream
   \param point0 - pointer to location in data stream
  */
bool det::unpack(unsigned short *point,int runno)
{
  Hira->reset();

  //cout << runno << endl;

  unsigned short  words = *point++;

  point++; //Reads b0fe

  Run = runno;
  
  bool stat = false;
  stat = Hira->unpack(point,runno);
  if (!stat) return stat;
  stat = Ceasar->unpack(point);
  
  return stat;


}
//*********************************

void det::analyze(int event)
{
  Correl.Reset();

  Eventt = event;

  float Nsol =0;
  int mult  =0;
  CsImult = 0;

  Histo_read->Blocker_ETOF->Fill(Ceasar->Txfp[0],Hira->Blocker_e);


  for (int itele=0;itele<14;itele++)
    {
      CsImult += Hira->Telescope[itele]->Csi.Nstore;
      if(Hira->Telescope[itele]->Csi.Nstore==0) continue;
      Hira->Telescope[itele]->analyze(Eventt);
      Nsol = Hira->Telescope[itele]->Nsolution;
      
      for(int ii=0;ii<Nsol;ii++)
	{
	  if(Hira->Telescope[itele]->Solution[ii].iZ>0)
	    {
	      Correl.load(&Hira->Telescope[itele]->Solution[ii]);
	      mult++;
	    }
	}
    } 

  if(mult ==1)
    {
      if(Correl.mult15O ==1)
	{
	  for(int i=0;i<Ceasar->Nadded;i++)
		{
		  float dopp = 0.;
		  phig = Ceasar->added[i].phi;
		  thetag = Ceasar->added[i].theta;
		  xg = sin(thetag)*cos(phig);
		  yg = sin(thetag)*sin(phig);
		  zg = cos(thetag);
		  dot = (xg*xL)+(yg*yL)+(zg*zL);
		  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
		  thetarel = acos(dot/mag);
		  dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.O15[0]->velocity);

		  if(dopp !=0)
		    Histo_read->TEC_15O_added->Fill(dopp);

		}
	} 
    }
  else if(mult >= 2)
    {
      corr_6Li();
      corr_5Be();
      corr_7Be();
      corr_9Be();
      corr_11C();
      corr_12C();
      corr_13C();
      corr_12N();
      corr_13N();
      corr_14N();
      corr_15N();
      corr_13O();
      corr_14O();
      corr_15O();
      corr_16O();
      corr_15F();
      corr_16F();
      corr_17F();
      corr_18F();
      corr_15Ne(); 
      corr_16Ne();
      corr_17Ne();
      corr_18Ne();
      corr_19Ne();
      corr_17Na();
      corr_18Na();
    }



}


void det::corr_16Ne()
{
  if(Correl.multProton ==2 && Correl.multfake13O ==1)
    {


      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskfake13O[0]=1;   
      Correl.makeArray(1);
      float Erel_fake15Ne = Correl.findErel();
      
      Histo_read->Erel15Ne_2pfake13O->Fill(Erel_fake15Ne);

      Correl.maskProton[1] = 0;
      Correl.makeArray(1);
      float ExY1 = Correl.findErel()/Erel_fake15Ne;

      Correl.maskProton[1] = 1;
      Correl.maskProton[0] = 0;
      Correl.makeArray(1);
      float ExY2 = Correl.findErel()/Erel_fake15Ne;

      Correl.maskProton[0] = 1;
      Correl.maskfake13O[0] = 0;

      Correl.makeArray(1);
      float ExT = Correl.findErel()/Erel_fake15Ne;

      Correl.maskfake13O[0] = 1;
      Correl.makeArray(1);
      Correl.findErel();
      Correl.getJacobi();

      float theta_k_T = Correl.cosThetaT;
      float theta_k_Y1 = Correl.cosThetaY[0];
      float theta_k_Y2 = Correl.cosThetaY[1];


      if(Erel_fake15Ne >2.25 && Erel_fake15Ne < 2.75)
	{
	  Histo_read->JacobiY_15Ne_2pfake13O->Fill(ExY1,theta_k_Y1);
	  Histo_read->JacobiY_15Ne_2pfake13O->Fill(ExY2,theta_k_Y2);
	  Histo_read->JacobiT_15Ne_2pfake13O->Fill(ExT,-theta_k_T);
	  Histo_read->JacobiT_15Ne_2pfake13O->Fill(ExT,theta_k_T);

	}

      
    }

  //2p+14O
  if (Correl.multProton == 2 && Correl.mult14O == 1)
    {
      N2p14O++;
     

      int CsImulti = 3;

      int Csimulti = 3;
      int pro1tele = Correl.proton[0]->itele;
      int pro2tele =Correl.proton[1]->itele;
      int oxytele = Correl.O14[0]->itele;

      if(pro1tele == pro2tele) CsImulti--;
      if(pro1tele == oxytele) CsImulti--;
      else if (pro2tele == oxytele) CsImulti--;

     
      phiL = Correl.O14[0]->phi;
      thetaL = Correl.O14[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);
    
      int imax = 0;
      float tot_energy =0.;
      for(int i=0;i<Ceasar->Nselect;i++)
	{
	  tot_energy += Ceasar->select[i].energy;
	  if(Ceasar->select[i].energy >= Ceasar->select[imax].energy) imax = i;
	  
	}
      float const Q16Ne = -1.401;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask14O[0]=1;   
      Correl.makeArray(1);
      float Erel_16Ne = Correl.findErel();


      for(int i =0;i<Ceasar->Nadded;i++)
	{
	  phig = Ceasar->select[i].phi;
	  thetag = Ceasar->select[i].theta;
	  xg = sin(thetag)*cos(phig);
	  yg = sin(thetag)*sin(phig);
	  zg = cos(thetag);
	  dot = (xg*xL)+(yg*yL)+(zg*zL);
	  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	  thetarel = acos(dot/mag);            
	  
	  float dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.O14[0]->velocity);
	  if(dopp !=0)
	    Histo_read->ETvTEC_16Ne2p14O->Fill(Erel_16Ne,dopp);
      
	}
        
      Histo_read->ErelEkp_16Ne2p14O->Fill(Correl.proton[0]->Ekin,Erel_16Ne);
      Histo_read->ErelEkp_16Ne2p14O->Fill(Correl.proton[1]->Ekin,Erel_16Ne);


      //cout << Correl.O14[0]->itele << " " << Correl.proton[0]->itele << " " <<
      //	Correl.proton[1]->itele << " " << Erel_16Ne << endl;

      float thetaCM = Correl.thetaCM;
      float Ex = Erel_16Ne + Q16Ne;
      //  cout << "Ex 16Ne " << Ex << endl;
      Histo_read->Ex16Ne_2p14O->Fill(Ex);
      if (CsImulti == 2)Histo_read->Ex16Ne_2p14O_m2->Fill(Ex);
      else Histo_read->Ex16Ne_2p14O_m3->Fill(Ex);
      Histo_read->vel_16Ne_2p14O->Fill(Correl.velocityCM);

      Histo_read->Erel_thetaCM_Ne16->Fill(thetaCM*180./acos(-1),Erel_16Ne);



      int oxycsi = Correl.O14[0]->icsi;

      if(oxytele == 6 && oxycsi == 1)
	{
	  Histo_read->Ex16Ne_2p14O_25->Fill(Ex);
	}
      else if(oxytele == 6 && oxycsi == 2)
	{
	  Histo_read->Ex16Ne_2p14O_26->Fill(Ex);
	}
      else if(oxytele == 7 && oxycsi == 0)
	{
	  Histo_read->Ex16Ne_2p14O_28->Fill(Ex);
	}
      else if(oxytele == 7 && oxycsi == 3)
	{
	  Histo_read->Ex16Ne_2p14O_31->Fill(Ex);
	}



      Histo_read->ET16Ne_2p14O->Fill(Erel_16Ne);

      Correl.maskProton[1] = 0;
      Correl.makeArray(1);
      float ExY1 = Correl.findErel()/Erel_16Ne;

      Correl.maskProton[1] = 1;
      Correl.maskProton[0] = 0;
      Correl.makeArray(1);
      float ExY2 = Correl.findErel()/Erel_16Ne;

      Correl.maskProton[0] = 1;
      Correl.mask14O[0] = 0;

      Correl.makeArray(1);
      float ExT = Correl.findErel()/Erel_16Ne;


      Histo_read->ErelExT_16Ne2p14O->Fill(ExT,Erel_16Ne);
      Histo_read->ExExT_16Ne2p14O->Fill(ExT,Ex);
      if (ExT < 0.33)
	{
	  float jj = 0;
	  for (int i=0;i<3;i++) jj += pow(Correl.O14[0]->MomCM[i],2);
	  jj = Correl.O14[0]->MomCM[2]/sqrt(jj);
          Histo_read->ErelDiAngle_16Ne2p14O->Fill(jj,Erel_16Ne);          
	}


      Correl.mask14O[0] = 1;
      Correl.makeArray(1);
      Correl.findErel();
      Correl.getJacobi();

      float theta_k_T = Correl.cosThetaT;
      float theta_k_Y1 = Correl.cosThetaY[0];
      float theta_k_Y2 = Correl.cosThetaY[1];

      // if(ExY1*Erel_16Ne > 4.0)
      //	cout << "Y " <<ExY1*Erel_16Ne << endl;

      Histo_read->ExvET_16Ne2p14O->Fill(Erel_16Ne,ExY1*Erel_16Ne);
      Histo_read->ExvET_16Ne2p14O->Fill(Erel_16Ne,ExY2*Erel_16Ne);

      Histo_read->AngvET_16Ne2p14O->Fill(Erel_16Ne,theta_k_Y1);
      Histo_read->ExvET_16Ne2p14O->Fill(Erel_16Ne,theta_k_Y2);

      if(Erel_16Ne >4 && Erel_16Ne < 5)
	{
	  Histo_read->Jacobi_T_16Ne_45->Fill(ExT,theta_k_T);
	  Histo_read->Jacobi_T_16Ne_45->Fill(ExT,-theta_k_T);
	  Histo_read->Jacobi_Y_16Ne_45->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_16Ne_45->Fill(ExY2,theta_k_Y2);
	}
      else if(Erel_16Ne > 5 && Erel_16Ne < 6)
	{
	  Histo_read->Jacobi_T_16Ne_56->Fill(ExT,theta_k_T);
	  Histo_read->Jacobi_T_16Ne_56->Fill(ExT,-theta_k_T);
	  Histo_read->Jacobi_Y_16Ne_56->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_16Ne_56->Fill(ExY2,theta_k_Y2);
	}

      else if (Erel_16Ne > 6 && Erel_16Ne < 9)
	{
	  Histo_read->Jacobi_Y_16Ne_69->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_16Ne_69->Fill(ExY2,theta_k_Y2);
	}

      if(Ex > -0.13 && Ex < 0.32)
	{
	  Histo_read->Jacobi_x_T_16Ne_gs->Fill(ExT);
	  Histo_read->Jacobi_x_Y_16Ne_gs->Fill(ExY1);
	  Histo_read->Jacobi_x_Y_16Ne_gs->Fill(ExY2);
	  Histo_read->Jacobi_T_16Ne_gs->Fill(ExT,theta_k_T);
	  Histo_read->Jacobi_T_16Ne_gs->Fill(ExT,-theta_k_T);
	  Histo_read->Jacobi_Y_16Ne_gs->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_16Ne_gs->Fill(ExY2,theta_k_Y2);

	  Histo_read->Ne16_gs_VelCM->Fill(Correl.velocityCM);
	  Histo_read->Ne16_gs_ThetaCM->Fill(thetaCM*180/acos(-1));

	  Histo_read->Ppara16Negs->Fill(Correl.Mtot[2]);

	}

      else if(Ex > 1.4 && Ex < 2.1)
	{
	  Histo_read->Jacobi_x_T_16Ne_1st->Fill(ExT);
	  Histo_read->Jacobi_x_Y_16Ne_1st->Fill(ExY1);
	  Histo_read->Jacobi_x_Y_16Ne_1st->Fill(ExY2);
	  Histo_read->Jacobi_T_16Ne_1st->Fill(ExT,theta_k_T);
	  Histo_read->Jacobi_T_16Ne_1st->Fill(ExT,-theta_k_T);
	  Histo_read->Jacobi_Y_16Ne_1st->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_16Ne_1st->Fill(ExY2,theta_k_Y2);


	  Histo_read->Ne16_1st_VelCM->Fill(Correl.velocityCM);
	  Histo_read->Ne16_1st_ThetaCM->Fill(thetaCM*180/acos(-1));

	  Histo_read->Ppara16Ne1st->Fill(Correl.Mtot[2]);
	}

      else if(Ex > 0.7 && Ex < 1.15)
	{
	  Histo_read->Jacobi_T_16Ne_1st_Back1->Fill(ExT);
	  Histo_read->Jacobi_Y_16Ne_1st_Back1->Fill(ExY1);
	  Histo_read->Jacobi_Y_16Ne_1st_Back1->Fill(ExY2);
	}

      else if(Ex > 2.4 && Ex < 2.85)
	{
	  Histo_read->Jacobi_T_16Ne_1st_Back2->Fill(ExT);
	  Histo_read->Jacobi_Y_16Ne_1st_Back2->Fill(ExY1);
	  Histo_read->Jacobi_Y_16Ne_1st_Back2->Fill(ExY2);
	}
      if(Ex > 2.5 && Ex < 5)
	{
	  Histo_read->Ppara16Ne2nd->Fill(Correl.Mtot[2]);
	}
      if(Ex > 5.3 && Ex < 7.)
	{
	  Histo_read->Ppara16Ne3rd->Fill(Correl.Mtot[2]);
	}
      if(Ex >1.4 && Ex < 1.8)
	{
	  Histo_read->Jacobi_T_16Ne_1st_Left->Fill(ExT);
	  Histo_read->Jacobi_Y_16Ne_1st_Left->Fill(ExY1);
	  Histo_read->Jacobi_Y_16Ne_1st_Left->Fill(ExY2);
	}

      if(Ex >=1.8 && Ex < 2.3)
	{
	  Histo_read->Jacobi_T_16Ne_1st_Right->Fill(ExT);
	  Histo_read->Jacobi_Y_16Ne_1st_Right->Fill(ExY1);
	  Histo_read->Jacobi_Y_16Ne_1st_Right->Fill(ExY2);
	}



      float p0 = 0.;
      float p1 = 0.;
      for(int i=0;i<2;i++)
	{
	  p0 += pow(Correl.proton[0]->MomCM[i],2);
	  p1 += pow(Correl.proton[1]->MomCM[i],2);
	}
      p0 = sqrt(p0);
      p1 = sqrt(p1);


      float costheta0 = 0.;
      float costheta1 = 0.;

      costheta0 = Correl.proton[0]->MomCM[2]/p0;
      costheta1 = Correl.proton[1]->MomCM[2]/p1;

      if(costheta0 > 0.5 && costheta1 > 0.5)
	{
	  Histo_read->Ex16Ne_2p14O_For->Fill(Ex);
	  Histo_read->Ex16Ne_2p14O_Long->Fill(Ex);
	}
      else if(costheta0 < -0.5 && costheta1 < -0.5)
	{
	  Histo_read->Ex16Ne_2p14O_Back->Fill(Ex);
	  Histo_read->Ex16Ne_2p14O_Long->Fill(Ex);
	}
      else if( costheta0 > -0.5 && costheta0 < 0.5)
	{
	  if(costheta1 > -0.5 && costheta1 < 0.5)
	    Histo_read->Ex16Ne_2p14O_Trans->Fill(Ex);
	}
      else if((costheta0 > 0.5 && costheta1 < -0.5) 
	      || (costheta0 < -0.5 && costheta1 > 0.5))
	{
	  Histo_read->Ex16Ne_2p14O_ForBack->Fill(Ex);
	}









      //What does 15F look like if we miss a proton from p+p+14O
      Correl.maskProton[0]=0;
      Correl.makeArray(1);
      float const Q15F = -1.5166;
      float Erel_15F = Correl.findErel();
      float ExF = Erel_15F + Q15F;
      if(Ex > 0.17 && Ex < 0.32)
	Histo_read->Ex15F_2p14O_onep->Fill(ExF);
      else if(Ex > 1.4 && Ex < 1.9)
	Histo_read->Ex15F_16Ne1st->Fill(ExF);

      Correl.maskProton[0]=1;
      Correl.maskProton[1]=0;
      Correl.makeArray(1);
      Erel_15F = Correl.findErel();
      ExF = Erel_15F + Q15F;
      if(Ex > 0.17 && Ex < 0.32)
	Histo_read->Ex15F_2p14O_onep->Fill(ExF);
      else if(Ex > 1.4 && Ex < 1.9)
	Histo_read->Ex15F_16Ne1st->Fill(ExF);

    }
  else if(Correl.multProton == 4 && Correl.mult12C ==1)
    {
      float const Q16Ne = 5.170;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.maskProton[3]=1;
      Correl.mask12C[0]=1;   
      Correl.makeArray(1);
      float Erel_16Ne = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_16Ne + Q16Ne;
      Histo_read->Ex16Ne_4p12C->Fill(Ex);
      Histo_read->vel_16Ne_4p12C->Fill(Correl.velocityCM);

    }
  else if(Correl.multProton == 4 && Correl.multAlpha ==3)
    {
      float const Q16Ne = 12.444;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.maskProton[3]=1;
      Correl.maskAlpha[0]=1;
      Correl.maskAlpha[1]=1;
      Correl.maskAlpha[2]=1;
      Correl.makeArray(1);
      float Erel_16Ne = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_16Ne + Q16Ne;
      Histo_read->Ex16Ne_4p3a->Fill(Ex);
      Histo_read->vel_16Ne_4p3a->Fill(Correl.velocityCM);

    }

  if(Correl.multProton == 3 && Correl.mult13N ==1)
    {
      float const Q16Ne = 3.226;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.mask13N[0]=1;
      Correl.makeArray(1);
      float Erel_16Ne = Correl.findErel();
      float Ex = Erel_16Ne + Q16Ne;
      Histo_read->Ex16Ne_3p13N->Fill(Ex);
      Histo_read->Et16Ne_3p13N->Fill(Erel_16Ne);

      float Q14O = 4.627;
      float Q15F = 3.113;
      for(int i = 0;i<3;i++)
	{
	  Correl.zeroMask();
	  Correl.maskProton[i]=1;
	  Correl.mask13N[0]=1;
	  Correl.makeArray(1);
	  float Ex_14O = Correl.findErel() + Q14O;
	  Histo_read->Ex14O_16Ne_3p13N->Fill(Ex_14O,Ex);

      	  
	  Correl.zeroMask();
	  Correl.maskProton[0]=1;
	  Correl.maskProton[1]=1;
	  Correl.maskProton[2]=1;
	  Correl.maskProton[i]=0;
	  Correl.mask13N[0]=1;
	  Correl.makeArray(1);
	  float Ex_15F = Correl.findErel() + Q15F;
	  Histo_read->Ex15F_16Ne_3p13N->Fill(Ex_15F,Ex);
	  
	}
      
    }
  if(Correl.multProton == 1 && Correl.mult3He ==1 && Correl.mult12N == 1)
    {
      float const Q16Ne = 15.572;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask3He[0]=1;
      Correl.mask12N[0]=1;
      Correl.makeArray(1);
      float Erel_16Ne = Correl.findErel();
      float Ex = Erel_16Ne + Q16Ne;
      Histo_read->Ex16Ne_p3He12N->Fill(Ex);
    }



}


void det::corr_15F()
{
  if (Correl.multProton == 1 && Correl.mult14O == 1)
    {


      phiL = Correl.O14[0]->phi;
      thetaL = Correl.O14[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);

      int imax = 0;
      float tot_energy =0.;
      for(int i=0;i<Ceasar->Nselect;i++)
	{
	  tot_energy += Ceasar->select[i].energy;
	  if(Ceasar->select[i].energy >= Ceasar->select[imax].energy) imax = i;
	}

      phig = Ceasar->select[imax].phi;
      thetag = Ceasar->select[imax].theta;
      xg = sin(thetag)*cos(phig);
      yg = sin(thetag)*sin(phig);
      zg = cos(thetag);
      dot = (xg*xL)+(yg*yL)+(zg*zL);
      mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
      thetarel = acos(dot/mag);

      float maxe = 0.;
      maxe = Ceasar->select[imax].energy;
      float dopp = Doppler->correct(tot_energy,thetarel,Correl.O14[0]->velocity);
      


      float const Q15F = -1.5166;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask14O[0]=1;   
      Correl.makeArray(1);
      float Erel_15F = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_15F + Q15F;
      Histo_read->Ex15F_p14O->Fill(Ex);
      Histo_read->vel_15F_p14O->Fill(Correl.velocityCM);

      if(dopp !=0)
	Histo_read->ETvTEC_15Fp14O->Fill(Erel_15F,dopp);

      float p =  0.;
      for (int i=0;i<3;i++) p += pow(Correl.proton[0]->MomCM[i],2);
      p = sqrt(p);
      float cosThetap = Correl.proton[0]->MomCM[2]/p;


      if(Correl.velocityCM < 9.26)
	{
	  Histo_read->Ex15F_p14O_15O->Fill(Ex);
	  Histo_read->ET15F_p14O_15O->Fill(Erel_15F);
	  Histo_read->costhetap15F->Fill(cosThetap);

	}
      else
	Histo_read->Ex15F_p14O_17Ne->Fill(Ex);
    }

  else if (Correl.multProton == 2 && Correl.mult13N == 1)
    {

      float const Q15F = 3.1166;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask13N[0]=1;   
      Correl.makeArray(1);
      float Erel_15F = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_15F + Q15F;
      Histo_read->Ex15F_2p13N->Fill(Ex);
    }
  else if(Correl.multProton ==3 && Correl.mult12C ==1)
    {  
      float const Q15F = 5.057;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.mask12C[0]=1;   
      Correl.makeArray(1);
      float Erel_15F = Correl.findErel();
      float Ex = Erel_15F + Q15F;
      Histo_read->Ex15F_3p12C->Fill(Ex);
    }
  else if (Correl.mult3He == 1 && Correl.mult12N == 1)
    {

      float const Q15F = 15.459;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask12N[0]=1;   
      Correl.makeArray(1);
      float Erel_15F = Correl.findErel();
      float Ex = Erel_15F + Q15F;
      Histo_read->Ex15F_3He12N->Fill(Ex);
    }
  else if (Correl.mult3He == 1 && Correl.mult11C == 1 && Correl.multProton ==1)
    {
      
      float const Q15F = 16.060;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.maskProton[0]=1;
      Correl.mask11C[0]=1;   
      Correl.makeArray(1);
      float Erel_15F = Correl.findErel();
      float Ex = Erel_15F + Q15F;
      Histo_read->Ex15F_p3He11C->Fill(Ex);
    }
  else if(Correl.multProton ==3 && Correl.multAlpha ==3)
    {
      //      cout << "HEY" << endl;
      float const Q15F = 12.332;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.maskAlpha[0]=1;
      Correl.maskAlpha[1]=1;
      Correl.maskAlpha[2]=1;
      Correl.makeArray(1);
      float Erel15F = Correl.findErel();
      float Ex15F = Erel15F + Q15F;
      
      Histo_read->Ex15F_3p3a->Fill(Ex15F);
      
    }
  

}



void det:: corr_16F()
{
  ////13N + 3He
  if(Correl.mult13N ==1 && Correl.mult3He ==1)
    {
      phiL = Correl.N13[0]->phi;
      thetaL = Correl.N13[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);
    
      
      float const Q16F = 9.596;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask13N[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float Ex = Erel_16F + Q16F;


      float thetaCM = Correl.thetaCM;
      Histo_read->Ex16F_3He13N->Fill(Ex);
      Histo_read->vel_16F_3He13N->Fill(Correl.velocityCM);


    }



  //2p+14N
  if (Correl.multProton == 2 && Correl.mult14N == 1)
    {

      N2p14N++;
     


      Histo_read->Vel14N->Fill(Correl.N14[0]->velocity);
   
      float const Q16F = 6.761;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask14N[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_16F + Q16F;
      Histo_read->Ex16F_2p14N->Fill(Ex);
      Histo_read->vel_16F_2p14N->Fill(Correl.velocityCM);


      if(Correl.velocityCM > 9.1)
	Histo_read->Ex16F_2p14N_17Ne->Fill(Ex);
      else
	Histo_read->Ex16F_2p14N_15O->Fill(Ex);

      phiL = Correl.N14[0]->phi;
      thetaL = Correl.N14[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);

      for(int i=0;i<Ceasar->Nxfp;i++)
	{
	  float txfp = Ceasar->Txfp[i];
	  Histo_read->Txfp_16F->Fill(txfp);
	}



      float dopp = 0.;
      float doppmax = 0.;
      int imax = 0;
      for(int i =0;i<Ceasar->Nadded;i++)
	{
	  phig = Ceasar->added[i].phi;
	  thetag = Ceasar->added[i].theta;
	  xg = sin(thetag)*cos(phig);
	  yg = sin(thetag)*sin(phig);
	  zg = cos(thetag);
	  dot = (xg*xL)+(yg*yL)+(zg*zL);
	  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	  thetarel = acos(dot/mag);
	  dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.N14[0]->velocity);
	  
	  if(dopp !=0)
	    {
	      if(Ex < 8.0 && Ex > 0.)
		Histo_read->TEC_Dop_2p14N->Fill(dopp);
	    }
	  
	  if(dopp > doppmax)
	    {
	      doppmax = dopp;
	      imax = i;
	    }
	  
	  if(dopp >0)Histo_read->Ex16F_2p_v_ceasar->Fill(dopp,Ex);
	  
	}
      
      bool GotGamma = 0;
      
      if(doppmax > 2.2 && doppmax < 2.4) 
	{
	  GotGamma =1;
	}
      bool background = 0;
      if(doppmax > 2.45 && doppmax < 3.05 )
	background =1;
      else 
	background =0;
      
      
      
      
      if(GotGamma)
	{
	  Histo_read->Ex16F_2p14N_Gamma->Fill(Ex);
	  Histo_read->CeasarT_2p14N->Fill(Ceasar->select[imax].time);
	  if(Ex < 8.5 && Ex > 7.5)
	    Histo_read->CeasarT_2p14N_IAS->Fill(Ceasar->select[imax].time);
	}
      if(Ceasar->Nadded ==0)
	{
	  Histo_read->Ex16F_2p14N_noGamma->Fill(Ex);
	  if(CsImult ==3)
	    Histo_read->Ex16F_2p14N_CsIMult->Fill(Ex);
	  
	}
      if(!GotGamma)
	{
	  Histo_read->Ex16F_2p14N_noGoodGamma->Fill(Ex);
	}
      if(background)
	{
	  Histo_read->Ex16F_2p14N_gammaBack->Fill(Ex);
	}



      float const Q15O = 7.297;
      Correl.maskProton[0]=0;
      Correl.makeArray(1);
      float Erel_15O = Correl.findErel();
      float Ex15O = Erel_15O + Q15O;


      Histo_read->Ex16Fv15O->Fill(Ex15O,Ex);
      Histo_read->Ex16Fv15Ovgamma->Fill(Ex15O,Ex,doppmax);

      Correl.maskProton[0]=1;
      Correl.maskProton[1]=0;
      Correl.makeArray(1);
      Erel_15O = Correl.findErel();
      Ex15O = Erel_15O + Q15O;

      Histo_read->Ex16Fv15O->Fill(Ex15O,Ex);
      Histo_read->Ex16Fv15Ovgamma->Fill(Ex15O,Ex,doppmax);



      if(Ex15O >8.0 && Ex15O < 8.5)
	Histo_read->Ex16F_peak1->Fill(Ex);
      else if(Ex15O > 8.7 && Ex15O < 9.04)
	{
	  Histo_read->Ex16F_peak2->Fill(Ex);
	  if(dopp !=0) 
	    Histo_read->TEC_16F_15Ogated->Fill(doppmax);
	  Histo_read->thetaCM_16F_peak2->Fill(thetaCM*180/acos(-1));
	  Histo_read->velCM_16F_peak2->Fill(Correl.velocityCM);
	}
      else if(Ex15O > 9.2)
	Histo_read->Ex16F_peak3->Fill(Ex);
      
      if(Ex15O > 9.65 && Ex15O < 10.0)
	Histo_read->Ex16F_backhi->Fill(Ex);
      else if(Ex15O > 7.8 && Ex15O < 8.2)
	Histo_read->Ex16F_backlo->Fill(Ex);



    }

  if (Correl.multProton == 1 && Correl.mult15O == 1)
    {
      float const Q16F_1p = -.535;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask15O[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float velCM = Correl.velocityCM;
      float Ex = Erel_16F + Q16F_1p;


      float p =  0.;
      for (int i=0;i<3;i++) p += pow(Correl.proton[0]->MomCM[i],2);
      p = sqrt(p);
      float cosThetap = Correl.proton[0]->MomCM[2]/p;

      if (cosThetap > -0.5 && cosThetap < 0.5)
	{
          Histo_read->Ex16F_p15O->Fill(Ex);
          Histo_read->vel_16F_p15O->Fill(Correl.velocityCM);

	  if(Correl.velocityCM < 9.4 && Correl.velocityCM > 9.1)
	    Histo_read->Ex16F_p15O_15O->Fill(Ex);
	  else if (Correl.velocityCM < 9.1)
	    Histo_read->Ex16F_p15O_14N->Fill(Ex);
	  else if (Correl.velocityCM > 9.4)
	    Histo_read->Ex16F_p15O_17Ne->Fill(Ex);

       
	  
	  phiL = Correl.O15[0]->phi;
	  thetaL = Correl.O15[0]->theta;
	  xL = sin(thetaL)*cos(phiL);
	  yL = sin(thetaL)*sin(phiL);
	  zL = cos(thetaL);
	  
	  float phiF = Correl.phiCM;
	  float thetaF = thetaCM;
	  float xF = sin(thetaF)*cos(phiF);
	  float yF = sin(thetaF)*sin(phiF);
	  float zF = cos(thetaF);

	  //	  cout <<"Theta  =" << thetaL << " " << thetaF << endl;

	  if(Correl.velocityCM < 9.4 && Correl.velocityCM > 9.1)
	    {
	      float dopp = 0.;
	      float dopp2 = 0.;
	      for(int i =0;i<Ceasar->Nadded;i++)
		{
		  phig = Ceasar->added[i].phi;
		  thetag = Ceasar->added[i].theta;
		  xg = sin(thetag)*cos(phig);
		  yg = sin(thetag)*sin(phig);
		  zg = cos(thetag);
		  dot = (xg*xL)+(yg*yL)+(zg*zL);
		  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
		  thetarel = acos(dot/mag);
		  dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.O15[0]->velocity);

		  float dot2 = (xg*xF)+(yg*yF)+(zg*zF);
		  float mag2 = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xF,2)+pow(yF,2)+pow(zF,2));
		  float thetarel2 = acos(dot2/mag2);
		  dopp2 = Doppler->correct(Ceasar->added[i].energy,thetarel2,Correl.velocityCM/29.9792);

		  if(dopp2 > 4.4 && dopp2 <5.75 )
		    {
		      Histo_read->Ex16F_p15O_gamma_O15->Fill(Ex);
		    }

		  if(dopp !=0)
		    {
		      if(Ex < -0.12)
			{    
			  Histo_read->TEC_16F_p15O_0_O15->Fill(dopp);
			  Histo_read->TEC_16F_p15O_0_16F_O15->Fill(dopp2);
			}
		      else if(Ex > -0.12 && Ex < 0.14)
			{
			  Histo_read->TEC_16F_p15O_5_O15->Fill(dopp);
			  Histo_read->TEC_16F_p15O_5_16F_O15->Fill(dopp2);
			  //			  Histo_read->TEC_16F_p15O_5_nodopp_O15->Fill(Ceasar->added[i].energy);
			}
		      else if(Ex > 3.42 && Ex < 3.88)
			{
			  Histo_read->TEC_16F_p15O_4_O15->Fill(dopp);
			  Histo_read->TEC_16F_p15O_4_16F_O15->Fill(dopp2);
			}
		      else if(Ex > 0.14 && Ex < 0.30)
			{
			  Histo_read->TEC_16F_p15O_1_O15->Fill(dopp);
			  Histo_read->TEC_16F_p15O_1_16F_O15->Fill(dopp2);
			}
		      else if(Ex > 0.33 && Ex < 0.53)
			{
			  Histo_read->TEC_16F_p15O_2_O15->Fill(dopp);
			  Histo_read->TEC_16F_p15O_2_16F_O15->Fill(dopp2);
			}
		      else if(Ex > 0.62 && Ex < 0.82)
			{
			  Histo_read->TEC_16F_p15O_3_O15->Fill(dopp);
			  Histo_read->TEC_16F_p15O_3_16F_O15->Fill(dopp2);
			}
		    }
		}

	    }
	  else if(Correl.velocityCM >9.4)
	    {
	      int igamma = -1.;
	      bool gotgamma = 0;
	      float dopp = 0.;
	      float dopp2 = 0.;
	      for(int i =0;i<Ceasar->Nadded;i++)
		{
		  phig = Ceasar->added[i].phi;
		  thetag = Ceasar->added[i].theta;
		  xg = sin(thetag)*cos(phig);
		  yg = sin(thetag)*sin(phig);
		  zg = cos(thetag);
		  dot = (xg*xL)+(yg*yL)+(zg*zL);
		  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
		  thetarel = acos(dot/mag);
		  //cout << "1 theta = " << thetarel << " vel = " << Correl.O15[0]->velocity << " e1 = " << Ceasar->added[i].energy;

		  dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.O15[0]->velocity);
		  //		  cout << " e= " << dopp << endl;


		  float dot2 = (xg*xF)+(yg*yF)+(zg*zF);
		  float mag2 = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xF,2)+pow(yF,2)+pow(zF,2));
		  float thetarel2 = acos(dot2/mag2);
		  //		  cout << "theta = " << thetarel << " vel = " << Correl.velocityCM << " e1 = " << Ceasar->added[i].energy;
		  dopp2 = Doppler->correct(Ceasar->added[i].energy,thetarel2,Correl.velocityCM/29.9792);
		  //		  cout << "vel" <<Correl.O15[0]->velocity << " " << Correl.velocityCM/TMath::C() << " " << TMath::C()<< endl;
		  //cout << " e= " << dopp << " " <<dopp2 << " " << Ceasar->added[i].energy << endl;

		  if(dopp2 > 4.4 && dopp2 <5.75 )
		    {
		      Histo_read->Ex16F_p15O_gamma->Fill(Ex);
		    }
		  else if(dopp2 > 5.75)
		    {
		      Histo_read->Ex16F_p15O_gammaHi->Fill(Ex);
		    }

		  if(dopp !=0)
		    {
		      if(Ex < -0.12)
			{    
			  Histo_read->TEC_16F_p15O_0->Fill(dopp);
			  Histo_read->TEC_16F_p15O_0_16F->Fill(dopp2);
			}
		      else if(Ex > -0.12 && Ex < 0.14)
			{
			  Histo_read->TEC_16F_p15O_5->Fill(dopp);
			  Histo_read->TEC_16F_p15O_5_16F->Fill(dopp2);
			  Histo_read->TEC_16F_p15O_5_nodopp->Fill(Ceasar->added[i].energy);
			}
		      else if(Ex > 3.42 && Ex < 3.88)
			{
			  Histo_read->TEC_16F_p15O_4->Fill(dopp);
			  Histo_read->TEC_16F_p15O_4_16F->Fill(dopp2);
			}
		      else if(Ex > 0.14 && Ex < 0.30)
			{
			  Histo_read->TEC_16F_p15O_1->Fill(dopp);
			  Histo_read->TEC_16F_p15O_1_16F->Fill(dopp2);
			  if(dopp2 > 4.4 && dopp2 < 5.75)
			    {
			      Histo_read->TEC_16F_p15O_1_times->Fill(Ceasar->added[i].time);
			      //			      cout << thetag << endl;
			      Histo_read->TEC_16F_p15O_1_theta->Fill(thetag*180./acos(-1));
			    }

			  if(dopp2> 4.4 && dopp2 < 5.75)
			    {
			      gotgamma = 1;
			      igamma = i;
			    }
			}
		      else if(Ex > 0.33 && Ex < 0.53)
			{
			  Histo_read->TEC_16F_p15O_2->Fill(dopp);
			  Histo_read->TEC_16F_p15O_2_16F->Fill(dopp2);
			}
		      else if(Ex > 0.62 && Ex < 0.82)
			{
			  Histo_read->TEC_16F_p15O_3->Fill(dopp);
			  Histo_read->TEC_16F_p15O_3_16F->Fill(dopp2);
			}
		      Histo_read->Ex16F_p15O_v_ceasar->Fill(dopp,Ex);
		      
		    }
		}
	    
	      if(gotgamma)
		{
		  for(int i =0;i<Ceasar->Nadded;i++)
		    {
		      if(i!=igamma)
			{
			  float dopp = 0.;
			  phig = Ceasar->added[i].phi;
			  thetag = Ceasar->added[i].theta;
			  xg = sin(thetag)*cos(phig);
			  yg = sin(thetag)*sin(phig);
			  zg = cos(thetag);
			  dot = (xg*xF)+(yg*yF)+(zg*zF);
			  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xF,2)+pow(yF,2)+pow(zF,2));
			  thetarel = acos(dot/mag);
			  
			  dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.velocityCM/29.9792);
			  
			  if(dopp !=0)
			    Histo_read->TEC_16F_p15O_others->Fill(dopp);
			}
		    }
		}
	    }
	}

    }
  if (Correl.multAlpha == 1 && Correl.mult12N == 1)
    {

     

      phiL = Correl.N12[0]->phi;
      thetaL = Correl.N12[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);
     

      float const Q16F_1a = 9.0827;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.mask12N[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_16F + Q16F_1a;


      Histo_read->Ex16F_a12N->Fill(Ex);
      Histo_read->vel_16F_a12N->Fill(Correl.velocityCM);



    }
  else if (Correl.multH2 == 1 && Correl.mult14O == 1)
    {

      float const Q16F = 10.463;
      Correl.zeroMask();
      Correl.mask2H[0]=1;
      Correl.mask14O[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float Ex = Erel_16F + Q16F;
      Histo_read->Ex16F_d14O->Fill(Ex);
    }
  else if (Correl.multProton == 3 && Correl.mult13C == 1)
    {

      float const Q16F = 14.312;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.mask13C[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float Ex = Erel_16F + Q16F;
      Histo_read->Ex16F_3p13C->Fill(Ex);


    }
  else if (Correl.multProton == 1 && Correl.multAlpha == 1 && Correl.mult11C == 1)
    {

      float const Q16F = 9.684;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskAlpha[0]=1;
      Correl.mask11C[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float Ex = Erel_16F + Q16F;
      Histo_read->Ex16F_pa11C->Fill(Ex);
    }
  else if (Correl.multProton == 1 && Correl.mult3He == 1 && Correl.mult12C == 1)
    {

      float const Q16F = 11.540;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask3He[0]=1;
      Correl.mask12C[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float Ex = Erel_16F + Q16F;
      Histo_read->Ex16F_p3He12C->Fill(Ex);
    }
  else if (Correl.multProton == 1 && Correl.multH3 == 1 && Correl.mult12N == 1)
    {

      float const Q16F = 28.897;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask3H[0]=1;
      Correl.mask12N[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float Ex = Erel_16F + Q16F;
      Histo_read->Ex16F_pt12N->Fill(Ex);
    }
  else if (Correl.multProton == 2 && Correl.multH3 == 1 && Correl.mult11C == 1)
    {

      float const Q16F = 29.498;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask3H[0]=1;
      Correl.mask11C[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float Ex = Erel_16F + Q16F;
      Histo_read->Ex16F_2pt11C->Fill(Ex);
    }
  else if (Correl.multProton == 4 && Correl.multAlpha == 1 && Correl.mult8Li == 1)
    {

      float const Q16F = 41.846;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.maskProton[3]=1;
      Correl.maskAlpha[0]=1;
      Correl.mask8Li[0]=1;   
      Correl.makeArray(1);
      float Erel_16F = Correl.findErel();
      float Ex = Erel_16F + Q16F;
      Histo_read->Ex16F_4pa8Li->Fill(Ex);
    }




}

void det::corr_17F()
{
  if (Correl.multAlpha == 1 && Correl.mult13N == 1)
    {



      float const Q17F = 5.8186;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.mask13N[0]=1;   
      Correl.makeArray(1);
      float Erel_17F = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_17F + Q17F;
      Histo_read->Ex17F_a13N->Fill(Ex);
      Histo_read->vel_17F_a13N->Fill(Correl.velocityCM);
    }

  if (Correl.multProton == 1 && Correl.mult16O == 1)
    {

      phiL = Correl.O16[0]->phi;
      thetaL = Correl.O16[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);



      float const Q17F = 0.6002;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask16O[0]=1;   
      Correl.makeArray(1);
      float Erel_17F = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_17F + Q17F;
      Histo_read->Ex17F_p16O->Fill(Ex);
      Histo_read->vel_17F_p16O->Fill(Correl.velocityCM);

      float tot_energy = 0.;
      int imax = 0;
      for (int i=0;i<Ceasar->Nselect;i++)
	{
	  tot_energy += Ceasar->select[i].energy;
	  if(Ceasar->select[i].energy >= Ceasar->select[imax].energy) imax = i;
	}
      
      phig = Ceasar->select[imax].phi;
      thetag = Ceasar->select[imax].theta;
      xg = sin(thetag)*cos(phig);
      yg = sin(thetag)*sin(phig);
      zg = cos(thetag);
      dot = (xg*xL)+(yg*yL)+(zg*zL);
      mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
      thetarel = acos(dot/mag);
           
      
      float dopp = Doppler->correct(tot_energy,thetarel,Correl.O16[0]->velocity);
      
     
      if(Ex > 1.66 && Ex <2.07 && dopp >0.)
	Histo_read->TEC_17F_p16O->Fill(dopp);
      



    }
  else if (Correl.mult3He == 1 && Correl.mult14N == 1)
    {

      float const Q17F = 15.843;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask14N[0]=1;   
      Correl.makeArray(1);
      float Erel_17F = Correl.findErel();
      float Ex = Erel_17F + Q17F;
      Histo_read->Ex17F_3He14N->Fill(Ex);
    }
  else if (Correl.multH3 == 1 && Correl.mult14O == 1)
    {

      float const Q17F = 21.006;
      Correl.zeroMask();
      Correl.mask3H[0]=1;
      Correl.mask14O[0]=1;   
      Correl.makeArray(1);
      float Erel_17F = Correl.findErel();
      float Ex = Erel_17F + Q17F;
      Histo_read->Ex17F_t14O->Fill(Ex);
    }
  else if (Correl.multH2 == 1 && Correl.mult15O == 1)
    {

      float const Q17F = 14.040;
      Correl.zeroMask();
      Correl.mask2H[0]=1;
      Correl.mask15O[0]=1;   
      Correl.makeArray(1);
      float Erel_17F = Correl.findErel();
      float Ex = Erel_17F + Q17F;
      Histo_read->Ex17F_d15O->Fill(Ex);
    }
  else if (Correl.multProton == 2 && Correl.mult15N == 1)
    {

      float const Q17F = 12.728;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask15N[0]=1;   
      Correl.makeArray(1);
      float Erel_17F = Correl.findErel();
      float Ex = Erel_17F + Q17F;
      Histo_read->Ex17F_2p15N->Fill(Ex);
    }
  else if(Correl.multProton ==1 && Correl.multAlpha ==1 && Correl.mult12C ==1)
    {
      float const Q17F = 7.762;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskAlpha[0]=1;
      Correl.mask12C[0]=1;
      Correl.makeArray(1);
      float Erel_17F = Correl.findErel();
      float Ex = Erel_17F + Q17F;
      Histo_read->Ex17F_pa12C->Fill(Ex);

      float velocityCM = Correl.velocityCM;

      Histo_read->vel17F_pa12C->Fill(velocityCM);


      Correl.maskProton[0]=0;
      Correl.makeArray(1);
      float Ex16O = Correl.findErel()+7.1619;
      Histo_read->Ex17F_pa12C_vs_16O->Fill(Ex16O,Ex);

      Correl.maskProton[0]=1;
      Correl.maskAlpha[0]=0;
      Correl.makeArray(1);
      float Ex13N = Correl.findErel()+1.9434;
      Histo_read->Ex17F_pa12C_vs_13N->Fill(Ex13N,Ex);


      if(velocityCM > 9.0)
	{
	  Histo_read->Ex17F_pa12C_Fast->Fill(Ex);
	  Histo_read->Ex17F_pa12C_vs_16O_Fast->Fill(Ex16O,Ex);
	  Histo_read->Ex17F_pa12C_vs_13N_Fast->Fill(Ex13N,Ex);
	}
      else
	{
	  Histo_read->Ex17F_pa12C_Slow->Fill(Ex);
	  Histo_read->Ex17F_pa12C_vs_16O_Slow->Fill(Ex16O,Ex);
	  Histo_read->Ex17F_pa12C_vs_13N_Slow->Fill(Ex13N,Ex);
	}





    }


}

void det::corr_18F()
{
  if(Correl.mult3He == 1 && Correl.mult15N == 1)
    {
      float const Q18F = 14.160;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask15N[0]=1;   
      Correl.makeArray(1);
      float Erel_18F = Correl.findErel();
      float Ex = Erel_18F + Q18F;
      Histo_read->Ex18F_3He15N->Fill(Ex);
    }
  else  if(Correl.multH2 == 1 && Correl.mult16O == 1)
    {
      float const Q18F = 7.526;
      Correl.zeroMask();
      Correl.mask2H[0]=1;
      Correl.mask16O[0]=1;   
      Correl.makeArray(1);
      float Erel_18F = Correl.findErel();
      float Ex = Erel_18F + Q18F;
      Histo_read->Ex18F_d16O->Fill(Ex);
    }
  else  if(Correl.multAlpha == 1 && Correl.mult14N == 1)
    {
      float const Q18F = 4.415;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.mask14N[0]=1;   
      Correl.makeArray(1);
      float Erel_18F = Correl.findErel();
      float Ex = Erel_18F + Q18F;
      Histo_read->Ex18F_a14N->Fill(Ex);
    }
  else  if(Correl.multH3 == 1 && Correl.mult15O == 1)
    {
      float const Q18F = 16.932;
      Correl.zeroMask();
      Correl.mask3H[0]=1;
      Correl.mask15O[0]=1;   
      Correl.makeArray(1);
      float Erel_18F = Correl.findErel();
      float Ex = Erel_18F + Q18F;
      Histo_read->Ex18F_t15O->Fill(Ex);
    }
  else  if(Correl.mult6Li == 1 && Correl.mult12C == 1)
    {
      float const Q18F = 13.214;
      Correl.zeroMask();
      Correl.mask6Li[0]=1;
      Correl.mask12C[0]=1;   
      Correl.makeArray(1);
      float Erel_18F = Correl.findErel();
      float Ex = Erel_18F + Q18F;
      Histo_read->Ex18F_6Li12C->Fill(Ex);
    }

}

void det:: corr_15O()
{
  //1p+14N
  if (Correl.multProton == 1 && Correl.mult14N == 1)
    {

      N1p14N++;
     

      phiL = Correl.N14[0]->phi;
      thetaL = Correl.N14[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);
     
      float const Q15O = 7.297;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask14N[0]=1;   
      Correl.makeArray(1);
      float Erel_15O = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_15O + Q15O;

      float p =  0.;
      for (int i=0;i<3;i++) p += pow(Correl.proton[0]->MomCM[i],2);
      p = sqrt(p);
      float cosThetap = Correl.proton[0]->MomCM[2]/p;

      Histo_read->costheta_15O->Fill(cosThetap);
      
      Histo_read->Ex15O_1p14N->Fill(Ex);
      Histo_read->vel_15O_1p14N->Fill(Correl.velocityCM);


      if(Correl.N14[0]->itele ==6)
	{
	  if(Correl.N14[0]->icsi ==1 || Correl.N14[0]->icsi==2)
	    {
	      Histo_read->Ex15O_1p14N_inner->Fill(Ex);
	    }
	}
      else if(Correl.N14[0]->itele==7)
	{
	  if(Correl.N14[0]->icsi ==0 || Correl.N14[0]->icsi ==3)
	    {
	      Histo_read->Ex15O_1p14N_inner->Fill(Ex);
	    }
	}



      if(cosThetap < -0.5)
	{
	  Histo_read->Ex15O_Back->Fill(Ex);
	  Histo_read->Ex15O_Long->Fill(Ex);
	}
      else if(cosThetap >0.5)
	{
	  Histo_read->Ex15O_For->Fill(Ex);
	  Histo_read->Ex15O_Long->Fill(Ex);
	}
      else
	Histo_read->Ex15O_Trans->Fill(Ex);



      
      int imax = 0;
      
      for (int i=0;i<Ceasar->Nselect;i++)
	{
	  if(Ceasar->select[i].energy >= Ceasar->select[imax].energy) imax = i;
	}

      phig = Ceasar->select[imax].phi;
      thetag = Ceasar->select[imax].theta;
      xg = sin(thetag)*cos(phig);
      yg = sin(thetag)*sin(phig);
      zg = cos(thetag);
      dot = (xg*xL)+(yg*yL)+(zg*zL);
      mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
      thetarel = acos(dot/mag);
      
      
      float cenergy = Ceasar->select[imax].energy;
      float ctime = Ceasar->select[imax].itime;
      float dopp = Doppler->correct(cenergy,thetarel,Correl.N14[0]->velocity);
      if(dopp !=0)
	{
	  Histo_read->ETOF_Ceasar->Fill(ctime/10.,dopp);
	  Histo_read->TEC_Dop_15O->Fill(dopp);
	}

      if(Ex < 7.7)
	{
	  for(int i = 0;i<Ceasar->Nadded;i++)
	    {
	      Histo_read->TEC_15O_p14N->Fill(Ceasar->added[i].energy);
	    }
	}

      if(Correl.velocityCM > 9.5)
	{
	  if(Ex > 7.4 && Ex < 7.7)
	    {
	      Histo_read->VelCM_15O_7MeV->Fill(Correl.velocityCM);
	      Histo_read->ThetaCM_15O_7MeV->Fill(thetaCM*180/acos(-1));
	    }
	  else if(Ex > 8.15 && Ex < 8.45)
	    {
	      Histo_read->VelCM_15O_8MeV->Fill(Correl.velocityCM);
	      Histo_read->ThetaCM_15O_8MeV->Fill(thetaCM*180/acos(-1));
	    }
	  Histo_read->Ex15O_1p14N_17Ne->Fill(Ex);
	  if(dopp !=0) Histo_read->TEC_15O_p14N_17Ne->Fill(dopp);
	  if(dopp <2.42 && dopp > 2.1)
	    {
	      Histo_read->Ex15O_p14N_17Ne_IAS->Fill(Ex);
	    }
	  else if (dopp < 2.92 && dopp > 2.5)
	    {
	      Histo_read->Ex15O_p14N_17Ne_Back->Fill(Ex);
	    }
	  
	  Histo_read->Ex15O_vs_caesar->Fill(Ex,dopp);
	  
	  if(cosThetap < -0.5)
	    {
	      Histo_read->Ex15O_Back_17Ne->Fill(Ex);
	      Histo_read->Ex15O_Long_17Ne->Fill(Ex);
	    }
	  else if(cosThetap >0.5)
	    {
	      Histo_read->Ex15O_For_17Ne->Fill(Ex);
	      Histo_read->Ex15O_Long_17Ne->Fill(Ex);
	    }
	  else
	    Histo_read->Ex15O_Trans_17Ne->Fill(Ex);


	  
	}
      else if(Correl.velocityCM < 9.5)
	{
	  if(dopp < 2.42 && dopp > 2.1)
	    Histo_read->Ex15O_p14N_15O_gamma->Fill(Ex);
	}    
      
      
      
    }
  
  else if(Correl.mult13C ==1 && Correl.multProton ==2)
    {  

      float const Q15O = 14.847;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask13C[0]=1;   
      Correl.makeArray(1);
      float Erel_15O = Correl.findErel();
      float Ex = Erel_15O + Q15O;
      Histo_read->Ex15O_2p13C->Fill(Ex);


      //Addback for nearest neighbors
      phiL = Correl.C13[0]->phi;
      thetaL = Correl.C13[0]->theta;
      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);

      for(int i=0;i<Ceasar->Nadded;i++)
	{
	  phig = Ceasar->added[i].phi;
	  thetag = Ceasar->added[i].theta;
	  xg = sin(thetag)*cos(phig);
	  yg = sin(thetag)*sin(phig);
	  zg = cos(thetag);
	  dot = (xg*xL)+(yg*yL)+(zg*zL);
	  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	  thetarel = acos(dot/mag);

	  float dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.C13[0]->velocity);

	  if(dopp != 0)
	    Histo_read->TEC15O_2p13C->Fill(dopp);
	}

    }
  else if(Correl.multAlpha ==1 && Correl.mult11C ==1)
    {  

      float const Q15O = 10.220;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.mask11C[0]=1;   
      Correl.makeArray(1);
      float Erel_15O = Correl.findErel();
      float Ex = Erel_15O + Q15O;
      Histo_read->Ex15O_a11C->Fill(Ex);

      //Addback for nearest neighbors
      phiL = Correl.C11[0]->phi;
      thetaL = Correl.C11[0]->theta;
      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);

      for(int i=0;i<Ceasar->Nadded;i++)
	{
	  phig = Ceasar->added[i].phi;
	  thetag = Ceasar->added[i].theta;
	  xg = sin(thetag)*cos(phig);
	  yg = sin(thetag)*sin(phig);
	  zg = cos(thetag);
	  dot = (xg*xL)+(yg*yL)+(zg*zL);
	  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	  thetarel = acos(dot/mag);

	  float dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.C11[0]->velocity);

	  if(dopp != 0)
	    Histo_read->TEC15O_a11C->Fill(dopp);
	}

    }
  else if(Correl.mult12C ==1 && Correl.mult3He ==1)
    {  

      float const Q15O = 12.0756;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask12C[0]=1;   
      Correl.makeArray(1);
      float Erel_15O = Correl.findErel();
      float Ex = Erel_15O + Q15O;
      Histo_read->Ex15O_3He12C->Fill(Ex);

      //Addback for nearest neighbors
      phiL = Correl.C12[0]->phi;
      thetaL = Correl.C12[0]->theta;
      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);

      for(int i=0;i<Ceasar->Nadded;i++)
	{
	  phig = Ceasar->added[i].phi;
	  thetag = Ceasar->added[i].theta;
	  xg = sin(thetag)*cos(phig);
	  yg = sin(thetag)*sin(phig);
	  zg = cos(thetag);
	  dot = (xg*xL)+(yg*yL)+(zg*zL);
	  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	  thetarel = acos(dot/mag);

	  float dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.C12[0]->velocity);

	  if(dopp != 0)
	    Histo_read->TEC15O_3He12C->Fill(dopp);
	}
    }
  else if(Correl.mult12N ==1 && Correl.multH3 ==1)
    {  

      float const Q15O = 29.432;
      Correl.zeroMask();
      Correl.mask3H[0]=1;
      Correl.mask12N[0]=1;   
      Correl.makeArray(1);
      float Erel_15O = Correl.findErel();
      float Ex = Erel_15O + Q15O;
      Histo_read->Ex15O_t12N->Fill(Ex);
    }
  else if(Correl.mult13N ==1 && Correl.multH2 ==1)
    {  

      float const Q15O = 15.626;
      Correl.zeroMask();
      Correl.mask2H[0]=1;
      Correl.mask13N[0]=1;   
      Correl.makeArray(1);
      float Erel_15O = Correl.findErel();
      float Ex = Erel_15O + Q15O;
      Histo_read->Ex15O_d13N->Fill(Ex);
    }
  else if(Correl.mult6Li ==1 && Correl.multAlpha ==2 && Correl.multProton ==1)
    {  

      float const Q15O = 23.370;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.maskAlpha[1]=1;
      Correl.maskProton[0]=1;
      Correl.mask6Li[0]=1;   
      Correl.makeArray(1);
      float Erel_15O = Correl.findErel();
      float Ex = Erel_15O + Q15O;
      Histo_read->Ex15O_p2a6Li->Fill(Ex);
    }

}

void det::corr_16O()
{
  if(Correl.mult15N ==1 && Correl.multProton ==1)
    {  
      float const Q16O = 12.127;
      Correl.zeroMask();
      Correl.mask15N[0]=1;
      Correl.maskProton[0]=1;
      Correl.makeArray(1);
      float Erel_16O = Correl.findErel();
      float Ex = Erel_16O + Q16O;
      Histo_read->Ex16O_p15N->Fill(Ex);
    }
  else if(Correl.mult12C ==1 && Correl.multAlpha ==1)
    {  
      float const Q16O = 7.162;
      Correl.zeroMask();
      Correl.mask12C[0]=1;
      Correl.maskAlpha[0]=1;
      Correl.makeArray(1);
      float Erel_16O = Correl.findErel();
      float Ex = Erel_16O + Q16O;
      Histo_read->Ex16O_a12C->Fill(Ex);
    }
  else if(Correl.multAlpha ==4)
    {  
      float const Q16O = 14.437;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.maskAlpha[1]=1;
      Correl.maskAlpha[2]=1;
      Correl.maskAlpha[3]=1;
      Correl.makeArray(1);
      float Erel_16O = Correl.findErel();
      float Ex = Erel_16O + Q16O;
      Histo_read->Ex16O_4a->Fill(Ex);
    }
  else if(Correl.mult14N ==1 && Correl.multH2 ==1)
    {  
      float const Q16O = 20.736;
      Correl.zeroMask();
      Correl.mask2H[0]=1;
      Correl.mask14N[0]=1;
      Correl.makeArray(1);
      float Erel_16O = Correl.findErel();
      float Ex = Erel_16O + Q16O;
      Histo_read->Ex16O_d14N->Fill(Ex);
    }
  else if(Correl.mult13N ==1 && Correl.multH3 ==1)
    {  
      float const Q16O = 25.032;
      Correl.zeroMask();
      Correl.mask3H[0]=1;
      Correl.mask13N[0]=1;
      Correl.makeArray(1);
      float Erel_16O = Correl.findErel();
      float Ex = Erel_16O + Q16O;
      Histo_read->Ex16O_t13N->Fill(Ex);
    }
  else if(Correl.mult3He ==1 && Correl.mult13C ==1)
    {  
      float const Q16O = 22.793;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask13C[0]=1;
      Correl.makeArray(1);
      float Erel_16O = Correl.findErel();
      float Ex = Erel_16O + Q16O;
      Histo_read->Ex16O_3He13C->Fill(Ex);
    }
  else if(Correl.mult6Li ==1 && Correl.mult10B ==1)
    {  
      float const Q16O = 30.874;
      Correl.zeroMask();
      Correl.mask6Li[0]=1;
      Correl.mask10B[0]=1;
      Correl.makeArray(1);
      float Erel_16O = Correl.findErel();
      float Ex = Erel_16O + Q16O;
      Histo_read->Ex16O_6Li10B->Fill(Ex);
    }
  else if(Correl.multProton ==1 && Correl.multAlpha == 1 && Correl.mult11B ==1)
    {  
      float const Q16O = 23.119;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskAlpha[0]=1;
      Correl.mask11B[0]=1;
      Correl.makeArray(1);
      float Erel_16O = Correl.findErel();
      float Ex = Erel_16O + Q16O;
      Histo_read->Ex16O_pa11B->Fill(Ex);
    }
}

void det:: corr_13O()
{
  //1p+12N
  if (Correl.multProton == 1 && Correl.mult12N == 1)
    {


     

      phiL = Correl.N12[0]->phi;
      thetaL = Correl.N12[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);
 
      float const Q13O = 1.512;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask12N[0]=1;   
      Correl.makeArray(1);
      float Erel_13O = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_13O + Q13O;


      float p =  0.;
      for (int i=0;i<3;i++) p += pow(Correl.proton[0]->MomCM[i],2);
      p = sqrt(p);
      float cosThetap = Correl.proton[0]->MomCM[2]/p;

      if (cosThetap > -.5 && cosThetap < 0.5)
	{
	  Histo_read->Ex13O_1p12N->Fill(Ex);
	  Histo_read->vel_13O_1p12N->Fill(Correl.velocityCM);
	}
    }
  else if (Correl.mult13O == 1)
    {
      for (int i=0;i<Ceasar->Nselect;i++)
	{
	  float dopp = Ceasar->select[i].dop_energy;
	  Histo_read->TEC_Dop_13O->Fill(dopp);
	}
      
    }
  else if(Correl.mult11C ==1 && Correl.multProton ==2)
    {  
      float const Q13O = 2.113;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask11C[0]=1;   
      Correl.makeArray(1);
      float Erel_13O = Correl.findErel();
      float Ex = Erel_13O + Q13O;
      Histo_read->Ex13O_2p11C->Fill(Ex);
    }

}

void det:: corr_14O()
{
  //1p+13N
  if (Correl.multProton == 1 && Correl.mult13N == 1)
    {


     

      phiL = Correl.N13[0]->phi;
      thetaL = Correl.N13[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);
     
      float const Q14O = 4.627;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask13N[0]=1;   
      Correl.makeArray(1);
      float Erel_14O = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_14O + Q14O;

      Histo_read->Ex14O_1p13N->Fill(Ex);
      Histo_read->vel_14O_1p13N->Fill(Correl.velocityCM);

      if(Correl.velocityCM > 9.55)
	{
	  Histo_read->Ex14O_1p13N_17Ne->Fill(Ex);
	}
      else 
	Histo_read->Ex14O_1p13N_15O->Fill(Ex);

      float p =  0.;
      for (int i=0;i<3;i++) p += pow(Correl.proton[0]->MomCM[i],2);
      p = sqrt(p);
      float cosThetap = Correl.proton[0]->MomCM[2]/p;

      if(cosThetap < -0.5)
	{
	  Histo_read->Ex14O_Back->Fill(Ex);
	  Histo_read->Ex14O_Long->Fill(Ex);
	}
      else if(cosThetap >0.5)
	{
	  Histo_read->Ex14O_For->Fill(Ex);
	  Histo_read->Ex14O_Long->Fill(Ex);
	}
      else
	Histo_read->Ex14O_Trans->Fill(Ex);



    }
  else if (Correl.mult14O == 1)
    {
      for (int i=0;i<Ceasar->Nselect;i++)
	{
	  float dopp = Ceasar->select[i].dop_energy;
	  Histo_read->TEC_Dop_14O->Fill(dopp);
	}
      
    }
  else if(Correl.mult12C ==1 && Correl.multProton ==2)
    {  
      float const Q14O = 6.57;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask12C[0]=1;   
      Correl.makeArray(1);
      float Erel_14O = Correl.findErel();
      float Ex = Erel_14O + Q14O;
      Histo_read->Ex14O_2p12C->Fill(Ex);


      if(Ex > 9.3 && Ex < 10.4)
	{
	  phiL = Correl.C12[0]->phi;
	  thetaL = Correl.C12[0]->theta;
	  
	  xL = sin(thetaL)*cos(phiL);
	  yL = sin(thetaL)*sin(phiL);
	  zL = cos(thetaL);
	  
	  
	  
	  float dopp = 0.;
	  for(int i=0;i<Ceasar->Nadded;i++)
	    {
	      phig = Ceasar->added[i].phi;
	      thetag = Ceasar->added[i].theta;
	      xg = sin(thetag)*cos(phig);
	      yg = sin(thetag)*sin(phig);
	      zg = cos(thetag);
	      dot = (xg*xL)+(yg*yL)+(zg*zL);
	      mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	      thetarel = acos(dot/mag);
	      dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.O15[0]->velocity);
	      
	      if(dopp !=0)
		{
		  Histo_read->TEC_14O_2p12C->Fill(dopp);
		}
	    }
	  
	}
      
      
      
    }
  else if(Correl.mult11C ==1 && Correl.mult3He ==1)
    {  
      float const Q14O = 17.847;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask11C[0]=1;   
      Correl.makeArray(1);
      float Erel_14O = Correl.findErel();
      float Ex = Erel_14O + Q14O;
      Histo_read->Ex14O_3He11C->Fill(Ex);
    }


}


void det::corr_17Ne()
{

  if(Correl.multProton ==2 && Correl.multfake13O2 ==1)
    {


      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskfake13O2[0]=1;   
      Correl.makeArray(1);
      float Erel_fake15Ne = Correl.findErel();
      
      Histo_read->Erel15Ne_2pfake13O2->Fill(Erel_fake15Ne);

      Correl.maskProton[1] = 0;
      Correl.makeArray(1);
      float ExY1 = Correl.findErel()/Erel_fake15Ne;

      Correl.maskProton[1] = 1;
      Correl.maskProton[0] = 0;
      Correl.makeArray(1);
      float ExY2 = Correl.findErel()/Erel_fake15Ne;

      Correl.maskProton[0] = 1;
      Correl.maskfake13O2[0] = 0;

      Correl.makeArray(1);
      float ExT = Correl.findErel()/Erel_fake15Ne;

      Correl.maskfake13O2[0] = 1;
      Correl.makeArray(1);
      Correl.findErel();
      Correl.getJacobi();

      float theta_k_T = Correl.cosThetaT;
      float theta_k_Y1 = Correl.cosThetaY[0];
      float theta_k_Y2 = Correl.cosThetaY[1];


      if(Erel_fake15Ne >1.1 && Erel_fake15Ne < 1.8)
	{
	  Histo_read->JacobiY_15Ne_2pfake13O2->Fill(ExY1,theta_k_Y1);
	  Histo_read->JacobiY_15Ne_2pfake13O2->Fill(ExY2,theta_k_Y2);
	  Histo_read->JacobiT_15Ne_2pfake13O2->Fill(ExT,-theta_k_T);
	  Histo_read->JacobiT_15Ne_2pfake13O2->Fill(ExT,theta_k_T);

	}

      
    }


  float fake16NeEx = 0.;
  if(Correl.multProton ==2 && Correl.multfake14O ==1)
    {
      float const Q16Ne = -1.401;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskfake14O[0]=1;   
      Correl.makeArray(1);
      float Erel_fake16Ne = Correl.findErel();
      float Ex  = Erel_fake16Ne + Q16Ne;

      fake16NeEx = Ex;
      int oxytele = Correl.fakeO14[0]->itele;
      int oxycsi = Correl.fakeO14[0]->icsi;

      if(oxytele == 6 && oxycsi == 1)
	{
	  Histo_read->Ex16Ne_2pfake14O_25->Fill(Ex);
	}
      else if(oxytele == 6 && oxycsi == 2)
	{
	  Histo_read->Ex16Ne_2pfake14O_26->Fill(Ex);
	}
      else if(oxytele == 7 && oxycsi == 0)
	{
	  Histo_read->Ex16Ne_2pfake14O_28->Fill(Ex);
	}
      else if(oxytele == 7 && oxycsi == 3)
	{
	  Histo_read->Ex16Ne_2pfake14O_31->Fill(Ex);
	}
      //   else 
      //	{
      //	  cout << "hey" << endl;
      //	}

      //  cout << "Erel = " << Erel_fake16Ne << endl;

      Histo_read->ET16Ne_2pfake14O->Fill(Erel_fake16Ne);
      Histo_read->Ex16Ne_2pfake14O->Fill(Ex);


      float p0 = 0.;
      float p1 = 0.;
      for(int i=0;i<2;i++)
	{
	  p0 += pow(Correl.proton[0]->MomCM[i],2);
	  p1 += pow(Correl.proton[1]->MomCM[i],2);
	}
      p0 = sqrt(p0);
      p1 = sqrt(p1);


      float costheta0 = 0.;
      float costheta1 = 0.;

      costheta0 = Correl.proton[0]->MomCM[2]/p0;
      costheta1 = Correl.proton[1]->MomCM[2]/p1;

      if(costheta0 > 0.5 && costheta1 > 0.5)
	{	
	  Histo_read->Ex16Ne_2pfake14O_For->Fill(Ex);
	  Histo_read->Ex16Ne_2pfake14O_Long->Fill(Ex);
	}
      else if(costheta0 < -0.5 && costheta1 < -0.5)
	{
	  Histo_read->Ex16Ne_2pfake14O_Back->Fill(Ex);
	  Histo_read->Ex16Ne_2pfake14O_Long->Fill(Ex);

	}
      else if( costheta0 > -0.5 && costheta0 < 0.5)
	{
	  if(costheta1 > -0.5 && costheta1 < 0.5)
	    Histo_read->Ex16Ne_2pfake14O_Trans->Fill(Ex);
	}



      Correl.maskProton[1] = 0;
      Correl.makeArray(1);
      float ExY1 = Correl.findErel()/Erel_fake16Ne;

      Correl.maskProton[1] = 1;
      Correl.maskProton[0] = 0;
      Correl.makeArray(1);
      float ExY2 = Correl.findErel()/Erel_fake16Ne;

      Correl.maskProton[0] = 1;
      Correl.maskfake14O[0] = 0;

      Correl.makeArray(1);
      float ExT = Correl.findErel()/Erel_fake16Ne;

      Correl.maskfake14O[0] = 1;
      Correl.makeArray(1);
      Correl.findErel();
      Correl.getJacobi();

      Histo_read->ErelExT_16Ne2pfake14O->Fill(ExT,Erel_fake16Ne);
      Histo_read->ExExT_16Ne2pfake14O->Fill(ExT,Ex);

      float theta_k_T = Correl.cosThetaT;
      float theta_k_Y1 = Correl.cosThetaY[0];
      float theta_k_Y2 = Correl.cosThetaY[1];

      if(Ex > -0.13 && Ex < 0.32)
	{
	  Histo_read->Jacobi_Y_16Ne_back->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_16Ne_back->Fill(ExY2,theta_k_Y2);
	  Histo_read->Jacobi_T_16Ne_back->Fill(ExT,theta_k_T);
	  Histo_read->Jacobi_T_16Ne_back->Fill(ExT,-theta_k_T);
	}
      else if(Ex > 1.4 && Ex < 2.1)
	{
	  Histo_read->Jacobi_Y_16Ne_fake_1st->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_16Ne_fake_1st->Fill(ExY2,theta_k_Y2);
	  Histo_read->Jacobi_T_16Ne_fake_1st->Fill(ExT,theta_k_T);
	  Histo_read->Jacobi_T_16Ne_fake_1st->Fill(ExT,-theta_k_T);
	}
      else if(Ex > 0.7 && Ex < 1.15)
	{
	  Histo_read->Jacobi_T_16Ne_fake_1st_Back1->Fill(ExT,theta_k_T);
	  Histo_read->Jacobi_T_16Ne_fake_1st_Back1->Fill(ExT,-theta_k_T);
	  Histo_read->Jacobi_Y_16Ne_fake_1st_Back1->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_16Ne_fake_1st_Back1->Fill(ExY2,theta_k_Y2);
	}

      else if(Ex > 2.4 && Ex < 2.85)
	{
	  Histo_read->Jacobi_T_16Ne_fake_1st_Back2->Fill(ExT,theta_k_T);
	  Histo_read->Jacobi_T_16Ne_fake_1st_Back2->Fill(ExT,-theta_k_T);
	  Histo_read->Jacobi_Y_16Ne_fake_1st_Back2->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_16Ne_fake_1st_Back2->Fill(ExY2,theta_k_Y2);
	}



    }


  //2p+15O
  if (Correl.multProton == 2 && Correl.mult15O == 1)
    {
      N2p15O++;
     
      //  cout << "mult fake = " << Correl.multfake14O <<endl;
      int CsImulti = 3;

      int pro1tele = Correl.proton[0]->itele;
      int pro2tele =Correl.proton[1]->itele;
      int oxytele = Correl.O15[0]->itele;

      if(pro1tele == pro2tele) CsImulti--;
      if(pro1tele == oxytele) CsImulti--;
      else if (pro2tele == oxytele) CsImulti--;

      
     
      for(int i=0;i<Ceasar->Nxfp;i++)
	{
	  float txfp = Ceasar->Txfp[i];
	  Histo_read->Txfp_17Ne->Fill(txfp);
	}
      
      float const Q17Ne = .933;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask15O[0]=1;   
      Correl.makeArray(1);
      float Erel_17Ne = Correl.findErel();



      float thetaCM = Correl.thetaCM;
      float Ex = Erel_17Ne + Q17Ne;
      //  cout << "Ex 16Ne " << Ex << endl;
      Histo_read->Ex17Ne_2p15O->Fill(Ex);
      Histo_read->vel_17Ne_2p15O->Fill(Correl.velocityCM);

      Histo_read->Ex17Ne_vs_fake16Ne->Fill(Ex,fake16NeEx);

      Histo_read->Erel_thetaCM_Ne17->Fill(thetaCM*180./acos(-1),Erel_17Ne);


      if(Ex > 1.6 && Ex < 2.1)
	{

	  phiL = Correl.O15[0]->phi;
	  thetaL = Correl.O15[0]->theta;
	  
	  xL = sin(thetaL)*cos(phiL);
	  yL = sin(thetaL)*sin(phiL);
	  zL = cos(thetaL);
	  


	  float dopp = 0.;
	  for(int i=0;i<Ceasar->Nadded;i++)
	    {
	      phig = Ceasar->added[i].phi;
	      thetag = Ceasar->added[i].theta;
	      xg = sin(thetag)*cos(phig);
	      yg = sin(thetag)*sin(phig);
	      zg = cos(thetag);
	      dot = (xg*xL)+(yg*yL)+(zg*zL);
	      mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	      thetarel = acos(dot/mag);
	      dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.O15[0]->velocity);

	      if(dopp !=0)
		{
		  Histo_read->TEC_17Ne_2p15O->Fill(dopp);
		}
	    }

	}
      

	

      int oxycsi = Correl.O15[0]->icsi;

      if(oxytele == 6 && oxycsi == 1)
	{
	  Histo_read->Ex17Ne_2p15O_25->Fill(Ex);
	  Histo_read->Ex17Ne_vs_fake16Ne_25->Fill(Ex,fake16NeEx);

	}
      else if(oxytele == 6 && oxycsi == 2)
	{
	  //	  if(Ex < 2. && fake16NeEx > 0.4)
	  //	    {
	  //	      cout << "Bad Event" << endl;
	  //	    }
	  Histo_read->Ex17Ne_2p15O_26->Fill(Ex);
	  Histo_read->Ex17Ne_vs_fake16Ne_26->Fill(Ex,fake16NeEx);

	}
      else if(oxytele == 7 && oxycsi == 0)
	{
	  Histo_read->Ex17Ne_2p15O_28->Fill(Ex);
	  Histo_read->Ex17Ne_vs_fake16Ne_28->Fill(Ex,fake16NeEx);

	}
      else if(oxytele == 7 && oxycsi == 3)
	{
	  Histo_read->Ex17Ne_2p15O_31->Fill(Ex);
	  Histo_read->Ex17Ne_vs_fake16Ne_31->Fill(Ex,fake16NeEx);

	}

      float p0 = 0.;
      float p1 = 0.;
      for(int i=0;i<2;i++)
	{
	  p0 += pow(Correl.proton[0]->MomCM[i],2);
	  p1 += pow(Correl.proton[1]->MomCM[i],2);
	}
      p0 = sqrt(p0);
      p1 = sqrt(p1);


      float costheta0 = 0.;
      float costheta1 = 0.;

      costheta0 = Correl.proton[0]->MomCM[2]/p0;
      costheta1 = Correl.proton[1]->MomCM[2]/p1;

      if(costheta0 > 0.5 && costheta1 > 0.5)
	{
	  Histo_read->Ex17Ne_2p15O_For->Fill(Ex);
	  Histo_read->Ex17Ne_2p15O_Long->Fill(Ex);
	  if(Ex > 1.8 && Ex < 2.3)
	    {
	      for(int i =0;i<2;i++)
		{
		  float theta = Correl.proton[i]->theta;
		  float phi = Correl.proton[i]->phi;
		  
		  float x = 85.*sin(theta)*cos(phi);
		  float y = 85.*sin(theta)*sin(phi);
		  if(Ex < 1.80 || (Ex > 2.13 && Ex < 2.30))
		    {
		      Histo_read->protonxy_out->Fill(x,y);
		    }
		  
		  else if(Ex > 1.8 && Ex < 2.13)
		    {
		      Histo_read->protonxy_in->Fill(x,y);
		    }
		}
	    }
	}
      else if(costheta0 < -0.5 && costheta1 < -0.5)
	{
	  Histo_read->Ex17Ne_2p15O_Back->Fill(Ex);
	  Histo_read->Ex17Ne_2p15O_Long->Fill(Ex);
	}
      else if( costheta0 > -0.5 && costheta0 < 0.5)
	{
	  if(costheta1 > -0.5 && costheta1 < 0.5)
	    Histo_read->Ex17Ne_2p15O_Trans->Fill(Ex);
	}
      else if((costheta0 > 0.5 && costheta1 < -0.5) 
	      || (costheta0 < -0.5 && costheta1 > 0.5))
	{
	  Histo_read->Ex17Ne_2p15O_ForBack->Fill(Ex);
	}







      //target excitation energy
      double pt[3];
      pt[0] = -Correl.proton[0]->Mvect[0] - Correl.proton[1]->Mvect[0] 
	- Correl.O15[0]->Mvect[0];
      pt[1] = -Correl.proton[0]->Mvect[1] - Correl.proton[1]->Mvect[1]  
	- Correl.O15[0]->Mvect[1];
      pt[2] = 5677.71 -Correl.proton[0]->Mvect[2] - Correl.proton[1]->Mvect[2] 
	- Correl.O15[0]->Mvect[2];      //Mom in center of target less the z components


      double ppt = sqrt(pow(pt[0],2) + pow(pt[1],2) + pow(pt[2],2));
      double et = sqrt(pow(931.478*9.,2) + pow(ppt,2));
      double vvt = ppt/et*30.; //target velocity
      double vt[3];
      for (int i=0;i<3;i++) vt[i] = vvt/ppt*pt[i];
      double ek = et - 9.*931.478;
      // Energy/A*A - Ekin - Q
      
      double Ex_target = 57.947*17. - Correl.proton[0]->Ekin - 
        Correl.proton[1]->Ekin - Correl.O15[0]->Ekin - ek - Q17Ne; 
      //cout << "tar "<< Ex_target << endl;
      Histo_read->ExTarget_17Ne_2p15O->Fill(Ex_target);



      Correl.maskProton[1] = 0;
      Correl.makeArray(1);
      float ExY1 = Correl.findErel()/Erel_17Ne;

      Correl.maskProton[1] = 1;
      Correl.maskProton[0] = 0;
      Correl.makeArray(1);
      float ExY2 = Correl.findErel()/Erel_17Ne;

      Correl.maskProton[0] = 1;
      Correl.mask15O[0] = 0;

      Correl.makeArray(1);
      float ExT = Correl.findErel()/Erel_17Ne;

      Correl.mask15O[0] = 1;
      Correl.makeArray(1);
      Correl.findErel();
      Correl.getJacobi();


      Histo_read->ExNe17_2p15O_onep->Fill(ExY1*Erel_17Ne-0.535);
      Histo_read->ExNe17_2p15O_onep->Fill(ExY2*Erel_17Ne-0.535);


      float theta_k_T = Correl.cosThetaT;
      float theta_k_Y1 = Correl.cosThetaY[0];
      float theta_k_Y2 = Correl.cosThetaY[1];

      Histo_read->ExExT_17Ne2p15O->Fill(ExT,Ex);


      Histo_read->ExvET_17Ne2p15O->Fill(Erel_17Ne,ExY1*Erel_17Ne);
      Histo_read->ExvET_17Ne2p15O->Fill(Erel_17Ne,ExY2*Erel_17Ne);


      if(CsImulti ==3)
	{
	  Histo_read->Ex17Ne_2p15O_m3->Fill(Ex);
	}
      else if(CsImulti ==2) 
	{
	  Histo_read->Ex17Ne_2p15O_m2->Fill(Ex);
	}
      
      float cosphi0 = Correl.proton[0]->MomCM[2]/Correl.proton[0]->momentumCM;
      float cosphi1 = Correl.proton[1]->MomCM[2]/Correl.proton[1]->momentumCM;
      
      
      if(Ex > 1.53 && Ex < 2.02)
	
	{

	  Histo_read->Jacobi_x_T_17Ne_2nd->Fill(ExT);
	  Histo_read->Jacobi_x_T_17Ne_2nd->Fill(-ExT);
	  Histo_read->Jacobi_x_Y_17Ne_2nd->Fill(ExY1);
	  Histo_read->Jacobi_x_Y_17Ne_2nd->Fill(ExY2);
	  Histo_read->Jacobi_T_17Ne_2nd->Fill(ExT,theta_k_T);
	  Histo_read->Jacobi_T_17Ne_2nd->Fill(ExT,-theta_k_T);
	  Histo_read->Jacobi_Y_17Ne_2nd->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_17Ne_2nd->Fill(ExY2,theta_k_Y2);

	  Histo_read->Cospsi_17Ne2nd->Fill(cosphi0);
	  Histo_read->Cospsi_17Ne2nd->Fill(cosphi1);

	  Histo_read->ExTarget_17Ne_2p15O_2nd->Fill(Ex_target);

	  if(Ex_target < 30.) 
	    {
	      Histo_read->Cospsi_17Ne2nd_elas->Fill(cosphi0);
	      Histo_read->Cospsi_17Ne2nd_elas->Fill(cosphi1);
	      if(ExY1 < 0.5)
		{
		  Histo_read->Cospsi_17Ne2nd_elas_p1->Fill(cosphi0);
		  Histo_read->Cospsi_17Ne2nd_elas_p2->Fill(cosphi1);
		}
	      else
		{
		  Histo_read->Cospsi_17Ne2nd_elas_p1->Fill(cosphi1);
		  Histo_read->Cospsi_17Ne2nd_elas_p2->Fill(cosphi0);
		}

	    }
	  else
	    {
	      Histo_read->Cospsi_17Ne2nd_inelas->Fill(cosphi0);
	      Histo_read->Cospsi_17Ne2nd_inelas->Fill(cosphi1);
	      if(ExY1 < 0.5)
		{
		  Histo_read->Cospsi_17Ne2nd_inelas_p1->Fill(cosphi0);
		  Histo_read->Cospsi_17Ne2nd_inelas_p2->Fill(cosphi1);
		}
	      else
		{
		  Histo_read->Cospsi_17Ne2nd_inelas_p1->Fill(cosphi1);
		  Histo_read->Cospsi_17Ne2nd_inelas_p2->Fill(cosphi0);
		}

	    }

	  


	  Histo_read->VelCM_17Ne_2p15O_2nd->Fill(Correl.velocityCM);
	  Histo_read->ThetaCM_17Ne_2p15O_2nd->Fill(thetaCM*180/acos(-1));

	  float thetacmdeg = thetaCM*180/acos(-1);

	  Histo_read->Ne17_CsImult->Fill(CsImulti);

	  if(CsImulti ==3)
	    {
	      Histo_read->JacobiY_mult3->Fill(ExY1,theta_k_Y1);
	      Histo_read->JacobiY_mult3->Fill(ExY2,theta_k_Y2);

	    }
	  else if(CsImulti ==2) 
	    {
	      Histo_read->JacobiY_mult2->Fill(ExY1,theta_k_Y1);
	      Histo_read->JacobiY_mult2->Fill(ExY2,theta_k_Y2);

	    }


	  if(oxytele ==6 && Correl.O15[0]->icsi ==1)
	    {
	      for(int i =0;i<2;i++)
		{
		  float theta = Correl.proton[i]->theta;
		  float phi = Correl.proton[i]->phi;
		  
		  float x = 85.*sin(theta)*cos(phi);
		  float y = 85.*sin(theta)*sin(phi);

		  if(Ex > 1.88 && Ex < 2.1)
		    Histo_read->protonxy25->Fill(x,y);
		}
	    }
	  else if(oxytele ==6 && Correl.O15[0]->icsi ==2)
	    {
	      for(int i =0;i<2;i++)
		{
		  float theta = Correl.proton[i]->theta;
		  float phi = Correl.proton[i]->phi;

		  float x = 85.*sin(theta)*cos(phi);
		  float y = 85.*sin(theta)*sin(phi);

		  if(Ex > 1.88 && Ex < 2.1)
		    Histo_read->protonxy26->Fill(x,y);
		}
	    }
	  else if(oxytele ==7 && Correl.O15[0]->icsi ==0)
	    {
	      for(int i =0;i<2;i++)
		{
		  float theta = Correl.proton[i]->theta;
		  float phi = Correl.proton[i]->phi;

		  float x = 85.*sin(theta)*cos(phi);
		  float y = 85.*sin(theta)*sin(phi);

		  // if (oxytele == Correl.proton[i]->itele)
		  //   {
		  //     cout << "Proton " << i << " CsI = "<< Correl.proton[i]->icsi ;
		  //     cout << " Si = " << Correl.proton[i]->itele << endl;
		  //     cout << "Event = " << Eventt << endl;
		  //   }
		  

		  if(Ex > 1.88 && Ex < 2.1)
		    Histo_read->protonxy28->Fill(x,y);
		}
	    }
	  else if(oxytele == 7 && Correl.O15[0]->icsi ==3)
	    {
	      for(int i =0;i<2;i++)
		{
		  float theta = Correl.proton[i]->theta;
		  float phi = Correl.proton[i]->phi;

		  float x = 85.*sin(theta)*cos(phi);
		  float y = 85.*sin(theta)*sin(phi);

		  if(Ex > 1.88 && Ex < 2.1)
		    Histo_read->protonxy31->Fill(x,y);
		}
	    }
	  if(thetacmdeg > 3.0)
	    {
	      Histo_read->Jacobi_T_17Ne_2nd_right->Fill(ExT,theta_k_T);
	      Histo_read->Jacobi_T_17Ne_2nd_right->Fill(ExT,-theta_k_T);
	      Histo_read->Jacobi_Y_17Ne_2nd_right->Fill(ExY1,theta_k_Y1);
	      Histo_read->Jacobi_Y_17Ne_2nd_right->Fill(ExY2,theta_k_Y2);
	    }
	  else 
	    {
	      Histo_read->Jacobi_T_17Ne_2nd_left->Fill(ExT,theta_k_T);
	      Histo_read->Jacobi_T_17Ne_2nd_left->Fill(ExT,-theta_k_T);
	      Histo_read->Jacobi_Y_17Ne_2nd_left->Fill(ExY1,theta_k_Y1);
	      Histo_read->Jacobi_Y_17Ne_2nd_left->Fill(ExY2,theta_k_Y2);
	    }


	}


    }


  if(Correl.multProton ==3 && Correl.multfake13N ==1)
    {
      float const Q16Ne = 3.226;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.maskfake13N[0]=1;   
      Correl.makeArray(1);
      float Erel_fake16Ne = Correl.findErel();
      float Ex  = Erel_fake16Ne + Q16Ne;
      
      Histo_read->Ex16Ne_3pfake13N->Fill(Ex);
      Histo_read->Et16Ne_3pfake13N->Fill(Erel_fake16Ne);
      
    }
  

  //p+d+14O
  if(Correl.multProton == 1 && Correl.mult14O == 1 && Correl.multH2 ==1)
    {

      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask2H[1]=1;
      Correl.mask14O[0]=1;   
      Correl.makeArray(1);
      float Erel_17Ne = Correl.findErel();
      float Q17Ne = 11.93165;

      float Ex  = Erel_17Ne+Q17Ne;


      float thetaCM = Correl.thetaCM;
      Histo_read->Ex17Ne_pd14O->Fill(Ex);
      Histo_read->vel17Ne_pd14O->Fill(Correl.velocityCM);
    }
  else if(Correl.mult3He == 1 && Correl.mult14O == 1)
    {
      float const Q17Ne = 6.438;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask14O[0]=1;
      Correl.makeArray(1);
      float Erel_17Ne = Correl.findErel();
      float Ex = Erel_17Ne + Q17Ne;
      Histo_read->Ex17Ne_3He14O->Fill(Ex);
    }
  else if(Correl.multProton == 1 &&Correl.multAlpha == 1 && Correl.mult12N == 1)
    {
      float const Q17Ne = 10.551;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskAlpha[0]=1;
      Correl.mask12N[0]=1;
      Correl.makeArray(1);
      float Erel_17Ne = Correl.findErel();
      float Ex = Erel_17Ne + Q17Ne;
      Histo_read->Ex17Ne_pa12N->Fill(Ex);
    }
  else if(Correl.multProton == 3 && Correl.mult14N == 1)
    {
      float const Q17Ne = 8.230;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.mask14N[0]=1;
      Correl.makeArray(1);
      float Erel_17Ne = Correl.findErel();
      float Ex = Erel_17Ne + Q17Ne;
      Histo_read->Ex17Ne_3p14N->Fill(Ex);
    }
  else if(Correl.multProton == 2 && Correl.multAlpha == 1 && Correl.mult11C == 1)
    {
      float const Q17Ne = 11.153;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskAlpha[0]=1;
      Correl.mask11C[0]=1;
      Correl.makeArray(1);
      float Erel_17Ne = Correl.findErel();
      float Ex = Erel_17Ne + Q17Ne;
      Histo_read->Ex17Ne_2pa11C->Fill(Ex);
    }


}

void det::corr_15Ne()
{



  //2p+13O
  if (Correl.multProton == 2 && Correl.mult13O == 1)
    {

      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask13O[0]=1;   
      Correl.makeArray(1);
      float Erel_15Ne = Correl.findErel();


      float thetaCM = Correl.thetaCM;
      Histo_read->Erel_15Ne_2p13O->Fill(Erel_15Ne);
      Histo_read->vel_15Ne_2p13O->Fill(Correl.velocityCM);

      Correl.maskProton[1] = 0;
      Correl.makeArray(1);
      float ExY1 = Correl.findErel()/Erel_15Ne;

      Correl.maskProton[1] = 1;
      Correl.maskProton[0] = 0;
      Correl.makeArray(1);
      float ExY2 = Correl.findErel()/Erel_15Ne;

      Correl.maskProton[0] = 1;
      Correl.mask13O[0] = 0;

      Correl.makeArray(1);
      float ExT = Correl.findErel()/Erel_15Ne;

      Correl.mask13O[0] = 1;
      Correl.makeArray(1);
      Correl.findErel();
      Correl.getJacobi();

      float theta_k_T = Correl.cosThetaT;
      float theta_k_Y1 = Correl.cosThetaY[0];
      float theta_k_Y2 = Correl.cosThetaY[1];


      if(Erel_15Ne >2.25 && Erel_15Ne < 2.75)
	{
	  Histo_read->JacobiY_15Ne_2p13O->Fill(ExY1,theta_k_Y1);
	  Histo_read->JacobiY_15Ne_2p13O->Fill(ExY2,theta_k_Y2);
	  Histo_read->JacobiT_15Ne_2p13O->Fill(ExT,-theta_k_T);
	  Histo_read->JacobiT_15Ne_2p13O->Fill(ExT,theta_k_T);

	}

    }
  else if(Correl.multProton ==3 && Correl.mult12N ==1)
    {
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.mask12N[0]=1;   
      Correl.makeArray(1);
      float Erel_15Ne = Correl.findErel();
      Histo_read->Erel15Ne_3p12N->Fill(Erel_15Ne);
    }

}

void det::corr_6Li()
{
  
  //d + alpha
  if (Correl.multAlpha == 1 && Correl.multH2 == 1)
    {
      float const Q6Li = 1.4738;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.mask2H[0]=1;   
      Correl.makeArray(1);
      float Erel_6Li = Correl.findErel();
      float thetaCM = Correl.thetaCM;
      float Ex = Erel_6Li + Q6Li;
      Histo_read->Erel_6Li->Fill(Ex);
      Histo_read->vel_6Li->Fill(Correl.velocityCM);

      int alphatele = Correl.alpha[0]->itele;
      int deuttele = Correl.H2[0]->itele;


      float totm = 0;
      for(int i =0;i<3;i++) totm += pow(Correl.alpha[0]->MomCM[i],2);
      totm = sqrt(totm);
      float costheta = Correl.H2[0]->MomCM[2]/totm;


      Histo_read->Costheta->Fill(costheta);
      
      if(alphatele == deuttele)
	{
	  
	  if(costheta < -0.5)
	    Histo_read->Erel_Back_Same->Fill(Ex);
	  else if(costheta >0.5)
	    Histo_read->Erel_For_Same->Fill(Ex);
	  else
	    Histo_read->Erel_Trans_Same->Fill(Ex);
	  
	}
      else 
	{
	  if(costheta < -0.5)
	    Histo_read->Erel_Back_Diff->Fill(Ex);
	  else if(costheta >0.5)
	    Histo_read->Erel_For_Diff->Fill(Ex);
	  else
	    Histo_read->Erel_Trans_Diff->Fill(Ex);
	}
      
      
    }
}

void det::corr_18Na()
{



  //3p+15O
  if (Correl.multProton == 3 && Correl.mult15O == 1)
    {

      float Q18Na = -0.3144;

      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.mask15O[0]=1;   
      Correl.makeArray(1);
      float Erel_18Na = Correl.findErel();

      Histo_read->Ex18Na_3p15O->Fill(Erel_18Na+Q18Na);
      Histo_read->vel_18Na_3p15O->Fill(Correl.velocityCM);
    }
  else if (Correl.multH2 == 1 && Correl.multProton ==2 && Correl.mult14O == 1)
    {
      float Q18Na = 10.681;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask2H[0]=1;
      Correl.mask14O[0]=1;   
      Correl.makeArray(1);
      float Erel_18Na = Correl.findErel();
      float Ex = Erel_18Na + Q18Na;
      Histo_read->Ex18Na_2pd14O->Fill(Ex);
    }
  else if (Correl.mult3He == 1 && Correl.multProton ==1 && Correl.mult14O == 1)
    {
      float Q18Na = 5.188;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask3He[0]=1;
      Correl.mask14O[0]=1;   
      Correl.makeArray(1);
      float Erel_18Na = Correl.findErel();
      float Ex = Erel_18Na + Q18Na;
      Histo_read->Ex18Na_p3He14O->Fill(Ex);
    }
}

void det::corr_18Ne()
{
  //2p+16O
  if (Correl.multProton == 2 && Correl.mult16O == 1)
    {

      float Q18Ne = 4.5233;

      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask16O[0]=1;   
      Correl.makeArray(1);
      float Erel_18Ne = Correl.findErel();

      float Ex = Erel_18Ne+Q18Ne;
      Histo_read->Ex18Ne_2p16O->Fill(Ex);
      Histo_read->ET18Ne_2p16O->Fill(Erel_18Ne);
      Histo_read->velCM_18Ne_2p16O->Fill(Correl.velocityCM);

      Histo_read->thetaCM_18Ne_2p16O->Fill(Correl.thetaCM*180./acos(-1));

      Correl.maskProton[1] = 0;
      Correl.makeArray(1);
      float ExY1 = Correl.findErel()/Erel_18Ne;

      Correl.maskProton[1] = 1;
      Correl.maskProton[0] = 0;
      Correl.makeArray(1);
      float ExY2 = Correl.findErel()/Erel_18Ne;

      Correl.maskProton[0] = 1;
      Correl.mask16O[0] = 0;

      Correl.makeArray(1);
      float ExT = Correl.findErel()/Erel_18Ne;

      Correl.mask16O[0] = 1;
      Correl.makeArray(1);
      Correl.findErel();
      Correl.getJacobi();

      float theta_k_T = Correl.cosThetaT;
      float theta_k_Y1 = Correl.cosThetaY[0];
      float theta_k_Y2 = Correl.cosThetaY[1];

      if(Ex > 5.16 && Ex < 5.66)
	{

	  float xhit  =85.*sin(Correl.proton[0]->theta)*cos(Correl.proton[0]->phi);
	  float yhit  =85.*sin(Correl.proton[0]->theta)*sin(Correl.proton[0]->phi);
	  Histo_read->Ne18_protonXY->Fill(xhit,yhit);

	  xhit  =85.*sin(Correl.proton[1]->theta)*cos(Correl.proton[1]->phi);
	  yhit  =85.*sin(Correl.proton[1]->theta)*sin(Correl.proton[1]->phi);
	  Histo_read->Ne18_protonXY->Fill(xhit,yhit);

	  xhit  =85.*sin(Correl.O16[0]->theta)*cos(Correl.O16[0]->phi);
	  yhit  =85.*sin(Correl.O16[0]->theta)*sin(Correl.O16[0]->phi);
	  Histo_read->Ne18_coreXY->Fill(xhit,yhit);


	  Histo_read->Jacobi_Y_18Ne_2p16O->Fill(ExY1,theta_k_Y1);
	  Histo_read->Jacobi_Y_18Ne_2p16O->Fill(ExY2,theta_k_Y2);
	  Histo_read->Jacobi_T_18Ne_2p16O->Fill(ExT,-theta_k_T);
	  Histo_read->Jacobi_T_18Ne_2p16O->Fill(ExT,theta_k_T);

	  phiL = Correl.O16[0]->phi;
	  thetaL = Correl.O16[0]->theta;
	  xL = sin(thetaL)*cos(phiL);
	  yL = sin(thetaL)*sin(phiL);
	  zL = cos(thetaL);
	  
	  float dopp = 0.;
	  for(int i =0;i<Ceasar->Nadded;i++)
	    {
	      phig = Ceasar->added[i].phi;
	      thetag = Ceasar->added[i].theta;
	      xg = sin(thetag)*cos(phig);
	      yg = sin(thetag)*sin(phig);
	      zg = cos(thetag);
	      dot = (xg*xL)+(yg*yL)+(zg*zL);
	      mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	      thetarel = acos(dot/mag);
	      dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.O16[0]->velocity);
	      
	      if(dopp !=0)
		{
		  Histo_read->TEC_18Ne_2p16O->Fill(dopp);
		}		

	    }



	}

    }
  else if (Correl.multAlpha == 1 && Correl.mult14O == 1)
    {
      float Q18Ne = 5.115;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.mask14O[0]=1;   
      Correl.makeArray(1);
      float Erel_18Ne = Correl.findErel();
      float Ex = Erel_18Ne + Q18Ne;
      Histo_read->Ex18Ne_a14O->Fill(Ex);
    }
  else if (Correl.mult3He == 1 && Correl.mult15O == 1)
    {
      float Q18Ne = 12.469;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask15O[0]=1;   
      Correl.makeArray(1);
      float Erel_18Ne = Correl.findErel();
      float Ex = Erel_18Ne + Q18Ne;
      Histo_read->Ex18Ne_3He15O->Fill(Ex);
    }
  else if (Correl.multProton == 1 && Correl.multH2 == 1 && Correl.mult15O == 1)
    {
      float Q18Ne = 17.963;
      Correl.zeroMask();
      Correl.mask2H[0]=1;
      Correl.maskProton[0]=1;
      Correl.mask15O[0]=1;   
      Correl.makeArray(1);
      float Erel_18Ne = Correl.findErel();
      float Ex = Erel_18Ne + Q18Ne;
      Histo_read->Ex18Ne_pd15O->Fill(Ex);
    }
  else if (Correl.multProton == 1 && Correl.multH3 == 1 && Correl.mult14O == 1)
    {
      float Q18Ne = 24.929;
      Correl.zeroMask();
      Correl.mask3H[0]=1;
      Correl.maskProton[0]=1;
      Correl.mask14O[0]=1;   
      Correl.makeArray(1);
      float Erel_18Ne = Correl.findErel();
      float Ex = Erel_18Ne + Q18Ne;
      Histo_read->Ex18Ne_pt14O->Fill(Ex);
    }
  else if (Correl.mult6Li == 1 && Correl.mult12N == 1)
    {
      float Q18Ne = 26.107;
      Correl.zeroMask();
      Correl.mask6Li[0]=1;
      Correl.mask12N[0]=1;   
      Correl.makeArray(1);
      float Erel_18Ne = Correl.findErel();
      float Ex = Erel_18Ne + Q18Ne;
      Histo_read->Ex18Ne_6Li12N->Fill(Ex);
    }
  else if(Correl.multProton ==1 && Correl.mult17F ==1)
    {
      float Q18Ne =  3.923;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask17F[0]=1;
      Correl.makeArray(1);
      float Erel_18Ne = Correl.findErel();
      float Ex = Erel_18Ne + Q18Ne;

      float thetaCM18Ne = Correl.thetaCM;
      float velCM18Ne = Correl.velocityCM;

      Histo_read->Ex18Ne_p17F->Fill(Ex);
      Histo_read->ET18Ne_p17F->Fill(Erel_18Ne);

      Histo_read->thetaCM_18Ne_p17F->Fill(thetaCM18Ne*180./acos(-1));
      Histo_read->velCM_18Ne_p17F->Fill(velCM18Ne);

      phiL = Correl.F17[0]->phi;
      thetaL = Correl.F17[0]->theta;
      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);

      float dopp = 0.;
      for(int i =0;i<Ceasar->Nadded;i++)
	{
	  phig = Ceasar->added[i].phi;
	  thetag = Ceasar->added[i].theta;
	  xg = sin(thetag)*cos(phig);
	  yg = sin(thetag)*sin(phig);
	  zg = cos(thetag);
	  dot = (xg*xL)+(yg*yL)+(zg*zL);
	  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	  thetarel = acos(dot/mag);
	  dopp = Doppler->correct(Ceasar->added[i].energy,thetarel,Correl.F17[0]->velocity);
	  
	  if(dopp !=0)
	    {
	      Histo_read->TEC_18Ne_p17F->Fill(dopp);
	      Histo_read->Ex_vs_gamma_18Ne_p17F->Fill(dopp,Ex);
	    }		

	}

      float p =  0.;
      for (int i=0;i<3;i++) p += pow(Correl.proton[0]->MomCM[i],2);
      p = sqrt(p);
      float cosThetap = Correl.proton[0]->MomCM[2]/p;

      if(cosThetap < -0.5)
	{
	  Histo_read->Ex18Ne_p17F_Back->Fill(Ex);
	  Histo_read->Ex18Ne_p17F_Long->Fill(Ex);
	}
      else if(cosThetap >0.5)
	{
	  Histo_read->Ex18Ne_p17F_For->Fill(Ex);
	  Histo_read->Ex18Ne_p17F_Long->Fill(Ex);
	}
      else
	{
	  Histo_read->Ex18Ne_p17F_Trans->Fill(Ex);
	  Histo_read->ET18Ne_p17F_Trans->Fill(Erel_18Ne);
	}

    }
}

void det::corr_19Ne()
{
  if (Correl.multAlpha == 1 && Correl.mult15O == 1)
    {
      float Q19Ne = 3.528;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.mask15O[0]=1;   
      Correl.makeArray(1);
      float Erel_19Ne = Correl.findErel();
      float Ex = Erel_19Ne + Q19Ne;
      Histo_read->Ex19Ne_a15O->Fill(Ex);
    }
  else if (Correl.mult3He == 1 && Correl.mult16O == 1)
    {
      float Q19Ne = 8.442;
      Correl.zeroMask();
      Correl.mask3He[0]=1;
      Correl.mask16O[0]=1;   
      Correl.makeArray(1);
      float Erel_19Ne = Correl.findErel();
      float Ex = Erel_19Ne + Q19Ne;
      Histo_read->Ex19Ne_3He16O->Fill(Ex);
    }
  else if (Correl.multProton == 1 && Correl.multH3 ==1 && Correl.mult15O == 1)
    {
      float Q19Ne = 23.342;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.mask3H[0]=1;   
      Correl.mask15O[0]=1;
      Correl.makeArray(1);
      float Erel_19Ne = Correl.findErel();
      float Ex = Erel_19Ne + Q19Ne;
      Histo_read->Ex19Ne_pt15O->Fill(Ex);
    }
}

void det::corr_5Be()
{
  //p+p+3He
  if (Correl.multProton == 2 && Correl.mult3He == 1)
    {

      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.mask3He[0]=1;   
      Correl.makeArray(1);
      float Erel_5Be = Correl.findErel();

      Histo_read->ET5Be_2p3He->Fill(Erel_5Be);
      Histo_read->vel5Be_2p3He->Fill(Correl.velocityCM);


      Correl.maskProton[1] = 0;
      Correl.makeArray(1);
      float ExY1 = Correl.findErel()/Erel_5Be;

      Correl.maskProton[1] = 1;
      Correl.maskProton[0] = 0;
      Correl.makeArray(1);
      float ExY2 = Correl.findErel()/Erel_5Be;

      Correl.maskProton[0] = 1;
      Correl.mask3He[0] = 0;

      Correl.makeArray(1);
      float ExT = Correl.findErel()/Erel_5Be;
      float Epp = Correl.findErel();

      Correl.mask3He[0] = 1;
      Correl.makeArray(1);
      Correl.findErel();
      Correl.getJacobi();

      float theta_k_T = Correl.cosThetaT;
      float theta_k_Y1 = Correl.cosThetaY[0];
      float theta_k_Y2 = Correl.cosThetaY[1];

      Histo_read->Epp_ET_5Be->Fill(Erel_5Be,Epp);
      Histo_read->Costheta_ET_5Be->Fill(Erel_5Be,theta_k_T);


      if(Correl.mult12C ==1)
	{
	  Histo_read->ET5Be_2p3He_12C->Fill(Erel_5Be);
	    
	    
	  phiL = Correl.C12[0]->phi;
	  thetaL = Correl.C12[0]->theta;
	  
	  xL = sin(thetaL)*cos(phiL);
	  yL = sin(thetaL)*sin(phiL);
	  zL = cos(thetaL);
	  
	  int imax = 0;
	  float tot_energy =0.;
	  for(int i=0;i<Ceasar->Nselect;i++)
	    {
	      tot_energy += Ceasar->select[i].energy;
	      if(Ceasar->select[i].energy >= Ceasar->select[imax].energy) imax = i;
	    }
	  
	  phig = Ceasar->select[imax].phi;
	  thetag = Ceasar->select[imax].theta;
	  xg = sin(thetag)*cos(phig);
	  yg = sin(thetag)*sin(phig);
	  zg = cos(thetag);
	  dot = (xg*xL)+(yg*yL)+(zg*zL);
	  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	  thetarel = acos(dot/mag);
	  
	  float maxe = 0.;
	  maxe = Ceasar->select[imax].energy;
	  float dopp = Doppler->correct(tot_energy,thetarel,Correl.C12[0]->velocity);

	  if(dopp !=0)
	    Histo_read->TEC12C_5Be->Fill(dopp);

	  Histo_read->Jacobi_x_T_5Be_12C->Fill(ExT);
	  Histo_read->Jacobi_y_T_5Be_12C->Fill(theta_k_T);	  
	  Histo_read->Jacobi_y_T_5Be_12C->Fill(-theta_k_T);	  


	  Histo_read->Jacobi_x_Y_5Be_12C->Fill(ExY1);
	  Histo_read->Jacobi_x_Y_5Be_12C->Fill(ExY2);

	  Histo_read->Jacobi_y_Y_5Be_12C->Fill(theta_k_Y1);
	  Histo_read->Jacobi_y_Y_5Be_12C->Fill(theta_k_Y2);


	  Correl.zeroMask();
	  Correl.maskProton[0]=1;
	  Correl.maskProton[1]=1;
	  Correl.mask3He[0]=1;   
	  Correl.mask12C[0] =1;
	  Correl.makeArray(1);
	  float Erel_17Ne = Correl.findErel();

	  Histo_read->ET_17Ne_2p3He12C->Fill(Erel_17Ne);

	  Correl.zeroMask();
	  Correl.maskProton[0]=1;
	  Correl.maskProton[1]=1;
	  Correl.mask12C[0] =1;
	  Correl.makeArray(1);
	  float Erel_14O = Correl.findErel();

	  Histo_read->ET_14O_2p12C->Fill(Erel_14O);

	  Correl.zeroMask();
	  Correl.mask3He[0]=1;   
	  Correl.mask12C[0] =1;
	  Correl.makeArray(1);
	  float Erel_15O = Correl.findErel();

	  Histo_read->ET_15O_3He12C->Fill(Erel_15O);

	  Correl.zeroMask();
	  Correl.maskProton[0]=1;
	  Correl.mask12C[0] =1;
	  Correl.makeArray(1);
	  float Erel_13N = Correl.findErel();

	  Histo_read->ET_13N_p12C->Fill(Erel_13N);






	}
      else if(Correl.mult12C ==0)
	{
	  Histo_read->ET5Be_2p3He_no12C->Fill(Erel_5Be);


	  Histo_read->Jacobi_x_T_5Be_no12C->Fill(ExT);
	  Histo_read->Jacobi_y_T_5Be_no12C->Fill(theta_k_T);	  
	  Histo_read->Jacobi_y_T_5Be_no12C->Fill(-theta_k_T);	  


	  Histo_read->Jacobi_x_Y_5Be_no12C->Fill(ExY1);
	  Histo_read->Jacobi_x_Y_5Be_no12C->Fill(ExY2);

	  Histo_read->Jacobi_y_Y_5Be_no12C->Fill(theta_k_Y1);
	  Histo_read->Jacobi_y_Y_5Be_no12C->Fill(theta_k_Y2);

	}
      

      if(Erel_5Be > 5. && Erel_5Be < 10.)
	{
	  Histo_read->Jacobi_x_T_5Be->Fill(ExT);
	  Histo_read->Jacobi_y_T_5Be->Fill(theta_k_T);	  
	  Histo_read->Jacobi_y_T_5Be->Fill(-theta_k_T);	  


	  Histo_read->Jacobi_x_Y_5Be->Fill(ExY1);
	  Histo_read->Jacobi_x_Y_5Be->Fill(ExY2);

	  Histo_read->Jacobi_y_Y_5Be->Fill(theta_k_Y1);
	  Histo_read->Jacobi_y_Y_5Be->Fill(theta_k_Y2);
	}

      if(Correl.velocityCM > 10.1)
	{
	  Histo_read->ET5Be_2p3He_17Ne->Fill(Erel_5Be);

	  Histo_read->Jacobi_x_T_5Be_17Ne->Fill(ExT);
	  Histo_read->Jacobi_y_T_5Be_17Ne->Fill(theta_k_T);	  
	  Histo_read->Jacobi_y_T_5Be_17Ne->Fill(-theta_k_T);	  


	  Histo_read->Jacobi_x_Y_5Be_17Ne->Fill(ExY1);
	  Histo_read->Jacobi_x_Y_5Be_17Ne->Fill(ExY2);

	  Histo_read->Jacobi_y_Y_5Be_17Ne->Fill(theta_k_Y1);
	  Histo_read->Jacobi_y_Y_5Be_17Ne->Fill(theta_k_Y2);

	  Histo_read->Epp_ET_5Be_17Ne->Fill(Erel_5Be,Epp);
	  Histo_read->Costheta_ET_5Be_17Ne->Fill(Erel_5Be,theta_k_T);


	}
      if(Correl.velocityCM < 9.27)
	{
	  Histo_read->ET5Be_2p3He_15O->Fill(Erel_5Be);

	  Histo_read->Jacobi_x_T_5Be_15O->Fill(ExT);
	  Histo_read->Jacobi_y_T_5Be_15O->Fill(theta_k_T);	  
	  Histo_read->Jacobi_y_T_5Be_15O->Fill(-theta_k_T);	  


	  Histo_read->Jacobi_x_Y_5Be_15O->Fill(ExY1);
	  Histo_read->Jacobi_x_Y_5Be_15O->Fill(ExY2);

	  Histo_read->Jacobi_y_Y_5Be_15O->Fill(theta_k_Y1);
	  Histo_read->Jacobi_y_Y_5Be_15O->Fill(theta_k_Y2);

	  Histo_read->Epp_ET_5Be_15O->Fill(Erel_5Be,Epp);
	  Histo_read->Costheta_ET_5Be_15O->Fill(Erel_5Be,theta_k_T);



	}

      if(Correl.multAlpha ==3)
	{
	  Correl.zeroMask();
	  Correl.maskAlpha[0]=1;   
	  Correl.maskAlpha[1]=1;   
	  Correl.maskAlpha[2]=1;   
	  Correl.makeArray(1);
	  float Erel_triplealpha = Correl.findErel();

	  float Q12C = 7.2747;

	  float Ex = Erel_triplealpha + Q12C;
	  Histo_read->Ex12C_3A_5Be->Fill(Ex);

	  Histo_read->ET5Be_2p3He_3A->Fill(Erel_5Be);	  
	}



    }
}

void det::corr_9Be()
{
  if(Correl.multProton == 1 && Correl.mult8Li ==1)
    {
      float const Q9Be = 16.886;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.mask8Li[0] =1;
      Correl.makeArray(1);
      float Erel_9Be = Correl.findErel();
      float Ex = Erel_9Be + Q9Be;

      Histo_read->vel_9Be_p8Li->Fill(Correl.velocityCM);
      Histo_read->Ex9Be_p8Li->Fill(Ex);
    }
}

void det::corr_7Be()
{
  if(Correl.multProton == 1 && Correl.mult6Li ==1)
    {
      float const Q7Be = 5.607;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.mask6Li[0] =1;
      Correl.makeArray(1);
      float Erel_7Be = Correl.findErel();
      float Ex = Erel_7Be + Q7Be;
      Histo_read->vel_7Be_p6Li->Fill(Correl.velocityCM);

      Histo_read->Ex7Be_p6Li->Fill(Ex);


      phiL = Correl.Li6[0]->phi;
      thetaL = Correl.Li6[0]->theta;
      
      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);
      
      float tot_energy = 0.;
      int imax = 0;
      for (int i=0;i<Ceasar->Nselect;i++)
	{
	  tot_energy += Ceasar->select[i].energy;
	  if(Ceasar->select[i].energy >= Ceasar->select[imax].energy) imax = i;
	}
      
      phig = Ceasar->select[imax].phi;
      thetag = Ceasar->select[imax].theta;
      xg = sin(thetag)*cos(phig);
      yg = sin(thetag)*sin(phig);
      zg = cos(thetag);
      dot = (xg*xL)+(yg*yL)+(zg*zL);
      mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
      thetarel = acos(dot/mag);
      
      float dopp = Doppler->correct(tot_energy,thetarel,Correl.Li6[0]->velocity);
      
      if(Ex > 6.7 && Ex < 7.9)
	{
	  if(dopp !=0)
	    {
	      Histo_read->TEC_7Be_p6Li->Fill(dopp);
	    }
	}

      if(dopp !=0)
	Histo_read->Caesarmult->Fill(dopp,Ceasar->Nselect);

      
      if(dopp > 3.3 && dopp < 3.9)
	{
	  Histo_read->Ex7Be_p6Li_gamma->Fill(Ex);
	  if(Ex < 6.8 && Ex > 5.6)
	    Histo_read->vel_7Be_p6Li_left->Fill(Correl.velocityCM);
	  else if(Ex > 6.8 && Ex < 7.9)
	    Histo_read->vel_7Be_p6Li_right->Fill(Correl.velocityCM);
	}
      if(dopp > 4.0 && dopp < 6.0)
	{
	  Histo_read->Ex7Be_p6Li_gammaback1->Fill(Ex);
	}
      if(dopp > 6.0 && dopp < 8.0)
	{
	  Histo_read->Ex7Be_p6Li_gammaback2->Fill(Ex);
	}
      if(dopp > 8.0 && dopp < 10.0)
	{
	  Histo_read->Ex7Be_p6Li_gammaback3->Fill(Ex);
	}

 
    }
}

void det::corr_12N()
{
  if(Correl.multProton == 1 && Correl.mult11C == 1)
    {
      float const Q12N = 0.601;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.mask11C[0] =1;
      Correl.makeArray(1);
      float Erel_12N = Correl.findErel();
      float Ex = Erel_12N + Q12N;
      Histo_read->Ex12N_p11C->Fill(Ex);
    }
  else if(Correl.multProton ==2 && Correl.mult10B ==1)
    {
      float const Q12N = 9.290;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.maskProton[1]=1;      
      Correl.mask10B[0] =1;
      Correl.makeArray(1);
      float Erel_12N = Correl.findErel();
      float Ex = Erel_12N + Q12N;
      Histo_read->Ex12N_2p10B->Fill(Ex);
    }
  else if(Correl.multAlpha ==2 && Correl.mult3He ==1 && Correl.multProton ==1)
    {
      float const Q12N = 9.290;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.maskAlpha[0]=1;
      Correl.maskAlpha[1]=1;
      Correl.mask3He[0] =1;
      Correl.makeArray(1);
      float Erel_12N = Correl.findErel();
      float Ex = Erel_12N + Q12N;
      Histo_read->Ex12N_p2a3He->Fill(Ex);
    }
  else if(Correl.multAlpha ==1 && Correl.mult6Li ==1 && Correl.multProton ==2)
    {
      float const Q12N = 13.752;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.maskProton[1]=1;
      Correl.maskAlpha[0]=1;
      Correl.mask6Li[0] =1;
      Correl.makeArray(1);
      float Erel_12N = Correl.findErel();
      float Ex = Erel_12N + Q12N;
      Histo_read->Ex12N_2pa6Li->Fill(Ex);
    }
}

void det::corr_13N()
{
  if(Correl.multProton == 1 && Correl.mult12C == 1)
    {
      float const Q13N = 1.943;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.mask12C[0] =1;
      Correl.makeArray(1);
      float Erel_13N = Correl.findErel();
      float Ex = Erel_13N + Q13N;
      Histo_read->Ex13N_p12C->Fill(Ex);
    }
  else if(Correl.multProton == 2 && Correl.mult11B == 1)
    {
      float const Q13N = 17.900;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.maskProton[1]=1;      
      Correl.mask11B[0] =1;
      Correl.makeArray(1);
      float Erel_13N = Correl.findErel();
      float Ex = Erel_13N + Q13N;
      Histo_read->Ex13N_2p11B->Fill(Ex);
    }
  else if(Correl.multH2 == 1 && Correl.mult11C == 1)
    {
      float const Q13N = 18.44;
      Correl.zeroMask();
      Correl.mask2H[0]=1;      
      Correl.mask11C[0] =1;
      Correl.makeArray(1);
      float Erel_13N = Correl.findErel();
      float Ex = Erel_13N + Q13N;
      Histo_read->Ex13N_d11C->Fill(Ex);
    }
  else if(Correl.multProton == 1 && Correl.multAlpha == 3)
    {
      float const Q13N = 9.218;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.maskAlpha[0] =1;
      Correl.maskAlpha[1] =1;
      Correl.maskAlpha[2] =1;
      Correl.makeArray(1);
      float Erel_13N = Correl.findErel();
      float Ex = Erel_13N + Q13N;
      Histo_read->Ex13N_p3a->Fill(Ex);
    }
  else if(Correl.mult3He == 1 && Correl.mult10B == 1)
    {
      float const Q13N = 21.636;
      Correl.zeroMask();
      Correl.mask3He[0]=1;      
      Correl.mask10B[0] =1;
      Correl.makeArray(1);
      float Erel_13N = Correl.findErel();
      float Ex = Erel_13N + Q13N;
      Histo_read->Ex13N_3He10B->Fill(Ex);
    }
 

}
void det::corr_14N()
{
  if(Correl.multProton == 1 && Correl.mult13C == 1)
    {
      float const Q14N = 7.550;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.mask13C[0] =1;
      Correl.makeArray(1);
      float Erel_14N = Correl.findErel();
      float Ex = Erel_14N + Q14N;
      Histo_read->Ex14N_p13C->Fill(Ex);
      Histo_read->vel14N_p13C->Fill(Correl.velocityCM);
    }
  else if(Correl.multH2 == 1 && Correl.mult12C == 1)
    {
      float const Q14N = 10.272;
      Correl.zeroMask();
      Correl.mask2H[0]=1;      
      Correl.mask12C[0] =1;
      Correl.makeArray(1);
      float Erel_14N = Correl.findErel();
      float Ex = Erel_14N + Q14N;
      Histo_read->Ex14N_d12C->Fill(Ex);
    }
  else if(Correl.multH3 == 1 && Correl.mult11C == 1)
    {
      float const Q14N = 22.737;
      Correl.zeroMask();
      Correl.mask3H[0]=1;      
      Correl.mask11C[0] =1;
      Correl.makeArray(1);
      float Erel_14N = Correl.findErel();
      float Ex = Erel_14N + Q14N;
      Histo_read->Ex14N_t11C->Fill(Ex);
    }
  else if(Correl.mult3He == 1 && Correl.mult11B == 1)
    {
      float const Q14N = 20.736;
      Correl.zeroMask();
      Correl.mask3He[0]=1;      
      Correl.mask11B[0] =1;
      Correl.makeArray(1);
      float Erel_14N = Correl.findErel();
      float Ex = Erel_14N + Q14N;
      Histo_read->Ex14N_3He11B->Fill(Ex);
    }
  else if(Correl.multAlpha == 1 && Correl.mult10B == 1)
    {
      float const Q14N = 11.612;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;      
      Correl.mask10B[0] =1;
      Correl.makeArray(1);
      float Erel_14N = Correl.findErel();
      float Ex = Erel_14N + Q14N;
      Histo_read->Ex14N_a10B->Fill(Ex);
    }
  else if(Correl.multAlpha == 2 && Correl.mult6Li == 1)
    {
      float const Q14N = 16.073;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;      
      Correl.maskAlpha[1]=1;      
      Correl.mask6Li[0] =1;
      Correl.makeArray(1);
      float Erel_14N = Correl.findErel();
      float Ex = Erel_14N + Q14N;
      Histo_read->Ex14N_2a6Li->Fill(Ex);
    }

}

void det::corr_15N()
{
  if(Correl.multH2 == 1 && Correl.mult13C == 1)
    {
      float const Q15N = 16.159;
      Correl.zeroMask();
      Correl.mask2H[0]=1;      
      Correl.mask13C[0] =1;
      Correl.makeArray(1);
      float Erel_15N = Correl.findErel();
      float Ex = Erel_15N + Q15N;
      Histo_read->Ex15N_d13C->Fill(Ex);
    }
  else if(Correl.multH3 == 1 && Correl.mult12C == 1)
    {
      float const Q15N = 14.848;
      Correl.zeroMask();
      Correl.mask3H[0]=1;      
      Correl.mask12C[0] =1;
      Correl.makeArray(1);
      float Erel_15N = Correl.findErel();
      float Ex = Erel_15N + Q15N;
      Histo_read->Ex15N_t12C->Fill(Ex);
    }
  else if(Correl.multAlpha == 1 && Correl.mult11B == 1)
    {
      float const Q15N = 10.991;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;      
      Correl.mask11B[0] =1;
      Correl.makeArray(1);
      float Erel_15N = Correl.findErel();
      float Ex = Erel_15N + Q15N;
      Histo_read->Ex15N_a11B->Fill(Ex);
    }

}

void det::corr_11C()
{
  if(Correl.multProton == 1 && Correl.mult10B == 1)
    {
      float const Q11C = 8.689;
      Correl.zeroMask();
      Correl.maskProton[0]=1;      
      Correl.mask10B[0] =1;
      Correl.makeArray(1);
      float Erel_11C = Correl.findErel();
      float Ex = Erel_11C + Q11C;
      Histo_read->Ex11C_p10B->Fill(Ex);
      Histo_read->vel11C_p10B->Fill(Correl.velocityCM);

      phiL = Correl.B10[0]->phi;
      thetaL = Correl.B10[0]->theta;

      xL = sin(thetaL)*cos(phiL);
      yL = sin(thetaL)*sin(phiL);
      zL = cos(thetaL);
    
      int imax = 0;
      float tot_energy =0.;
      float noadd = 0.;
      for(int i=0;i<Ceasar->Nselect;i++)
	{
	  tot_energy += Ceasar->select[i].energy;
	  if(Ceasar->select[i].energy >= Ceasar->select[imax].energy) imax = i;
	}

      phig = Ceasar->select[imax].phi;
      thetag = Ceasar->select[imax].theta;
      xg = sin(thetag)*cos(phig);
      yg = sin(thetag)*sin(phig);
      zg = cos(thetag);
      dot = (xg*xL)+(yg*yL)+(zg*zL);
      mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
      thetarel = acos(dot/mag);            
      
      float maxe = 0.;
      maxe = Ceasar->select[imax].energy;
      float dopp = Doppler->correct(tot_energy,thetarel,Correl.B10[0]->velocity);
      
      //No addback included
      for(int i =0;i<Ceasar->Nselect;i++)
	{
	  phig = Ceasar->select[i].phi;
	  thetag = Ceasar->select[i].theta;
	  xg = sin(thetag)*cos(phig);
	  yg = sin(thetag)*sin(phig);
	  zg = cos(thetag);
	  dot = (xg*xL)+(yg*yL)+(zg*zL);
	  mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	  thetarel = acos(dot/mag);            
	  noadd = Doppler->correct(Ceasar->select[i].energy,thetarel,Correl.B10[0]->velocity);
	  Histo_read->TEC11C_p10B_noadd->Fill(dopp);
	}
      
      if(dopp != 0)
	{
	  Histo_read->TEC11C_p10B->Fill(dopp);
	}
      
      
      //Addback for nearest neighbors
      std::vector<float> sumgamma;
      std::vector<int> myvector;
      for(int i =0;i<Ceasar->Nselect;i++)
	{
	  
	  bool addedback = 0;
	  float sum = 0.;
	  sum = Ceasar->select[i].energy;
	  if(i!=0)
	    {

	      for(int v = 0;v<(int)myvector.size();v++)
		{
		  if(i == myvector.at(v))
		    addedback =1;
		}
	    }
	  if(addedback) 
	    continue;
	  else
	    {
	      for(int j = i+1;j<Ceasar->Nselect;j++)
		{
		  if(abs(Ceasar->select[i].iRing - Ceasar->select[j].iRing) <=1)
		    {
		      if(abs(Ceasar->select[i].phi - Ceasar->select[j].phi) < 0.5)
			{
			  sum += Ceasar->select[j].energy;
			}

		    }
		}
	      phig = Ceasar->select[i].phi;
	      thetag = Ceasar->select[i].theta;
	      xg = sin(thetag)*cos(phig);
	      yg = sin(thetag)*sin(phig);
	      zg = cos(thetag);
	      dot = (xg*xL)+(yg*yL)+(zg*zL);
	      mag = sqrt(pow(xg,2)+pow(yg,2)+pow(zg,2))*sqrt(pow(xL,2)+pow(yL,2)+pow(zL,2));
	      thetarel = acos(dot/mag);            
	      float doppsum = Doppler->correct(sum,thetarel,Correl.B10[0]->velocity);
	      if(doppsum !=0)
		{
		  sumgamma.push_back(doppsum);
		  Histo_read->CeasarT_p10B->Fill(Ceasar->select[i].time);
		}
	    }
	}
      for(int i =0;i<(int)sumgamma.size();i++)
	{
	  Histo_read->TEC11C_p10B_neighbors->Fill(sumgamma.at(i));
	  if(sumgamma.at(i) > 0.93 && sumgamma.at(i) < 1.08)
	    {
	      Histo_read->Ex11C_p10B_gamma->Fill(Ex);
	    }
	}
      //      cout << "number of gammas " << (int)sumgamma.size() << endl;
      //Creat the gamma gamma matrix
      for(int i = 0;i<(int)sumgamma.size()-1;i++)
	{
	  for(int j =i+1;j<(int)sumgamma.size();j++)
	    {
	      Histo_read->GammaGamma_11C_p10B->Fill(sumgamma.at(i),sumgamma.at(j));
	      Histo_read->GammaGamma_11C_p10B->Fill(sumgamma.at(j),sumgamma.at(i)); //symmetrizes
	      if(sumgamma.at(i) > 0.93 && sumgamma.at(i) < 1.08)
		{
		  if(sumgamma.at(j) > 0.58 && sumgamma.at(j) < 0.75)
		    {
		      Histo_read->Ex11C_p10B_gammagamma->Fill(Ex);
		    }
		}
	    }
	}




    }
  else if(Correl.multProton == 1 && Correl.multAlpha == 2 && Correl.multH2 ==1)
    {
      float const Q11C = 14.624;
      Correl.zeroMask();
      Correl.maskProton[0]=1; 
      Correl.maskAlpha[0]=1;
      Correl.maskAlpha[1]=1;     
      Correl.mask2H[0] =1;
      Correl.makeArray(1);
      float Erel_11C = Correl.findErel();
      float Ex = Erel_11C + Q11C;
      Histo_read->Ex11C_pd2a->Fill(Ex);
    }
  else if(Correl.multAlpha == 1 && Correl.mult7Be == 1)
    {
      float const Q11C = 7.544;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;      
      Correl.mask7Be[0]=1;
      Correl.makeArray(1);
      float Erel_11C = Correl.findErel();
      float Ex = Erel_11C + Q11C;
      Histo_read->Ex11C_a7Be->Fill(Ex);
    }
  else if(Correl.multProton==1 && Correl.multAlpha == 1 && Correl.mult6Li == 1)
    {
      float const Q11C = 13.150;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1; 
      Correl.maskProton[0]=1;     
      Correl.mask6Li[0] =1;
      Correl.makeArray(1);
      float Erel_11C = Correl.findErel();
      float Ex = Erel_11C + Q11C;
      Histo_read->Ex11C_pa6Li->Fill(Ex);
    }
  else if(Correl.multAlpha == 2 && Correl.mult3He == 1)
    {
      float const Q11C = 9.131;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1; 
      Correl.maskAlpha[1]=1;     
      Correl.mask3He[0] =1;
      Correl.makeArray(1);
      float Erel_11C = Correl.findErel();
      float Ex = Erel_11C + Q11C;
      Histo_read->Ex11C_3He2a->Fill(Ex);
    }
}

void det::corr_12C()
{
  if(Correl.multProton == 1 && Correl.mult11B == 1)
    {
      float const Q12C = 15.957;
      Correl.zeroMask();
      Correl.maskProton[0]=1; 
      Correl.mask11B[0] =1;
      Correl.makeArray(1);
      float Erel_12C = Correl.findErel();
      float Ex = Erel_12C + Q12C;
      Histo_read->Ex12C_p11B->Fill(Ex);
    }
  else if(Correl.multH2 == 1 && Correl.mult10B == 1)
    {
      float const Q12C = 25.186;
      Correl.zeroMask();
      Correl.mask2H[0]=1; 
      Correl.mask10B[0] =1;
      Correl.makeArray(1);
      float Erel_12C = Correl.findErel();
      float Ex = Erel_12C + Q12C;
      Histo_read->Ex12C_d10B->Fill(Ex);
    }
  else if(Correl.multH2 == 1 && Correl.multAlpha == 1 && Correl.mult6Li == 1)
    {
      float const Q12C = 29.648;
      Correl.zeroMask();
      Correl.mask2H[0]=1; 
      Correl.maskAlpha[0]=1;
      Correl.mask6Li[0]=1;
      Correl.makeArray(1);
      float Erel_12C = Correl.findErel();
      float Ex = Erel_12C + Q12C;
      Histo_read->Ex12C_da6Li->Fill(Ex);
    }
  else if(Correl.multH3 == 1 && Correl.multAlpha == 2 && Correl.multProton == 1)
    {
      float const Q12C = 27.089;
      Correl.zeroMask();
      Correl.mask3H[0]=1; 
      Correl.maskAlpha[0]=1;
      Correl.maskAlpha[1]=1;
      Correl.maskProton[0]=1;
      Correl.makeArray(1);
      float Erel_12C = Correl.findErel();
      float Ex = Erel_12C + Q12C;
      Histo_read->Ex12C_tp2a->Fill(Ex);
    }
  else if(Correl.multAlpha == 3)
    {
      float const Q12C = 7.275;
      Correl.zeroMask();
      Correl.maskAlpha[0]=1;
      Correl.maskAlpha[1]=1;
      Correl.maskAlpha[2]=1;
      Correl.makeArray(1);
      float Erel_12C = Correl.findErel();
      float Ex = Erel_12C + Q12C;
      Histo_read->Ex12C_3a->Fill(Ex);


      float aveE =0.;
      float ave2E = 0.;

      if(Ex > 7.3 && Ex < 8)
	{
	  for(int i=0;i<3;i++)
	    {	
	      aveE += Correl.alpha[i]->energyCM;
	      ave2E += pow(Correl.alpha[i]->energyCM,2.);    
	    }
	  aveE /=3.;
	  aveE = pow(aveE,2.);

	  ave2E /=3.;
	  float Erms = sqrt(ave2E-aveE);
	  Histo_read->Erms12C_3a->Fill(Erms);
	}
    }
  else if(Correl.mult6Li == 2)
    {
      float const Q12C = 28.174;
      Correl.zeroMask();
      Correl.mask6Li[0]=1;
      Correl.mask6Li[1]=1;
      Correl.makeArray(1);
      float Erel_12C = Correl.findErel();
      float Ex = Erel_12C + Q12C;
      Histo_read->Ex12C_6Li6Li->Fill(Ex);
    }
  else if(Correl.mult7Li == 1 && Correl.multProton==1 && Correl.multAlpha ==1)
    {
      float const Q12C = 24.621;
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskAlpha[0]=1;
      Correl.mask7Li[0]=1;
      Correl.makeArray(1);
      float Erel_12C = Correl.findErel();
      float Ex = Erel_12C + Q12C;
      Histo_read->Ex12C_pa7Li->Fill(Ex);
    }
}
void det::corr_13C()
{
  if(Correl.multH2 == 1 && Correl.mult11B == 1)
    {
      float const Q13C = 20.491;
      Correl.zeroMask();
      Correl.mask2H[0]=1;
      Correl.mask11B[0]=1;
      Correl.makeArray(1);
      float Erel_13C = Correl.findErel();
      float Ex = Erel_13C + Q13C;
      Histo_read->Ex13C_d11B->Fill(Ex);
    }
  else if(Correl.multH3 == 1 && Correl.mult10B == 1)
    {
      float const Q13C = 23.875;
      Correl.zeroMask();
      Correl.mask3H[0]=1;
      Correl.mask10B[0]=1;
      Correl.makeArray(1);
      float Erel_13C = Correl.findErel();
      float Ex = Erel_13C + Q13C;
      Histo_read->Ex13C_t10B->Fill(Ex);
    }

}

void det::corr_17Na()
{
  if(Correl.multProton ==3 && Correl.multfake14O ==1)
    {
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.maskfake14O[0]=1;
      Correl.makeArray(1);
      float Erel_17Na = Correl.findErel();
      Histo_read->Erel17Na_3pfake14O->Fill(Erel_17Na);
      Histo_read->vel_17Na->Fill(Correl.velocityCM);

      //target excitation energy
      double pt[3];
      pt[0] = -Correl.proton[0]->Mvect[0]  - Correl.proton[1]->Mvect[0]
              - Correl.proton[2]->Mvect[0] - Correl.O14[0]->Mvect[0];
      pt[1] = -Correl.proton[0]->Mvect[1] - Correl.proton[1]->Mvect[1]
              - Correl.proton[2]->Mvect[1] - Correl.O14[0]->Mvect[1];
      pt[2] = 5677.71 -Correl.proton[0]->Mvect[2]  
              - Correl.proton[1]->Mvect[2]      
              - Correl.proton[2]->Mvect[2] - Correl.O14[0]->Mvect[2];

      float dmass_target = 11.348; //mass excess of target
      float dmass_pro = 16.5005; //mass excess of projectile
      float dmass_pieces = 8.00746+3.*7.2889;  //mass excess of projectile pieces
      float dmass_star = 24.9549; // mass excess of excited target ground state
      float Astar = 9.;
      float Ekin_entrance= 17.*57.949; //entrance channel kinetic
      double ppt = sqrt(pow(pt[0],2) + pow(pt[1],2)+pow(pt[2],2));
      //total energy consevation, gives us totla energy of excited target
      double et = Ekin_entrance  - Correl.proton[0]->Ekin - 
Correl.proton[1]->Ekin - Correl.proton[2]->Ekin - Correl.O14[0]->Ekin  
+ dmass_pro + dmass_target - dmass_pieces + Astar*931.478 ;

      double Erest = sqrt(pow(et,2)-pow(ppt,2));  // rest mass of excited target
      double Ex_target = Erest - Astar*931.478 - dmass_star;


      Histo_read->ErelEx_17Na->Fill(Ex_target,Erel_17Na);

      
    }

  if(Correl.multProton == 3 && Correl.mult14O ==1)
    {
      Correl.zeroMask();
      Correl.maskProton[0]=1;
      Correl.maskProton[1]=1;
      Correl.maskProton[2]=1;
      Correl.mask14O[0]=1;
      Correl.makeArray(1);
      float Erel_17Na = Correl.findErel();
      Histo_read->Erel17Na_3p14O->Fill(Erel_17Na);


      Correl.maskProton[2]=0;
      Correl.makeArray(1);
      float Erel_16Ne = Correl.findErel();
      Histo_read->Erel17Na_Erel16Ne->Fill(Erel_16Ne,Erel_17Na);

      Correl.maskProton[1]=0;
      Correl.maskProton[2]=1;
      Correl.makeArray(1);
      Erel_16Ne = Correl.findErel();
      Histo_read->Erel17Na_Erel16Ne->Fill(Erel_16Ne,Erel_17Na);

      Correl.maskProton[0]=0;
      Correl.maskProton[1]=1;
      Correl.makeArray(1);
      Erel_16Ne = Correl.findErel();
      Histo_read->Erel17Na_Erel16Ne->Fill(Erel_16Ne,Erel_17Na);

    }
}

void det::treeGrow()
{
  int ngood = 0;
  int Ntele = 14;
  Forest->reset(); //clear the event here. reset function in forest, I think
  for (int itele = 0; itele < Ntele; itele++)
    {
      if(!(Hira->Telescope[itele]->Front.Nstore > 0
	   && Hira->Telescope[itele]->Back.Nstore > 0
	   && Hira->Telescope[itele]->Csi.Nstore > 0))
	{
	  continue;
	}
      ngood++;

      for (int i = 0; i < Hira->Telescope[itele]->Front.Nstore; i++)
	{
	  Forest->event->frontE[Forest->event->nfront] = Hira->Telescope[itele]->Front.Order[i].energy;
	  Forest->event->frontT[Forest->event->nfront] = Hira->Telescope[itele]->Front.Order[i].time;
	  Forest->event->frontID[Forest->event->nfront] = 32*itele + Hira->Telescope[itele]->Front.Order[i].strip;
	  Forest->event->nfront++;
	}
      for (int i = 0; i < Hira->Telescope[itele]->Back.Nstore; i++)
	{
	  Forest->event->backE[Forest->event->nback] = Hira->Telescope[itele]->Back.Order[i].energy;
	  Forest->event->backT[Forest->event->nback] = Hira->Telescope[itele]->Back.Order[i].time;
	  Forest->event->backID[Forest->event->nback] = 32*itele + Hira->Telescope[itele]->Back.Order[i].strip;
	  Forest->event->nback++;
	}
      for (int i = 0; i < Hira->Telescope[itele]->Csi.Nstore; i++)
	{
	  Forest->event->csiE[Forest->event->ncsi] = Hira->Telescope[itele]->Csi.Order[i].energy;
	  Forest->event->csiER[Forest->event->ncsi] = Hira->Telescope[itele]->Csi.Order[i].energyR;
	  Forest->event->csiT[Forest->event->ncsi] = Hira->Telescope[itele]->Csi.Order[i].time;
	  Forest->event->csiID[Forest->event->ncsi] = 4*itele + Hira->Telescope[itele]->Csi.Order[i].strip;
	  Forest->event->ncsi++;
	}
    }
  Forest->tree->Fill();
}

void det::loadTree(Event *event)
{
  int itele = 0;
  int strip = 0;
  Hira->reset();
 
  for (int j=0; j<event->nfront; j++)
    {
      itele = event->frontID[j]/32; //32 strips on the front
      strip = event->frontID[j]%32;
      Hira->Telescope[itele]->Front.Add(strip, event->frontE[j], 0, event->frontT[j]);
    }
  for (int j=0; j<event->nback; j++)
    {
      itele = event->backID[j]/32;
      strip = event->backID[j]%32;
      Hira->Telescope[itele]->Back.Add(strip, event->frontE[j], 0, event->frontT[j]);
    }
  for (int j=0; j<event->ncsi; j++)
    {
      itele = event->csiID[j]/4; //4 CsIs
      strip = event->csiID[j]%4;
      Hira->Telescope[itele]->Csi.Add(strip, event->csiE[j], event->csiER[j], event->csiT[j]);
    }
}
