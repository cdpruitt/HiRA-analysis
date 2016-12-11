#include "histo_read.h"


histo_read::histo_read()
{

  ostringstream outstring;
  string name;

  Ntele = 14;
  Nstrip = 32;
  NCsI = 56;

  //create root file
  file = new TFile ("read.root","RECREATE");
  file->cd();


  //directories for Correl
  dirCorr = new TDirectoryFile("corr","corr");

  dir11O = dirCorr->mkdir("O11","O11");  
  dir12O = dirCorr->mkdir("O12","O12");
  dir6Li = dirCorr->mkdir("Li6","6Li");
  dir7Li = dirCorr->mkdir("Li7","Li7");  
  dir7Be = dirCorr->mkdir("Be7","Be7");  
  dir12C = dirCorr->mkdir("C12","C12");

  //delta E vs E
  dirdEE = new TDirectoryFile("dEE","dEE");

  dEE = new TH2I*[NCsI];

  for(int icsi =0;icsi <NCsI;icsi++)
    {
      outstring.str("");
      outstring << "dEE_" << icsi;
      name = outstring.str();
      dirdEE->cd();

      dEE[icsi] = new TH2I(name.c_str(),"",1024,0,4095,5000,0,500);//4096

    }
  dirdEE->cd();
  HitMap = new TH2I("HitMap","",1200,-30,30,800,-20,20);


  
  Blocker_ETOF = new TH2D("Blocker_ETOF","",200,-500,1000,1024,-0.5,4095.5);

  dir11O->cd();
  Erel_11O_2p9C = new TH1I("Erel11O_2p9C","",200,0,15);
  Erel_11O_2p9C->GetXaxis()->SetTitle("Erel [MeV]");
  vel_11O_2p9C = new TH1I("vel_11O_2p9C","",100,0,14);
  vel_11O_2p9C->GetXaxis()->SetTitle("velocity [cm/ns]");

  JacobiY_11O_2p9C = new TH2I("JacobiY_11O_2p9C","",25,0.,1.,20,-1.,1.);
  JacobiY_11O_2p9C->GetXaxis()->SetTitle("E_{core-p}");
  JacobiY_11O_2p9C->GetYaxis()->SetTitle("#theta_{k}");

  JacobiT_11O_2p9C = new TH2I("JacobiT_11O_2p9C","",25,0.,1.,20,-1.,1.);
  JacobiT_11O_2p9C->GetXaxis()->SetTitle("E_{pp}");
  JacobiT_11O_2p9C->GetYaxis()->SetTitle("#theta_{k}");

  dir12O->cd();
  Erel_12O_2p10C = new TH1I("Erel12O_2p10C","",200,0,15);
  Erel_12O_2p10C->GetXaxis()->SetTitle("Erel [MeV]");
  vel_12O_2p10C = new TH1I("vel_12O_2p10C","",100,0,14);
  vel_12O_2p10C->GetXaxis()->SetTitle("velocity [cm/ns]");

  JacobiY_12O_2p10C = new TH2I("JacobiY_12O_2p10C","",25,0.,1.,20,-1.,1.);
  JacobiY_12O_2p10C->GetXaxis()->SetTitle("E_{core-p}");
  JacobiY_12O_2p10C->GetYaxis()->SetTitle("#theta_{k}");

  JacobiT_12O_2p10C = new TH2I("JacobiT_12O_2p10C","",25,0.,1.,20,-1.,1.);
  JacobiT_12O_2p10C->GetXaxis()->SetTitle("E_{pp}");
  JacobiT_12O_2p10C->GetYaxis()->SetTitle("#theta_{k}");


  //Li7 correlations

  dir7Li->cd();
  Erel_7Li = new TH1I("Erel_7Li","",400,0,8);
  Erel_7Li->GetXaxis()->SetTitle("Erel ^{7}Li");

  Costheta = new TH1I("Costheta","",400,-1,1);
  Costheta->GetXaxis()->SetTitle("Cos(#theta)");

  Erel_Back_Diff = new TH1I("Erel_Backward_2_tele","",400,0,8);
  Erel_Back_Diff->GetXaxis()->SetTitle("Erel ^{7}Li");

  Erel_For_Diff = new TH1I("Erel_Forward_2_tele","",400,0,8);
  Erel_For_Diff->GetXaxis()->SetTitle("Erel ^{7}Li");

  Erel_Trans_Diff = new TH1I("Erel_Transverse_2_tele","",400,0,8);
  Erel_Trans_Diff->GetXaxis()->SetTitle("Erel ^{7}Li");

  Erel_Back_Same = new TH1I("Erel_Backward_1_tele","",400,0,8);
  Erel_Back_Same->GetXaxis()->SetTitle("Erel ^{7}Li");

  Erel_For_Same = new TH1I("Erel_Forward_1_tele","",400,0,8);
  Erel_For_Same->GetXaxis()->SetTitle("Erel ^{7}Li");

  Erel_Trans_Same = new TH1I("Erel_Transverse_1_tele","",400,0,8);
  Erel_Trans_Same->GetXaxis()->SetTitle("Erel ^{7}Li");

  vel_7Li = new TH1I("vel_7Li","",100,0,14);
  vel_7Li->GetXaxis()->SetTitle("velocity [cm/ns]");

  //Li6
  dir6Li->cd();
  Erel_6Li = new TH1I("Erel_6Li","",400,0,8);
  Erel_6Li->GetXaxis()->SetTitle("Erel ^{7}Li");

  
  //Be7
  dir7Be->cd();
  Ex7Be_alpha3He = new TH1I("Ex7Be_p6Li","",800,0,20);
  vel_7Be_alpha3He = new TH1I("vel_7Be_p6Li","",100,0,14);
  vel_7Be_alpha3He_left = new TH1I("vel_7Be_p6Li_left","",100,0,14);
  vel_7Be_alpha3He_right = new TH1I("vel_7Be_p6Li_right","",100,0,14);
  TEC_7Be_alpha3He = new TH1I("TEC_7Be_p6Li","",128,0,10);

  Ex7Be_alpha3He_gamma = new TH1I("Ex7Be_p6Li_gamma","",800,0,20);
  Ex7Be_alpha3He_gammaback1 = new TH1I("Ex7Be_p6Li_gammaback1","",800,0,20);
  Ex7Be_alpha3He_gammaback2 = new TH1I("Ex7Be_p6Li_gammaback2","",800,0,20);
  Ex7Be_alpha3He_gammaback3 = new TH1I("Ex7Be_p6Li_gammaback3","",800,0,20);

  Caesarmult = new TH2I("Caesarmult","",128,0,10,100,0.5,99.5);

  //C12
  dir12C->cd();
  Ex12C_da6Li = new TH1I("Ex12C_da6Li","",200,25,45);
  
}
//*********************************************
void histo_read::write()
{
  file->Write();

  cout << "histo written" << endl;
  /*
    for (int i=0;i<Ntele;i++)
    {
    delete red[i];
    }
    delete [] red;
  */
}
