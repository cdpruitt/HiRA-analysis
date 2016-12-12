#include "../include/histo_read.h"


histo_read::histo_read()
{

  ostringstream outstring;
  string name;

  Ntele = 14;
  Nstrip = 32;
  Nceasar = 192;
  NCsI = 56;
  Nring = 7;

  //create root file
  file = new TFile ("read.root","RECREATE");
  file->cd();

  //directories for Correl
  dirCorr = new TDirectoryFile("corr","corr");
  dir13O = dirCorr->mkdir("O13","O13");
  dir14O = dirCorr->mkdir("O14","O14");
  dir15O = dirCorr->mkdir("O15","O15");
  dir16O = dirCorr->mkdir("O16","O16");
  dir15F = dirCorr->mkdir("F15","F15");
  dir16F = dirCorr->mkdir("F16","F16");
  dir17F = dirCorr->mkdir("F17","F17");
  dir18F = dirCorr->mkdir("F18","F18");
  dir15Ne = dirCorr->mkdir("Ne15","Ne15");  
  dir16Ne = dirCorr->mkdir("Ne16","Ne16");  
  dir17Ne = dirCorr->mkdir("Ne17","Ne17");  
  dir17Na = dirCorr->mkdir("Na17","Na17");  
  dir18Na = dirCorr->mkdir("Na18","Na18");  
  dir18Ne = dirCorr->mkdir("Ne18","Ne18");  
  dir19Ne = dirCorr->mkdir("Ne19","Ne19");  
  dir6Li = dirCorr->mkdir("Li6","Li6");  
  dir5Be = dirCorr->mkdir("Be5","Be5");  
  dir7Be = dirCorr->mkdir("Be7","Be7");  
  dir9Be = dirCorr->mkdir("Be9","Be9");  
  dir12N = dirCorr->mkdir("N12","N12");
  dir13N = dirCorr->mkdir("N13","N13");
  dir14N = dirCorr->mkdir("N14","N14");
  dir15N = dirCorr->mkdir("N15","N15");
  dir11C = dirCorr->mkdir("C11","C11");
  dir12C = dirCorr->mkdir("C12","C12");
  dir13C = dirCorr->mkdir("C13","C13");

  
 
  
  Blocker_ETOF = new TH2D("Blocker_ETOF","",200,-500,1000,1024,-0.5,4095.5);



  ETOF_Ceasar = new TH2I("ETOF_Ceasar","",250,0,1000,1024,-0.5,9.5);
  CeasarT_2p14N = new TH1I("CeasarT_2p14N","",250,0,1000);
  CeasarT_2p14N_IAS = new TH1I("CeasarT_2p14N_IAS","",250,0,1000);
   
 
  dir15Ne->cd();
  Erel_15Ne_2p13O = new TH1I("Erel15Ne_2p13O","",200,0,15);
  Erel_15Ne_2p13O->GetXaxis()->SetTitle("Erel [MeV]");
  vel_15Ne_2p13O = new TH1I("vel_15Ne_2p13O","",100,0,14);
  vel_15Ne_2p13O->GetXaxis()->SetTitle("velocity [cm/ns]");
  Erel15Ne_3p12N = new TH1I("Erel15Ne_3p12N","",200,-2,20);

  JacobiY_15Ne_2p13O = new TH2I("JacobiY_15Ne_2p13O","",25,0.,1.,20,-1.,1.);
  JacobiY_15Ne_2p13O->GetXaxis()->SetTitle("E_{core-p}");
  JacobiY_15Ne_2p13O->GetYaxis()->SetTitle("#theta_{k}");

  JacobiT_15Ne_2p13O = new TH2I("JacobiT_15Ne_2p13O","",25,0.,1.,20,-1.,1.);
  JacobiT_15Ne_2p13O->GetXaxis()->SetTitle("E_{pp}");
  JacobiT_15Ne_2p13O->GetYaxis()->SetTitle("#theta_{k}");


  Erel15Ne_2pfake13O = new TH1I("Erel15Ne_2pfake13O","",200,0,15);
  JacobiY_15Ne_2pfake13O = new TH2I("JacobiY_15Ne_2pfake13O","",25,0.,1.,20,-1.,1.);
  JacobiY_15Ne_2pfake13O->GetXaxis()->SetTitle("E_{core-p}");
  JacobiY_15Ne_2pfake13O->GetYaxis()->SetTitle("#theta_{k}");
  JacobiT_15Ne_2pfake13O = new TH2I("JacobiT_15Ne_2pfake13O","",25,0.,1.,20,-1.,1.);
  JacobiT_15Ne_2pfake13O->GetXaxis()->SetTitle("E_{pp}");
  JacobiT_15Ne_2pfake13O->GetYaxis()->SetTitle("#theta_{k}");

  Erel15Ne_2pfake13O2 = new TH1I("Erel15Ne_2pfake13O2","",200,0,15);
  JacobiY_15Ne_2pfake13O2 = new TH2I("JacobiY_15Ne_2pfake13O2","",25,0.,1.,20,-1.,1.);
  JacobiY_15Ne_2pfake13O2->GetXaxis()->SetTitle("E_{core-p}");
  JacobiY_15Ne_2pfake13O2->GetYaxis()->SetTitle("#theta_{k}");
  JacobiT_15Ne_2pfake13O2 = new TH2I("JacobiT_15Ne_2pfake13O2","",25,0.,1.,20,-1.,1.);
  JacobiT_15Ne_2pfake13O2->GetXaxis()->SetTitle("E_{pp}");
  JacobiT_15Ne_2pfake13O2->GetYaxis()->SetTitle("#theta_{k}");



  dir16Ne->cd();
  ErelEkp_16Ne2p14O = new TH2I("ErelEkp_16Ne2p14O","",100,0,100,100,0,5);

  ErelExT_16Ne2p14O = new TH2I("ErelExT_16Ne2p14O","",100,0,1,800,0,17);
  ExExT_16Ne2p14O = new TH2I("ExExT_16Ne2p14O","",100,0,1,800,-2,15);
  ErelExT_16Ne2pfake14O = new TH2I("ErelExT_16Ne2pfake14O","",100,0,1,800,0,17);
  ExExT_16Ne2pfake14O = new TH2I("ExExT_16Ne2pfake14O","",100,0,1,800,-2,15);

  ErelDiAngle_16Ne2p14O = new TH2I("ErelDiANgle_16Ne2p14O","",50,0,1,100,0,5);
  Ex16Ne_2p14O = new TH1I("Ex16Ne_2p14O","",800,-2,15);
  Ex16Ne_2p14O->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_m2 = new TH1I("Ex16Ne_2p14O_m2","",800,-2,15);
  Ex16Ne_2p14O_m2->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_m3 = new TH1I("Ex16Ne_2p14O_m3","",800,-2,15);
  Ex16Ne_2p14O_m3->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_25 = new TH1I("Ex16Ne_2p14O_25","",800,-2,15);
  Ex16Ne_2p14O_25->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_26 = new TH1I("Ex16Ne_2p14O_26","",800,-2,15);
  Ex16Ne_2p14O_26->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_28 = new TH1I("Ex16Ne_2p14O_28","",800,-2,15);
  Ex16Ne_2p14O_28->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_31 = new TH1I("Ex16Ne_2p14O_31","",800,-2,15);
  Ex16Ne_2p14O_31->GetXaxis()->SetTitle("Ex [MeV]");


  ET16Ne_2p14O = new TH1I("ET16Ne_2p14O","",800,0,17);
  ET16Ne_2p14O->GetXaxis()->SetTitle("E_{T} [MeV]");
  vel_16Ne_2p14O = new TH1I("vel_16Ne_2p14O","",100,0,14);
  vel_16Ne_2p14O->GetXaxis()->SetTitle("velocity [cm/ns]");

  ET16Ne_2pfake14O = new TH1I("ET16Ne_2pfake14O","",800,0,17);
  ET16Ne_2pfake14O->GetXaxis()->SetTitle("^{\"16\"}Ne E_{T} [MeV]");
  Ex16Ne_2pfake14O = new TH1I("Ex16Ne_2pfake14O","",800,-2,15);
  Ex16Ne_2pfake14O->GetXaxis()->SetTitle("^{\"16\"}Ne E_{x} [MeV]");

  Ex16Ne_2pfake14O_25 = new TH1I("Ex16Ne_2pfake14O_25","",800,-2,15);
  Ex16Ne_2pfake14O_25->GetXaxis()->SetTitle("^{\"16\"}Ne E_{x} [MeV]");
  Ex16Ne_2pfake14O_26 = new TH1I("Ex16Ne_2pfake14O_26","",800,-2,15);
  Ex16Ne_2pfake14O_26->GetXaxis()->SetTitle("^{\"16\"}Ne E_{x} [MeV]");
  Ex16Ne_2pfake14O_28 = new TH1I("Ex16Ne_2pfake14O_28","",800,-2,15);
  Ex16Ne_2pfake14O_28->GetXaxis()->SetTitle("^{\"16\"}Ne E_{x} [MeV]");
  Ex16Ne_2pfake14O_31 = new TH1I("Ex16Ne_2pfake14O_31","",800,-2,15);
  Ex16Ne_2pfake14O_31->GetXaxis()->SetTitle("^{\"16\"}Ne E_{x} [MeV]");


  ETvTEC_16Ne2p14O = new TH2I("ETvTEC_16Ne2p14O","",400,0,17,150,0,10);
  ETvTEC_16Ne2p14O->GetXaxis()->SetTitle("ET [MeV]");
  ETvTEC_16Ne2p14O->GetYaxis()->SetTitle("E_{#gamma} [MeV]");

  Jacobi_x_T_16Ne_gs = new TH1I("Jacobi_x_T_16Ne_gs","",25,0.,1.);
  Jacobi_x_T_16Ne_gs->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_x_Y_16Ne_gs = new TH1I("Jacobi_x_Y_16Ne_gs","",25,0.,1.);
  Jacobi_x_Y_16Ne_gs->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_x_T_16Ne_1st = new TH1I("Jacobi_x_T_16Ne_1st","",25,0.,1.);
  Jacobi_x_T_16Ne_1st->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_x_Y_16Ne_1st = new TH1I("Jacobi_x_Y_16Ne_1st","",25,0.,1.);
  Jacobi_x_Y_16Ne_1st->GetXaxis()->SetTitle("Ex/Et");

  Jacobi_T_16Ne_gs = new TH2I("Jacobi_T_16Ne_gs","",25,0.,1.,20,-1.,1.);
  Jacobi_T_16Ne_gs->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_16Ne_gs->GetYaxis()->SetTitle("Cos(#theta_k)");
  Jacobi_Y_16Ne_gs = new TH2I("Jacobi_Y_16Ne_gs","",25,0.,1.,20,-1.,1.);
  Jacobi_Y_16Ne_gs->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_gs->GetYaxis()->SetTitle("Cos(#theta_k)");

  Jacobi_T_16Ne_back = new TH2I("Jacobi_T_16Ne_back","",25,0.,1.,20,-1.,1.);
  Jacobi_T_16Ne_back->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_16Ne_back->GetYaxis()->SetTitle("Cos(#theta_k)");
  Jacobi_Y_16Ne_back = new TH2I("Jacobi_Y_16Ne_back","",25,0.,1.,20,-1.,1.);
  Jacobi_Y_16Ne_back->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_back->GetYaxis()->SetTitle("Cos(#theta_k)");

  Jacobi_T_16Ne_1st = new TH2I("Jacobi_T_16Ne_1st","",25,0.,1.,20,-1.,1.);
  Jacobi_T_16Ne_1st->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_16Ne_1st->GetYaxis()->SetTitle("Cos(#theta_k)");
  Jacobi_Y_16Ne_1st = new TH2I("Jacobi_Y_16Ne_1st","",25,0.,1.,20,-1.,1.);
  Jacobi_Y_16Ne_1st->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_1st->GetYaxis()->SetTitle("Cos(#theta_k)");
    
  Jacobi_Y_16Ne_1st_Back1 = new TH1I("Jacobi_Y_16Ne_1st_Back1","",25,0.,1.);
  Jacobi_Y_16Ne_1st_Back1->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_1st_Back2 = new TH1I("Jacobi_Y_16Ne_1st_Back2","",25,0.,1.);
  Jacobi_Y_16Ne_1st_Back2->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_16Ne_1st_Back1 = new TH1I("Jacobi_T_16Ne_1st_Back1","",25,0.,1.);
  Jacobi_T_16Ne_1st_Back1->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_16Ne_1st_Back2 = new TH1I("Jacobi_T_16Ne_1st_Back2","",25,0.,1.);
  Jacobi_T_16Ne_1st_Back2->GetXaxis()->SetTitle("Ex/Et");

  Jacobi_T_16Ne_1st_Left = new TH1I("Jacobi_T_16Ne_1st_Left","",25,0.,1.);
  Jacobi_T_16Ne_1st_Left->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_1st_Left = new TH1I("Jacobi_Y_16Ne_1st_Left","",25,0.,1.);
  Jacobi_Y_16Ne_1st_Left->GetXaxis()->SetTitle("Ex/Et");  
  Jacobi_T_16Ne_1st_Right = new TH1I("Jacobi_T_16Ne_1st_Right","",25,0.,1.);
  Jacobi_T_16Ne_1st_Right->GetXaxis()->SetTitle("Ex/Et");  
  Jacobi_Y_16Ne_1st_Right = new TH1I("Jacobi_Y_16Ne_1st_Right","",25,0.,1.);
  Jacobi_Y_16Ne_1st_Right->GetXaxis()->SetTitle("Ex/Et");  
  
  Jacobi_T_16Ne_45 = new TH2I("Jacobi_T_16Ne_45","",25,0.,1.,20,-1.,1.);
  Jacobi_T_16Ne_45->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_16Ne_45->GetYaxis()->SetTitle("Cos(#theta_k)");
  Jacobi_Y_16Ne_45 = new TH2I("Jacobi_Y_16Ne_45","",25,0.,1.,20,-1.,1.);
  Jacobi_Y_16Ne_45->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_45->GetYaxis()->SetTitle("Cos(#theta_k)");

  Jacobi_T_16Ne_56 = new TH2I("Jacobi_T_16Ne_56","",25,0.,1.,20,-1.,1.);
  Jacobi_T_16Ne_56->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_16Ne_56->GetYaxis()->SetTitle("Cos(#theta_k)");
  Jacobi_Y_16Ne_56 = new TH2I("Jacobi_Y_16Ne_56","",25,0.,1.,20,-1.,1.);
  Jacobi_Y_16Ne_56->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_56->GetYaxis()->SetTitle("Cos(#theta_k)");

  Jacobi_Y_16Ne_69 = new TH2I("Jacobi_Y_16Ne_69","",25,0.,1.,20,-1.,1.);
  Jacobi_Y_16Ne_69->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_69->GetYaxis()->SetTitle("Cos(#theta_k)");

  Ne16_gs_VelCM = new TH1I("Ne16_gs_VelCM","",100,0,14);
  Ne16_gs_VelCM->GetXaxis()->SetTitle("Velocity CM [cm/ns]");
  Ne16_gs_ThetaCM = new TH1I("Ne16_gs_ThetaCM","",200,0,10);
  Ne16_gs_ThetaCM->GetXaxis()->SetTitle("#Theta_{CM} [degrees]");

  Erel_thetaCM_Ne16 = new TH2I("Erel_thetaCM_Ne16","",200,0,10,400,0,17);


  Ne16_1st_VelCM = new TH1I("Ne16_1st_VelCM","",100,0,14);
  Ne16_1st_VelCM->GetXaxis()->SetTitle("Velocity CM [cm/ns]");
  Ne16_1st_ThetaCM = new TH1I("Ne16_1st_ThetaCM","",200,0,10);
  Ne16_1st_ThetaCM->GetXaxis()->SetTitle("#Theta_{CM} [degrees]");

  Ppara16Negs = new TH1I ("Ppara16Negs","",500,0,6500);
  Ppara16Ne1st = new TH1I ("Ppara16Ne1st","",500,0,6500);
  Ppara16Ne2nd = new TH1I ("Ppara16Ne2nd","",500,0,6500);
  Ppara16Ne3rd = new TH1I ("Ppara16Ne3rd","",500,0,6500);

  Ex16Ne_4p12C = new TH1I("Ex16Ne_4p12C","",400,0,40);
  vel_16Ne_4p12C = new TH1I("vel_16Ne_4p12C","",100,0,14);
  Ex16Ne_4p3a = new TH1I("Ex16Ne_4p3a","",300,10,50);
  vel_16Ne_4p3a = new TH1I("vel_16Ne_4p3a","",100,0,14);

  ExvET_16Ne2p14O = new TH2I("ExvET_16Ne2p14O","",400,0,17,400,0,15);
  ExvET_16Ne2p14O->GetXaxis()->SetTitle("E_{T}");
  ExvET_16Ne2p14O->GetXaxis()->SetTitle("Ex");

  AngvET_16Ne2p14O = new TH2I("AngvET_16Ne2p14O","",400,0,17,20,-1,1);
  AngvET_16Ne2p14O->GetXaxis()->SetTitle("E_{T}");
  AngvET_16Ne2p14O->GetXaxis()->SetTitle("Cos(#theta_k)");

  Ex16Ne_2p14O_For = new TH1I("Ex16Ne_2p14O_For","",800,-2,15);
  Ex16Ne_2p14O_For->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_Trans = new TH1I("Ex16Ne_2p14O_Trans","",800,-2,15);
  Ex16Ne_2p14O_Trans->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_Back = new TH1I("Ex16Ne_2p14O_Back","",800,-2,15);
  Ex16Ne_2p14O_Back->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_Long = new TH1I("Ex16Ne_2p14O_Long","",800,-2,15);
  Ex16Ne_2p14O_Long->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2p14O_ForBack = new TH1I("Ex16Ne_2p14O_ForBack","",800,-2,15);
  Ex16Ne_2p14O_ForBack->GetXaxis()->SetTitle("Ex [MeV]");


  Ex16Ne_2pfake14O_For = new TH1I("Ex16Ne_2pfake14O_For","",800,-2,15);
  Ex16Ne_2pfake14O_For->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2pfake14O_Trans = new TH1I("Ex16Ne_2pfake14O_Trans","",800,-2,15);
  Ex16Ne_2pfake14O_Trans->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2pfake14O_Back = new TH1I("Ex16Ne_2pfake14O_Back","",800,-2,15);
  Ex16Ne_2pfake14O_Back->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16Ne_2pfake14O_Long = new TH1I("Ex16Ne_2pfake14O_Long","",800,-2,15);
  Ex16Ne_2pfake14O_Long->GetXaxis()->SetTitle("Ex [MeV]");

  Jacobi_Y_16Ne_fake_1st = new TH2I("Jacobi_Y_16Ne_fake_1st","",25,0.,1.,20,-1.,1.);
  Jacobi_Y_16Ne_fake_1st->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_fake_1st_Back1 = new TH2I("Jacobi_Y_16Ne_fake_1st_Back1","",25,0.,1.,20,-1.,1.);
  Jacobi_Y_16Ne_fake_1st_Back1->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_16Ne_fake_1st_Back2 = new TH2I("Jacobi_Y_16Ne_fake_1st_Back2","",25,0.,1.,20,-1.,1.);
  Jacobi_Y_16Ne_fake_1st_Back2->GetXaxis()->SetTitle("Ex/Et");

  Jacobi_T_16Ne_fake_1st = new TH2I("Jacobi_T_16Ne_fake_1st","",25,0.,1.,20,-1.,1.);
  Jacobi_T_16Ne_fake_1st->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_16Ne_fake_1st_Back1 = new TH2I("Jacobi_T_16Ne_fake_1st_Back1","",25,0.,1.,20,-1.,1.);
  Jacobi_T_16Ne_fake_1st_Back1->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_16Ne_fake_1st_Back2 = new TH2I("Jacobi_T_16Ne_fake_1st_Back2","",25,0.,1.,20,-1.,1.);
  Jacobi_T_16Ne_fake_1st_Back2->GetXaxis()->SetTitle("Ex/Et");


  Ex16Ne_3p13N = new TH1I("Ex16Ne_3p13N","",200,0,20);
  Et16Ne_3p13N = new TH1I("Et16Ne_3p13N","",200,0,20);
  Ex16Ne_p3He12N = new TH1I("Ex16Ne_p3He12N","",200,15,35);

  Ex14O_16Ne_3p13N = new TH2I("Ex14O_16Ne_3p13N","",800,0,20,200,0,20);
  Ex15F_16Ne_3p13N = new TH2I("Ex15F_16Ne_3p13N","",800,0,20,200,0,20);

  Ex16Ne_3pfake13N = new TH1I("Ex16Ne_3pfake13N","",200,0,20);
  Et16Ne_3pfake13N = new TH1I("Et16Ne_3pfake13N","",200,0,20);


  dir17Ne->cd(); 
  ExExT_17Ne2p15O = new TH2I("ExExT_17Ne2p15O","",100,0,1,800,-2,15);

  Ex17Ne_2p15O = new TH1I("Ex17Ne_2p15O","",800,-2,15);
  Ex17Ne_2p15O->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_m2 = new TH1I("Ex17Ne_2p15O_m2","",800,-2,15);
  Ex17Ne_2p15O_m2->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_m3 = new TH1I("Ex17Ne_2p15O_m3","",800,-2,15);
  Ex17Ne_2p15O_m3->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_pd14O = new TH1I("Ex17Ne_pd14O","",800,0,30);
  Ex17Ne_pd14O->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_25 = new TH1I("Ex17Ne_2p15O_25","",800,-2,15);
  Ex17Ne_2p15O_25->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_26 = new TH1I("Ex17Ne_2p15O_26","",800,-2,15);
  Ex17Ne_2p15O_26->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_28 = new TH1I("Ex17Ne_2p15O_28","",800,-2,15);
  Ex17Ne_2p15O_28->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_31 = new TH1I("Ex17Ne_2p15O_31","",800,-2,15);
  Ex17Ne_2p15O_31->GetXaxis()->SetTitle("Ex [MeV]");



  Ex17Ne_vs_fake16Ne = new TH2I("Ex17Ne_vs_fake16Ne","",800,-2,15,800,-2,15);
  Ex17Ne_vs_fake16Ne_25 = new TH2I("Ex17Ne_vs_fake16Ne_25","",800,-2,15,800,-2,15);
  Ex17Ne_vs_fake16Ne_26 = new TH2I("Ex17Ne_vs_fake16Ne_26","",800,-2,15,800,-2,15);
  Ex17Ne_vs_fake16Ne_28 = new TH2I("Ex17Ne_vs_fake16Ne_28","",800,-2,15,800,-2,15);
  Ex17Ne_vs_fake16Ne_31 = new TH2I("Ex17Ne_vs_fake16Ne_31","",800,-2,15,800,-2,15);

  vel_17Ne_2p15O = new TH1I("vel_17Ne_2p15O","",100,0,14);
  vel_17Ne_2p15O->GetXaxis()->SetTitle("velocity [cm/ns]");

  vel17Ne_pd14O = new TH1I("vel17Ne_pd14O","",100,0,14);
  vel17Ne_pd14O->GetXaxis()->SetTitle("velocity [cm/ns]");

  ExTarget_17Ne_2p15O = new TH1I("ExTarget_17Ne_2p15O","",100,-50,180);
  ExTarget_17Ne_2p15O_2nd = new TH1I("ExTarget_17Ne_2p15O_2nd","",100,-50,180);

  Jacobi_x_T_17Ne_2nd = new TH1I("Jacobi_x_T_17Ne_2nd","",50,0.,1.);
  Jacobi_x_T_17Ne_2nd->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_x_Y_17Ne_2nd = new TH1I("Jacobi_x_Y_17Ne_2nd","",50,0.,1.);
  Jacobi_x_Y_17Ne_2nd->GetXaxis()->SetTitle("Ex/Et");

  Jacobi_T_17Ne_2nd = new TH2I("Jacobi_T_17Ne_2nd","",50,0.,1.,40,-1.,1.);
  Jacobi_T_17Ne_2nd->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_17Ne_2nd->GetYaxis()->SetTitle("Cos(#theta_k)");
  Jacobi_Y_17Ne_2nd = new TH2I("Jacobi_Y_17Ne_2nd","",50,0.,1.,40,-1.,1.);
  Jacobi_Y_17Ne_2nd->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_17Ne_2nd->GetYaxis()->SetTitle("Cos(#theta_k)");

  Jacobi_T_17Ne_2nd_right = new TH2I("Jacobi_T_17Ne_2nd_right","",50,0.,1.,40,-1.,1.);
  Jacobi_T_17Ne_2nd_right->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_17Ne_2nd_right->GetYaxis()->SetTitle("Cos(#theta_k)");
  Jacobi_Y_17Ne_2nd_right = new TH2I("Jacobi_Y_17Ne_2nd_right","",50,0.,1.,40,-1.,1.);
  Jacobi_Y_17Ne_2nd_right->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_17Ne_2nd_right->GetYaxis()->SetTitle("Cos(#theta_k)");

  Jacobi_T_17Ne_2nd_left = new TH2I("Jacobi_T_17Ne_2nd_left","",50,0.,1.,40,-1.,1.);
  Jacobi_T_17Ne_2nd_left->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_T_17Ne_2nd_left->GetYaxis()->SetTitle("Cos(#theta_k)");
  Jacobi_Y_17Ne_2nd_left = new TH2I("Jacobi_Y_17Ne_2nd_left","",50,0.,1.,40,-1.,1.);
  Jacobi_Y_17Ne_2nd_left->GetXaxis()->SetTitle("Ex/Et");
  Jacobi_Y_17Ne_2nd_left->GetYaxis()->SetTitle("Cos(#theta_k)");
  Txfp_17Ne = new TH1I("Txfp_17Ne","",200,-500,1000);

  ExvET_17Ne2p15O = new TH2I("ExvET_17Ne2p15O","",400,0,17,400,0,17);
  ExvET_17Ne2p15O->GetXaxis()->SetTitle("ET [MeV]");
  ExvET_17Ne2p15O->GetYaxis()->SetTitle("Ex Jacobi Y [MeV]");


  VelCM_17Ne_2p15O_2nd = new TH1I("VelCM_17Ne_2p15O","",100,0,14);
  VelCM_17Ne_2p15O_2nd->GetXaxis()->SetTitle("Velocity CM [cm/ns]");
  ThetaCM_17Ne_2p15O_2nd = new TH1I("ThetaCM_17Ne_2p15O_2nd","",200,0,10);
  ThetaCM_17Ne_2p15O_2nd->GetXaxis()->SetTitle("#Theta_{CM} [degrees]");

  Erel_thetaCM_Ne17 = new TH2I("Erel_thetaCM_Ne17","",200,0,10,400,0,17);

  Ne17_CsImult = new TH1I("Ne17_CsImult","",4,-0.5,3.5);
  Ne17_CsImult->GetXaxis()->SetTitle("Number of distinct Si fired");

  JacobiY_mult3 = new TH2I("JacobiY_mult3","",50,0.,1.,40,-1.,1.);
  JacobiY_mult3->GetXaxis()->SetTitle("Ex/Et");
  JacobiY_mult3->GetYaxis()->SetTitle("Cos(#theta_k)");
  JacobiY_mult2 = new TH2I("JacobiY_mult2","",50,0.,1.,40,-1.,1.);
  JacobiY_mult2->GetXaxis()->SetTitle("Ex/Et");
  JacobiY_mult2->GetYaxis()->SetTitle("Cos(#theta_k)");

  protonxy25 = new TH2I("protonxy25","",200,-20,20,200,-20,20);
  protonxy26 = new TH2I("protonxy26","",200,-20,20,200,-20,20);
  protonxy28 = new TH2I("protonxy28","",200,-20,20,200,-20,20);
  protonxy31 = new TH2I("protonxy31","",200,-20,20,200,-20,20);
    
  protonxy_out = new TH2I("protonxy_out","",200,-20,20,200,-20,20);
  protonxy_in = new TH2I("protonxy_in","",200,-20,20,200,-20,20);

  Ex17Ne_2p15O_For = new TH1I("Ex17Ne_2p15O_For","",800,-2,15);
  Ex17Ne_2p15O_For->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_Trans = new TH1I("Ex17Ne_2p15O_Trans","",800,-2,15);
  Ex17Ne_2p15O_Trans->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_Back = new TH1I("Ex17Ne_2p15O_Back","",800,-2,15);
  Ex17Ne_2p15O_Back->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_Long = new TH1I("Ex17Ne_2p15O_Long","",800,-2,15);
  Ex17Ne_2p15O_Long->GetXaxis()->SetTitle("Ex [MeV]");
  Ex17Ne_2p15O_ForBack = new TH1I("Ex17Ne_2p15O_ForBack","",800,-2,15);
  Ex17Ne_2p15O_ForBack->GetXaxis()->SetTitle("Ex [MeV]");


  Cospsi_17Ne2nd = new TH1I("Cospsi_17Ne2nd","",50,-1,1);
  Cospsi_17Ne2nd_elas = new TH1I("Cospsi_17Ne2nd_elas","",50,-1,1);
  Cospsi_17Ne2nd_elas_p1 = new TH1I("Cospsi_17Ne2nd_elas_p1","",50,-1,1);
  Cospsi_17Ne2nd_elas_p2 = new TH1I("Cospsi_17Ne2nd_elas_p2","",50,-1,1);
  Cospsi_17Ne2nd_inelas = new TH1I("Cospsi_17Ne2nd_inelas","",50,-1,1);
  Cospsi_17Ne2nd_inelas_p1 = new TH1I("Cospsi_17Ne2nd_inelas_p1","",50,-1,1);
  Cospsi_17Ne2nd_inelas_p2 = new TH1I("Cospsi_17Ne2nd_inelas_p2","",50,-1,1);

  Ex17Ne_3He14O = new TH1I("Ex17Ne_3He14O","",200,5,25);
  Ex17Ne_pa12N = new TH1I("Ex17Ne_pa12N","",200,10,30);
  Ex17Ne_3p14N = new TH1I("Ex17Ne_3p14N","",200,5,25);
  Ex17Ne_2pa11C = new TH1I("Ex17Ne_2pa11C","",200,10,30);

  ExNe17_2p15O_onep = new TH1I("Ne17_2p15O_onep","",400,-1,12);


  //F15
  dir15F->cd();
  Ex15F_p14O = new TH1I("Ex15F_p14O","",400,-2,10);
  Ex15F_p14O->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15F_p14O_15O = new TH1I("Ex15F_p14O_15O","",400,-2,10);
  Ex15F_p14O_15O->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15F_p14O_17Ne = new TH1I("Ex15F_p14O_17Ne","",400,-2,10);
  Ex15F_p14O_17Ne->GetXaxis()->SetTitle("Ex [MeV]");
  ET15F_p14O_15O = new TH1I("ET15F_p14O_15O","",400,0,12);
  ET15F_p14O_15O->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15F_2p14O_onep = new TH1I("Ex15F_2p14O_onep","",400,-5,10);
  Ex15F_2p14O_onep->GetXaxis()->SetTitle("Ex [MeV]");
  vel_15F_p14O = new TH1I("vel_15F_p14O","",400,0,20);
  vel_15F_p14O->GetXaxis()->SetTitle("velocity [cm/ns]");
  Ex15F_16Ne1st = new TH1I("Ex15F_16Ne1st","",400,-5,10);
  costhetap15F = new TH1I("costheta15F","",400,-1.,1.);

  ETvTEC_15Fp14O = new TH2I("ETvTEC_15Fp14O","",400,0,17,150,0,10);
  ETvTEC_15Fp14O->GetXaxis()->SetTitle("ET [MeV]");
  ETvTEC_15Fp14O->GetYaxis()->SetTitle("E_{#gamma} [MeV]");


  Ex15F_2p13N = new TH1I("Ex15F_2p13N","",400,0,25);
  Ex15F_3p12C = new TH1I("Ex15F_3p12C","",200,0,20);
  Ex15F_3He12N = new TH1I("Ex15F_3He12N","",200,15,35);
  Ex15F_p3He11C = new TH1I("Ex15F_p3He11C","",200,15,35);

  Ex15F_3p3a = new TH1I("Ex15F_3p3a","",300,10,40);


  dir17F->cd();
  Ex17F_p16O = new TH1I("Ex17F_p16O","",400,0,15);
  Ex17F_p16O->GetXaxis()->SetTitle("Ex [MeV]");
  vel_17F_p16O = new TH1I("vel_17F_p16O","",100,0,20);
  vel_17F_p16O->GetXaxis()->SetTitle("velocity [cm/ns]");
  Ex17F_a13N = new TH1I("Ex17F_a13N","",400,0,15);
  Ex17F_a13N->GetXaxis()->SetTitle("Ex [MeV]");
  vel_17F_a13N = new TH1I("vel_17F_a13N","",100,0,20);
  vel_17F_a13N->GetXaxis()->SetTitle("velocity [cm/ns]");
  TEC_17F_p16O = new TH1I("TEC_17F_p16O","",150,0,10);
  TEC_17F_p16O->GetXaxis()->SetTitle("E_{#gamma} [MeV]");

  Ex17F_3He14N = new TH1I("Ex17F_3He14N","",200,15,35);
  Ex17F_t14O = new TH1I("Ex17F_t14O","",200,20,40);
  Ex17F_d15O = new TH1I("Ex17F_d15O","",200,10,30);
  Ex17F_2p15N = new TH1I("Ex17F_2p15N","",200,10,30);
  Ex17F_pa12C = new TH1I("Ex17F_pa12C","",200,5,25);
  Ex17F_pa12C_Fast = new TH1I("Ex17F_pa12C_Fast","",200,5,25);
  Ex17F_pa12C_Slow = new TH1I("Ex17F_pa12C_Slow","",200,5,25);

  Ex17F_pa12C_vs_16O = new TH2I("Ex17F_pa12C_vs_16O","",200,0,20,200,5,25);
  Ex17F_pa12C_vs_16O->GetXaxis()->SetTitle("Ex 16O");
  Ex17F_pa12C_vs_16O->GetYaxis()->SetTitle("Ex 17F");
  Ex17F_pa12C_vs_13N = new TH2I("Ex17F_pa12C_vs_13N","",200,0,20,200,5,25);
  Ex17F_pa12C_vs_13N->GetXaxis()->SetTitle("Ex 13N");
  Ex17F_pa12C_vs_13N->GetYaxis()->SetTitle("Ex 17F");

  Ex17F_pa12C_vs_16O_Fast = new TH2I("Ex17F_pa12C_vs_16O_Fast","",200,0,20,200,5,25);
  Ex17F_pa12C_vs_16O_Fast->GetXaxis()->SetTitle("Ex 16O");
  Ex17F_pa12C_vs_16O_Fast->GetYaxis()->SetTitle("Ex 17F");
  Ex17F_pa12C_vs_13N_Fast= new TH2I("Ex17F_pa12C_vs_13N_Fast","",200,0,20,200,5,25);
  Ex17F_pa12C_vs_13N_Fast->GetXaxis()->SetTitle("Ex 13N");
  Ex17F_pa12C_vs_13N_Fast->GetYaxis()->SetTitle("Ex 17F");

  Ex17F_pa12C_vs_16O_Slow = new TH2I("Ex17F_pa12C_vs_16O_Slow","",200,0,20,200,5,25);
  Ex17F_pa12C_vs_16O_Slow->GetXaxis()->SetTitle("Ex 16O");
  Ex17F_pa12C_vs_16O_Slow->GetYaxis()->SetTitle("Ex 17F");
  Ex17F_pa12C_vs_13N_Slow = new TH2I("Ex17F_pa12C_vs_13N_Slow","",200,0,20,200,5,25);
  Ex17F_pa12C_vs_13N_Slow->GetXaxis()->SetTitle("Ex 13N");
  Ex17F_pa12C_vs_13N_Slow->GetYaxis()->SetTitle("Ex 17F");

  vel17F_pa12C = new TH1I("vel17F_pa12C","",100,0,20);
  vel17F_pa12C->GetXaxis()->SetTitle("velocity [cm/ns]");


  //F18
  dir18F->cd();
  Ex18F_3He15N = new TH1I("Ex18F_3He15N","",200,10,30);
  Ex18F_d16O = new TH1I("Ex18F_d16O","",200,5,25);
  Ex18F_a14N = new TH1I("Ex18F_a14N","",200,0,20);
  Ex18F_t15O = new TH1I("Ex18F_t15O","",200,15,35);
  Ex18F_6Li12C = new TH1I("Ex18F_6Li12C","",200,10,30);

  dir16F->cd();
  TEC_16F_p15O_0 = new TH1I("TEC_16F_p15O_0","",400,0,12);
  TEC_16F_p15O_1 = new TH1I("TEC_16F_p15O_1","",400,0,12);
  TEC_16F_p15O_2 = new TH1I("TEC_16F_p15O_2","",400,0,12);
  TEC_16F_p15O_3 = new TH1I("TEC_16F_p15O_3","",400,0,12);
  TEC_16F_p15O_4 = new TH1I("TEC_16F_p15O_4","",400,0,12);
  TEC_16F_p15O_5 = new TH1I("TEC_16F_p15O_5","",400,0,12);

  TEC_16F_p15O_0_16F = new TH1I("TEC_16F_p15O_0_16F","",400,0,12);
  TEC_16F_p15O_1_16F = new TH1I("TEC_16F_p15O_1_16F","",400,0,12);
  TEC_16F_p15O_2_16F = new TH1I("TEC_16F_p15O_2_16F","",400,0,12);
  TEC_16F_p15O_3_16F = new TH1I("TEC_16F_p15O_3_16F","",400,0,12);
  TEC_16F_p15O_4_16F = new TH1I("TEC_16F_p15O_4_16F","",400,0,12);
  TEC_16F_p15O_5_16F = new TH1I("TEC_16F_p15O_5_16F","",400,0,12);
 
  TEC_16F_p15O_0_O15 = new TH1I("TEC_16F_p15O_0_O15","",400,0,12);
  TEC_16F_p15O_1_O15 = new TH1I("TEC_16F_p15O_1_O15","",400,0,12);
  TEC_16F_p15O_2_O15 = new TH1I("TEC_16F_p15O_2_O15","",400,0,12);
  TEC_16F_p15O_3_O15 = new TH1I("TEC_16F_p15O_3_O15","",400,0,12);
  TEC_16F_p15O_4_O15 = new TH1I("TEC_16F_p15O_4_O15","",400,0,12);
  TEC_16F_p15O_5_O15 = new TH1I("TEC_16F_p15O_5_O15","",400,0,12);

  TEC_16F_p15O_0_16F_O15 = new TH1I("TEC_16F_p15O_0_16F_O15","",400,0,12);
  TEC_16F_p15O_1_16F_O15 = new TH1I("TEC_16F_p15O_1_16F_O15","",400,0,12);
  TEC_16F_p15O_2_16F_O15 = new TH1I("TEC_16F_p15O_2_16F_O15","",400,0,12);
  TEC_16F_p15O_3_16F_O15 = new TH1I("TEC_16F_p15O_3_16F_O15","",400,0,12);
  TEC_16F_p15O_4_16F_O15 = new TH1I("TEC_16F_p15O_4_16F_O15","",400,0,12);
  TEC_16F_p15O_5_16F_O15 = new TH1I("TEC_16F_p15O_5_16F_O15","",400,0,12);

  TEC_15O_added = new TH1I("TEC_15O_added","",400,0,12);
  TEC_16F_p15O_1_times = new TH1I("TEC_16F_p15O_1_times","",250,0,1000);
  TEC_16F_p15O_others = new TH1I("TEC_16F_p15O_others","",400,0,12);
  TEC_16F_p15O_1_theta = new TH1I("TEC_16F_p15O_1_theta","",360,0,180);


  TEC_16F_p15O_5_nodopp = new TH1I("TEC_16F_p15O_5_nodopp","",400,0,12);
  TEC_17Ne_2p15O = new TH1I("TEC_17Ne_2p15O","",400,0,12);
  TEC_15O_p14N = new TH1I("TEC_15O_p14N","",400,0,12);

  Ex16F_2p14N = new TH1I("Ex16F_2p14N","",800,0,20);
  Ex16F_2p14N->GetXaxis()->SetTitle("Ex [MeV]");
  Ex16F_2p14N_17Ne = new TH1I("Ex16F_2p14N_17Ne","",800,0,20);
  Ex16F_2p14N_15O = new TH1I("Ex16F_2p14N_15O","",800,0,20);
  vel_16F_2p14N = new TH1I("vel_16F_2p14N","",100,0,20);
  vel_16F_2p14N->GetXaxis()->SetTitle("velocity [cm/ns]");
  TEC_Dop_2p14N = new TH1I("TEC_Dop_2p14N","",4096,0,10);
  Ex16F_2p14N_Gamma = new TH1I("Ex16F_2p14N_Gamma","",800,0,20);
  Ex16F_2p14N_Gamma->GetXaxis()->SetTitle("Ex [MeV]");
  Txfp_16F = new TH1I("Txfp_16F","",200,-500,1000);

  Ex16F_2p_v_ceasar = new TH2I("Ex16F_2p_v_ceasar","",800,0,10,200,0,20);
  Ex16F_2p_v_ceasar->GetXaxis()->SetTitle("E_{#gamma} [MeV]");
  Ex16F_2p_v_ceasar->GetYaxis()->SetTitle("E* [MeV]");
  Ex16F_p15O_v_ceasar = new TH2I("Ex16Fp_15O_v_ceasar","",800,0,10,400,-1,12);
  Ex16F_p15O_v_ceasar->GetYaxis()->SetTitle("E* [MeV]");
  Ex16F_p15O_v_ceasar->GetXaxis()->SetTitle("E_{#gamma} [MeV]");

  Ex16F_2p14N_noGamma = new TH1I("Ex16F_2p14N_noGamma","",800,0,20);
  Ex16F_2p14N_noGoodGamma = new TH1I("Ex16F_2p14N_noGoodGamma","",800,0,20);
  Ex16F_2p14N_gammaBack = new TH1I("Ex16F_2p14N_gammaBack","",800,0,20);

  Ex16F_2p14N_CsIMult = new TH1I("Ex16F_2p14N_CsIMult","",800,0,20);
  Ex16F_2p14N_CsIMult->GetXaxis()->SetTitle("Ex [MeV]");

  Ex16Fv15O = new TH2I("Ex16Fv15O","",800,0,20,800,0,20);
  Ex16Fv15O->GetXaxis()->SetTitle("Ex(^{15}O) [MeV]");
  Ex16Fv15O->GetYaxis()->SetTitle("Ex(^{16}F) [MeV]");

  Ex16Fv15Ovgamma = new TH3I("Ex16Fv15Ovgamma","",800,0,20,800,0,20,400,0,10);
  Ex16Fv15Ovgamma->GetXaxis()->SetTitle("Ex(^{15}O) [MeV]");
  Ex16Fv15Ovgamma->GetYaxis()->SetTitle("Ex(^{16}F) [MeV]"); 
  Ex16Fv15Ovgamma->GetZaxis()->SetTitle("E_{#gamma} [MeV]");

  Ex16F_peak1 = new TH1I("Ex16F_peak1","",800,0,20);
  Ex16F_peak1->GetXaxis()->SetTitle("Ex(^{16}F) [MeV]");
  Ex16F_peak2 = new TH1I("Ex16F_peak2","",800,0,20);
  Ex16F_peak2->GetXaxis()->SetTitle("Ex(^{16}F) [MeV]");
  Ex16F_peak3 = new TH1I("Ex16F_peak3","",800,0,20);
  Ex16F_peak3->GetXaxis()->SetTitle("Ex(^{16}F) [MeV]");

  thetaCM_16F_peak2 = new TH1I("thetaCM_16F_peak2","",200,0,10);
  velCM_16F_peak2 = new TH1I("velCM_16F_peak2","",100,0,14);

  Ex16F_backhi = new TH1I("Ex16F_backhi","",800,0,20);
  Ex16F_backhi->GetXaxis()->SetTitle("Ex(^{16}F) [MeV]");
  Ex16F_backlo = new TH1I("Ex16F_backlo","",800,0,20);
  Ex16F_backlo->GetXaxis()->SetTitle("Ex(^{16}F) [MeV]");

  TEC_16F_15Ogated = new TH1I("TEC_16F_15Ogated","",400,0,10);

  Ex16F_3He13N = new TH1I("Ex16F_3He13N","",400,0,20);
  Ex16F_3He13N->GetXaxis()->SetTitle("E* [MeV]");

  vel_16F_3He13N = new TH1I("vel_16F_3He13N","",100,0,20);
  vel_16F_3He13N->GetXaxis()->SetTitle("velocity [cm/ns]");

  Vel14N = new TH1I("Vel14N","",100,0,0.5);

  Ex16F_p15O = new TH1I("Ex16F_p15O","",400,-1,12);
  Ex16F_p15O->GetXaxis()->SetTitle("Ex [MeV]");
  vel_16F_p15O = new TH1I("vel_16F_p15O","",400,0,20);
  vel_16F_p15O->GetXaxis()->SetTitle("velocity [cm/ns]");
  Ex16F_p15O_15O = new TH1I("Ex16F_p15O_15O","",400,-1,12);
  Ex16F_p15O_14N = new TH1I("Ex16F_p15O_14N","",400,-1,12);
  Ex16F_p15O_17Ne = new TH1I("Ex16F_p15O_17Ne","",400,-1,12);

  Ex16F_p15O_gamma = new TH1I("Ex16F_p15O_gamma","",400,-1,12); 
  Ex16F_p15O_gammaHi = new TH1I("Ex16F_p15O_gammaHi","",400,-1,12); 
  Ex16F_p15O_gamma_O15 = new TH1I("Ex16F_p15O_gamma_15O","",400,-1,12); 

  Ex16F_a12N = new TH1I("Ex16F_a12N","",400,0,20);
  Ex16F_a12N->GetXaxis()->SetTitle("Ex [MeV]");
  vel_16F_a12N = new TH1I("vel_16F_a12N","",100,0,20);
  vel_16F_a12N->GetXaxis()->SetTitle("velocity [cm/ns]");

  Ex16F_d14O = new TH1I("Ex16F_d14O","",200,10,30);
  Ex16F_3p13C = new TH1I("Ex16F_3p13C","",200,10,30);
  Ex16F_pa11C = new TH1I("Ex16F_pa11C","",200,5,25);
  Ex16F_p3He12C = new TH1I("Ex16F_p3He12C","",200,10,30);
  Ex16F_pt12N = new TH1I("Ex16F_pt12N","",200,25,45);
  Ex16F_2pt11C = new TH1I("Ex16F_2pt11C","",200,25,45);
  Ex16F_4pa8Li = new TH1I("Ex16F_4pa8Li","",200,40,60);

  //O13
  dir13O->cd();
  Ex13O_1p12N = new TH1I("Ex13O_1p12N","",200,0,20);
  Ex13O_1p12N->GetXaxis()->SetTitle("Ex [MeV]");
  vel_13O_1p12N = new TH1I("vel_13O_1p12N","",100,0,20);
  vel_13O_1p12N->GetXaxis()->SetTitle("velocity [cm/ns]");
  TEC_Dop_13O = new TH1I("TEC_Dop_13O","",4095,0,10);

  Ex13O_2p11C = new TH1I("Ex13O_2p11C","",200,0,20);

  //O14
  dir14O->cd();
  Ex14O_1p13N = new TH1I("Ex14O_1p13N","",800,0,20);
  Ex14O_1p13N->GetXaxis()->SetTitle("Ex [MeV]");
  Ex14O_1p13N_17Ne = new TH1I("Ex14O_1p13N_17Ne","",800,0,20);
  Ex14O_1p13N_17Ne->GetXaxis()->SetTitle("Ex [MeV]");
  Ex14O_1p13N_15O = new TH1I("Ex14O_1p13N_15O","",800,0,20);
  Ex14O_1p13N_15O->GetXaxis()->SetTitle("Ex [MeV]");
  vel_14O_1p13N = new TH1I("vel_14O_1p13N","",800,0,20);
  vel_14O_1p13N->GetXaxis()->SetTitle("velocity [cm/ns]");
  TEC_Dop_14O = new TH1I("TEC_Dop_14O","",4095,0,10);

  Ex14O_Back = new TH1I("Ex14O_Back","",800,0,20);
  Ex14O_Back->GetXaxis()->SetTitle("Ex [MeV]");
  Ex14O_For = new TH1I("Ex14O_For","",800,0,20);
  Ex14O_For->GetXaxis()->SetTitle("Ex [MeV]");
  Ex14O_Trans = new TH1I("Ex14O_Trans","",800,0,20);
  Ex14O_Trans->GetXaxis()->SetTitle("Ex [MeV]");
  Ex14O_Long = new TH1I("Ex14O_Long","",800,0,20);
  Ex14O_Long->GetXaxis()->SetTitle("Ex [MeV]");

  Ex14O_2p12C = new TH1I("Ex14O_2p12C","",800,0,20);
  Ex14O_3He11C = new TH1I("Ex14O_3He11C","",200,15,35);

  TEC_14O_2p12C = new TH1I("TEC_14O_2p12C","",500,0,10);


  //O15
  dir15O->cd();
  Ex15O_1p14N = new TH1I("Ex15O_1p14N","",800,0,20);
  Ex15O_1p14N->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15O_1p14N_17Ne = new TH1I("Ex15O_1p14N_17Ne","",800,0,20);
  Ex15O_1p14N_17Ne->GetXaxis()->SetTitle("Ex [MeV]");
  vel_15O_1p14N = new TH1I("vel_15O_1p14N","",100,0,20);
  vel_15O_1p14N->GetXaxis()->SetTitle("velocity [cm/ns]");
  TEC_Dop_15O = new TH1I("TEC_Dop_15O","",4095,0,10);
  TEC_15O_p14N_17Ne = new TH1I("TEC_15O_p14N_17Ne","",4095,0,10);
  Ex15O_p14N_17Ne_IAS = new TH1I("Ex15O_p14N_17Ne_IAS","",800,0,20);
  Ex15O_p14N_17Ne_IAS->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15O_p14N_17Ne_Back = new TH1I("Ex15O_p14N_17Ne_Back","",800,0,20);
  Ex15O_p14N_17Ne_Back->GetXaxis()->SetTitle("Ex [MeV]");

  Ex15O_1p14N_inner = new TH1I("Ex15O_1p14N_inner","",800,0,20);
  Ex15O_p14N_15O_gamma = new TH1I("Ex15O_p14N_15O_gamma","",800,0,20);

  Ex15O_vs_caesar = new TH2I("Ex15O_vs_ceasar","",800,0,20,1024,0,10);
  Ex15O_vs_caesar->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15O_vs_caesar->GetYaxis()->SetTitle("E_{#gamma} [MeV]");

  Ex15O_Back = new TH1I("Ex15O_Back","",800,0,20);
  Ex15O_Back->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15O_For = new TH1I("Ex15O_For","",800,0,20);
  Ex15O_For->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15O_Long = new TH1I("Ex15O_Long","",800,0,20);
  Ex15O_Long->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15O_Trans = new TH1I("Ex15O_Trans","",800,0,20);
  Ex15O_Trans->GetXaxis()->SetTitle("Ex [MeV]");

  Ex15O_Back_17Ne = new TH1I("Ex15O_Back_17Ne","",800,0,20);
  Ex15O_Back_17Ne->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15O_For_17Ne = new TH1I("Ex15O_For_17Ne","",800,0,20);
  Ex15O_For_17Ne->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15O_Long_17Ne = new TH1I("Ex15O_Long_17Ne","",800,0,20);
  Ex15O_Long_17Ne->GetXaxis()->SetTitle("Ex [MeV]");
  Ex15O_Trans_17Ne = new TH1I("Ex15O_Trans_17Ne","",800,0,20);
  Ex15O_Trans_17Ne->GetXaxis()->SetTitle("Ex [MeV]");   

  VelCM_15O_7MeV = new TH1I("VelCM_15O_7MeV","",100,0,14);
  VelCM_15O_7MeV->GetXaxis()->SetTitle("Velocity CM [cm/ns]");
  ThetaCM_15O_7MeV= new TH1I("ThetaCM_15O_7MeV","",200,0,10);
  ThetaCM_15O_7MeV->GetXaxis()->SetTitle("#Theta_{CM} [degrees]");
    
  VelCM_15O_8MeV = new TH1I("VelCM_15O_8MeV","",100,0,14);
  VelCM_15O_8MeV->GetXaxis()->SetTitle("Velocity CM [cm/ns]");
  ThetaCM_15O_8MeV= new TH1I("ThetaCM_15O_8MeV","",200,0,10);
  ThetaCM_15O_8MeV->GetXaxis()->SetTitle("#Theta_{CM} [degrees]");

  costheta_15O = new TH1I("costheta_15O","",100,-1,1);

  Ex15O_2p13C = new TH1I("Ex15O_2p13C","",200,10,30);
  TEC15O_2p13C = new TH1I("TEC15O_2p13C","",500,0,10);


  Ex15O_a11C = new TH1I("Ex15O_a11C","",200,10,30);
  TEC15O_a11C = new TH1I("TEC15O_a11C","",500,0,10);

  Ex15O_3He12C = new TH1I("Ex15O_3He12C","",200,10,30);
  TEC15O_3He12C = new TH1I("TEC15O_3He12C","",500,0,10);

  Ex15O_t12N = new TH1I("Ex15O_t12N","",200,25,45);
  Ex15O_d13N = new TH1I("Ex15O_d13N","",200,15,35);
  Ex15O_p2a6Li = new TH1I("Ex15O_p2a6Li","",200,20,40);



  //O16
  dir16O->cd();

  Ex16O_p15N = new TH1I("Ex16O_p15N","",200,10,30);
  Ex16O_a12C = new TH1I("Ex16O_a12C","",200,5,25);
  Ex16O_4a = new TH1I("Ex16O_4a","",400,10,50);
  Ex16O_d14N = new TH1I("Ex16O_d14N","",200,20,40);
  Ex16O_t13N = new TH1I("Ex16O_t13N","",200,25,45);
  Ex16O_3He13C = new TH1I("Ex16O_3He13C","",200,20,40);
  Ex16O_6Li10B = new TH1I("Ex16O_6Li10B","",200,30,50);
  Ex16O_pa11B = new TH1I("Ex16O_pa11B","",200,20,40);

  //Li6 correlations

  dir6Li->cd();
  Erel_6Li = new TH1I("Erel_6Li","",400,0,8);
  Erel_6Li->GetXaxis()->SetTitle("Erel ^{6}Li");

  Costheta = new TH1I("Costheta","",400,-1,1);
  Costheta->GetXaxis()->SetTitle("Cos(#theta)");

  Erel_Back_Diff = new TH1I("Erel_Backward_2_tele","",400,0,8);
  Erel_Back_Diff->GetXaxis()->SetTitle("Erel ^{6}Li");

  Erel_For_Diff = new TH1I("Erel_Forward_2_tele","",400,0,8);
  Erel_For_Diff->GetXaxis()->SetTitle("Erel ^{6}Li");

  Erel_Trans_Diff = new TH1I("Erel_Transverse_2_tele","",400,0,8);
  Erel_Trans_Diff->GetXaxis()->SetTitle("Erel ^{6}Li");

  Erel_Back_Same = new TH1I("Erel_Backward_1_tele","",400,0,8);
  Erel_Back_Same->GetXaxis()->SetTitle("Erel ^{6}Li");

  Erel_For_Same = new TH1I("Erel_Forward_1_tele","",400,0,8);
  Erel_For_Same->GetXaxis()->SetTitle("Erel ^{6}Li");

  Erel_Trans_Same = new TH1I("Erel_Transverse_1_tele","",400,0,8);
  Erel_Trans_Same->GetXaxis()->SetTitle("Erel ^{6}Li");
  vel_6Li = new TH1I("vel_6Li","",100,0,14);
  vel_6Li->GetXaxis()->SetTitle("velocity [cm/ns]");

  dir18Na->cd();
  Ex18Na_3p15O = new TH1I("Ex18Na_3p15O","",800,0,20);
  Ex18Na_3p15O->GetXaxis()->SetTitle("Ex [MeV]");
  vel_18Na_3p15O = new TH1I("vel_18Na_3p15O","",800,0,20);
  vel_18Na_3p15O->GetXaxis()->SetTitle("velocity [cm/ns]");
  Ex18Na_2pd14O = new TH1I("Ex18Na_2pd14O","",200,10,30);
  Ex18Na_p3He14O = new TH1I("Ex18Na_p3He14O","",200,5,25);


  dir18Ne->cd();
  Ex18Ne_2p16O = new TH1I("Ex18Ne_2p16O","",800,0,20);
  Ex18Ne_2p16O->GetXaxis()->SetTitle("Ex [MeV]");
  ET18Ne_2p16O = new TH1I("ET18Ne_2p16O","",800,0,20);

  Jacobi_Y_18Ne_2p16O = new TH2I("Jacobi_Y_18Ne_2p16O","",24,0.,1.,24,-1.,1.);
  Jacobi_T_18Ne_2p16O = new TH2I("Jacobi_T_18Ne_2p16O","",24,0.,1.,24,-1.,1.);

  TEC_18Ne_2p16O = new TH1I("TEC_18Ne_2p16O","",128,0,10);
  TEC_18Ne_p17F = new TH1I("TEC_18Ne_p17F","",128,0,10);


  Ex18Ne_a14O = new TH1I("Ex18Ne_a14O","",200,5,25);
  Ex18Ne_3He15O = new TH1I("Ex18Ne_3He15O","",200,10,30);
  Ex18Ne_pd15O = new TH1I("Ex18Ne_pd15O","",200,15,35);
  Ex18Ne_pt14O = new TH1I("Ex18Ne_pt14O","",200,15,35);
  Ex18Ne_6Li12N = new TH1I("Ex18Ne_6Li12N","",200,25,45);
  Ex18Ne_p17F = new TH1I("Ex18Ne_p17F","",200,0,20);
  ET18Ne_p17F = new TH1I("ET18Ne_p17F","",200,0,20);

  Ex_vs_gamma_18Ne_p17F = new TH2I("Ex_vs_gamma_18Ne_p17F","",128,0,10,200,0,20);
  Ex_vs_gamma_18Ne_p17F->GetXaxis()->SetTitle("E_{#gamma} [MeV]");
  Ex_vs_gamma_18Ne_p17F->GetYaxis()->SetTitle("Ex [MeV]");

  Ex18Ne_p17F_Back = new TH1I("Ex18NE_p17F_Back","",200,0,20);
  Ex18Ne_p17F_For = new TH1I("Ex18NE_p17F_For","",200,0,20);
  Ex18Ne_p17F_Long = new TH1I("Ex18NE_p17F_Long","",200,0,20);
  Ex18Ne_p17F_Trans = new TH1I("Ex18NE_p17F_Trans","",200,0,20);
  ET18Ne_p17F_Trans = new TH1I("ET18Ne_p17F_Trans","",200,0,20);

  thetaCM_18Ne_p17F = new TH1I("thetaCM_18Ne_p17F","",200,0,10);
  velCM_18Ne_p17F = new TH1I("velCM_18Ne_p17F","",100,0,14);

  thetaCM_18Ne_2p16O = new TH1I("thetaCM_18Ne_2p16O","",200,0,10);
  velCM_18Ne_2p16O = new TH1I("velCM_18Ne_2p16O","",100,0,14);

  Ne18_protonXY = new TH2I("Ne18_protonXY","",1200,-30,30,800,-20,20);
  Ne18_coreXY = new TH2I("Ne18_coreXY","",1200,-30,30,800,-20,20);



  dir5Be->cd();
  ET5Be_2p3He = new TH1I("ET5Be_2p3He","",200,0,20);
  ET5Be_2p3He->GetXaxis()->SetTitle("ET [MeV]");
  vel5Be_2p3He = new TH1I("vel_5Be_2p3He","",800,0,20);
  vel5Be_2p3He->GetXaxis()->SetTitle("velocity [cm/ns]");
  Jacobi_x_T_5Be = new TH1I("Jacobi_x_T_5Be","",50,0,1);
  Jacobi_x_Y_5Be = new TH1I("Jacobi_x_Y_5Be","",50,0,1);
  Jacobi_y_T_5Be = new TH1I("Jacobi_y_T_5Be","",50,-1,1);
  Jacobi_y_Y_5Be = new TH1I("Jacobi_y_Y_5Be","",50,-1,1);

  ET5Be_2p3He_17Ne = new TH1I("ET5Be_2p3He_17Ne","",200,0,20);
  ET5Be_2p3He_17Ne->GetXaxis()->SetTitle("ET [MeV]");
  Jacobi_x_T_5Be_17Ne = new TH1I("Jacobi_x_T_5Be_17Ne","",50,0,1);
  Jacobi_x_Y_5Be_17Ne = new TH1I("Jacobi_x_Y_5Be_17Ne","",50,0,1);
  Jacobi_y_T_5Be_17Ne = new TH1I("Jacobi_y_T_5Be_17Ne","",50,-1,1);
  Jacobi_y_Y_5Be_17Ne = new TH1I("Jacobi_y_Y_5Be_17Ne","",50,-1,1);

  ET5Be_2p3He_15O = new TH1I("ET5Be_2p3He_15O","",200,0,20);
  ET5Be_2p3He_15O->GetXaxis()->SetTitle("ET [MeV]");
  Jacobi_x_T_5Be_15O = new TH1I("Jacobi_x_T_5Be_15O","",50,0,1);
  Jacobi_x_Y_5Be_15O = new TH1I("Jacobi_x_Y_5Be_15O","",50,0,1);
  Jacobi_y_T_5Be_15O = new TH1I("Jacobi_y_T_5Be_15O","",50,-1,1);
  Jacobi_y_Y_5Be_15O = new TH1I("Jacobi_y_Y_5Be_15O","",50,-1,1);

  ET5Be_2p3He_12C = new TH1I("ET5Be_2p3He_12C","",200,0,20);
  ET5Be_2p3He_12C->GetXaxis()->SetTitle("ET [MeV]");
  Jacobi_x_T_5Be_12C = new TH1I("Jacobi_x_T_5Be_12C","",50,0,1);
  Jacobi_x_Y_5Be_12C = new TH1I("Jacobi_x_Y_5Be_12C","",50,0,1);
  Jacobi_y_T_5Be_12C = new TH1I("Jacobi_y_T_5Be_12C","",50,-1,1);
  Jacobi_y_Y_5Be_12C = new TH1I("Jacobi_y_Y_5Be_12C","",50,-1,1);

  ET5Be_2p3He_no12C = new TH1I("ET5Be_2p3He_no12C","",200,0,20);
  ET5Be_2p3He_no12C->GetXaxis()->SetTitle("ET [MeV]");
  Jacobi_x_T_5Be_no12C = new TH1I("Jacobi_x_T_5Be_no12C","",50,0,1);
  Jacobi_x_Y_5Be_no12C = new TH1I("Jacobi_x_Y_5Be_no12C","",50,0,1);
  Jacobi_y_T_5Be_no12C = new TH1I("Jacobi_y_T_5Be_no12C","",50,-1,1);
  Jacobi_y_Y_5Be_no12C = new TH1I("Jacobi_y_Y_5Be_no12C","",50,-1,1);

  ET5Be_2p3He_3A = new TH1I("ET5Be_2p3He_3A","",200,0,20);
  ET5Be_2p3He_3A->GetXaxis()->SetTitle("ET [MeV]");

  Ex12C_3A_5Be = new TH1I("Ex12C_3A_5Be","",200,0,20);

  TEC12C_5Be = new TH1I("TEC12C_5Be","",128,0,10);

  Epp_ET_5Be = new TH2I("Epp_ET_5Be","",200,0,20,200,0,20);
  Costheta_ET_5Be = new TH2I("Costheta_ET_5Be","",200,0,20,50,-1,1);

  Epp_ET_5Be_15O = new TH2I("Epp_ET_5Be_15O","",200,0,20,200,0,20);
  Costheta_ET_5Be_15O = new TH2I("Costheta_ET_5Be_15O","",200,0,20,50,-1,1);

  Epp_ET_5Be_17Ne = new TH2I("Epp_ET_5Be_17Ne","",200,0,20,200,0,20);
  Costheta_ET_5Be_17Ne = new TH2I("Costheta_ET_5Be_17Ne","",200,0,20,50,-1,1);

  ET_17Ne_2p3He12C = new TH1I("ET_17Ne_2p3He12C","",200,0,20);
  ET_15O_3He12C = new TH1I("ET_15O_3He12C","",200,0,20);
  ET_14O_2p12C = new TH1I("ET_14O_2p12C","",200,0,20);
  ET_13N_p12C = new TH1I("ET_13N_p12C","",200,0,20);

  //Be7
  dir7Be->cd();
  Ex7Be_p6Li = new TH1I("Ex7Be_p6Li","",800,0,20);
  vel_7Be_p6Li = new TH1I("vel_7Be_p6Li","",100,0,14);
  vel_7Be_p6Li_left = new TH1I("vel_7Be_p6Li_left","",100,0,14);
  vel_7Be_p6Li_right = new TH1I("vel_7Be_p6Li_right","",100,0,14);
  TEC_7Be_p6Li = new TH1I("TEC_7Be_p6Li","",128,0,10);

  Ex7Be_p6Li_gamma = new TH1I("Ex7Be_p6Li_gamma","",800,0,20);
  Ex7Be_p6Li_gammaback1 = new TH1I("Ex7Be_p6Li_gammaback1","",800,0,20);
  Ex7Be_p6Li_gammaback2 = new TH1I("Ex7Be_p6Li_gammaback2","",800,0,20);
  Ex7Be_p6Li_gammaback3 = new TH1I("Ex7Be_p6Li_gammaback3","",800,0,20);

  Caesarmult = new TH2I("Caesarmult","",128,0,10,100,0.5,99.5);

  //Be9
  dir9Be->cd();
  Ex9Be_p8Li = new TH1I("Ex9Be_p8Li","",800,0,20);
  vel_9Be_p8Li = new TH1I("vel_9Be_p8Li","",100,0,14);

  //N12
  dir12N->cd();
  Ex12N_p11C = new TH1I("Ex12N_p11C","",200,0,20);
  Ex12N_2p10B = new TH1I("Ex12N_2p10B","",200,5,25);
  Ex12N_p2a3He = new TH1I("Ex12N_p2a3He","",200,5,25);
  Ex12N_2pa6Li = new TH1I("Ex12N_2pa6Li","",200,10,30);

  //N13
  dir13N->cd();
  Ex13N_p12C = new TH1I("Ex13N_p12C","",200,0,20);
  Ex13N_2p11B = new TH1I("Ex13N_2p11B","",200,15,35);
  Ex13N_d11C = new TH1I("Ex13N_d11C","",200,15,35);
  Ex13N_p3a = new TH1I("Ex13N_p3a","",200,5,25);
  Ex13N_3He10B = new TH1I("Ex13N_3He10B","",200,20,40);

  //N14
  dir14N->cd();
  Ex14N_p13C = new TH1I("Ex14N_p13C","",200,5,25);
  vel14N_p13C = new TH1I("vel14N_p13C","",200,0,20);
  Ex14N_d12C = new TH1I("Ex14N_d12C","",200,10,30);
  Ex14N_t11C = new TH1I("Ex14N_t11C","",200,20,40);
  Ex14N_3He11B = new TH1I("Ex14N_3He11B","",200,20,40);
  Ex14N_a10B = new TH1I("Ex14N_a10B","",200,10,30);
  Ex14N_2a6Li = new TH1I("Ex14N_2a6Li","",200,15,35);

  //N15
  dir15N->cd();
  Ex15N_d13C = new TH1I("Ex15N_d13C","",200,15,35);
  Ex15N_t12C = new TH1I("Ex15N_t12C","",200,10,30);
  Ex15N_a11B = new TH1I("Ex15N_a11B","",200,10,30);

  //C11
  dir11C->cd();
  Ex11C_p10B = new TH1I("Ex11C_p10B","",200,5,25);
  vel11C_p10B = new TH1I("vel11C_p10B","",100,0,14);
  TEC11C_p10B = new TH1I("TEC11C_p10B","",500,0,10);
  TEC11C_p10B_noadd = new TH1I("TEC11C_p10B_noadd","",500,0,10);
  TEC11C_p10B_neighbors = new TH1I("TEC11C_p10B_neighbors","",500,0,10);
  GammaGamma_11C_p10B = new TH2I("GammaGamma_11C_p10B","",500,0,5,500,0,5);
  Ex11C_p10B_gamma = new TH1I("Ex11C_p10B_gamma","",200,5,25);
  Ex11C_p10B_gammagamma = new TH1I("Ex11C_p10B_gammagamma","",200,5,25);
  CeasarT_p10B = new TH1I("CeasarT_p10B","",250,0,1000);

  Ex11C_pd2a = new TH1I("Ex11C_pd2a","",200,10,30);
  Ex11C_a7Be = new TH1I("Ex11C_a7Be","",200,5,25);
  Ex11C_pa6Li = new TH1I("Ex11C_pa6Li","",200,10,30);
  Ex11C_3He2a = new TH1I("Ex11C_3He2a","",200,5,25);

  //C12
  dir12C->cd();
  Ex12C_p11B = new TH1I("Ex12C_p11B","",200,15,35);
  Ex12C_d10B = new TH1I("Ex12C_d10B","",200,25,45);
  Ex12C_da6Li = new TH1I("Ex12C_da6Li","",200,25,45);
  Ex12C_tp2a = new TH1I("Ex12C_tp2a","",200,25,45);
  Ex12C_3a = new TH1I("Ex12C_3a","",1000,0,25);
  Ex12C_6Li6Li = new TH1I("Ex12C_6Li6Li","",200,25,45);
  Ex12C_pa7Li = new TH1I("Ex12C_pa7Li","",200,20,40);

  Erms12C_3a = new TH1I("Erms12C_3a","",200,0,0.2);

  //C13
  dir13C->cd();
  Ex13C_d11B = new TH1I("Ex13C_d11B","",200,20,40);
  Ex13C_t10B = new TH1I("Ex13C_t10B","",200,20,40);


  //Ne19
  dir19Ne->cd();
  Ex19Ne_a15O = new TH1I("Ex19Ne_a15O","",200,0,20);
  Ex19Ne_3He16O = new TH1I("Ex19Ne_3He16O","",200,5,25);
  Ex19Ne_pt15O = new TH1I("Ex19Ne_pt15O","",200,20,40);

  //Na17
  dir17Na->cd();
  Erel17Na_3p14O = new TH1I("Erel17Na_3p14O","",200,0,20);
  Erel17Na_3pfake14O = new TH1I("Erel17Na_3pfake14O","",200,0,20);
  Erel17Na_Erel16Ne = new TH2I("Erel17Na_Erel16Ne","",800,0,20,200,0,20);
  vel_17Na = new TH1I("vel_17Na","",100,0,14);
  ErelEx_17Na = new TH2I("ErelEx_17Na","",400,-200,400,200,0,20);

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
