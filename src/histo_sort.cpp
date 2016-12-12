#include "../include/histo_sort.h"
#include "../include/geometry.h"

#include <iostream>
#include <string>

histo_sort::histo_sort()
{
    ostringstream outstring;
    string name;

    //create root file
    file = new TFile ("sort.root","RECREATE");
    file->cd();

    timeRFScintillator = new TH1I("timeRFScintillator","",1024,-600,400);
    timeRFCyclotron = new TH1I("timeRFCyclotron","",1024,-600,400);
    timeA1900 = new TH1I("timeA1900","",1024,-600,400);

    //directory for Front back mixed spectra
    dirFB = new TDirectoryFile("fb","fb");

    //directory for Summary Spectra
    dirSum = new TDirectoryFile("Sum","Hira_Sum");

    //front strips of Si E detectors
    dirFront = new TDirectoryFile("front","front");
    dirFrontRaw = dirFront->mkdir("raw","raw");
    dirFrontCal = dirFront->mkdir("cal","cal");
    dirFrontLGR = dirFront->mkdir("lgr","lgr");
    dirFrontLGC = dirFront->mkdir("lgc","lgc");
    dirFrontTime = dirFront->mkdir("time","Time");
    dirFrontTLG = dirFront->mkdir("timelg","Timelg");
    dirTotCal = dirFront->mkdir("TotCal","TotCal");

    //back strips of Si E detectors
    dirBack = new TDirectoryFile("back","back");
    dirBackRaw = dirBack->mkdir("raw","raw");
    dirBackCal = dirBack->mkdir("cal","cal");
    dirBackLGR = dirBack->mkdir("lgr","lgr");
    dirBackLGC = dirBack->mkdir("lgc","lgc");
    dirBackTime = dirBack->mkdir("time","Time");
    dirBackTLG = dirBack->mkdir("timelg","Timelg");

    //CsI 
    dirCsI = new TDirectoryFile("CsI","CsI");
    dirCsIRaw = dirCsI->mkdir("CsIRaw","raw");
    dirCsITime = dirCsI->mkdir("CsITime","time");
    dirCsISum = dirCsI->mkdir("CsISum","sum");
    dirCsIBlocker = dirCsI->mkdir("CsIBlock","CsIBlock");
    dirCsIGate = dirCsI->mkdir("CsIGate","CsIGate");
    dirCsIMult = dirCsI->mkdir("CsIMult","CsIMult");

    dirCsIBlocker->cd();

    Blocker_E1 = new TH1I("Blocker_E1","",1024,0,4096);
    Blocker_E2 = new TH1I("Blocker_E2","",1024,0,4096);
    Blocker_E3 = new TH1I("Blocker_E3","",1024,0,4096);
    Blocker_E4 = new TH1I("Blocker_E4","",1024,0,4096);
    Blocker_ESum = new TH1I("Blocker_ESum","",1024,0,4096);
    Blocker_ETOF = new TH2I("Blocker_ETOF","",200,-600,4000,1024,0,4096);

    //  dirRSum->cd();
    //ECSum = new TH2I("ECSum","",196,-0.5,195.5,512,0,4095);
    //ECMSum = new TH2I("ECMSum","",196,-0.5,195.5,512,0,4095);
    //TCSum = new TH2I("TCSum","",196,-0.5,195.5,125,0,1000);
    //Txfp = new TH1I("Txpf","",200,-500,1000);
    // TRF = new TH1I("TRF","",125,-500,500);

    ECsI = new TH1I*[TOTAL_CSIS];
    TCsI = new TH1I*[TOTAL_CSIS];
    dirCsIMult->cd();
    CsIMult = new TH1I("CsIMult","",10,0,10);
    ECsI_Zgate = new TH1I*[TOTAL_CSIS];
    CaldEE = new TH2I*[TOTAL_CSIS];
    CsIOver = new TH1I*[TOTAL_CSIS];
    dirCsISum->cd();
    ECsISum = new TH2I("ECsISum","",56,-0.5,55.5,1024,0,4095);
    ECsICSum = new TH2I("ECsICSum","",56,-0.5,55.5,1024,0,500);

    Light = new TH1I*[TOTAL_CSIS];

    Etot = new TH1I("Etot","",10000,0,1000);

    for(int icsi = 0;icsi <TOTAL_CSIS;icsi++)
    {
        outstring.str("");
        outstring << "ECsI_" << icsi;
        name = outstring.str();
        dirCsIRaw->cd();
        ECsI[icsi] = new TH1I(name.c_str(),"",1024,0,4095);

        outstring.str("");
        outstring << "ECsI_" << icsi << "_Gate";
        name = outstring.str();
        dirCsIGate->cd();
        ECsI_Zgate[icsi] = new TH1I(name.c_str(),"",1024,0,4095);

        outstring.str("");
        outstring << "CsIOver_" << icsi;
        name = outstring.str();
        dirCsISum->cd();
        CsIOver[icsi] = new TH1I(name.c_str(),"",1024,0,4095);

        outstring.str("");
        outstring << "TCsI_" << icsi;
        name = outstring.str();
        dirCsITime->cd();
        TCsI[icsi] = new TH1I(name.c_str(),"",1024,-600,400);

        outstring.str("");
        outstring << "Light_" << icsi;
        name = outstring.str();
        dirCsISum->cd();
        Light[icsi] = new TH1I(name.c_str(),"",10000,0,1000);

        //      outstring.str("");
        //       outstring << "Cal_dEE_" << icsi;
        //       name = outstring.str();
        //       dirdEE->cd();

        //       if (icsi > 23 && icsi < 32)
        // 	CaldEE[icsi] = new TH2I(name.c_str(),"",1600,0,800,4096,0,500);
        //       else
        // 	CaldEE[icsi] = new TH2I(name.c_str(),"",1600,0,800,1024,0,80);
    }

    EfrontR = new TH1I**[NUMBER_OF_TELESCOPES];
    EfrontC = new TH1I**[NUMBER_OF_TELESCOPES];
    EfrontLR = new TH1I**[NUMBER_OF_TELESCOPES];
    EfrontLC = new TH1I**[NUMBER_OF_TELESCOPES];
    TfrontR = new TH1I**[NUMBER_OF_TELESCOPES];

    EbackR  = new TH1I**[NUMBER_OF_TELESCOPES];
    EbackC  = new TH1I**[NUMBER_OF_TELESCOPES];
    EbackLR = new TH1I**[NUMBER_OF_TELESCOPES];
    EbackLC = new TH1I**[NUMBER_OF_TELESCOPES];
    TbackR  = new TH1I**[NUMBER_OF_TELESCOPES];

    FBMult = new TH2I*[NUMBER_OF_TELESCOPES];

    EFCSum = new TH2I*[NUMBER_OF_TELESCOPES];
    EBCSum = new TH2I*[NUMBER_OF_TELESCOPES];
    EFSum = new TH2I*[NUMBER_OF_TELESCOPES];
    EBSum = new TH2I*[NUMBER_OF_TELESCOPES];

    dirSum->cd();
    EFTSum = new TH2I*[NUMBER_OF_TELESCOPES];
    EBTSum = new TH2I*[NUMBER_OF_TELESCOPES];

    SiBTime = new TH1I("Si Time Back","",2000,0,16000);
    SiFTime = new TH1I("Si Time Front","",2000,0,16000);

    FBDiff = new TH1I*[NUMBER_OF_TELESCOPES];
    FBDiffLG = new TH1I*[NUMBER_OF_TELESCOPES];
    FB = new TH2I*[NUMBER_OF_TELESCOPES];

    dirFB->cd();

    for (int itele=0;itele<NUMBER_OF_TELESCOPES;itele++)
    {
        EfrontR[itele] = new TH1I*[NUMBER_OF_STRIPS];
        EfrontC[itele] = new TH1I*[NUMBER_OF_STRIPS];
        EfrontLR[itele] = new TH1I*[NUMBER_OF_STRIPS];
        EfrontLC[itele] = new TH1I*[NUMBER_OF_STRIPS];
        TfrontR[itele] = new TH1I*[NUMBER_OF_STRIPS];

        EbackR[itele]  = new TH1I*[NUMBER_OF_STRIPS];
        EbackC[itele]  = new TH1I*[NUMBER_OF_STRIPS];
        EbackLR[itele]  = new TH1I*[NUMBER_OF_STRIPS];
        EbackLC[itele]  = new TH1I*[NUMBER_OF_STRIPS];
        TbackR[itele]  = new TH1I*[NUMBER_OF_STRIPS];

        outstring.str("");
        outstring << "FBMult_" << itele;
        name = outstring.str();
        dirFB->cd();
        FBMult[itele] = new TH2I(name.c_str(),"",10,-0.5,9.5,10,-0.5,9.5);
        FBMult[itele]->GetXaxis()->SetTitle("Front Multiplicity");
        FBMult[itele]->GetYaxis()->SetTitle("Back Multiplicity");

        outstring.str("");
        outstring << "FBDiff_" << itele;
        name = outstring.str();
        dirFB->cd();
        FBDiff[itele] = new TH1I(name.c_str(),"",100,0,20);
        FBDiff[itele]->GetXaxis()->SetTitle("Front - Back");

        outstring.str("");
        outstring << "FB_" << itele;
        name = outstring.str();
        dirFB->cd();
        if(itele !=6 && itele!=7)
            FB[itele] = new TH2I(name.c_str(),"",1024,0,80,1024,0,80);
        else
            FB[itele] = new TH2I(name.c_str(),"",4096,0,500,4096,0,500);
        FB[itele]->GetXaxis()->SetTitle("Front");
        FB[itele]->GetYaxis()->SetTitle("Back");

        outstring.str("");
        outstring << "FBDiffLG_" << itele;
        name = outstring.str();
        dirFB->cd();
        FBDiffLG[itele] = new TH1I(name.c_str(),"",100,0,20);
        FBDiffLG[itele]->GetXaxis()->SetTitle("Front - Back");

        outstring.str("");
        outstring << "EFTSum_" << itele;
        name = outstring.str();
        dirSum->cd();
        EFTSum[itele] = new TH2I(name.c_str(),"",32,-0.5,31.5,4000,0,16000);
        EFTSum[itele]->GetXaxis()->SetTitle("Strip Num");
        EFTSum[itele]->GetYaxis()->SetTitle("Time [channel]");

        outstring.str("");
        outstring << "EBTSum_" << itele;
        name = outstring.str();
        dirSum->cd();
        EBTSum[itele] = new TH2I(name.c_str(),"",32,-0.5,31.5,4000,0,16000);
        EBTSum[itele]->GetXaxis()->SetTitle("Strip Num");
        EBTSum[itele]->GetYaxis()->SetTitle("Time [channel]");

        outstring.str("");
        outstring << "EFCSum_" << itele;
        name = outstring.str();
        dirSum->cd();
        EFCSum[itele] = new TH2I(name.c_str(),"",32,-0.5,31.5,10000,0,10);
        EFCSum[itele]->GetXaxis()->SetTitle("Strip Num");
        EFCSum[itele]->GetYaxis()->SetTitle("Energy [channel]");

        outstring.str("");
        outstring << "EFSum_" << itele;
        name = outstring.str();
        dirSum->cd();
        EFSum[itele] = new TH2I(name.c_str(),"",32,-0.5,31.5,10000,0,2000);
        EFSum[itele]->GetXaxis()->SetTitle("Strip Num");
        EFSum[itele]->GetYaxis()->SetTitle("Energy [channel]");

        outstring.str("");
        outstring << "EBCSum_" << itele;
        name = outstring.str();
        dirSum->cd();
        EBCSum[itele] = new TH2I(name.c_str(),"",32,-0.5,31.5,10000,0,10);
        EBCSum[itele]->GetXaxis()->SetTitle("Strip Num");
        EBCSum[itele]->GetYaxis()->SetTitle("Energy [channel]");

        outstring.str("");
        outstring << "EBSum_" << itele;
        name = outstring.str();
        dirSum->cd();
        EBSum[itele] = new TH2I(name.c_str(),"",32,-0.5,31.5,1000,0,500);
        EBSum[itele]->GetXaxis()->SetTitle("Strip Num");
        EBSum[itele]->GetYaxis()->SetTitle("Energy [channel]");

        for (int istrip=0;istrip<NUMBER_OF_STRIPS;istrip++)
        {
            outstring.str("");
            outstring << "EF"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirFrontRaw->cd();
            EfrontR[itele][istrip] = new TH1I(name.c_str(),"",4000,0,16000);
            EfrontR[itele][istrip]->GetXaxis()->SetTitle("Energy High Gain[channel]");

            outstring.str("");
            outstring << "EFLG"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirFrontLGR->cd();
            EfrontLR[itele][istrip] = new TH1I(name.c_str(),"",4000,0,16000);
            EfrontLR[itele][istrip]->GetXaxis()->SetTitle("Energy Low Gain [channel]");

            outstring.str("");
            outstring << "EFC"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirFrontCal->cd();
            EfrontC[itele][istrip] = new TH1I(name.c_str(),"",4000,0,500);
            EfrontC[itele][istrip]->GetXaxis()->SetTitle("Energy [MeV]");

            outstring.str("");
            outstring << "EFLC"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirFrontLGC->cd();
            EfrontLC[itele][istrip] = new TH1I(name.c_str(),"",4000,0,500);
            EfrontLC[itele][istrip]->GetXaxis()->SetTitle("Energy [MeV]");

            outstring.str("");
            outstring << "TF"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirFrontTime->cd();
            TfrontR[itele][istrip] = new TH1I(name.c_str(),"",2000,0,16000);
            TfrontR[itele][istrip]->GetXaxis()->SetTitle("time [channels]");

            outstring.str("");
            outstring << "EB"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirBackRaw->cd();
            EbackR[itele][istrip] = new TH1I(name.c_str(),"",4000,0,16000);
            EbackR[itele][istrip]->GetXaxis()->SetTitle("Energy High Gain[channel]");

            outstring.str("");
            outstring << "EBLG"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirBackLGR->cd();
            EbackLR[itele][istrip] = new TH1I(name.c_str(),"",4000,0,16000);
            EbackLR[itele][istrip]->GetXaxis()->SetTitle("Energy Low Gain[channel]");

            outstring.str("");
            outstring << "EBC"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirBackCal->cd();
            EbackC[itele][istrip] = new TH1I(name.c_str(),"",4000,0,500);
            EbackC[itele][istrip]->GetXaxis()->SetTitle("Energy [MeV]");

            outstring.str("");
            outstring << "EBLC"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirBackLGC->cd();
            EbackLC[itele][istrip] = new TH1I(name.c_str(),"",4000,0,500);
            EbackLC[itele][istrip]->GetXaxis()->SetTitle("Energy [MeV]");

            outstring.str("");
            outstring << "TB"<<itele<<"_"<<istrip;
            name = outstring.str();
            dirBackTime->cd();
            TbackR[itele][istrip] = new TH1I(name.c_str(),"",2000,0,16000);
            TbackR[itele][istrip]->GetXaxis()->SetTitle("time [channels]");
        }
    }

}
//*********************************************
void histo_sort::write()
{
    file->Write();

    cout << "histo written" << endl;
}
