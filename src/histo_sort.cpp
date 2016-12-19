#include "../include/histo_sort.h"
#include "../include/detector/geometry.h"

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

    for (int telescopeNumber=0;telescopeNumber<NUMBER_OF_TELESCOPES;telescopeNumber++)
    {
        EfrontR[telescopeNumber] = new TH1I*[NUMBER_OF_STRIPS];
        EfrontC[telescopeNumber] = new TH1I*[NUMBER_OF_STRIPS];
        EfrontLR[telescopeNumber] = new TH1I*[NUMBER_OF_STRIPS];
        EfrontLC[telescopeNumber] = new TH1I*[NUMBER_OF_STRIPS];
        TfrontR[telescopeNumber] = new TH1I*[NUMBER_OF_STRIPS];

        EbackR[telescopeNumber]  = new TH1I*[NUMBER_OF_STRIPS];
        EbackC[telescopeNumber]  = new TH1I*[NUMBER_OF_STRIPS];
        EbackLR[telescopeNumber]  = new TH1I*[NUMBER_OF_STRIPS];
        EbackLC[telescopeNumber]  = new TH1I*[NUMBER_OF_STRIPS];
        TbackR[telescopeNumber]  = new TH1I*[NUMBER_OF_STRIPS];

        outstring.str("");
        outstring << "FBMult_" << telescopeNumber;
        name = outstring.str();
        dirFB->cd();
        FBMult[telescopeNumber] = new TH2I(name.c_str(),"",10,-0.5,9.5,10,-0.5,9.5);
        FBMult[telescopeNumber]->GetXaxis()->SetTitle("Front Multiplicity");
        FBMult[telescopeNumber]->GetYaxis()->SetTitle("Back Multiplicity");

        outstring.str("");
        outstring << "FBDiff_" << telescopeNumber;
        name = outstring.str();
        dirFB->cd();
        FBDiff[telescopeNumber] = new TH1I(name.c_str(),"",100,0,20);
        FBDiff[telescopeNumber]->GetXaxis()->SetTitle("Front - Back");

        outstring.str("");
        outstring << "FB_" << telescopeNumber;
        name = outstring.str();
        dirFB->cd();
        if(telescopeNumber !=6 && telescopeNumber!=7)
            FB[telescopeNumber] = new TH2I(name.c_str(),"",1024,0,80,1024,0,80);
        else
            FB[telescopeNumber] = new TH2I(name.c_str(),"",4096,0,500,4096,0,500);
        FB[telescopeNumber]->GetXaxis()->SetTitle("Front");
        FB[telescopeNumber]->GetYaxis()->SetTitle("Back");

        outstring.str("");
        outstring << "FBDiffLG_" << telescopeNumber;
        name = outstring.str();
        dirFB->cd();
        FBDiffLG[telescopeNumber] = new TH1I(name.c_str(),"",100,0,20);
        FBDiffLG[telescopeNumber]->GetXaxis()->SetTitle("Front - Back");

        outstring.str("");
        outstring << "EFTSum_" << telescopeNumber;
        name = outstring.str();
        dirSum->cd();
        EFTSum[telescopeNumber] = new TH2I(name.c_str(),"",32,-0.5,31.5,4000,0,16000);
        EFTSum[telescopeNumber]->GetXaxis()->SetTitle("Strip Num");
        EFTSum[telescopeNumber]->GetYaxis()->SetTitle("Time [channel]");

        outstring.str("");
        outstring << "EBTSum_" << telescopeNumber;
        name = outstring.str();
        dirSum->cd();
        EBTSum[telescopeNumber] = new TH2I(name.c_str(),"",32,-0.5,31.5,4000,0,16000);
        EBTSum[telescopeNumber]->GetXaxis()->SetTitle("Strip Num");
        EBTSum[telescopeNumber]->GetYaxis()->SetTitle("Time [channel]");

        outstring.str("");
        outstring << "EFCSum_" << telescopeNumber;
        name = outstring.str();
        dirSum->cd();
        EFCSum[telescopeNumber] = new TH2I(name.c_str(),"",32,-0.5,31.5,10000,0,10);
        EFCSum[telescopeNumber]->GetXaxis()->SetTitle("Strip Num");
        EFCSum[telescopeNumber]->GetYaxis()->SetTitle("Energy [channel]");

        outstring.str("");
        outstring << "EFSum_" << telescopeNumber;
        name = outstring.str();
        dirSum->cd();
        EFSum[telescopeNumber] = new TH2I(name.c_str(),"",32,-0.5,31.5,10000,0,2000);
        EFSum[telescopeNumber]->GetXaxis()->SetTitle("Strip Num");
        EFSum[telescopeNumber]->GetYaxis()->SetTitle("Energy [channel]");

        outstring.str("");
        outstring << "EBCSum_" << telescopeNumber;
        name = outstring.str();
        dirSum->cd();
        EBCSum[telescopeNumber] = new TH2I(name.c_str(),"",32,-0.5,31.5,10000,0,10);
        EBCSum[telescopeNumber]->GetXaxis()->SetTitle("Strip Num");
        EBCSum[telescopeNumber]->GetYaxis()->SetTitle("Energy [channel]");

        outstring.str("");
        outstring << "EBSum_" << telescopeNumber;
        name = outstring.str();
        dirSum->cd();
        EBSum[telescopeNumber] = new TH2I(name.c_str(),"",32,-0.5,31.5,1000,0,500);
        EBSum[telescopeNumber]->GetXaxis()->SetTitle("Strip Num");
        EBSum[telescopeNumber]->GetYaxis()->SetTitle("Energy [channel]");

        for (int istrip=0;istrip<NUMBER_OF_STRIPS;istrip++)
        {
            outstring.str("");
            outstring << "EF"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirFrontRaw->cd();
            EfrontR[telescopeNumber][istrip] = new TH1I(name.c_str(),"",4000,0,16000);
            EfrontR[telescopeNumber][istrip]->GetXaxis()->SetTitle("Energy High Gain[channel]");

            outstring.str("");
            outstring << "EFLG"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirFrontLGR->cd();
            EfrontLR[telescopeNumber][istrip] = new TH1I(name.c_str(),"",4000,0,16000);
            EfrontLR[telescopeNumber][istrip]->GetXaxis()->SetTitle("Energy Low Gain [channel]");

            outstring.str("");
            outstring << "EFC"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirFrontCal->cd();
            EfrontC[telescopeNumber][istrip] = new TH1I(name.c_str(),"",4000,0,500);
            EfrontC[telescopeNumber][istrip]->GetXaxis()->SetTitle("Energy [MeV]");

            outstring.str("");
            outstring << "EFLC"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirFrontLGC->cd();
            EfrontLC[telescopeNumber][istrip] = new TH1I(name.c_str(),"",4000,0,500);
            EfrontLC[telescopeNumber][istrip]->GetXaxis()->SetTitle("Energy [MeV]");

            outstring.str("");
            outstring << "TF"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirFrontTime->cd();
            TfrontR[telescopeNumber][istrip] = new TH1I(name.c_str(),"",2000,0,16000);
            TfrontR[telescopeNumber][istrip]->GetXaxis()->SetTitle("time [channels]");

            outstring.str("");
            outstring << "EB"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirBackRaw->cd();
            EbackR[telescopeNumber][istrip] = new TH1I(name.c_str(),"",4000,0,16000);
            EbackR[telescopeNumber][istrip]->GetXaxis()->SetTitle("Energy High Gain[channel]");

            outstring.str("");
            outstring << "EBLG"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirBackLGR->cd();
            EbackLR[telescopeNumber][istrip] = new TH1I(name.c_str(),"",4000,0,16000);
            EbackLR[telescopeNumber][istrip]->GetXaxis()->SetTitle("Energy Low Gain[channel]");

            outstring.str("");
            outstring << "EBC"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirBackCal->cd();
            EbackC[telescopeNumber][istrip] = new TH1I(name.c_str(),"",4000,0,500);
            EbackC[telescopeNumber][istrip]->GetXaxis()->SetTitle("Energy [MeV]");

            outstring.str("");
            outstring << "EBLC"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirBackLGC->cd();
            EbackLC[telescopeNumber][istrip] = new TH1I(name.c_str(),"",4000,0,500);
            EbackLC[telescopeNumber][istrip]->GetXaxis()->SetTitle("Energy [MeV]");

            outstring.str("");
            outstring << "TB"<<telescopeNumber<<"_"<<istrip;
            name = outstring.str();
            dirBackTime->cd();
            TbackR[telescopeNumber][istrip] = new TH1I(name.c_str(),"",2000,0,16000);
            TbackR[telescopeNumber][istrip]->GetXaxis()->SetTitle("time [channels]");
        }
    }

}
//*********************************************
void histo_sort::write()
{
    file->Write();

    cout << "histo written" << endl;
}
