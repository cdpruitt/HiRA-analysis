#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include <fstream>
#include <sstream>
#include "loss.h"

/******Click on each side of the peaks to set the ranges then fits multiple peaks*******/
/******Will also calculate residual energy and then store that in appropriate file (not working atm)***/
void multifit()
{

  TCanvas mycan;
  //mycan.SetCanvasSize(800,800);
  TFile file("~/root/sort.root");
  mycan.SetCrosshair(1);
  mycan.SetWindowSize(900,700);
  mycan.ToggleEventStatus();

  ostringstream outstring;
  string name;

  int n = 4; // number of calibration points
  int CsIno=15;
  outstring.str("");
  outstring << "cal/rPies";// << CsIno;
  name = outstring.str();
  name = name + ".dat"; //"_d.cal"; // "_a.cal" for alpha files;
  //mass=2.014;

  cout << name << endl;

  ofstream ifile(name.c_str());
  if(!ifile.is_open()){cout << "Could not open " << name << endl; return;}


  float theta[8]= {6.445,7.755,9.0577,10.3506,11.633,12.904,14.1614,15.4055};//Rus blocks
  //float theta[8]={2.05126,2.543,3.0343,3.5252,4.0156,4.5054,4.9945,0}; //S2 blocks

  /*
  name = "Hydrogen_Ta.loss";
  CLoss * d_loss_Ta = new CLoss(name,2.014);
  name = "Hydrogen_Si.loss";
  CLoss * d_loss_Si = new CLoss(name,2.014);
  name = "Hydorgen_Au.loss";
  CLoss * d_loss_Au = new CLoss(name,2.014);
  name = "~Hydrogen_27Al.loss";
  CLoss * d_loss_Al = new CLoss(name,2.014);



  
  name = "Helium_Ta.loss";
  CLoss * a_loss_Ta = new CLoss(name,4.003);
  name = "Helium_Si.loss";
  CLoss * a_loss_Si = new CLoss(name,4.003);
  name = "Helium_Au.loss";
  CLoss * a_loss_Au = new CLoss(name,4.003);
  name = "Helium_27Al.loss";
  CLoss * a_loss_Al = new CLoss(name,4.003);
  */

  cout << "here1" << endl;
  double par[12];

  //total->SetLineColor(2);

  float thickTa;
  float thickSi;
  float thickAu;
  float thickAl1;
  float thickAl2;
  float E1;
  float E2;
  float E3;
  float E4;

  /*
  E1=12*mass;
  E2=24*mass;
  E3=E2;
  E4=E2;

  E1 = d_loss_Au->getEout(E1,10);
  E2 = d_loss_Al->getEout(E2,428.9425);
  E3 = d_loss_Al->getEout(E3,428.9425/2.);
  E4 = d_loss_Au->getEout(E4,10);

  cout << "E1 = " << E1 << endl;
  cout << "E2 = " << E2 << endl;
  cout << "E3 = " << E3 << endl;
  cout << "E4 = " << E4 << endl;
  */

  for(int i=0;i<64;i++)
    {

      thickTa = 13.8/cos(theta[i]);
      if(CsIno<16)thickSi = 121.863/cos(theta[i]);
      if(CsIno>=16)thickSi = 116.524/cos(theta[i]);
      
      //Assumme interaction is after going through half the target, will need to get residual after going straight through half the target

      thickAu = 20./cos(theta[i])/2;
      thickAl1 = 428.9425/cos(theta[i])/2;
      thickAl2 = 428.9425/cos(theta[i]);



  
      outstring.str("");
      outstring << "rus/Rpies/RusPieR_" << i;
      name = outstring.str();
      TH1I * hist = (TH1I*)file.Get(name.c_str());
      hist->GetXaxis()->SetRangeUser(200,1000);//(400,1500);//(500,2600);
      hist->Draw();
      //TPad::setCrosshair(1);
      // hist->setCrosshair(1);
      //TCanvas::Canvas_1_n2->setCrosshair(1);
      mycan.Update();

      TMarker * mark;
      mark=(TMarker*)mycan.WaitPrimitive("TMarker"); 
      int r1lo = mark->GetX();
      delete mark;


      mark=(TMarker*)mycan.WaitPrimitive("TMarker"); 
      int r1hi = mark->GetX();
      delete mark;

 
      mark=(TMarker*)mycan.WaitPrimitive("TMarker"); 
      int r2lo = mark->GetX();
      delete mark;

 
      mark=(TMarker*)mycan.WaitPrimitive("TMarker"); 
      int r2hi = mark->GetX();
      delete mark;

 
      mark=(TMarker*)mycan.WaitPrimitive("TMarker"); 
      int r3lo = mark->GetX();
      delete mark;


      mark=(TMarker*)mycan.WaitPrimitive("TMarker"); 
      int r3hi = mark->GetX();
      delete mark;

      
      mark=(TMarker*)mycan.WaitPrimitive("TMarker"); 
      int r4lo = mark->GetX();
      delete mark;

  
      mark=(TMarker*)mycan.WaitPrimitive("TMarker"); 
      int r4hi = mark->GetX();
      delete mark;
      

      TF1 *g1    = new TF1("g1","gaus",r1lo,r1hi);
      TF1 *g2    = new TF1("g2","gaus",r2lo,r2hi);
      TF1 *g3    = new TF1("g3","gaus",r3lo,r3hi);
      TF1 *g4    = new TF1("g4","gaus",r4lo,r4hi);
      //TF1 *total = new TF1("total","gaus(0)+gaus(3)+gaus(6)+gaus(9)",r1lo,r4hi);
      

      hist->Fit(g1,"R");
      hist->Fit(g2,"R+");
      hist->Fit(g3,"R+");
      hist->Fit(g4,"R+");
      g1->GetParameters(&par[0]);
      g2->GetParameters(&par[3]);
      g3->GetParameters(&par[6]);
      g4->GetParameters(&par[9]);

      for(int p = 0;p<n;p++)
	{
	  ifile << par[1+3*p] << endl;
	}


      // cout << par[10] << endl;
      //total->SetParameters(par);
      //hist->Fit(total,"R+");
      delete g1;
      delete g2;
      delete g3;
      delete g4;
      //delete total;
      delete hist;

    }
}
