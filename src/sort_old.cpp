// C++ file to read MSU event files 
//file numbers.beam contains runs to sort
//uses class hira to unpack hira data
//write out spectra in file sort.root 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../include/det.h"
#include <ctime>
#include "../include/histo_sort.h"
#include "../include/histo_read.h"
#include "../include/forest.h"
using namespace std;



int main(int argc, char* argv[])
{

  histo_sort * Histo_sort = new histo_sort();
  //histo_read * Histo_read = new histo_read();
  forest * Forest = new forest();
  unsigned short *point,*epoint;
  int unsigned words;
  int unsigned type;
  
  int physicsEvent = 0;
  int physicsEventGood = 0;
  int physicsEventCounter = 0;
  int scalerBuffer = 0;
  int Npauses = 0;
  int Nresumes = 0;
  int runno = 0;
  
  
  
  det Det(Histo_sort, Forest);
  
  
  
  //open file with run numbers to sort
  ifstream runFile;
  runFile.open("numbers.beam");
  //check if this file exists
  if (runFile.is_open() == 0 )
    {
      cout << " could open runfile " << endl;
      return 1;
    }
  
  
  ostringstream outstring;
  int number;
  ifstream evtfile;
  FILE *pFile;
  bool fileProblem = false;
  bool endOfRun = false;
  bool first = true; 
  int argcounter = 1;
  for (;;)  // loop over run numbers
    {
      
      if (evtfile.is_open()) 
        cout << "problem previous file not closed" << endl;
      if (argc == 1)
	{
	  runFile >> number;
	  
	  //check to see if at end of file
	  if (runFile.eof())break;
	  if (runFile.bad())break; 
	}
      else if (argc > 1)
	{
	  if (argcounter == argc) 
	    {
	      break;
	    }
	  else
	    {
	      number = atoi(argv[argcounter]);
	      argcounter++;
	    }
	}
      Forest->newTree(number);
      for (int iExtra=0;iExtra<3;iExtra++) //loop over split evtfiles
	{
	  
	  //the following loop accounts for files that were split
	  endOfRun=false;
	  fileProblem = 0;
	  outstring.str("");
	  
	  
	  // if (number < 10)
	  //   outstring << "/data1/Ne17_may2013/run-000"<<number;
	  // else if(number >= 10 && number <100)
	  //   outstring << "/data1/Ne17_may2013/run-00"<<number;
	  // else 
	  //   outstring << "/data1/Ne17_may2013/run-0"<<number;

	  if (number < 10)
	    outstring << "/events/e14002/complete/run-000"<<number;
	  else if(number >= 10 && number <100)
	    outstring << "/events/e14002/complete/run-00"<<number;
	  else 
	    outstring << "/events/e14002/complete/run-0"<<number;
	  
	  if (iExtra == 0)
	    outstring<<"-00.evt";
	  else 
	    outstring<<"_"<<iExtra<<"-13328.evt";
	  
	  string name = outstring.str();
	  
	  //open new tree for current run number
	     
	  
	  //open evt file
	  
	  
	  evtfile.clear();
	  evtfile.open(name.c_str(),ios::binary);
	  
	  
	  //check to see if there are extra files     
	  if (iExtra>0 && !fileProblem && !evtfile) 
	    {
	      break;
	    }
	  
	  cout << '\n'<<name << endl;
	  
	  if (evtfile.bad()) cout << "bad" << endl;
	  if (evtfile.fail()) cout << "fail" << endl;
	  

	  
	  if (!evtfile)
	    {
	      cout << "could not open event file" << endl;
	      return 1;
	    }
	  
	  
	  
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
		      //	       cout << "Bad Event found" << endl;
		    }
		  
		  unsigned short * badpig = point;
		  //cout << "in unpack " << endl;
		  bool stat = Det.unpack(point,runno);
		  //cout << "out unpack " << endl;
		  
		  if (stat) 
		    {
		      physicsEventGood++;
		      //Det.analyze(physicsEvent);
		      Det.treeGrow();
		    }
		  /*		
		    else 
		    {
		    cout <<"Event " << physicsEvent << endl;
		    for (int i=0;i<200;i++)
		    {
		    cout << dec<< *badpig << " " << hex<< *badpig << endl;
		    badpig++;
		    }
		    abort();
		    
		    }
		  */
		}
	      else if (type == 31)
		{
		  physicsEventCounter++;
		}
	      else if (type == 2)
		{
		  endOfRun = true;
		  break;
		}
	      else if (type == 20)
		{
		  scalerBuffer++;
		}
	      else if (type == 3) Npauses++;
	      else if (type == 4) Nresumes++;
	      
	      
	    } //loop over items in a evtfile
	  evtfile.close();
	  evtfile.clear(); // clear event status in case we had a bad file
	  
	} //end loop over file subsections
      Forest->writeTree();
    } //end loop of run file numbers
  
  
  cout << '\n'<<"physics Events = " << physicsEvent << endl;
  cout << "Good physics Events = " << physicsEventGood << endl;
  
  if (physicsEvent > 0)cout << "bad/total = " << 
    (1.-(double)physicsEventGood/(double)physicsEvent)*100. << 
    " %"<< endl;
  
  cout << "physics Event Counters = " << physicsEventCounter << endl;
  cout << "scaler buffers = " << scalerBuffer << endl;
  cout << "Numbers of pauses = " << Npauses << endl;
  cout << "Number of resumes = " << Nresumes << endl;
  //         cout << "number of 2p+6Li = " << Det.N2p6Li << endl;
  //cout << "number of 8B(IAS) = " << Det.N_IAS << endl;
   
  Histo_sort->write(); // this forces the histrograms to be read out to file
	
}
