#include "../include/telescope.h"

bool const telescope::relativity=1;

telescope::telescope(int id0, histo_read * Histo1)
{
    id = id0;
    Histo = Histo1;
    ran = new TRandom();

    ostringstream outstring;
    for (int icsi=0;icsi<4;icsi++)
    {
        int ii = icsi + 4*id;
        outstring.str("");
        outstring << "pid"<<ii;
        string name = outstring.str();
        Pid[icsi] = new pid(name); 
    }

    string name="Hydrogen.loss";
    Loss[0] = new CLoss(name,1.);
    Loss[1] = new CLoss(name,2.);
    Loss[2] = new CLoss(name,3.);
    name="Helium.loss";
    Loss[3] = new CLoss(name,3.);
    Loss[4] = new CLoss(name,4.);
    name = "Lithium.loss";
    Loss[6] = new CLoss(name,6.);
    Loss[7] = new CLoss(name,7.);
    Loss[8] = new CLoss(name,8.);
    name = "Beryllium.loss";
    Loss[9] = new CLoss(name,7.);
    name = "Boron.loss";
    Loss[10] = new CLoss(name,10.);
    Loss[11] = new CLoss(name,11.);
    name = "Carbon.loss";
    Loss[12] = new CLoss(name,11.);
    Loss[13] = new CLoss(name,12.);
    Loss[14] = new CLoss(name,13.);
    name = "Nitrogen.loss";
    Loss[15] = new CLoss(name,12.);
    Loss[16] = new CLoss(name,13.);
    Loss[17] = new CLoss(name,14.);
    Loss[18] = new CLoss(name,15.);
    name = "Oxygen.loss";
    Loss[19] = new CLoss(name,13.);
    Loss[20] = new CLoss(name,14.);
    Loss[21] = new CLoss(name,15.);
    Loss[22] = new CLoss(name,16.);
    name = "Fluorine.loss";
    Loss[23] = new CLoss(name,17.);


    /*name = "cal/Proton_New_ang.cal";
    calCsi = new calibrate(1,56,name,1);
    name = "cal/Deuteron_New_ang.cal";
    calCsid = new calibrate(1,56,name,1);
    name = "cal/Triton_New_ang.cal";
    calCsit = new calibrate(1,56,name,1);

    name = "cal/Helium3_New_ang.cal";
    calCsiHe3 = new calibrate(1,56,name,1);
    name = "cal/Alpha_New_ang.cal";
    calCsiA = new calibrate(1,56,name,1);


    name = "cal/Lithium6_New_ang.cal";
    calCsiLi6 = new calibrate(1,56,name,1);

    name = "cal/Beryllium7_New_ang.cal";
    calCsiBe7 =new calibrate(1,56,name,1);

    name = "cal/Boron10_New_ang.cal";
    calCsiB10 = new calibrate(1,56,name,1);
    name = "cal/Boron11_New_ang.cal";
    calCsiB11 = new calibrate(1,56,name,1);


    name = "cal/C11_New_ang.cal";
    calCsiC11 = new calibrate(1,56,name,1);
    name = "cal/C12_New_ang.cal";
    calCsiC12 = new calibrate(1,56,name,1);
    name = "cal/C13_New_ang.cal";
    calCsiC13 = new calibrate(1,56,name,1);

    name = "cal/Nitrogen12_New_ang.cal";
    calCsiN12 = new calibrate(1,56,name,1);
    name = "cal/Nitrogen13_New_ang.cal";
    calCsiN13 = new calibrate(1,56,name,1);
    name = "cal/Nitrogen_New_ang.cal"; //N14
    calCsiN14 = new calibrate(1,56,name,1);
    name = "cal/Nitrogen15_New_ang.cal";
    calCsiN15 = new calibrate(1,56,name,1);


    name = "cal/O13_New_ang.cal";
    calCsiO13 = new calibrate(1,56,name,1);
    name = "cal/O14_New_ang.cal";
    calCsiO14 = new calibrate(1,56,name,1);
    name = "cal/O15_New_ang.cal";
    calCsiO15 = new calibrate(1,56,name,1);
    name = "cal/O16_New_ang.cal";
    calCsiO16 = new calibrate(1,56,name,1);

    name = "cal/F17_New_ang.cal";
    calCsiF17 = new calibrate(1,56,name,1);

    name = "cal/fb.cal";
    calFB =new calibrate(1,14,name,2);
    */
}

//************************************************
telescope::~telescope()
{
    for(int ii =0;ii<25;ii++)
    {
        delete Loss[ii];
    }
}
//***********************************************
void telescope::analyze(int event)
{
    Nsolution = 0;
    Np = 0;
    N6 = 0;
    fenergy = 0;
    benergy = 0;

    Event = event;

    // if(Front.Nstore !=0 || Back.Nstore !=0)
    //   Histo->FBMult[id]->Fill(Front.Nstore,Back.Nstore);

    //if(Front.Nstore != 1) return;
    //if(Back.Nstore != 1) return;

    //  //get rid of xtalk between neighboring strips in Front
    //  if (Front.Nstore > 1)
    //    {
    //      if (abs(Front.Order[0].strip - Front.Order[1].strip) == 1)
    //	{
    //	  if (Front.Order[0].energy > 100. && Front.Order[1].energy > Front.Order[0].energy/20.) return; 
    //	}
    //  }

    // //Getting rid of the xtalk for front and back for large particles
    // if(id ==6 || id==7)
    //   {
    //     if(Front.Nstore >1 && Front.Order[0].energy >100.) //Is it a large pulse?
    // 	{
    // 	  for(int ff = 1;ff<Front.Nstore;ff++)
    // 	    {

    // 	      if(abs(Front.Order[0].strip - Front.Order[ff].strip) <=2) //Is it a neighbor?
    // 		{
    // 		  if(ff != Front.Nstore-1) //Is it the last in the list?
    // 		    {
    // 		      for(int i = ff+1;i<Front.Nstore;i++)
    // 			{
    // 			  Front.Order[i-1] =  Front.Order[i];  //Move everything up.
    // 			}
    // 		    }
    // 		  Front.Nstore--; //Decrease the number of entries
    // 		  ff--;
    // 		}
    // 	    }
    // 	}
    //     if(Back.Nstore >1 && Back.Order[0].energy >100.) //Is it a large pulse?
    // 	{
    // 	  for(int ff = 1;ff<Back.Nstore;ff++)
    // 	    {
    // 	      if(abs(Back.Order[0].strip - Back.Order[ff].strip) <=2) //Is it a neighbor?
    // 		{
    // 		  if(ff != Back.Nstore-1) //Is it the last in the list?
    // 		    {
    // 		      for(int i = ff+1;i<Back.Nstore;i++)
    // 			{
    // 			  Back.Order[i-1] =  Back.Order[i];  //Move everything up.
    // 			}
    // 		    }
    // 		  Back.Nstore--; //Decrease the number of entries
    // 		  ff--;

    // 		}
    // 	    }
    // 	}
    //     if(Front.Nstore > 1)
    // 	{
    // 	  for(int ii =0;ii<Front.Nstore;ii++)
    // 	    {
    // 	      if(Front.Order[ii].strip == 0 || Front.Order[ii].strip == 31)
    // 		{
    // 		  if(ii != Front.Nstore-1)
    // 		    {
    // 		      for(int jj = ii+1;jj<Front.Nstore;jj++)
    // 			{
    // 			  Front.Order[jj-1] = Front.Order[jj];
    // 			}
    // 		    }
    // 		  Front.Nstore--;
    // 		  ii--;
    // 		}
    // 	    }
    // 	}
    //     if(Back.Nstore > 1)
    // 	{
    // 	  for(int ii =0;ii<Back.Nstore;ii++)
    // 	    {
    // 	      if(Back.Order[ii].strip == 0 || Back.Order[ii].strip == 31)
    // 		{
    // 		  if(ii != Back.Nstore-1)
    // 		    {
    // 		      for(int jj = ii+1;jj<Back.Nstore;jj++)
    // 			{
    // 			  Back.Order[jj-1] = Back.Order[jj];
    // 			}
    // 		    }
    // 		  Back.Nstore--;
    // 		  ii--;
    // 		}
    // 	    }
    // 	}
    //   } //end if for id = 6,7

    //for(int i =0;i<Front.Nstore;i++)
    //  if(Front.Order[i].energylow !=0)
    //   Front.Order[i].energylow +=4;

    int mult = min(Front.Nstore,Back.Nstore);

    if(Front.Nstore ==0 || Back.Nstore ==0) return;
    fenergy = Front.Order[0].energy;
    benergy = Back.Order[0].energy;

    // cout << fenergy << " vs " << benergy << endl;

    for(int i =0;i<mult;i++)
    {
        float accept = 0.2;
        float fdum = Front.Order[i].energy;
        float bdum = Back.Order[i].energy;
        if(fdum <10.) accept =1./fdum;
        if(fabs(fdum-bdum) < fdum*accept)
        {
            //	  Histo->FBDiff[id]->Fill(fabs(fdum-bdum));
            //Histo->FB[id]->Fill(fdum,bdum);
        }
    }

    fhit = Front.Order[0].strip;
    bhit = Back.Order[0].strip;

    theta = Pixels.getAngle(id,fhit,bhit);
    phi = Pixels.phi;

    xhit = 85.*sin(theta)*cos(phi);
    yhit = 85.*sin(theta)*sin(phi);

    CsIhit = Csi.Order[0].strip + id*4;

    bool matched =0;
    if(Csi.Nstore ==1)
    {
        int icsi = Csi.Order[0].strip;
        int ifront = Front.Order[0].strip;
        int iback = Back.Order[0].strip;

        if (ifront >= FrontLow[icsi] &&
                ifront <= FrontHigh[icsi] &&
                iback  >= BackLow[icsi]  &&
                iback  <= BackHigh[icsi])
        {
            matched = 1;
        }	
        if(!matched)return;

        if(icsi == 0)
        {
            Histo->HitMap->Fill(xhit,yhit);
        }

        CsIenergy = Csi.Order[0].energyR;

        SuncalEnergy = fenergy;

        Histo->dEE[CsIhit]->Fill(CsIenergy,SuncalEnergy);

        float energy = Csi.Order[0].energy;

        bool stat = Pid[Csi.Order[0].strip]->getPID(CsIenergy,SuncalEnergy);
        if (!stat) return;

        int Z = Pid[Csi.Order[0].strip]->Z;
        int A = Pid[Csi.Order[0].strip]->A;

        /*
        if (Z > 0 && A >0)
        {

            if(Z ==1)
            { 
                if(A ==2)
                    energy = calCsid->getEnergy(0,CsIhit,energy);
                if(A ==3)
                    energy = calCsit->getEnergy(0,CsIhit,energy);
            }
            else if(Z == 2)
            {
                if(A ==3)
                    energy = calCsiHe3->getEnergy(0,CsIhit,energy);
                else if(A ==4)
                    energy = calCsiA->getEnergy(0,CsIhit,energy);
                else 
                {
                    cout << "found no calib for" << Z << " " << A << endl; 
                    abort();
                }
            }
            else if (Z == 3) 
            {
                energy = calCsiLi6->getEnergy(0,CsIhit,energy);
            }
            else if(Z ==4)
            {
                if(A ==7)
                    energy = calCsiBe7->getEnergy(0,CsIhit,energy);
                else 
                {
                    cout << "found no calib for" << Z << " " << A << endl; 
                    abort();
                }
            }
            else if(Z ==5)
            {
                if(A==10)
                    energy = calCsiB10->getEnergy(0,CsIhit,energy);
                else if(A==11)
                    energy = calCsiB11->getEnergy(0,CsIhit,energy);
                else 
                {
                    cout << "found no calib for" << Z << " " << A << endl; 
                    abort();
                }
            }
            else if(Z ==6)
            {
                if(A == 11)
                    energy = calCsiC11->getEnergy(0,CsIhit,energy);
                else if(A ==12)
                    energy = calCsiC12->getEnergy(0,CsIhit,energy);
                else if(A==13)
                    energy = calCsiC13->getEnergy(0,CsIhit,energy);
                else 
                {
                    cout << "found no calib for" << Z << " " << A << endl; 
                    abort();
                }
            }
            else if(Z==8)
            {
                if(A==13)
                    energy = calCsiO13->getEnergy(0,CsIhit,energy);
                else if(A==14)
                    energy = calCsiO14->getEnergy(0,CsIhit,energy);
                else if(A==15)
                    energy = calCsiO15->getEnergy(0,CsIhit,energy);
                else if(A==16)
                    energy = calCsiO16->getEnergy(0,CsIhit,energy);
                else 
                {
                    cout << "found no calib for" << Z << " " << A << endl; 
                    abort();
                }

            }
            else if(Z==7)
            {
                if(A==12)
                    energy = calCsiN12->getEnergy(0,CsIhit,energy);
                else if(A==13)
                    energy = calCsiN13->getEnergy(0,CsIhit,energy);
                else if(A==14)
                    energy = calCsiN14->getEnergy(0,CsIhit,energy);
                else if(A==15)
                    energy = calCsiN15->getEnergy(0,CsIhit,energy);
                else 
                {
                    cout << "found no calib for" << Z << " " << A << endl; 
                    abort();
                }
            }
            else if(Z==9)
                energy  =calCsiF17->getEnergy(0,CsIhit,energy);

            float sumEnergy = energy + SuncalEnergy;


            //energy loss in target
            int ipid =0;

            if(Z ==1)
                ipid = A-1;
            else if(Z==4)
                ipid = 9;
            else if (Z==6)
            {
                ipid = A+1;
            }
            else if(Z==7)
            {
                ipid =A+3;	      
            }
            else if(Z==8)
            {
                ipid = A+6;
            }
            else if(Z==9)
            {
                ipid = A+6;
            }
            else 
                ipid = A;


            //	  cout << "ipid" << ipid << " " << A <<  " " << Z << endl;

            float light = Csi.Order[0].energy;

            float thick = 193./2./cos(theta);
            float Ein = Loss[ipid]->getEin(sumEnergy,thick);
            float Ekin = Ein;


            if(Z==8 && A ==15)
            {
                //	      Histo->Etot->Fill(Ekin);
                //Histo->Light[CsIhit]->Fill(light);
            }


            Solution[Nsolution].energy = energy;
            Solution[Nsolution].energyR = Csi.Order[0].energyR;
            Solution[Nsolution].denergy = SuncalEnergy;
            Solution[Nsolution].ifront = fhit;
            Solution[Nsolution].iback = bhit;
            Solution[Nsolution].icsi = Csi.Order[0].strip;
            Solution[Nsolution].telescopeNumber = id;
            Solution[Nsolution].iZ = Z;
            Solution[Nsolution].iA = A;
            Solution[Nsolution].mass = A;
            Solution[Nsolution].Ekin = Ekin;
            Solution[Nsolution].theta = theta;
            Solution[Nsolution].phi = phi;
            Nsolution=1;
        }
    */
    }

    else if(Csi.Nstore>1 && Front.Nstore >=1 && Back.Nstore >=1)
    {
        multiHitCsi();
    }

    //  Addfake();
    //Addfake2();
    //Addfake3();
    //Addfake4();
    getMomentum(); //Adds energytotal and momentum to Solutions
}

//*******************************************************
void telescope::reset()
{

    multFront = 0;
    multBack = 0;

    for(int i =0;i<Nsolution;i++)
    {
        Solution[i].energy = 0;
        Solution[i].energyR = 0;
        Solution[i].denergy = 0;
        Solution[i].ifront =0;
        Solution[i].iback = 0;
        Solution[i].icsi = 0;
        Solution[i].telescopeNumber = 0;
        Solution[i].iZ = 0;
        Solution[i].iA = 0;
        Solution[i].mass = 0;
        Solution[i].Ekin = 0;
        Solution[i].theta = 0;
        Solution[i].phi = 0;
        Solution[i].energyTot = 0.;
        Solution[i].momentum = 0.;
        Solution[i].velocity = 0.;
        Solution[i].Mvect[0] = 0.;
        Solution[i].Mvect[1] = 0.;
        Solution[i].Mvect[2] = 0.;

    }

    Nsolution =0;

    Front.reset();
    Back.reset();
    Csi.reset();
}

//*************************************************************
int telescope::multiHitCsi()
{
    // find number of soultions ,i.e. back and front pairs of strips 
    // with the same energy 

    int isol = multiHit();
    if (isol <=0) return 0;

    //now assign each of these solutions a Csi detector location 
    int mult[4]={0};  //array for multipility of Si solution for each Csi
    int sil[4][10]; //contains a lits of solutions for each Csi
    for (int i=0;i<Nsolution;i++)
    {
        int ifront = Solution[i].ifront;
        int iback = Solution[i].iback;
        for (int icsi=0;icsi<4;icsi++)
        {
            if (ifront >= FrontLow[icsi] &&
                    ifront <= FrontHigh[icsi] &&
                    iback  >= BackLow[icsi]  &&
                    iback  <= BackHigh[icsi])
            {
                sil[icsi][mult[icsi]] = i;
                mult[icsi]++;
                Solution[i].icsi = icsi;
                break;
            }
        }
    }

    //make array of detect csi energies
    float energy[4]={-1.};
    float energyR[4]={-1};
    for (int i=0;i<Csi.mult;i++) 
    {
        energy[Csi.Order[i].strip] = Csi.Order[i].energy;
        energyR[Csi.Order[i].strip] = Csi.Order[i].energyR;
    }

    //loop over csi location
    for (int icsi = 0;icsi<4;icsi++)

    {
        // no solution for this location, ignore
        if (mult[icsi] == 0) continue;
        // more than one si solution for a single Csi location
        else if (mult[icsi] > 1)
        {
            for (int j=0;j<mult[icsi];j++) 
                Solution[sil[icsi][j]].ipid = 0;
            continue;
        }
        // only one si solution for this csi location
        else
        {
            int ii = sil[icsi][0];
            //now see if this csi fired 
            if (energy[icsi] <= 0.) 
            {
                //no csi recorded for this event
                //stopped in silicon
                Solution[ii].energy = 0.;
                Solution[ii].iZ = 0;
                continue;
            }

            CsIhit = icsi + id*4;


            // Histo->dEE[CsIhit]->Fill(energyR[icsi],Solution[ii].denergy);


            bool stat = Pid[icsi]->getPID(energyR[icsi],Solution[ii].denergy);
            if(!stat)
            {
                Solution[ii].energy =0.;
                Solution[ii].iZ =0;
                continue;
            }
            int Z = Pid[icsi]->Z;
            int A = Pid[icsi]->A;


            int ifront = Solution[ii].ifront;
            int iback = Solution[ii].iback;

            theta = Pixels.getAngle(id,ifront,iback);
            phi = Pixels.phi;

            /*
            if(Z >0 && A>0)
            {
                if(Z ==1)
                {
                    if(A ==2)
                        energy[icsi] = calCsid->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==3)
                        energy[icsi] = calCsit->getEnergy(0,CsIhit,energy[icsi]);
                }

                else if(Z == 2)
                {
                    if(A==3)
                        energy[icsi] = calCsiHe3->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A ==4)
                        energy[icsi] = calCsiA->getEnergy(0,CsIhit,energy[icsi]);
                    else 
                    {
                        cout << "found no calib for" << Z << " " << A << endl; 
                        abort();
                    }
                }
                else if (Z == 3) 
                {
                    energy[icsi] = calCsiLi6->getEnergy(0,CsIhit,energy[icsi]);
                }
                else if(Z ==4)
                {
                    if(A ==7)
                        energy[icsi] = calCsiBe7->getEnergy(0,CsIhit,energy[icsi]);
                    else 
                    {
                        cout << "found no calib for" << Z << " " << A << endl; 
                        abort();
                    }
                }
                else if(Z ==5)
                {
                    if(A==10)
                        energy[icsi] = calCsiB10->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==11)
                        energy[icsi] = calCsiB11->getEnergy(0,CsIhit,energy[icsi]);
                    else 
                    {
                        cout << "found no calib for" << Z << " " << A << endl; 
                        abort();
                    }
                }
                else if(Z==6)
                {
                    if(A==11)
                        energy[icsi] = calCsiC11->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==12)
                        energy[icsi] = calCsiC12->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==13)
                        energy[icsi] = calCsiC13->getEnergy(0,CsIhit,energy[icsi]);
                    else 
                    {
                        cout << "found no calib for" << Z << " " << A << endl; 
                        abort();
                    }
                }
                else if(Z==8)
                {
                    if(A==13)
                        energy[icsi] = calCsiO13->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==14)
                        energy[icsi] = calCsiO14->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==15)
                        energy[icsi] = calCsiO15->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==16)
                        energy[icsi] = calCsiO16->getEnergy(0,CsIhit,energy[icsi]);
                    else 
                    {
                        cout << "found no calib for" << Z << " " << A << endl; 
                        abort();
                    }
                }
                else if(Z==7)
                {
                    if(A==12)
                        energy[icsi] = calCsiN12->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==13)
                        energy[icsi] = calCsiN13->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==14)
                        energy[icsi] = calCsiN14->getEnergy(0,CsIhit,energy[icsi]);
                    else if(A==15)
                        energy[icsi] = calCsiN15->getEnergy(0,CsIhit,energy[icsi]);
                    else 
                    {
                        cout << "found no calib for" << Z << " " << A << endl; 
                        abort();
                    }
                }
                else if(Z==9)
                {
                    energy[icsi] = calCsiF17->getEnergy(0,CsIhit,energy[icsi]);
                }



                float sumenergy = energy[icsi] + Solution[ii].denergy;
                //energy loss in target
                int ipid =0;

                if(Z ==1)
                    ipid = A-1;
                else if(Z==4)
                    ipid = 9;
                else if (Z==6)
                {
                    ipid = A+1;
                }
                else if(Z==7)
                {
                    ipid =A+3;	      
                }
                else if(Z==8)
                {
                    ipid = A+6;
                }
                else if(Z==9)
                {
                    ipid = A+6;
                }
                else 
                    ipid = A;

                float thick = 193./2./cos(theta);
                float Ein = Loss[ipid]->getEin(sumenergy,thick);
                float Ekin = Ein;

                Solution[ii].energy = energy[icsi];
                Solution[ii].energyR = energyR[icsi];
                Solution[ii].icsi = Csi.Order[ii].strip;
                Solution[ii].iZ = Z;
                Solution[ii].iA = A;
                Solution[ii].mass = A;
                Solution[ii].Ekin = Ekin;
                Solution[ii].theta = theta;
                Solution[ii].phi = phi;
                Solution[ii].telescopeNumber = id;
            }
        */
        }
    }

    return 1;
}

//****************************************************
//recursive subroutine  used for multihit subroutine
void telescope::loop(int depth)
{
    if (depth == NestDim )
    {
        // do stuff here
        int dstrip = 0;
        float de = 0.;
        for (int i=0;i<NestDim;i++)
        {
            benergy = Back.Order[NestArray[i]].energy;
            fenergy = Front.Order[i].energy;
            de += abs(benergy-fenergy);
        }


        if (dstrip < dstripMin)
        {
            dstripMin = dstrip;
            for (int i=0;i<NestDim;i++) 
                arrayD[i] = NestArray[i];
        }


        if (de < deMin)
        {
            deMin = de;
            for (int i=0;i<NestDim;i++) 
                arrayB[i] = NestArray[i];
        }
        return;

    }

    for (int i=0;i<NestDim;i++)
    {
        NestArray[depth] = i;
        int leave = 0;
        for (int j=0;j<depth;j++)
        {
            if (NestArray[j] == i)
            {
                leave =1;
                break; 
            } 
        }
        if (leave) continue;
        loop(depth+1);
    }
}

//***************************************************
//extracts multiple particle from strip data 
int telescope::multiHit()
{
    int Ntries = min(Front.Nstore,Back.Nstore);
    if (Ntries > 4) Ntries =4;
    Nsolution = 0;
    if (Ntries <= 0) return 0;


    for (NestDim = Ntries;NestDim>0;NestDim--)
    {
        dstripMin = 1000;
        deMin = 10000.;

        //look for best solution
        loop(0);

        //check to see if best possible solution is reasonable
        int leave = 0;
        for (int i=0;i<NestDim;i++)
        {
            benergy = Back.Order[arrayB[i]].energy;
            fenergy = Front.Order[i].energy;
            float accept = 0.2;
            if(fenergy < 10.) accept =1.5/fenergy;

            if (fabs(benergy-fenergy) >fenergy*accept)
            {
                leave = 1;
                break;
            }
        }

        if (leave) continue;
        // now load solution
        for (int i=0;i<NestDim;i++)
        {
            fenergy = Front.Order[i].energy;
            Solution[i].denergy = fenergy;
            Solution[i].ifront = Front.Order[i].strip;
            Solution[i].iback = Back.Order[arrayB[i]].strip;
            Solution[i].telescopeNumber = id;
        }

        Nsolution = NestDim;

        break;
    }

    return Nsolution;
}

//***********************************************************
void telescope::load(int F0low, int F1low,int F2low, int F3low,
        int F0hi,  int F1hi, int F2hi,  int F3hi,
        int B0low, int B1low,int B2low, int B3low,
        int B0hi,  int B1hi, int B2hi,  int B3hi)
{
    FrontLow[0] = F0low;
    FrontLow[1] = F1low;
    FrontLow[2] = F2low;
    FrontLow[3] = F3low;

    FrontHigh[0] = F0hi;
    FrontHigh[1] = F1hi;
    FrontHigh[2] = F2hi;
    FrontHigh[3] = F3hi;

    BackLow[0] = B0low;
    BackLow[1] = B1low;
    BackLow[2] = B2low;
    BackLow[3] = B3low;

    BackHigh[0] = B0hi;
    BackHigh[1] = B1hi;
    BackHigh[2] = B2hi;
    BackHigh[3] = B3hi;
}

//***************************************************************
void telescope::Addfake()
{

    for(int i =0;i<Nsolution;i++)
    {
        if(Solution[i].iZ == 8 && Solution[i].iA ==15)
        {
            // cout << "solution = " << Nsolution << endl;
            Solution[Nsolution].ifront = Solution[i].ifront;
            Solution[Nsolution].iback = Solution[i].iback;
            Solution[Nsolution].icsi = Solution[i].icsi;
            Solution[Nsolution].telescopeNumber = Solution[i].telescopeNumber;
            Solution[Nsolution].theta = Solution[i].theta;
            Solution[Nsolution].phi = Solution[i].phi;
            Solution[Nsolution].denergy = Solution[i].denergy;

            float CsI0 = Solution[i].energyR;
            float fakeE = 0.;
            int counter = 0;
            float CsIE = 0.;
            float dE0 = Solution[i].denergy;
            float dE = 0.;
            for(;;)
            {
                // CsIE = CsI0 - ran->Rndm()*min((float)500,CsI0);
                CsIE = CsI0;

                dE = dE0 - ran->Rndm()*min((float)50,dE0);
                //dE = dE0;

                bool stat = Pid[Solution[i].icsi]->getPID(CsIE,dE);
                if(stat)
                {
                    int Z = Pid[Solution[i].icsi]->Z;
                    int A = Pid[Solution[i].icsi]->A;
                    if(Z == 8 && A ==14)
                    {
                        break;
                    }

                }
                counter++;
                if(counter ==100) break;
            }

            //if(counter > 30)cout << "counter " <<counter << endl;
            int CsiHit = Solution[i].icsi + id*4;

            //fakeE  =calCsi->getEnergy(0,CsiHit,CsIE);
            // fakeE = calCsiO14->getEnergy(0,CsiHit,fakeE);

            fakeE = Solution[i].energy;

            float sumEnergy = fakeE + dE;
            float thick = 193./2./cos(theta);

            float Ein = Loss[15]->getEin(sumEnergy,thick);
            float Ekin = Ein;

            Solution[Nsolution].Ekin =Ekin;
            Solution[Nsolution].energy = fakeE;
            Solution[Nsolution].denergy = dE;
            Solution[Nsolution].energyR = CsIE;
            Solution[Nsolution].iZ = 99;
            Solution[Nsolution].iA = 99;
            Solution[Nsolution].mass =14;
            if(counter !=100) Nsolution++;

        }

    }


}
//***************************************************************
void telescope::Addfake2()
{

    for(int i =0;i<Nsolution;i++)
    {
        if(Solution[i].iZ == 7 && Solution[i].iA ==14)
        {
            //cout << "solution = " << Nsolution << endl;
            Solution[Nsolution].ifront = Solution[i].ifront;
            Solution[Nsolution].iback = Solution[i].iback;
            Solution[Nsolution].icsi = Solution[i].icsi;
            Solution[Nsolution].telescopeNumber = Solution[i].telescopeNumber;
            Solution[Nsolution].theta = Solution[i].theta;
            Solution[Nsolution].phi = Solution[i].phi;
            Solution[Nsolution].denergy = Solution[i].denergy;

            float CsI0 = Solution[i].energyR;
            float fakeE = 0.;
            int counter = 0;
            float CsIE = 0.;
            float dE0 = Solution[i].denergy;
            float dE = 0.;
            for(;;)
            {
                // CsIE = CsI0 - ran->Rndm()*min((float)500,CsI0);
                CsIE = CsI0;

                dE = dE0 - ran->Rndm()*min((float)50,dE0);
                //dE = dE0;

                bool stat = Pid[Solution[i].icsi]->getPID(CsIE,dE);
                if(stat)
                {
                    int Z = Pid[Solution[i].icsi]->Z;
                    int A = Pid[Solution[i].icsi]->A;
                    if(Z == 7 && A ==13)
                    {
                        break;
                    }

                }
                counter++;
                if(counter ==100) break;
            }

            //if(counter > 30)cout << "counter " <<counter << endl;
            int CsiHit = Solution[i].icsi + id*4;

            //fakeE  =calCsi->getEnergy(0,CsiHit,CsIE);
            // fakeE = calCsiO14->getEnergy(0,CsiHit,fakeE);

            fakeE = Solution[i].energy;

            float sumEnergy = fakeE + dE;
            float thick = 193./2./cos(theta);

            float Ein = Loss[15]->getEin(sumEnergy,thick);
            float Ekin = Ein;

            Solution[Nsolution].Ekin =Ekin;
            Solution[Nsolution].energy = fakeE;
            Solution[Nsolution].denergy = dE;
            Solution[Nsolution].energyR = CsIE;
            Solution[Nsolution].iZ = 98;
            Solution[Nsolution].iA = 98;
            Solution[Nsolution].mass =13;
            if(counter !=100) Nsolution++;

        }

    }


}
//***************************************************************
void telescope::Addfake3()
{


    for(int i =0;i<Nsolution;i++)
    {
        if(Solution[i].iZ == 8 && Solution[i].iA ==14)
        {


            // cout << "solution = " << Nsolution << endl;
            Solution[Nsolution].ifront = Solution[i].ifront;
            Solution[Nsolution].iback = Solution[i].iback;
            Solution[Nsolution].icsi = Solution[i].icsi;
            Solution[Nsolution].telescopeNumber = Solution[i].telescopeNumber;
            Solution[Nsolution].theta = Solution[i].theta;
            Solution[Nsolution].phi = Solution[i].phi;
            Solution[Nsolution].denergy = Solution[i].denergy;

            float CsI0 = Solution[i].energyR;
            float fakeE = 0.;
            int counter = 0;
            float CsIE = 0.;
            float dE0 = Solution[i].denergy;
            float dE = 0.;
            for(;;)
            {
                // CsIE = CsI0 - ran->Rndm()*min((float)500,CsI0);
                CsIE = CsI0;

                dE = dE0 - ran->Rndm()*min((float)50,dE0);
                //dE = dE0;

                bool stat = Pid[Solution[i].icsi]->getPID(CsIE,dE);
                if(stat)
                {
                    int Z = Pid[Solution[i].icsi]->Z;
                    int A = Pid[Solution[i].icsi]->A;
                    if(Z == 8 && A ==13)
                    {
                        break;
                    }

                }
                counter++;
                if(counter ==100) break;
            }

            //if(counter > 30)cout << "counter " <<counter << endl;
            int CsiHit = Solution[i].icsi + id*4;

            //fakeE  =calCsi->getEnergy(0,CsiHit,CsIE);
            // fakeE = calCsiO14->getEnergy(0,CsiHit,fakeE);

            fakeE = Solution[i].energy;

            float sumEnergy = fakeE + dE;
            float thick = 193./2./cos(theta);

            float Ein = Loss[19]->getEin(sumEnergy,thick);
            float Ekin = Ein;



            Solution[Nsolution].Ekin =Ekin;
            Solution[Nsolution].energy = fakeE;
            Solution[Nsolution].denergy = dE;
            Solution[Nsolution].energyR = CsIE;
            Solution[Nsolution].iZ = 97;
            Solution[Nsolution].iA = 97;
            Solution[Nsolution].mass =13;
            if(counter !=100) Nsolution++;

        }

    }


}

//***************************************************************
void telescope::Addfake4()
{


    for(int i =0;i<Nsolution;i++)
    {
        if(Solution[i].iZ == 8 && Solution[i].iA ==15)
        {


            // cout << "solution = " << Nsolution << endl;
            Solution[Nsolution].ifront = Solution[i].ifront;
            Solution[Nsolution].iback = Solution[i].iback;
            Solution[Nsolution].icsi = Solution[i].icsi;
            Solution[Nsolution].telescopeNumber = Solution[i].telescopeNumber;
            Solution[Nsolution].theta = Solution[i].theta;
            Solution[Nsolution].phi = Solution[i].phi;
            Solution[Nsolution].denergy = Solution[i].denergy;

            float CsI0 = Solution[i].energyR;
            float fakeE = 0.;
            int counter = 0;
            float CsIE = 0.;
            float dE0 = Solution[i].denergy;
            float dE = 0.;
            for(;;)
            {
                // CsIE = CsI0 - ran->Rndm()*min((float)500,CsI0);
                CsIE = CsI0;

                dE = dE0 - ran->Rndm()*min((float)50,dE0);
                //dE = dE0;

                bool stat = Pid[Solution[i].icsi]->getPID(CsIE,dE);
                if(stat)
                {
                    int Z = Pid[Solution[i].icsi]->Z;
                    int A = Pid[Solution[i].icsi]->A;
                    if(Z == 8 && A ==13)
                    {
                        break;
                    }

                }
                counter++;
                if(counter ==100) break;
            }

            //if(counter > 30)cout << "counter " <<counter << endl;
            int CsiHit = Solution[i].icsi + id*4;

            //fakeE  =calCsi->getEnergy(0,CsiHit,CsIE);
            // fakeE = calCsiO14->getEnergy(0,CsiHit,fakeE);

            fakeE = Solution[i].energy;

            float sumEnergy = fakeE + dE;
            float thick = 193./2./cos(theta);

            float Ein = Loss[19]->getEin(sumEnergy,thick);
            float Ekin = Ein;



            Solution[Nsolution].Ekin =Ekin;
            Solution[Nsolution].energy = fakeE;
            Solution[Nsolution].denergy = dE;
            Solution[Nsolution].energyR = CsIE;
            Solution[Nsolution].iZ = 96;
            Solution[Nsolution].iA = 96;
            Solution[Nsolution].mass =13;
            if(counter !=100) Nsolution++;

        }

    }


}



//**********************************************************
void telescope::getMomentum()
{
    for(int i = 0;i<Nsolution;i++)
    {	

        float theta = Solution[i].theta;
        float phi = Solution[i].phi;
        float momentum;
        if (relativity)
        {
            Solution[i].mass *= 931.478;
            momentum = Solution[i].Kinematics.
                getMomentum(Solution[i].Ekin,Solution[i].mass);
            Solution[i].energyTot = Solution[i].Ekin + Solution[i].mass;

        }
        else
        {
            momentum = sqrt(2.*Solution[i].mass*Solution[i].Ekin);
            Solution[i].mass = 0.;
        }

        Solution[i].Mvect[0] = momentum*sin(theta)*cos(phi);
        Solution[i].Mvect[1] = momentum*sin(theta)*sin(phi);
        Solution[i].Mvect[2] = momentum*cos(theta); 
        Solution[i].momentum = sqrt(pow(Solution[i].Mvect[0],2)
                +pow(Solution[i].Mvect[1],2)
                +pow(Solution[i].Mvect[2],2));
        Solution[i].velocity = Solution[i].momentum/Solution[i].energyTot;
    }
}
