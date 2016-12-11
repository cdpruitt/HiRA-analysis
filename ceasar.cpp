#include "ceasar.h"



ceasar::ceasar(TRandom *ran0, histo_sort * Histo0)
{
  ran = ran0;
  Histo = Histo0;
  //  Doppler = new doppler(0.362); // beta for 65 Mev/A C-9
  Doppler = new doppler(0.326477); // beta for 54 MeV/A Ne-17
  tdc = new TDC1190*[2];
  tdc[0] = new TDC1190(1,112,115);
  tdc[1] = new TDC1190(1,95,80);

  //make map of chips
  ifstream ifile("ceasar.map");

  if (!ifile.is_open())
    {
      cout << "ceasar map not found" << endl;
      abort();
    }

  string name;
  //  getline(ifile,name);
  int iring,iloc,ibank,ichan;
  for(int ib = 0;ib<12;ib++)
    {
      for(int ich = 0;ich<16;ich++)
	{
	  MapC[ib][ich].iRing = -1;
	  MapC[ib][ich].iLoc = -1;
	}
    }

  for (;;)
    {
      ifile >> iring >> iloc >> ibank >> ichan;
      if (ifile.eof()) break;
      if (ifile.bad()) break;

      ibank -=1;
      ichan-=1;
      iloc-=1;
      MapC[ibank][ichan].iRing = iring;
      MapC[ibank][ichan].iLoc = iloc;
      // cout << iring << " " << iloc << " " << ibank << " " << ichan << endl;
    }
  ifile.close();
  ifile.clear();
  // abort();
  //read in calibrations
  name = "cal/ceasar.cal";
  int Ntele = 1;
  int Nstrip = 192;
  calCeasar = new calibrate(Ntele,Nstrip,name,1);
  name = "cal/ceasar_time.cal";
  calCeasarT = new calibrate(Ntele,Nstrip,name,1);

  // read in detector angles
  ifile.close();
  ifile.clear();
  float x,y,z;

  ifile.open("cal/DetPos.txt");
  for (;;)
    {
      ifile >> iring >>  iloc >>  x >> y >> z;
      if (ifile.eof()) break;
      if (ifile.bad()) break;
      if (iring == 1) continue;
      if (iring > 8) break;
      float r = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
      float theta = acos(z/r);
      float phi = atan2(y,-x);
      angle[iring-2][iloc-1] = theta; 
      angle2[iring-2][iloc-1] = phi; 
      //cout << iring-2 << " " << " " << iloc-1 << " " << phi*180/acos(-1) << endl;

    }
//   for(int i = 0;i<9;i++)
//     {
//       cout << abs(angle2[0][i] - angle2[0][i+2]) << endl;
//     }
//    abort();
}


bool ceasar::unpack(unsigned short * point)
{

  Nxfp = 0;

  /*
  unsigned short * pig = point;
  for (int i=0;i<100;i++)
    {
      cout << dec<< *pig << " " << hex<< *pig << endl;
      pig++;
    }
  cout << endl;
  abort();
  //return true;
  */

  /*
  if (*point != 0xffff)
    {
      cout << "here" << endl;
    }
  */


  NE = 0;
  int Ring = 0;
  int Bank =0;
  int Loc =0;
  int chan = 0;

  for (int iqdc = 0;iqdc<6;iqdc++)
    {

      //check for ffff's
     unsigned short f3 = *point;
     unsigned short f4 = *(point+1);
     if (f3 == 0xffff && f4 == 0xffff) 
       {
	point+=2;
        continue; 
       }


      Caen.number = 0;
      point = Caen.read(point);  // suck out the info in the qdc
      for (int i=0;i<Caen.number;i++)
	{
	  bool overflow = 0.;
	  bool underflow = 0.;


	  if (Caen.underflow[i])
	    {
	      underflow  =1;
	    }
	  if (Caen.overflow[i])
	    {
	      overflow =1;
	    }
          int id = Caen.channel[i] + 32*iqdc;
          int ienergy = Caen.data[i];
	  chan = Caen.channel[i];

	  if(overflow)
	    {
	      cout << "overflow " << id << endl;
	      ienergy = 5000;
	    }
	  if(underflow)
	    {
	      cout << "underflow " << id << endl;
	      ienergy = 5100;
	    }

	  DataEC[NE].id = id;
          DataEC[NE].ienergy = ienergy;
	  if(chan < 16)
	    {
	      Bank = iqdc*2;
	    }
	  else
	    {
	      Bank = iqdc*2+1;
	      chan = chan -16;
	    }
	  
	  // cout << ienergy << " "<< Bank << " " << chan << " ";
	  Ring = MapC[Bank][chan].iRing;
	  Loc = MapC[Bank][chan].iLoc;

          DataEC[NE].iRing = Ring;
          DataEC[NE].iLoc = Loc;
          DataEC[NE].theta = angle[Ring][Loc];
	  DataEC[NE].phi= angle2[Ring][Loc];
        
	  // cout << Ring << " " << Loc << endl;
	  if(Ring == -1 || Loc == -1) continue;
	  Histo->RingSum[Ring]->Fill(Loc,ienergy);

	  float energy = calCeasar->getEnergy(0,id,ienergy+ran->Rndm());
          DataEC[NE].energy = energy;

	  //Histo->RingSum_Cal[Ring]->Fill(Loc,energy);
	  Histo->ECeasar[id]->Fill(ienergy);
	  //	  Histo->ECSum->Fill(id,ienergy);
	  Histo->ECCeasar[id]->Fill(energy);
	  Histo->TECeasar->Fill(energy);
	  DataEC[NE].Total += energy;




	  NE++;
	}


      //check for ffff's
     unsigned short f1 = *point;
     point++;
     unsigned short f2 = *point;
     point++;
     if(f1 != 0xffff && f2 != 0xffff) return false;
    }
  Histo->CEMult->Fill(NE);


  NT = 0;
  for (int itdc = 0;itdc<2;itdc++)
    {




      //check for ffff's
     unsigned short f3 = *point;
     unsigned short f4 = *(point+1);
     if (f3 == 0xffff && f4 == 0xffff)
       {
	 point += 2;
        continue;
       } 
    
      point = tdc[itdc]->read(point);
      for (int i =0;i< tdc[itdc]->Ndata;i++)
	{
          int id = tdc[itdc]->dataOut[i].channel + 112*itdc;
          int itime = tdc[itdc]->dataOut[i].time;
	  if (tdc[itdc]->dataOut[i].channel == 113 && itdc ==0)
	    {

              Txfp[Nxfp] = itime/10.;
              //cout << Txfp << endl;
              Histo->Txfp->Fill(Txfp[Nxfp]);
              Nxfp++;
	      continue;
	    }
	  if (tdc[itdc]->dataOut[i].channel == 114 && itdc ==0)
	    {
	      TRF = itime/10.;
              Histo->TRF->Fill(TRF);
	      continue;
	    }

	  if (tdc[itdc]->dataOut[i].order > 0) continue;

          DataTC[NT].id  = id;
          DataTC[NT].itime = itime;

	  float time = calCeasarT->getEnergy(0,id,itime/10.+ran->Rndm());
	  DataTC[NT].time = time;
	  Histo->TCeasarCal->Fill(time);
	  Histo->TCeasar[id]->Fill(itime/10.);
	  Histo->TCSum->Fill(id,itime/10.);
          Histo->TCtotal->Fill(itime/10.);
          NT++; 
	}

     unsigned short f1 = *point;
     point++;
     unsigned short f2 = *point;
     point++;
     if(f1 != 0xffff && f2 != 0xffff) return false;

    }
      Histo->CTMult->Fill(NT);

  Nselect = 0;
  // match up energies to times
  for (int ie=0;ie<NE;ie++)
    {
      DataEC[ie].itime = -1;
      DataEC[ie].time = -1;
      for (int it=0;it<NT;it++)
	{
          if (DataEC[ie].id == DataTC[it].id ) 	      //we have matched
	    {

	      //cout << DataEC[ie].id << endl;
	      DataEC[ie].itime = DataTC[it].itime;
	      DataEC[ie].time = DataTC[it].time;
              if (DataEC[ie].itime > 3000 && DataEC[ie].itime < 6000 
		  && DataEC[ie].iRing != -1)
		{
  	         Histo->ECMSum->Fill(DataEC[ie].id,DataEC[ie].ienergy);

    	         float dop_energy = Doppler->correct(DataEC[ie].energy,DataEC[ie].theta);
	         Histo->TEC_Dop->Fill(dop_energy);
                 select[Nselect] = DataEC[ie];
                 select[Nselect].dop_energy = dop_energy;
		 Nselect++;

		}

	    }
	  else if (DataEC[ie].id < DataTC[it].id) break; // no match found
	}
    }

  Nadded = 0;
  std::vector<float> sumgamma;
  std::vector<int> myvector;
  for(int i =0;i< Nselect;i++)
    {
      bool addedback =0;
      float sum = 0.;
      sum = select[i].energy;
      if(i != 0)
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
	  for(int j = i+1;j<Nselect;j++)
	    {
	      if(abs(select[i].iRing - select[j].iRing) <= 1)
		{
		  if(abs(select[i].phi - select[j].phi) < 0.5)
		    {
		      sum += select[j].energy;
		      myvector.push_back(j);
		    }
		}
	    }
	  added[Nadded]=select[i];
	  added[Nadded].energy = sum;
	  Nadded++;
	}
    }

  //cout << "Nselect " << Nselect << " Nadded " << Nadded << endl;
  Histo->CETMult->Fill(Nselect);
  return true;

}
//************************************
ceasar::~ceasar()
{
  cout << "start ceasar destr" << endl;

  delete tdc[0];
  delete tdc[1];
  delete [] tdc;
  delete Doppler;
  cout << " stop ceasar destr" << endl;
}
//****************************
void ceasar::reset()
{
  Nselect = 0;
}
