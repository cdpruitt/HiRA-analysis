#include "../include/TCanvas.h"
#include "../include/TFile.h"
#include "../include/TF1.h"
#include "../include/TH2S.h"
#include "../include/TH1S.h"
#include "../include/TGraph.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <sstream>

using namespace std;


double gauss(float x,float mean,float sig)
{
double const pi=3.14159;
 return exp(-pow((x-mean)/sig,2)/2.)/(sqrt(2.*pi)*sig);
}



double ThPeaks(double *x , double *par)
{

  int const Npeak = 7;
  double const intensity[7]={.374,.741,.978,.358,1.06,1.,.593};
  //double const intensity[7]={.53,1.1,1.1,.25,1.,1.,.5};
  //double const intensity[7]={.53,.5,1.1,.25,1.,1.,.5};
  double const energy[7]={5.122,5.207,5.478,5.854,6.093,6.595,8.632};
  //double const energy[7]={5.073,5.159,5.432,5.811,6.050,6.555,8.598};
  //double const energy[7]={5.116,5.204,5.472,5.848,6.088,6.589,8.627};
  //double const energy[7]={5.3403,5.4232,5.6854,6.059,6.2881,6.7783,8.7848};
  double tot = 0.;
  double e = par[0] + par[1]*x[0];
  for (int i=0;i<Npeak;i++)
    {
      tot+= intensity[i]*gauss(e,energy[i],par[2]);
    }
  return tot*par[3];
}

int main()
{

  float const conS =.2/2.35;
  ofstream fout("cal/frontN.cal");
  ofstream fwhm("cal/fwhmFront.dat");
  TFile f("sort.root");
  TCanvas* canvas[14];
  int Ntele = 14;
  int Nstrip = 32;
  ostringstream outstring;
  TH2I frame("frame","",10,4.5,9.5,10,0,130);
  frame.SetStats(kFALSE);

  double xx[14*32];
  double yy[14*32];

  TF1 *func = new TF1("fit",ThPeaks,3,9,4);
  double para[5];
  ifstream file("cal/front.cal");
  float intercept, slope;
  int i1,i2;
  string name;

  TH1F con("con","",500,0,10);
  for (int telescopeNumber=0;telescopeNumber<Ntele;telescopeNumber++)
    {
      outstring.str("");
      outstring << "F"<<telescopeNumber;
      name = outstring.str();
      canvas[telescopeNumber] = new TCanvas(name.c_str());
      canvas[telescopeNumber]->Divide(6,6);
      for (int istrip =0;istrip<Nstrip;istrip++)
        {
      
          canvas[telescopeNumber]->cd(istrip+1);
          file >> i1 >> i2  >> slope >> intercept;


          outstring.str("");
          outstring << "front/cal/EFC"<<telescopeNumber<<"_"<<istrip;
          string name = outstring.str();
          cout <<  name << endl;
          TH1I * hist = (TH1I*) f.Get(name.c_str());
	  frame.Draw();


          hist->SetStats(kFALSE);
          hist->GetXaxis()->SetRangeUser(4.5,9.5);
          con.GetXaxis()->SetRangeUser(4.5,9.5);
	  for (int i=1;i<=500;i++)
	    for (int j=1;j<500;j++)
	    {
              float deltax = hist->GetBinCenter(i)-con.GetBinCenter(j);
	      if (fabs(deltax) > 10.*conS)continue;
              float fact = gauss(deltax,0.,conS);
	      float y = fact*hist->GetBinContent(i)*hist->GetBinWidth(i);
              con.SetBinContent(j,y+con.GetBinContent(j));
	    }



	  for (int i=1;i<=500;i++) 
	    {
	      // hist->SetBinContent(i,con.GetBinContent(i));
	     con.SetBinContent(i,0.);
	    }

          hist->Draw("same");



          func->SetParameter(0,0);
          func->SetParameter(1,1.);
	  //          func->FixParameter(2,conS);
          func->SetParameter(2,0.1);
          func->SetParameter(3,8.);
          func->SetLineColor(2);
          //func->Draw("same");



	  hist->Fit(func);
          func->GetParameters(para);
          cout << "chisq=" << func->GetChisquare() << endl;
           if (fabs(para[1]-1.) < .2) 
	     { 

              slope *= para[1];
              intercept = intercept*para[1] + para[0];
	     }
            fout << telescopeNumber << " " << istrip << " " 
                 << slope << " " << intercept << endl;
            fwhm << telescopeNumber << " " << istrip << " " 
                  << para[2]*2.35 << endl;
            int ii = telescopeNumber*32+istrip;
            xx[ii] = (float)ii;
            yy[ii] = para[2]*2.35;
            cout << para[0] << " " << para[1] << " " << para[2] << endl;

        }
    }

  TFile g("ThFront.root","RECREATE");
  for (int telescopeNumber=0;telescopeNumber<Ntele;telescopeNumber++) canvas[telescopeNumber]->Write();
  TCanvas fwhmCan("fwhm");
  TH2I frame2("frame2","",10,0,448,10,0,0.12);
  frame2.SetStats(kFALSE);
  frame2.GetYaxis()->SetTitle("FWHM [MeV]");
  frame2.GetXaxis()->SetTitle("front strip");
  frame2.Draw();
  TGraph graph(32*14,xx,yy);
  graph.Draw("*");
  graph.Write();
  fwhmCan.Write();
  g.Write();
}
