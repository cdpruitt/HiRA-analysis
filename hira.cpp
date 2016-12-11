#include "hira.h"


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
  int i1,iMB,cbf,cbb,cbfl,cbbl;
  for (;;)
    {
      ifile >> i1 >> iMB >> cbf>> cbb;
      if (ifile.eof()) break;
      if (ifile.bad()) break;

      iMB -= 1;
      Map[iMB][cbf].front = true;
      Map[iMB][cbf].itele = i1;

      Map[iMB][cbb].front = false;
      Map[iMB][cbb].itele = i1;

      
      
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
      CsIMap[icsi].itele = itel;
      CsIMap[icsi].iCsi = ich;
    }
  csimap.close();
  csimap.clear();
  
  //read in calibrations
  int Ntele = 14;
  int Nstrip = 32;
  name = "cal/front.cal";
  calFront = new calibrate(Ntele,Nstrip,name,1);
  name = "cal/back.cal";
  calBack = new calibrate(Ntele,Nstrip,name,1);

  name = "cal/HL_Front.cal";
  calLFront = new calibrate(Ntele,Nstrip,name,1);
  name = "cal/HL_Back.cal";
  calLBack = new calibrate(Ntele,Nstrip,name,1);


  name = "cal/Proton_New_ang.cal";
  calCsi = new calibrate(1,56,name,1);

  //TIME Calibrations

  name = "cal/timePulser_front.cal";
  calFrontT = new calibrate(Ntele,Nstrip,name,1);
  name = "cal/timePulser_frontLG.cal";
  calFrontTLG = new calibrate(Ntele,Nstrip,name,1);

  name = "cal/timePulser_back.cal";
  calBackT = new calibrate(Ntele,Nstrip,name,1);
  name = "cal/timePulser_backLG.cal";
  calBackTLG = new calibrate(Ntele,Nstrip,name,1);




  Telescope = new telescope*[14];
  for (int i=0;i<14;i++)
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
  for (int i=0;i<14;i++)
    for (int j=0;j<32;j++)
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
bool hira::unpack(unsigned short *&point,int runno)
{
  bool stat = true;
  stat = unpackSi_HINP4(point);
  if (!stat) return stat;
  //  cout <<"before CsI " << hex << *point << " " << *(point+1) << dec << endl;
  stat = unpackCsi(point,runno); 
}
//*************************************************************
bool hira::unpackCsi(unsigned short *&point,int runno)
{
  NE = 0;
  CsIM = 0;

  // cout << "point in Csi " << hex << *point << endl; 
  
  /*
    unsigned short * pig = point;
    for (int i=0;i<50;i++)
    {
    cout << dec<< *pig << " " << hex<< *pig << endl;
    pig++;
    }
    cout << endl;
    abort();
    //return true;
    */

  BlockerESum = 0;
  haveBlocker = false;

  
  for (int iadc = 0;iadc<2;iadc++)
    {
      //check for ffff's
      unsigned short f3 = *point;
      unsigned short f4 = *(point+1);
      if (f3 == 0xffff && f4 == 0xffff)
	{
	  point += 2;
	  continue;
	} 
      ADC.number = 0;
      point = ADC.read(point);  // suck out the info in the qdc
      for (int i=0;i<ADC.number;i++)
	{

	  if (ADC.underflow[i]) continue;
	  if (ADC.overflow[i]) continue;
          int id = ADC.channel[i] + 32*iadc;
          int ienergy = ADC.data[i];
	  //	  cout << id << " " << ienergy << endl;
	  
	  if(id < 56)
	    {
	      int iCsi = CsIMap[id].iCsi;
	      int itele = CsIMap[id].itele;

	      float energy = calCsi->getEnergy(0,iCsi,ienergy+ran->Rndm());
	      //   cout << "ID = " << id << endl;
	      //	      cout << id << " " << iCsi << endl;
	      
	      DataE[NE].id = iCsi;
	      DataE[NE].ienergy = ienergy;
	      DataE[NE].energy = energy;
	      Histo->ECsI[iCsi]->Fill(ienergy);
	      Histo->ECsISum->Fill(iCsi,ienergy);
	      Histo->ECsICSum->Fill(iCsi,energy);


	      int icsi = DataE[NE].id%4; 
	      // if(iCsi == 22)
	      // 	{
	      // 	  cout << itele << " " << icsi << endl;
	      // 	}
	      //	      cout << itele << " " << id << " "<< iCsi << " " << icsi <<endl;
	      
	      if (ienergy > 50)
		{
		  CsIM++;
		  Telescope[itele]->Csi.Add(icsi,energy,0.,DataE[NE].ienergy,0.);
		}
	      
	      //cout << Telescope[itele]->Csi.Order[0].strip;
	      //cout << " " << Telescope[itele]->Csi.Order[0].energy << endl;
	      
	      NE++;
	    }
	  if (id >= 56 && id<= 59)
	    {
	      haveBlocker = true;
	      if(id == 56)
		Histo->Blocker_E1->Fill(ienergy);
	      else if (id == 57)
		Histo->Blocker_E2->Fill(ienergy);
	      else if (id == 58)
		Histo->Blocker_E3->Fill(ienergy);
	      else
		Histo->Blocker_E4->Fill(ienergy);
	      
	      BlockerESum += ienergy;
	    }

	  Histo->Blocker_ESum->Fill(BlockerESum);
	  
	  //	  else if(id ==56 && runno >= 170 && runno <=172) //Blocker CsI
	  //  {
	  //    Histo->Blocker_E->Fill(ienergy);
	  //    Blocker_e = ienergy;
	  //  }


	}
      
      Histo->CsIMult->Fill(CsIM);
      //cout << hex << *point << " " << *(point+1) << dec << endl;
      
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
      //     int itime = TDC.data[i];
      // 	  if (id < 56)
      // 	    {
      // 	      DataT[NT].id = id;
      // 	      DataT[NT].itime = itime;
      // 	      Histo->TCsI[id]->Fill(itime);
      // 	      NT++;
      // 	    }
      // 	}
      //      cout << "TDC being called" << endl;
      point = TDC->read(point);  // suck out the info in the tdc
      //      if(TDC->notTDCerror){ cout << eventNum << endl; return false;}
      for (int i=0;i<TDC->Ndata;i++)
	{
	  
          int id = TDC->dataOut[i].channel;
	  
          int itime = TDC->dataOut[i].time;
	  if (id < 56)
	    {
	      DataT[NT].id = id;
	      DataT[NT].itime = itime;
	      Histo->TCsI[id]->Fill(itime/10.);
	      NT++;
	    }
	  else if (id == 65)
	    {
	      T_RFSCIN = itime;
	      Histo->T_RFSCIN->Fill(itime/10.);
	    }
	  else if (id == 66)
	    {
	      T_A1900 = itime;
	      Histo->T_A1900->Fill(itime/10.);
	      if (haveBlocker)Histo->Blocker_ETOF->Fill(BlockerESum,itime);
	    }
	   else if (id == 67)
	    {
	      T_RFCYC = itime;
	      Histo->T_RFCYC->Fill(itime/10.);
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
      DataE[ie].itime = -1;
      for (int it=0;it<NT;it++)
	{
          if (DataE[ie].id == DataT[it].id ) 	      //we have matched
	    {
	      DataE[ie].itime = DataT[it].itime;
	      int itele = DataE[ie].id/4;
	      int icsi = DataE[ie].id%4;
	      if(DataE[ie].energy >1.)// && DataE[ie].itime > 500 && DataE[ie].itime < 1500)
		Telescope[itele]->Csi.Add(icsi,DataE[ie].energy,0.,DataE[ie].ienergy,DataE[ie].itime);

	    }
	  else if (DataE[ie].id < DataT[it].id) break; // no match found
	}
    }
  






  bool stat = true;
  return stat;
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
//           unsigned short ienergy = *point++;
// 	  unsigned short ilowenergy = *point++;
//           unsigned short itime =  *point++;



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
	  
// 	  //  cout << id << " " << chipNum << " " << chanNum << " " << ienergy << " " << itime << " " << iMB << endl;
//           bool bfront = Map[iMB][chipNum].front;
//           bool bA = Map[iMB][chipNum].A;
//           int itele = Map[iMB][chipNum].itele - 1;



// 	  //if (iMB == 0) cout << itele << " " << chipNum << endl;

// 	  if(itele <0)	 return false;
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

// 	      float energy = calFront->getEnergy(itele,chanNum,ienergy+ran->Rndm());
// 	      //if(itele ==6 || itele ==7) energy = energy*1.05; //Second Run!!!!!!


// 	      float time = calFrontT->getEnergy(itele,chanNum,itime+ran->Rndm());
// 	      Histo->EFTSum[itele]->Fill(chanNum,time);
// 	      //	      Histo->SiFTime->Fill(time);

// 	      //Recalibrating
// 	      if(itele ==6 || itele ==7)
// 	       	energy = calrecal->getEnergy(itele-6,chanNum,energy);

// 	      Histo->EfrontR[itele][chanNum]->Fill(ienergy);
// 	      Histo->TfrontR[itele][chanNum]->Fill(time);
// 	      Histo->EfrontC[itele][chanNum]->Fill(energy);
// 	      //Histo->EFSum[itele]->Fill(chanNum,ienergy);
// 	      // Histo->EFCSum[itele]->Fill(chanNum,energy);

//               if(energy >0.75)
// 		{
// 		  Telescope[itele]->Front.Add(chanNum,underOver,energy,ienergy,time);
// 		}
		
// 	    }
// 	  if (bhigh && !bfront)
// 	    {
// 	      float energy = calBack->getEnergy(itele,chanNum,ienergy+ran->Rndm());
// 	      float time = calBackT->getEnergy(itele,chanNum,itime+ran->Rndm());
// 	      Histo->EBTSum[itele]->Fill(chanNum,time);
// 	      Histo->SiBTime->Fill(time);




// 	      Histo->EbackR[itele][chanNum]->Fill(ienergy);
// 	      Histo->TbackR[itele][chanNum]->Fill(time);
// 	      Histo->EbackC[itele][chanNum]->Fill(energy);
// 	      //Histo->EBSum[itele]->Fill(chanNum,ienergy);
// 	      // Histo->EBCSum[itele]->Fill(chanNum,energy);

// 	      if(energy > 0.75)
// 		Telescope[itele]->Back.Add(chanNum,underOver,energy,ienergy,time);

// 	    }

// 	  if(bhigh && bfront)
// 	    {
// 	      for (int i=0;i<Telescope[itele]->Front.Nstore;i++)
// 		{
//                   if (Telescope[itele]->Front.Order[i].strip == chanNum)
// 		    Telescope[itele]->Front.Order[i].energylow = 0.;

// 		}
// 	    }

// 	  if(bhigh && !bfront)
// 	    {
// 	      for (int i=0;i<Telescope[itele]->Back.Nstore;i++)
// 		{
//                   if (Telescope[itele]->Back.Order[i].strip == chanNum)
// 		    Telescope[itele]->Back.Order[i].energylow = 0.;
// 		}
// 	    }


// 	  if (!bhigh && bfront)
// 	    {

// 	      float time = calFrontTLG->getEnergy(itele,chanNum,itime+ran->Rndm());
// 	      Histo->SiFTime->Fill(time);

// 	      Histo->EfrontLGR[itele][chanNum]->Fill(ienergy);
// 	      Histo->TfrontLG[itele][chanNum]->Fill(time);
// 	      //Histo->EFLSum[itele]->Fill(chanNum,ienergy);

// 	      float energy = calLFront->getEnergy(itele-6,chanNum,ienergy+ran->Rndm());
// 	      energy = calFront->getEnergy(itele,chanNum,energy+ran->Rndm());
// 	      Histo->EfrontLGC[itele][chanNum]->Fill(energy);

// 	      //Recalibrating
// 	      if(itele ==6 || itele ==7)
// 		energy = calrecal->getEnergy(itele-6,chanNum,energy);


// 	      for (int i=0;i<Telescope[itele]->Front.Nstore;i++)
// 		{
//                   if (Telescope[itele]->Front.Order[i].strip == chanNum)
// 		    {
// 		      Telescope[itele]->Front.Order[i].energyRlow = ienergy;
// 		      Telescope[itele]->Front.Order[i].energylow = energy;
//                       double ienergyHigh=Telescope[itele]->Front.Order[i].energyR;
//                       if (ienergyHigh < 15000 && ienergy > 50 && ienergy <15000)
// 			{
// 			  float Ratio = 0.;
//                           fsumN[itele-6][chanNum]++;
//                           fsumx[itele-6][chanNum] += (double)ienergy;
//                           fsumxx[itele-6][chanNum] += pow((double)ienergy,2);
//                           fsumy[itele-6][chanNum] += ienergyHigh;
//                           fsumyx[itele-6][chanNum] += ienergyHigh*(double)ienergy;
// 			}
                      
// 		      break;
// 		    }
// 		}
	      
// 	    }
	  
	  
// 	  if (!bhigh && !bfront)
// 	    {

// 	      float time = calBackTLG->getEnergy(itele,chanNum,itime+ran->Rndm());
// 	      Histo->SiBTime->Fill(time);
// 	      Histo->EbackLGR[itele][chanNum]->Fill(ienergy);
// 	      Histo->TbackLG[itele][chanNum]->Fill(time);
//               float energy = ienergy;
// 	      energy = calBDual->getEnergy(itele-6,chanNum,ienergy+ran->Rndm());
// 	      if(ienergy > energy)energy = ienergy;
// 	      float corenergy = energy;
// 	      Histo->EbackLGCC[itele][chanNum]->Fill(energy);
// 	      energy = calLBack->getEnergy(itele-6,chanNum,energy+ran->Rndm());
// 	      energy = calBack->getEnergy(itele,chanNum,energy+ran->Rndm());
// 	      Histo->EbackLGC[itele][chanNum]->Fill(energy);
// 	      //Histo->EBLSum[itele]->Fill(chanNum,ienergy);

// 	      for (int i=0;i<Telescope[itele]->Back.Nstore;i++)
// 		{
// 		  if (Telescope[itele]->Back.Order[i].strip == chanNum)
// 		    {

//                       Telescope[itele]->Back.Order[i].energyRlow = ienergy;
//                       Telescope[itele]->Back.Order[i].energylow = energy;
//                       double ienergyHigh=Telescope[itele]->Back.Order[i].energyR;
//                       if (ienergyHigh < 15000 && ienergy > 50 && ienergy <15000)
// 			{
//                           bsumN[itele-6][chanNum]++;
//                           bsumx[itele-6][chanNum] += (double)corenergy;
//                           bsumxx[itele-6][chanNum] += pow((double)corenergy,2);
//                           bsumy[itele-6][chanNum] += ienergyHigh;
//                           bsumyx[itele-6][chanNum] += ienergyHigh*(double)corenergy;
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
         
//           unsigned short ienergy = *point++;
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
//           int itele = Map[iXLM][chipNum].itele - 1;
// 	  if(itele <0)	 return false;
//           bool bhigh = true; 

// 	  if (iXLM == 2)
// 	    {
//               if (secondChip) bhigh = false;
//               else bhigh = true;

//               if (!bA) chanNum += 16;

// 	    }

//           /*
// 	    cout << i << " e= " << ienergy << " t= " << time << " chipNum= " << 
// 	    chipNum << " chanNum= " << chanNum << " bfront= " 
// 	    << bfront << " bA=" << bA << " itele= " << itele << " bhigh= "
// 	    << bhigh << endl; 
// 	  */
// 	  if (bhigh && bfront)
// 	    {
// 	      float energy = calFront->getEnergy(itele,chanNum,ienergy+ran->Rndm());
// 	      Histo->EfrontR[itele][chanNum]->Fill(ienergy);
// 	      Histo->TfrontR[itele][chanNum]->Fill(time);
// 	      Histo->EfrontC[itele][chanNum]->Fill(energy);


//               Telescope[itele]->Front.Add(chanNum,underOver,energy,ienergy,time);

// 	    }
// 	  if (bhigh && !bfront)
// 	    {
//               float energy = calBack->getEnergy(itele,chanNum,ienergy+ran->Rndm());
// 	      Histo->EbackR[itele][chanNum]->Fill(ienergy);
// 	      Histo->TbackR[itele][chanNum]->Fill(time);
// 	      Histo->EbackC[itele][chanNum]->Fill(energy);

//               Telescope[itele]->Back.Add(chanNum,underOver,energy,ienergy,time);

// 	    }

// 	  if (!bhigh && bfront)
// 	    {
// 	      Histo->EfrontLGR[itele][chanNum]->Fill(ienergy);
// 	      Histo->TfrontLG[itele][chanNum]->Fill(time);


// 	      for (int i=0;i<Telescope[itele]->Front.Nstore;i++)
// 		{
//                   if (Telescope[itele]->Front.Order[i].strip == chanNum)
// 		    {
//                       Telescope[itele]->Front.Order[i].energyRlow = ienergy;
//                       double ienergyHigh=Telescope[itele]->Front.Order[i].energyR;
//                       if (ienergyHigh < 15000 && ienergy > 50 && ienergy <15000)
// 			{
// 			  float Ratio = 0.;
//                           fsumN[itele-6][chanNum]++;
//                           fsumx[itele-6][chanNum] += (double)ienergy;
//                           fsumxx[itele-6][chanNum] += pow((double)ienergy,2);
//                           fsumy[itele-6][chanNum] += ienergyHigh;
//                           fsumyx[itele-6][chanNum] += ienergyHigh*(double)ienergy;
// 			}
                      
// 		      break;
// 		    }
// 		}


// 	    }          

// 	  if (!bhigh && !bfront)
// 	    {
// 	      Histo->EbackLGR[itele][chanNum]->Fill(ienergy);
// 	      Histo->TbackLG[itele][chanNum]->Fill(time);

// 	      for (int i=0;i<Telescope[itele]->Back.Nstore;i++)
// 		{
//                   if (Telescope[itele]->Back.Order[i].strip == chanNum)
// 		    {
//                       Telescope[itele]->Back.Order[i].energyRlow = ienergy;
//                       double ienergyHigh=Telescope[itele]->Back.Order[i].energyR;
//                       if (ienergyHigh < 15000 && ienergy > 50 && ienergy <15000)
// 			{
//                           bsumN[itele-6][chanNum]++;
//                           bsumx[itele-6][chanNum] += (double)ienergy;
//                           bsumxx[itele-6][chanNum] += pow((double)ienergy,2);
//                           bsumy[itele-6][chanNum] += ienergyHigh;
//                           bsumyx[itele-6][chanNum] += ienergyHigh*(double)ienergy;
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
bool hira::unpackSi_HINP4(unsigned short *&point)
{

  /*
      cout << "HEY" << endl;
      unsigned short * pig = point;
      for (int i=0;i<30;i++)
      {
      cout << dec<< *pig << " " << hex<< *pig << endl;
      pig++;
      }
      abort();
  */
  



  unsigned short marker;
  point += 1;
  //    cout << iMB << " " << hex <<*point;
  for(int iMB = 0;iMB<2;iMB++)
    {
      marker = *point++;
      if (marker != xmarker[iMB])
	{ cout << "Did not read the proper XLM marker. Was " << hex << marker << " expected " << xmarker[iMB] << dec <<endl;
	  return false;
	}
      //      cout << hex << marker << dec <<endl;
      int NstripsRead = 0;
      unsigned short chipWords = *point;
      int NWords = *point;
      unsigned short * endHINP = point;
      endHINP += NWords+2;
      //  if (chipWords == 8)
      //  {
      //    NstripsRead = 0;
      //    point += 10;
      //    return (bool) 1;
      //  }
      point += 2;
      NstripsRead = *point;

      if(NstripsRead %4 !=0) 
	return false;
      
      //  cout << "NStripsRead = " << NstripsRead << endl;
      if (NstripsRead > 384)
	{
	  point +=8;
	  return false; // bad buffer
	}
      point += 5;
      
      NstripsRead /= 4;
      // cout << NstripsRead << endl;
      
      //cout << NstripsRead << endl;
      
      //       cout <<  "NStrips " << NstripsRead <<  endl;
      for (int istrip = 0;istrip < NstripsRead;istrip++)
	{
	  
	  unsigned short id = *point++;
	  unsigned short chipNum = (id&0x1FE0)>>5;
	  unsigned short chanNum = id & 0x1F;
	  unsigned short ienergy = *point++;
	  unsigned short ilowenergy = *point++;
	  unsigned short itime =  *point++;
	  unsigned short underOver = 0;   //No under or overflow in HINP4
      


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
	  
	  //cout << id << " " << chipNum << " " << chanNum << " " << ienergy << " " << itime << " " << iMB << endl;
	  bool bfront = Map[iMB][chipNum].front;
	  int itele = Map[iMB][chipNum].itele;

	  if (bfront) chanNum  = 31 - chanNum;
	  
	  //if(itele==6)cout << "tele 6 found" << endl;
	  
	  
	  
	  //if (iMB == 0) cout << itele << " " << chipNum << endl;
	  
	  if(itele <0)	 return false;
	  
	  if (chanNum > 31)
	    {
	      cout << "chanNum too big" << endl;
	      return false;
	    }
	  if (chipNum > 14)
	    {
	      cout << "chipNum too big " << chipNum << endl;
	      return false;
	    }
	  
	  //Filling the front histograms and calibrating
	  if (bfront)
	    {
	      
	      float energy = calFront->getEnergy(itele,chanNum,ienergy+ran->Rndm());
	      float lowenergy = calLFront->getEnergy(itele,chanNum,ilowenergy+ran->Rndm());
	      lowenergy = calFront->getEnergy(itele,chanNum,lowenergy);
	      // if(energy > 4.0)
	      // 	{
	      // 	  cout << "channel = " << chanNum << endl;
	      // 	  cout << energy << " " << lowenergy << endl;
	      // 	}
	      if(lowenergy > 70. && energy > 20.) //cut off for switching high to low gain energy
		energy = lowenergy;
	      
	      float time = calFrontT->getEnergy(itele,chanNum,itime+ran->Rndm());
	      Histo->EFTSum[itele]->Fill(chanNum,time);
	      //	      Histo->SiFTime->Fill(time);

	      //Front raw spectra
	      Histo->EfrontR[itele][chanNum]->Fill(ienergy); //high gain
	      Histo->EfrontLR[itele][chanNum]->Fill(ilowenergy); //low gain
	      Histo->TfrontR[itele][chanNum]->Fill(time); //time
	      Histo->EFSum[itele]->Fill(chanNum,ienergy);
	      
	      //Front calibrated spectra
	      Histo->EfrontC[itele][chanNum]->Fill(energy); //high gain
	      Histo->EfrontLC[itele][chanNum]->Fill(lowenergy); //low gain
	      Histo->EFCSum[itele]->Fill(chanNum,energy);

	      if (ienergy < 15000 && ienergy > 50 && ilowenergy <15000)
		{
		  fsumN[itele][chanNum]++;
		  fsumx[itele][chanNum] += (double)ilowenergy;
		  fsumxx[itele][chanNum] += pow((double)ilowenergy,2);
		  fsumy[itele][chanNum] += ienergy;
		  fsumyx[itele][chanNum] += ienergy*(double)ilowenergy;
		}
	      

	      
	      if(energy >0.75)
		{
		  Telescope[itele]->Front.Add(chanNum,energy,ilowenergy,ienergy,time);
		}
	      
	    }
	  
	  //filling the back histograms and calibrating
	  if (!bfront)
	    {

	      float energy = calBack->getEnergy(itele,chanNum,ienergy+ran->Rndm());
	      float lowenergy = calLBack->getEnergy(itele,chanNum,ilowenergy+ran->Rndm());
	      lowenergy = calBack->getEnergy(itele,chanNum,lowenergy);
	      if(lowenergy > 70. && energy > 20.) //cut off for switching high to low gain energy
		energy = lowenergy;
	      
	      float time = calBackT->getEnergy(itele,chanNum,itime+ran->Rndm());
	      Histo->EBTSum[itele]->Fill(chanNum,time);
	      //	      Histo->SiFTime->Fill(time);

	      //Back raw spectra
	      Histo->EbackR[itele][chanNum]->Fill(ienergy); //high gain
	      Histo->EbackLR[itele][chanNum]->Fill(ilowenergy); //low gain
	      Histo->TbackR[itele][chanNum]->Fill(time); //time
	      Histo->EBSum[itele]->Fill(chanNum,ienergy);
	      
	      //Back calibrated spectra
	      Histo->EbackC[itele][chanNum]->Fill(energy); //high gain
	      Histo->EbackLC[itele][chanNum]->Fill(lowenergy); //low gain
	      Histo->EBCSum[itele]->Fill(chanNum,energy);

	      
	      if (ienergy < 15000 && ienergy > 50 && ilowenergy <15000)
		{
		  bsumN[itele][chanNum]++;
		  bsumx[itele][chanNum] += (double)ilowenergy;
		  bsumxx[itele][chanNum] += pow((double)ilowenergy,2);
		  bsumy[itele][chanNum] += ienergy;
		  bsumyx[itele][chanNum] += ienergy*(double)ilowenergy;
		}
	      
	      
	      
	      if(energy >0.75)
		{
		  Telescope[itele]->Back.Add(chanNum,energy,ilowenergy,ienergy,time);
		}
	    }	  
	}
      
      point = endHINP;

    }

      



  if (*point == 0xe0fe) return false;
  //  cout << "point in si " << hex<< *point << " " << dec << *point << endl;
  //  point++;
  return true;
}


//***************************************************************
hira::~hira()
{

  cout << "start Hira destr" << endl;
  //high-low correlations

  ofstream iFfile("f.dat");
  ofstream iBfile("b.dat");
  
  for (int i=0;i<14;i++)
    {
      for (int j=0;j<32;j++)
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
  for (int i=0;i<14;i++)
    {
      for (int j=0;j<32;j++)
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


  delete calFront;
  delete calBack;
  delete calCsi;
  delete calLBack;
  delete calLFront;
  delete calBDual;
  delete calFrontT;
  delete calBackT;
  delete calFrontTLG;
  delete calBackTLG;


  for(int i=0;i<14;i++)
    {
      delete Telescope[i];
    }
  delete [] Telescope;

  cout << "stop Hira destr" << endl;
}




//**********************************************
void hira::reset()
{
  fred = false;
  for (int i=0;i<14;i++) Telescope[i]->reset();
}
