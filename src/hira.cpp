#include "../include/hira.h"
#include "../include/readData.h"

hira::hira(TRandom * ran0, histo_sort * Histo0)
{
    type = 0;
    Histo = Histo0;
    ran = ran0;
    init();
}

hira::hira(TRandom * ran0, histo_read * Histo1)
{
    type = 1;
    Histo_read = Histo1;
    ran = ran0;
    init();
}

void hira::init()
{
    xmarker[0]=0x1ff3;
    xmarker[1]=0x2ff3;

    TDC =  new TDC1190(1,64,128);

    //make map of chips
    ifstream ifile("chipmap.dat");
    if (!ifile.is_open())
    {
        cout << "chip map not found" << endl;
        abort();
    }

    string name;
    getline(ifile,name);
    int i1,iMB,cbf,cbb;///cbfl,///cbbl;
    for (;;)
    {
        ifile >> i1 >> iMB >> cbf>> cbb;
        if (ifile.eof()) break;
        if (ifile.bad()) break;

        iMB -= 1;
        Map[iMB][cbf].front = true;
        Map[iMB][cbf].telescopeNumber = i1;

        Map[iMB][cbb].front = false;
        Map[iMB][cbb].telescopeNumber = i1;



    }
    ifile.close();
    ifile.clear();

    ifstream csimap("csimap.dat");
    if(!csimap.is_open())
    {
        cout << "Could not find CsI mapping file" << endl;
        abort();
    }

    getline(csimap,name);

    int itel,ich,icsi;
    for(;;)
    {
        csimap >> itel >> ich >> icsi;
        if(csimap.eof()) break;
        if(csimap.bad()) break;
        //      cout << itel << " " << ich << " " << icsi <<endl;
        CsIMap[icsi].telescopeNumber = itel;
        CsIMap[icsi].CsINumber = ich;
    }
    csimap.close();
    csimap.clear();

    //read in calibrations
    ///int Ntele = NUMBER_OF_TELESCOPES;
    ///int Nstrip = NUMBER_OF_STRIPS;
    /*name = "cal/front.cal";
      calFront = new calibrate(Ntele,Nstrip,name,1);
      name = "cal/back.cal";
      calBack = new calibrate(Ntele,Nstrip,name,1);

      name = "cal/HL_Front.cal";
      calLFront = new calibrate(Ntele,Nstrip,name,1);
      name = "cal/HL_Back.cal";
      calLBack = new calibrate(Ntele,Nstrip,name,1);


      name = "cal/Proton_New_ang.cal";
      calCsi = new calibrate(1,TOTAL_CSIS,name,1);

    //TIME Calibrations

    name = "cal/timePulser_front.cal";
    calFrontT = new calibrate(Ntele,Nstrip,name,1);
    name = "cal/timePulser_frontLG.cal";
    calFrontTLG = new calibrate(Ntele,Nstrip,name,1);

    name = "cal/timePulser_back.cal";
    calBackT = new calibrate(Ntele,Nstrip,name,1);
    name = "cal/timePulser_backLG.cal";
    calBackTLG = new calibrate(Ntele,Nstrip,name,1);
    */

    Telescope = new telescope*[NUMBER_OF_TELESCOPES];
    for (int i=0;i<NUMBER_OF_TELESCOPES;i++)
    {
        Telescope[i] = new telescope(ran,i,Histo_read);

        Telescope[i]->load(0, 0, 15,15,
                16,16,31,31,
                0, 15,15,0,
                16,31,31,16);

        /*
           Telescope[i]->load(15, 15,0,0,
           31,31,16,16,
           0, 15,15,0,
           16,31,31,16);
           */
    }

    //high low correlations zero arrays
    for (int i=0;i<NUMBER_OF_TELESCOPES;i++)
        for (int j=0;j<NUMBER_OF_STRIPS;j++)
        {
            fsumN[i][j] = 0.;
            fsumx[i][j] = 0.;
            fsumxx[i][j] = 0.;
            fsumyx[i][j] = 0.;
            fsumy[i][j] = 0.;

            bsumN[i][j] = 0.;
            bsumx[i][j] = 0.;
            bsumxx[i][j] = 0.;
            bsumyx[i][j] = 0.;
            bsumy[i][j] = 0.;
        }
}

//*************************************************************
bool hira::unpack(ifstream& evtfile)
{
    return unpackSi_HINP4(evtfile) && unpackCsi(evtfile);
}

//*************************************************************
bool hira::unpackCsi(ifstream& evtfile)
{
    NE = 0;
    CsIM = 0;

    BlockerESum = 0;
    haveBlocker = false;

    unsigned long* point;

    for (int iadc = 0;iadc<2;iadc++)
    {
        //check for ffff's
        unsigned long f = *point;
        if (f==0xffffffff)
        {
            point += 2;
            continue;
        } 
        ///point = ADC.read(point);  // suck out the info in the qdc
        for (int i=0;i<(int)ADC.channelsHit.value;i++)
        {
            if (ADC.allChannelsData[i].underflow.value)
            {
                continue;
            }

            if (ADC.allChannelsData[i].overflow.value)
            {
                continue;
            }
            int id = ADC.allChannelsData[i].channelID.value + 32*iadc;
            int uncalEnergy = ADC.allChannelsData[i].ADCValue.value;

            if(id < TOTAL_CSIS)
            {
                int CsINumber = CsIMap[id].CsINumber;
                int telescopeNumber = CsIMap[id].telescopeNumber;

                float energy = 0; //= calCsi->getEnergy(0,CsINumber,uncalEnergy+ran->Rndm());

                DataE[NE].id = CsINumber;
                DataE[NE].uncalEnergy = uncalEnergy;
                DataE[NE].energy = energy;
                Histo->ECsI[CsINumber]->Fill(uncalEnergy);
                Histo->ECsISum->Fill(CsINumber,uncalEnergy);
                Histo->ECsICSum->Fill(CsINumber,energy);


                int icsi = DataE[NE].id%4; 
                if (uncalEnergy > 50)
                {
                    CsIM++;
                    Telescope[telescopeNumber]->Csi.Add(icsi,energy,0.,DataE[NE].uncalEnergy,0.);
                }

                //cout << Telescope[telescopeNumber]->Csi.Order[0].strip;
                //cout << " " << Telescope[telescopeNumber]->Csi.Order[0].energy << endl;

                NE++;
            }
            if (id >= TOTAL_CSIS && id<= 59)
            {
                haveBlocker = true;
                if(id == TOTAL_CSIS)
                    Histo->Blocker_E1->Fill(uncalEnergy);
                else if (id == 57)
                    Histo->Blocker_E2->Fill(uncalEnergy);
                else if (id == 58)
                    Histo->Blocker_E3->Fill(uncalEnergy);
                else
                    Histo->Blocker_E4->Fill(uncalEnergy);

                BlockerESum += uncalEnergy;
            }

            Histo->Blocker_ESum->Fill(BlockerESum);
        }

        Histo->CsIMult->Fill(CsIM);

        //check for ffff's
        unsigned short f1 = *point;
        point++;
        unsigned short f2 = *point;
        point++;
        if(f1 != 0xffff && f2 != 0xffff) return false;
    }

    //  return true;

    NT = 0;
    for (int itdc = 0;itdc<1;itdc++)
    {

        //check for ffff's
        unsigned short f3 = *point;
        unsigned short f4 = *(point+1);
        if (f3 == 0xffff && f4 == 0xffff) 
        {
            point += 2;
            continue;
        } 

        // TDC.number = 0;
        // point = TDC.read(point);  // suck out the info in the qdc
        // for (int i=0;i<TDC.number;i++)
        // 	{


        // 	  if (TDC.underflow[i]) continue;
        // 	  if (TDC.overflow[i]) continue;
        //     int id = TDC.channel[i] + 32*itdc;
        //     int uncalTime = TDC.data[i];
        // 	  if (id < TOTAL_CSIS)
        // 	    {
        // 	      DataT[NT].id = id;
        // 	      DataT[NT].uncalTime = uncalTime;
        // 	      Histo->TCsI[id]->Fill(uncalTime);
        // 	      NT++;
        // 	    }
        // 	}
        //      cout << "TDC being called" << endl;
        TDC->read(evtfile);  // suck out the info in the tdc
        //      if(TDC->notTDCerror){ cout << eventNum << endl; return false;}
        for (int i=0;i<TDC->Ndata;i++)
        {

            int id = TDC->dataOut[i].channel;

            int uncalTime = TDC->dataOut[i].time;
            if (id < TOTAL_CSIS)
            {
                DataT[NT].id = id;
                DataT[NT].uncalTime = uncalTime;
                Histo->TCsI[id]->Fill(uncalTime/10.);
                NT++;
            }
            else if (id == 65)
            {
                timeRFScintillator = uncalTime;
                Histo->timeRFScintillator->Fill(uncalTime/10.);
            }
            else if (id == 66)
            {
                timeA1900 = uncalTime;
                Histo->timeA1900->Fill(uncalTime/10.);
                if (haveBlocker)Histo->Blocker_ETOF->Fill(BlockerESum,uncalTime);
            }
            else if (id == 67)
            {
                timeRFCyclotron = uncalTime;
                Histo->timeRFCyclotron->Fill(uncalTime/10.);
            }

        }


        //check for ffff's
        unsigned short f1 = *point;
        point++;
        unsigned short f2 = *point;
        point++;
        if(f1 != 0xffff && f2 != 0xffff) return false;
    }



    // match up energies to times
    for (int ie=0;ie<NE;ie++)
    {
        DataE[ie].uncalTime = -1;
        for (int it=0;it<NT;it++)
        {
            if (DataE[ie].id == DataT[it].id ) 	      //we have matched
            {
                DataE[ie].uncalTime = DataT[it].uncalTime;
                int telescopeNumber = DataE[ie].id/4;
                int icsi = DataE[ie].id%4;
                if(DataE[ie].energy >1.)// && DataE[ie].uncalTime > 500 && DataE[ie].uncalTime < 1500)
                    Telescope[telescopeNumber]->Csi.Add(icsi,DataE[ie].energy,0.,DataE[ie].uncalEnergy,DataE[ie].uncalTime);

            }
            else if (DataE[ie].id < DataT[it].id) break; // no match found
        }
    }

    return true;
}
// //***************************************************************
// //unpacking the XLM with internal ADC's
// bool hira::unpackSi_adc(unsigned short *&point)
// {

//   /*
//     unsigned short * pig = point;
//     for (int i=0;i<100;i++)
//     {
//     cout << dec<< *pig << " " << hex<< *pig << endl;
//     pig++;
//     }
//     abort();
//   */




//   unsigned short marker;
//   unsigned short * nextMB = point;
//   for (int iMB=0;iMB<2;iMB++)
//     {
//       point = nextMB;
//       cout << iMB << " " << hex <<*point;
//       marker = *point++;
//       //if(marker !=xmarker[iMB])point++;
//       if (marker != xmarker[iMB]) return false;

//       int NstripsRead = 0;
//       unsigned short chipWords = *point;
//       nextMB = point + chipWords + 2;
//       //if (chipWords > 400) return false;  // please fix Kyle
//       if (chipWords == 0)
//         {
// 	  NstripsRead = 0;
// 	  return (bool) 1;
//         }
//       point += 2;
//       NstripsRead = *point;

//       if (NstripsRead > 384) return false; // bad buffer
//       point += 5;

//       //  cout << chipWords << " " << NstripsRead*3+7 <<  " " << NstripsRead <<  endl;
//       for (int istrip = 0;istrip < NstripsRead;istrip++)
//         {

//           unsigned short id = *point++;
//           unsigned short chipNum = (id&0x1FE0)>>5;
//           unsigned short chanNum = id & 0x1F;
//           unsigned short uncalEnergy = *point++;
// 	  unsigned short ilowenergy = *point++;
//           unsigned short uncalTime =  *point++;



//           unsigned short underOver = 0;


//           if (chipNum%2 == 0)
// 	    {
// 	      chanNum = 31- 2*chanNum-1;
// 	      chipNum /= 2;
// 	    }
// 	  else
// 	    {
// 	      chipNum = chipNum/2 + 1;
// 	      if(iMB !=2) chanNum = 31 - 2*chanNum;
// 	    }

// 	  //  cout << id << " " << chipNum << " " << chanNum << " " << uncalEnergy << " " << uncalTime << " " << iMB << endl;
//           bool bfront = Map[iMB][chipNum].front;
//           bool bA = Map[iMB][chipNum].A;
//           int telescopeNumber = Map[iMB][chipNum].telescopeNumber - 1;



// 	  //if (iMB == 0) cout << telescopeNumber << " " << chipNum << endl;

// 	  if(telescopeNumber <0)	 return false;
//           bool bhigh = true; 

// 	  if (iMB == 2)
// 	    {
// 	      bhigh = true;

//               if (!bA) chanNum += 16;

// 	    }

// 	  if(bfront) chanNum = 31- chanNum;


// 	  if (chanNum > 31)
// 	    {
// 	      cout << "chanNum too big" << endl;
//               return false;
// 	    }
//           if (chipNum > 12)
// 	    {
// 	      cout << "chipNum too big " << chipNum << endl;
//               return false;
// 	    }


// 	  if (bhigh && bfront)
// 	    {

// 	      float energy = calFront->getEnergy(telescopeNumber,chanNum,uncalEnergy+ran->Rndm());
// 	      //if(telescopeNumber ==6 || telescopeNumber ==7) energy = energy*1.05; //Second Run!!!!!!


// 	      float time = calFrontT->getEnergy(telescopeNumber,chanNum,uncalTime+ran->Rndm());
// 	      Histo->EFTSum[telescopeNumber]->Fill(chanNum,time);
// 	      //	      Histo->SiFTime->Fill(time);

// 	      //Recalibrating
// 	      if(telescopeNumber ==6 || telescopeNumber ==7)
// 	       	energy = calrecal->getEnergy(telescopeNumber-6,chanNum,energy);

// 	      Histo->EfrontR[telescopeNumber][chanNum]->Fill(uncalEnergy);
// 	      Histo->TfrontR[telescopeNumber][chanNum]->Fill(time);
// 	      Histo->EfrontC[telescopeNumber][chanNum]->Fill(energy);
// 	      //Histo->EFSum[telescopeNumber]->Fill(chanNum,uncalEnergy);
// 	      // Histo->EFCSum[telescopeNumber]->Fill(chanNum,energy);

//               if(energy >0.75)
// 		{
// 		  Telescope[telescopeNumber]->Front.Add(chanNum,underOver,energy,uncalEnergy,time);
// 		}

// 	    }
// 	  if (bhigh && !bfront)
// 	    {
// 	      float energy = calBack->getEnergy(telescopeNumber,chanNum,uncalEnergy+ran->Rndm());
// 	      float time = calBackT->getEnergy(telescopeNumber,chanNum,uncalTime+ran->Rndm());
// 	      Histo->EBTSum[telescopeNumber]->Fill(chanNum,time);
// 	      Histo->SiBTime->Fill(time);




// 	      Histo->EbackR[telescopeNumber][chanNum]->Fill(uncalEnergy);
// 	      Histo->TbackR[telescopeNumber][chanNum]->Fill(time);
// 	      Histo->EbackC[telescopeNumber][chanNum]->Fill(energy);
// 	      //Histo->EBSum[telescopeNumber]->Fill(chanNum,uncalEnergy);
// 	      // Histo->EBCSum[telescopeNumber]->Fill(chanNum,energy);

// 	      if(energy > 0.75)
// 		Telescope[telescopeNumber]->Back.Add(chanNum,underOver,energy,uncalEnergy,time);

// 	    }

// 	  if(bhigh && bfront)
// 	    {
// 	      for (int i=0;i<Telescope[telescopeNumber]->Front.Nstore;i++)
// 		{
//                   if (Telescope[telescopeNumber]->Front.Order[i].strip == chanNum)
// 		    Telescope[telescopeNumber]->Front.Order[i].energylow = 0.;

// 		}
// 	    }

// 	  if(bhigh && !bfront)
// 	    {
// 	      for (int i=0;i<Telescope[telescopeNumber]->Back.Nstore;i++)
// 		{
//                   if (Telescope[telescopeNumber]->Back.Order[i].strip == chanNum)
// 		    Telescope[telescopeNumber]->Back.Order[i].energylow = 0.;
// 		}
// 	    }


// 	  if (!bhigh && bfront)
// 	    {

// 	      float time = calFrontTLG->getEnergy(telescopeNumber,chanNum,uncalTime+ran->Rndm());
// 	      Histo->SiFTime->Fill(time);

// 	      Histo->EfrontLGR[telescopeNumber][chanNum]->Fill(uncalEnergy);
// 	      Histo->TfrontLG[telescopeNumber][chanNum]->Fill(time);
// 	      //Histo->EFLSum[telescopeNumber]->Fill(chanNum,uncalEnergy);

// 	      float energy = calLFront->getEnergy(telescopeNumber-6,chanNum,uncalEnergy+ran->Rndm());
// 	      energy = calFront->getEnergy(telescopeNumber,chanNum,energy+ran->Rndm());
// 	      Histo->EfrontLGC[telescopeNumber][chanNum]->Fill(energy);

// 	      //Recalibrating
// 	      if(telescopeNumber ==6 || telescopeNumber ==7)
// 		energy = calrecal->getEnergy(telescopeNumber-6,chanNum,energy);


// 	      for (int i=0;i<Telescope[telescopeNumber]->Front.Nstore;i++)
// 		{
//                   if (Telescope[telescopeNumber]->Front.Order[i].strip == chanNum)
// 		    {
// 		      Telescope[telescopeNumber]->Front.Order[i].energyRlow = uncalEnergy;
// 		      Telescope[telescopeNumber]->Front.Order[i].energylow = energy;
//                       double uncalEnergyHigh=Telescope[telescopeNumber]->Front.Order[i].energyR;
//                       if (uncalEnergyHigh < 15000 && uncalEnergy > 50 && uncalEnergy <15000)
// 			{
// 			  float Ratio = 0.;
//                           fsumN[telescopeNumber-6][chanNum]++;
//                           fsumx[telescopeNumber-6][chanNum] += (double)uncalEnergy;
//                           fsumxx[telescopeNumber-6][chanNum] += pow((double)uncalEnergy,2);
//                           fsumy[telescopeNumber-6][chanNum] += uncalEnergyHigh;
//                           fsumyx[telescopeNumber-6][chanNum] += uncalEnergyHigh*(double)uncalEnergy;
// 			}

// 		      break;
// 		    }
// 		}

// 	    }


// 	  if (!bhigh && !bfront)
// 	    {

// 	      float time = calBackTLG->getEnergy(telescopeNumber,chanNum,uncalTime+ran->Rndm());
// 	      Histo->SiBTime->Fill(time);
// 	      Histo->EbackLGR[telescopeNumber][chanNum]->Fill(uncalEnergy);
// 	      Histo->TbackLG[telescopeNumber][chanNum]->Fill(time);
//               float energy = uncalEnergy;
// 	      energy = calBDual->getEnergy(telescopeNumber-6,chanNum,uncalEnergy+ran->Rndm());
// 	      if(uncalEnergy > energy)energy = uncalEnergy;
// 	      float corenergy = energy;
// 	      Histo->EbackLGCC[telescopeNumber][chanNum]->Fill(energy);
// 	      energy = calLBack->getEnergy(telescopeNumber-6,chanNum,energy+ran->Rndm());
// 	      energy = calBack->getEnergy(telescopeNumber,chanNum,energy+ran->Rndm());
// 	      Histo->EbackLGC[telescopeNumber][chanNum]->Fill(energy);
// 	      //Histo->EBLSum[telescopeNumber]->Fill(chanNum,uncalEnergy);

// 	      for (int i=0;i<Telescope[telescopeNumber]->Back.Nstore;i++)
// 		{
// 		  if (Telescope[telescopeNumber]->Back.Order[i].strip == chanNum)
// 		    {

//                       Telescope[telescopeNumber]->Back.Order[i].energyRlow = uncalEnergy;
//                       Telescope[telescopeNumber]->Back.Order[i].energylow = energy;
//                       double uncalEnergyHigh=Telescope[telescopeNumber]->Back.Order[i].energyR;
//                       if (uncalEnergyHigh < 15000 && uncalEnergy > 50 && uncalEnergy <15000)
// 			{
//                           bsumN[telescopeNumber-6][chanNum]++;
//                           bsumx[telescopeNumber-6][chanNum] += (double)corenergy;
//                           bsumxx[telescopeNumber-6][chanNum] += pow((double)corenergy,2);
//                           bsumy[telescopeNumber-6][chanNum] += uncalEnergyHigh;
//                           bsumyx[telescopeNumber-6][chanNum] += uncalEnergyHigh*(double)corenergy;
// 			}

// 		      break;
// 		    }
// 		}

// 	    }          

// 	}

//     }
//   point = nextMB;
//   if (*point == 0xe0fe) return false;
//   //  cout << "point in si " << hex<< *point << " " << dec << *point << endl;
//   return true;
// }


// //***************************************************************
// //** unpacking with XLM and sis modules
// bool hira::unpackSi_sis(unsigned short *&point)
// {

//   unsigned short marker;
//   for (int iXLM=0;iXLM<3;iXLM++)
//     {
//       marker = *point++;
//       unsigned short *epoint = point;
//       if (marker == xmarker[iXLM]) //XLM1
//         {

// 	  // in princle words and words2 are both part of 
// 	  // a int32 parameter, but in practicable terms
// 	  // words2 should always be zero
//           unsigned short words = *epoint++;   // 
//           unsigned short words2 = *epoint++;
// 	  if (words2 != 0) 
// 	    {
//               cout << "words2= " << words2 << endl;
//               return false;
// 	    }
//           point = epoint + words;
//           unsigned short channelCount = *epoint++;  //number of data in XLM
// 	  //if (channelCount > 0) cout << iXLM << "  " << channelCount << endl;
//           epoint += 4;
// 	  nXLM[iXLM] = channelCount;

// 	  for (int idata = 0;idata<channelCount;idata++)
// 	    {
//               chanXLM[iXLM][idata] = *epoint++;
//               if (chanXLM[iXLM][idata] == 0) return false;
// 	    }

//         }
//       else cout << "XLM" << iXLM << "  missing" << endl; 
//     }
//   int itry = 0;
//   for(;;)
//     {
//       if (itry >10) return false;
//       marker = *point++;
//       if (marker == 0xfadc) break;
//       itry++;
//     }

//   point++;
//   for (int iXLM = 0; iXLM<3;iXLM++)
//     {
//       unsigned short words = *point++;
//       unsigned short words2 = *point++;

//       //cout << iXLM << " " << words << endl;

//       if (words2 != 0) 
// 	{
// 	  cout << "Words in FADC .ne. 0" << endl;
// 	  return false;
// 	}

//       if (words != nXLM[iXLM])
//         {
//           cout << " XLM and FDC channels do not match for XLM " << iXLM <<endl;
//           return false;
// 	}


//       for (int i=0;i<words;i++)
// 	{

//           unsigned short uncalEnergy = *point++;
//           unsigned short time = *point++;
//           unsigned short chan = chanXLM[iXLM][i];

// 	  //by chip
//           unsigned short chipNum =  (chan >> 5) & 0xff;
//           unsigned short chanNum = chan & 0x0F;
// 	  unsigned short underOver = 0;
//           //cout << "chipNum = " << chipNum << " chanNum= " << chanNum << endl;


// 	  //by chip board
// 	  bool secondChip = false;
//           if (chipNum%2 == 0)
// 	    {
// 	      if (iXLM ==2) secondChip = true;
// 	      else chanNum += 16;
// 	      chipNum /= 2;
// 	    }
// 	  else
// 	    {
// 	      chipNum = chipNum/2 + 1;
// 	    }


// 	  bool bfront = Map[iXLM][chipNum].front;
//           bool bA = Map[iXLM][chipNum].A;
//           int telescopeNumber = Map[iXLM][chipNum].telescopeNumber - 1;
// 	  if(telescopeNumber <0)	 return false;
//           bool bhigh = true; 

// 	  if (iXLM == 2)
// 	    {
//               if (secondChip) bhigh = false;
//               else bhigh = true;

//               if (!bA) chanNum += 16;

// 	    }

//           /*
// 	    cout << i << " e= " << uncalEnergy << " t= " << time << " chipNum= " << 
// 	    chipNum << " chanNum= " << chanNum << " bfront= " 
// 	    << bfront << " bA=" << bA << " telescopeNumber= " << telescopeNumber << " bhigh= "
// 	    << bhigh << endl; 
// 	  */
// 	  if (bhigh && bfront)
// 	    {
// 	      float energy = calFront->getEnergy(telescopeNumber,chanNum,uncalEnergy+ran->Rndm());
// 	      Histo->EfrontR[telescopeNumber][chanNum]->Fill(uncalEnergy);
// 	      Histo->TfrontR[telescopeNumber][chanNum]->Fill(time);
// 	      Histo->EfrontC[telescopeNumber][chanNum]->Fill(energy);


//               Telescope[telescopeNumber]->Front.Add(chanNum,underOver,energy,uncalEnergy,time);

// 	    }
// 	  if (bhigh && !bfront)
// 	    {
//               float energy = calBack->getEnergy(telescopeNumber,chanNum,uncalEnergy+ran->Rndm());
// 	      Histo->EbackR[telescopeNumber][chanNum]->Fill(uncalEnergy);
// 	      Histo->TbackR[telescopeNumber][chanNum]->Fill(time);
// 	      Histo->EbackC[telescopeNumber][chanNum]->Fill(energy);

//               Telescope[telescopeNumber]->Back.Add(chanNum,underOver,energy,uncalEnergy,time);

// 	    }

// 	  if (!bhigh && bfront)
// 	    {
// 	      Histo->EfrontLGR[telescopeNumber][chanNum]->Fill(uncalEnergy);
// 	      Histo->TfrontLG[telescopeNumber][chanNum]->Fill(time);


// 	      for (int i=0;i<Telescope[telescopeNumber]->Front.Nstore;i++)
// 		{
//                   if (Telescope[telescopeNumber]->Front.Order[i].strip == chanNum)
// 		    {
//                       Telescope[telescopeNumber]->Front.Order[i].energyRlow = uncalEnergy;
//                       double uncalEnergyHigh=Telescope[telescopeNumber]->Front.Order[i].energyR;
//                       if (uncalEnergyHigh < 15000 && uncalEnergy > 50 && uncalEnergy <15000)
// 			{
// 			  float Ratio = 0.;
//                           fsumN[telescopeNumber-6][chanNum]++;
//                           fsumx[telescopeNumber-6][chanNum] += (double)uncalEnergy;
//                           fsumxx[telescopeNumber-6][chanNum] += pow((double)uncalEnergy,2);
//                           fsumy[telescopeNumber-6][chanNum] += uncalEnergyHigh;
//                           fsumyx[telescopeNumber-6][chanNum] += uncalEnergyHigh*(double)uncalEnergy;
// 			}

// 		      break;
// 		    }
// 		}


// 	    }          

// 	  if (!bhigh && !bfront)
// 	    {
// 	      Histo->EbackLGR[telescopeNumber][chanNum]->Fill(uncalEnergy);
// 	      Histo->TbackLG[telescopeNumber][chanNum]->Fill(time);

// 	      for (int i=0;i<Telescope[telescopeNumber]->Back.Nstore;i++)
// 		{
//                   if (Telescope[telescopeNumber]->Back.Order[i].strip == chanNum)
// 		    {
//                       Telescope[telescopeNumber]->Back.Order[i].energyRlow = uncalEnergy;
//                       double uncalEnergyHigh=Telescope[telescopeNumber]->Back.Order[i].energyR;
//                       if (uncalEnergyHigh < 15000 && uncalEnergy > 50 && uncalEnergy <15000)
// 			{
//                           bsumN[telescopeNumber-6][chanNum]++;
//                           bsumx[telescopeNumber-6][chanNum] += (double)uncalEnergy;
//                           bsumxx[telescopeNumber-6][chanNum] += pow((double)uncalEnergy,2);
//                           bsumy[telescopeNumber-6][chanNum] += uncalEnergyHigh;
//                           bsumyx[telescopeNumber-6][chanNum] += uncalEnergyHigh*(double)uncalEnergy;
// 			}

// 		      break;
// 		    }
// 		}

// 	    }          

// 	  if (chanNum > 31)
// 	    {
// 	      cout << "chanNum too big" << endl;
//               return false;
// 	    }
//           if (chipNum > 11)
// 	    {
// 	      cout << "chipNum too big" << endl;
//               return false;
// 	    }
// 	}

//       marker = *point++;
//       if (marker != 0xaaaa) cout << "marker = " << marker << endl;
//     }




//   return true;
// }
//***************************************************************

//unpacking the XLM with ADC on the CHIP BOARDS (HINP 4)
bool hira::unpackSi_HINP4(ifstream& evtfile)
{
    vector<unsigned short> marker(1);
    readWord(evtfile,marker);

    for(int iMB = 0;iMB<2;iMB++)
    {
        vector<unsigned short> dummy(2);
        readWord(evtfile,dummy);

        if (marker[0] != xmarker[iMB])
        { cout << "Did not read the proper XLM marker. Was " << hex << marker[0] << " expected " << xmarker[iMB] << dec <<endl;
            return false;
        }

        vector<unsigned int> NstripsRead(1);
        unsigned short chipWords;
        int NWords;

        readWord(evtfile,NstripsRead);

        if(NstripsRead[0] %4 !=0) 
        {
            return false;
        }

        if (NstripsRead[0] > 384)
        {
            vector<unsigned int> dummy(2);
            // supposed to be 8 bytes? or 16?

            return false; // bad buffer
        }

        vector<char> dummy2(5);
        readWord(evtfile,dummy2); 
        // supposed to be 5 bytes? or 10?

        NstripsRead[0] /= 4;

        for (int istrip = 0;istrip <(int)NstripsRead[0];istrip++)
        {
            vector<unsigned short> id(1);
            readWord(evtfile,id);

            unsigned short chipNum = (id[0]&0x1FE0)>>5;
            unsigned short chanNum = id[0] & 0x1F;

            vector<unsigned short> uncalEnergy(1);
            readWord(evtfile,uncalEnergy);

            vector<unsigned short> ilowenergy(1);
            readWord(evtfile,ilowenergy);

            vector<unsigned short> uncalTime(1);
            readWord(evtfile,uncalTime);

            if (chipNum%2 == 0)
            {
                chanNum = 31- 2*chanNum-1;
                chipNum /= 2;
            }

            else
            {
                chipNum = chipNum/2 + 1;
                chanNum = 31 - 2*chanNum;
            }

            bool bfront = Map[iMB][chipNum].front;
            int telescopeNumber = Map[iMB][chipNum].telescopeNumber;

            if (bfront) chanNum  = 31 - chanNum;

            if(telescopeNumber < 0)
            {
                return false;
            }

            if (chanNum > 31)
            {
                cout << "chanNum too big" << endl;
                return false;
            }

            if (chipNum > NUMBER_OF_TELESCOPES)
            {
                cout << "chipNum too big " << chipNum << endl;
                return false;
            }

            //Filling the front histograms and calibrating
            if (bfront)
            {
                float energy; // = calFront->getEnergy(telescopeNumber,chanNum,uncalEnergy+ran->Rndm());
                float lowenergy; // = calLFront->getEnergy(telescopeNumber,chanNum,ilowenergy+ran->Rndm());
                //lowenergy = calFront->getEnergy(telescopeNumber,chanNum,lowenergy);
                if(lowenergy > 70. && energy > 20.) //cut off for switching high to low gain energy
                {
                    energy = lowenergy;
                }

                float time;// = calFrontT->getEnergy(telescopeNumber,chanNum,uncalTime+ran->Rndm());
                Histo->EFTSum[telescopeNumber]->Fill(chanNum,time);

                //Front raw spectra
                Histo->EfrontR[telescopeNumber][chanNum]->Fill(uncalEnergy[0]); //high gain
                Histo->EfrontLR[telescopeNumber][chanNum]->Fill(ilowenergy[0]); //low gain
                Histo->TfrontR[telescopeNumber][chanNum]->Fill(time); //time
                Histo->EFSum[telescopeNumber]->Fill(chanNum,uncalEnergy[0]);

                //Front calibrated spectra
                Histo->EfrontC[telescopeNumber][chanNum]->Fill(energy); //high gain
                Histo->EfrontLC[telescopeNumber][chanNum]->Fill(lowenergy); //low gain
                Histo->EFCSum[telescopeNumber]->Fill(chanNum,energy);

                if (uncalEnergy[0] < 15000 && uncalEnergy[0] > 50 && ilowenergy[0] <15000)
                {
                    fsumN[telescopeNumber][chanNum]++;
                    fsumx[telescopeNumber][chanNum] += (double)ilowenergy[0];
                    fsumxx[telescopeNumber][chanNum] += pow((double)ilowenergy[0],2);
                    fsumy[telescopeNumber][chanNum] += uncalEnergy[0];
                    fsumyx[telescopeNumber][chanNum] += uncalEnergy[0]*(double)ilowenergy[0];
                }

                if(energy >0.75)
                {
                    Telescope[telescopeNumber]->Front.Add(chanNum,energy,ilowenergy[0],uncalEnergy[0],time);
                }

            }

            //filling the back histograms and calibrating
            if (!bfront)
            {

                float energy;// = calBack->getEnergy(telescopeNumber,chanNum,uncalEnergy+ran->Rndm());
                float lowenergy;// = calLBack->getEnergy(telescopeNumber,chanNum,ilowenergy+ran->Rndm());
                //lowenergy = calBack->getEnergy(telescopeNumber,chanNum,lowenergy);
                if(lowenergy > 70. && energy > 20.) //cut off for switching high to low gain energy
                    energy = lowenergy;

                float time; // = calBackT->getEnergy(telescopeNumber,chanNum,uncalTime+ran->Rndm());
                Histo->EBTSum[telescopeNumber]->Fill(chanNum,time);
                //	      Histo->SiFTime->Fill(time);

                //Back raw spectra
                Histo->EbackR[telescopeNumber][chanNum]->Fill(uncalEnergy[0]); //high gain
                Histo->EbackLR[telescopeNumber][chanNum]->Fill(ilowenergy[0]); //low gain
                Histo->TbackR[telescopeNumber][chanNum]->Fill(time); //time
                Histo->EBSum[telescopeNumber]->Fill(chanNum,uncalEnergy[0]);

                //Back calibrated spectra
                Histo->EbackC[telescopeNumber][chanNum]->Fill(energy); //high gain
                Histo->EbackLC[telescopeNumber][chanNum]->Fill(lowenergy); //low gain
                Histo->EBCSum[telescopeNumber]->Fill(chanNum,energy);


                if (uncalEnergy[0] < 15000 && uncalEnergy[0] > 50 && ilowenergy[0] <15000)
                {
                    bsumN[telescopeNumber][chanNum]++;
                    bsumx[telescopeNumber][chanNum] += (double)ilowenergy[0];
                    bsumxx[telescopeNumber][chanNum] += pow((double)ilowenergy[0],2);
                    bsumy[telescopeNumber][chanNum] += uncalEnergy[0];
                    bsumyx[telescopeNumber][chanNum] += uncalEnergy[0]*(double)ilowenergy[0];
                }

                if(energy >0.75)
                {
                    Telescope[telescopeNumber]->Back.Add(chanNum,energy,ilowenergy[0],uncalEnergy[0],time);
                }
            }	  
        }
    }

    return true; // need to return != 0xe0fe
}

//***************************************************************
hira::~hira()
{
    cout << "start Hira destr" << endl;
    //high-low correlations

    ofstream iFfile("f.dat");
    ofstream iBfile("b.dat");

    for (int i=0;i<NUMBER_OF_TELESCOPES;i++)
    {
        for (int j=0;j<NUMBER_OF_STRIPS;j++)
        {
            double delta = fsumN[i][j]*fsumxx[i][j]- pow(fsumx[i][j],2);
            if (delta == 0)
            {
                iBfile << i << " " << j << " " << 1 << " " << 0 << endl;
                continue;

            }

            double slope = fsumN[i][j]*fsumyx[i][j] - fsumx[i][j]*fsumy[i][j];
            slope /= delta;
            double intercept = (fsumy[i][j] - slope*fsumx[i][j])/fsumN[i][j];
            iFfile << i << " " << j << " " << slope << " " << intercept << endl;

        }
    }

    iFfile.close();
    for (int i=0;i<NUMBER_OF_TELESCOPES;i++)
    {
        for (int j=0;j<NUMBER_OF_STRIPS;j++)
        {
            double delta = bsumN[i][j]*bsumxx[i][j]- pow(bsumx[i][j],2);

            if (delta == 0)
            {
                iBfile << i << " " << j << " " << 1 << " " << 0 << endl;
                continue;

            }

            double slope = bsumN[i][j]*bsumyx[i][j] - bsumx[i][j]*bsumy[i][j];
            slope /= delta;
            double intercept = (bsumy[i][j] - slope*bsumx[i][j])/bsumN[i][j];
            iBfile << i << " " << j << " " << slope << " " << intercept << endl;
        }
    }

    iBfile.close();

    /*delete calFront;
      delete calBack;
      delete calCsi;
      delete calLBack;
      delete calLFront;
      delete calBDual;
      delete calFrontT;
      delete calBackT;
      delete calFrontTLG;
      delete calBackTLG;
      */

    for(int i=0;i<NUMBER_OF_TELESCOPES;i++)
    {
        delete Telescope[i];
    }

    delete [] Telescope;

    cout << "stop Hira destr" << endl;
}

//**********************************************
void hira::reset()
{
    for (int i=0;i<NUMBER_OF_TELESCOPES;i++) Telescope[i]->reset();
}
