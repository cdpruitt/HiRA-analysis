
	 for(;;)  // loop over items in a evtfile
	   {
            int const hBufferWords = 4;
            int const hBufferBytes = hBufferWords*2;
            unsigned short hBuffer[hBufferWords];
   	    evtfile.read((char*)hBuffer,hBufferBytes);

	    if(evtfile.eof())
	      {
                cout << "eof found" << endl;
		fileProblem = true;
                break;
	      }
	    if(evtfile.bad())
	      {
                cout << " bad found" << endl;
                fileProblem = true;
                break;
	      }

	    point = hBuffer;
            int nbytes = *point++;
            int nbytes2 = *point++;
            int type = *point++;
            int type2 = *point;

            //cout << nbytes << " " << nbytes2 << " " << type << " " <<type2 << endl; 

            int dBufferBytes = nbytes - 8;
            int dBufferWords = dBufferBytes/2;

  	    //cout << "bytes= " << dBufferBytes << ", words " << dBufferWords << endl;
            unsigned short dBuffer[dBufferWords];
            evtfile.read((char*)dBuffer,dBufferBytes);
            point = dBuffer;

	    if (type == 1)
	      {

               runno = *point;
               cout << "run number = " << runno << endl; 
	      }
            else if (type == 30)
	      {
		physicsEvent++;
                if (physicsEvent%30000 == 0) 
                     cout << '\xd'<< physicsEvent << flush;

		//cout << "events= " << physicsEvent << endl;
		
			
		/*
		if(physicsEvent == 155)
		  {

		    cout << "here" << endl;
		    
		      unsigned short * pig = point;
		      for (int i=0;i<200;i++)
		      {
		      cout << dec<< *pig << " " << hex<< *pig << endl;
		      pig++;
		      }
		      abort();
		  }
		    */
		
		if(physicsEvent ==355)
		  {
		       cout << "Bad Event found" << endl;
		  }
		
		unsigned short * badpig = point;
                //cout << "in unpack " << endl;
		bool stat = Det.unpack(point,runno);


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

bool hira::unpack(unsigned short *&point,int runno)
{
  bool stat = true;
  //  stat = unpackSi_sis(point);
   stat = unpackSi_adc(point);
    if (!stat) return stat;
    stat = unpackCsi(point,runno); 
}
bool hira::unpackSi_adc(unsigned short *&point)
{
  
  /*
  unsigned short * pig = point;
  for (int i=0;i<800;i++)
    {
      cout << dec<< *pig << " " << hex<< *pig << endl;
      pig++;
    }
  abort();
  */




  unsigned short marker;
  unsigned short * nextMB = point;
  for (int iMB=0;iMB<3;iMB++)
    {
      point = nextMB;
      //    cout << iMB << " " << hex <<*point;
      marker = *point++;
      //if(marker !=xmarker[iMB])point++;
      if (marker != xmarker[iMB]) return false;
      
     int NstripsRead = 0;
     unsigned short chipWords = *point;
     nextMB = point + chipWords + 2;
     //if (chipWords > 400) return false;  // please fix Kyle
     if (chipWords == 0)
        {
         NstripsRead = 0;
         return (bool) 1;
        }
     point += 2;
     NstripsRead = *point;

     if (NstripsRead > 384) return false; // bad buffer
     point += 5;

     //  cout << chipWords << " " << NstripsRead*3+7 <<  " " << NstripsRead <<  endl;
     for (int istrip = 0;istrip < NstripsRead;istrip++)
        {

          unsigned short id = *point++;
          unsigned short chipNum = (id&0x1FE0)>>5;
          unsigned short chanNum = id & 0x1F;
          unsigned short ienergy = *point++;
          unsigned short itime =  *point++;

  point = nextMB;
  if (*point == 0xe0fe) return false;
  //  cout << "point in si " << hex<< *point << " " << dec << *point << endl;
  return true;
}
