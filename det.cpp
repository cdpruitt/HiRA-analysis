#include "det.h"

#include <fstream>
#include <iostream>

// Constructor
det::det(histo_sort * Histo0, forest * Forest0)
{
    type = 0;
    Histo_sort = Histo0;
    Forest = Forest0;
    ran = new TRandom;
    Hira = new hira(ran,Histo_sort);
}

det::det(histo_read * Histo1)
{
    type = 1;
    Histo_read = Histo1;
    ran = new TRandom;
    Hira = new hira(ran,Histo_read);
}

// Destructor
det::~det()
{
    delete Hira;
    delete ran;
    if (type == 0)delete Forest;
    cout << "number of good tree events " << ngood << endl;
    cout << "det removed" << endl;
}

/* unpacks a physics event from the data stream
 * param point0 - pointer to location in data stream
 */
bool det::unpack(unsigned short *point,int runno)
{
    Hira->reset();
    unsigned short  words = *point++;
    point++; //Reads b0fe
    Run = runno;
    return Hira->unpack(point,runno);
}

//*********************************

void det::analyze(int event)
{
    Correl.Reset();
    Eventt = event;
    float Nsol = 0;
    int mult = 0;
    CsImult = 0;

    for (int itele=0;itele<NUMBER_OF_TELESCOPES;itele++)
    {
        CsImult += Hira->Telescope[itele]->Csi.Nstore;

        if(Hira->Telescope[itele]->Csi.Nstore==0)
        {
            continue;
        }

        Hira->Telescope[itele]->analyze(Eventt);
        Nsol = Hira->Telescope[itele]->Nsolution;

        for(int ii=0;ii<Nsol;ii++)
        {
            if(Hira->Telescope[itele]->Solution[ii].iZ>0)
            {
                Correl.load(&Hira->Telescope[itele]->Solution[ii]);
                mult++;
            }
        }
    } 

    if(mult >= 2)
    {
        corr_6Li();
        corr_7Li();
        corr_7Be();
        corr_12C();
        corr_11O();
    }
}

void det::corr_11O()
{
    //2p+11O
    if (Correl.multProton == 2 && Correl.mult9C == 1)
    {
        Correl.zeroMask();
        Correl.maskProton[0]=1;
        Correl.maskProton[1]=1;
        Correl.mask9C[0]=1;   
        Correl.makeArray(1);
        float Erel_11O = Correl.findErel();

        float thetaCM = Correl.thetaCM;
        Histo_read->Erel_11O_2p9C->Fill(Erel_11O);
        Histo_read->vel_11O_2p9C->Fill(Correl.velocityCM);

        Correl.maskProton[1] = 0;
        Correl.makeArray(1);
        float ExY1 = Correl.findErel()/Erel_11O;

        Correl.maskProton[1] = 1;
        Correl.maskProton[0] = 0;
        Correl.makeArray(1);

        float ExY2 = Correl.findErel()/Erel_11O;

        Correl.maskProton[0] = 1;
        Correl.mask9C[0] = 0;

        Correl.makeArray(1);
        float ExT = Correl.findErel()/Erel_11O;

        Correl.mask9C[0] = 1;
        Correl.makeArray(1);
        Correl.findErel();
        Correl.getJacobi();

        float theta_k_T = Correl.cosThetaT;
        float theta_k_Y1 = Correl.cosThetaY[0];
        float theta_k_Y2 = Correl.cosThetaY[1];

        if(Erel_11O > REL_ENERGY_LOWER_BOUND && Erel_11O < REL_ENERGY_UPPER_BOUND) //these need to be changed
        {
            Histo_read->JacobiY_11O_2p9C->Fill(ExY1,theta_k_Y1);
            Histo_read->JacobiY_11O_2p9C->Fill(ExY2,theta_k_Y2);
            Histo_read->JacobiT_11O_2p9C->Fill(ExT,-theta_k_T);
            Histo_read->JacobiT_11O_2p9C->Fill(ExT,theta_k_T);
        }
    }
}

void det::corr_6Li()
{
    //d + alpha
    if(Correl.multAlpha ==1 && Correl.multH2 ==1)
    {
        Correl.zeroMask();
        Correl.maskAlpha[0]=1;
        Correl.mask2H[0]=1;   
        Correl.makeArray(1);
        float Erel_6Li = Correl.findErel();
        float thetaCM = Correl.thetaCM;
        float Ex = Erel_6Li + Q6Li;
        Histo_read->Erel_6Li->Fill(Erel_6Li);
    }
}

void det::corr_7Li()
{  
    //t + alpha
    if (Correl.multAlpha == 1 && Correl.multH3 == 1)
    {
        Correl.zeroMask();
        Correl.maskAlpha[0]=1;
        Correl.mask3H[0]=1;   
        Correl.makeArray(1);
        float Erel_7Li = Correl.findErel();
        float thetaCM = Correl.thetaCM;
        float Ex = Erel_7Li + Q7Li;
        Histo_read->Erel_7Li->Fill(Ex);
        Histo_read->vel_7Li->Fill(Correl.velocityCM);

        int alphatele = Correl.alpha[0]->itele;
        int trittele = Correl.H3[0]->itele;

        float totm = 0;
        for(int i =0;i<3;i++) totm += pow(Correl.alpha[0]->MomCM[i],2);
        totm = sqrt(totm);
        float costheta = Correl.H3[0]->MomCM[2]/totm;

        Histo_read->Costheta->Fill(costheta);

        if(alphatele == trittele)
        {

            if(costheta < -0.5)
                Histo_read->Erel_Back_Same->Fill(Ex);
            else if(costheta >0.5)
                Histo_read->Erel_For_Same->Fill(Ex);
            else
                Histo_read->Erel_Trans_Same->Fill(Ex);
        }

        else 
        {
            if(costheta < -0.5)
                Histo_read->Erel_Back_Diff->Fill(Ex);
            else if(costheta >0.5)
                Histo_read->Erel_For_Diff->Fill(Ex);
            else
                Histo_read->Erel_Trans_Diff->Fill(Ex);
        }   
    }
}

void det::corr_7Be()
{
    if (Correl.mult3He == 1 && Correl.multAlpha == 1) 
    {
        Correl.zeroMask();
        Correl.mask3He[0]=1;
        Correl.maskAlpha[0]=1;
        Correl.makeArray(1);
        float Erel_7Be = Correl.findErel();
        float Ex = Erel_7Be + Q7Be;
        Histo_read->vel_7Be_alpha3He->Fill(Correl.velocityCM);

        Histo_read->Ex7Be_alpha3He->Fill(Ex);
    }
}

void det::treeGrow()
{
    Forest->reset(); //clear the event here. reset function in forest, I think
    for (int itele = 0; itele<NUMBER_OF_TELESCOPES; itele++)
    {
        if(!(Hira->Telescope[itele]->Front.Nstore > 0
                    && Hira->Telescope[itele]->Back.Nstore > 0
                    && Hira->Telescope[itele]->Csi.Nstore > 0))
        {
            continue;
        }
        ngood++;

        for (int i=0; i < Hira->Telescope[itele]->Front.Nstore; i++)
        {
            Forest->event->frontE[Forest->event->nfront] = Hira->Telescope[itele]->Front.Order[i].energy;
            Forest->event->frontT[Forest->event->nfront] = Hira->Telescope[itele]->Front.Order[i].time;
            Forest->event->frontID[Forest->event->nfront] = NUMBER_OF_STRIPS*itele + Hira->Telescope[itele]->Front.Order[i].strip;
            Forest->event->nfront++;
        }

        for (int i=0; i < Hira->Telescope[itele]->Back.Nstore; i++)
        {
            Forest->event->backE[Forest->event->nback] = Hira->Telescope[itele]->Back.Order[i].energy;
            Forest->event->backT[Forest->event->nback] = Hira->Telescope[itele]->Back.Order[i].time;
            Forest->event->backID[Forest->event->nback] = NUMBER_OF_STRIPS*itele + Hira->Telescope[itele]->Back.Order[i].strip;
            Forest->event->nback++;
        }

        for (int i = 0; i < Hira->Telescope[itele]->Csi.Nstore; i++)
        {
            Forest->event->csiE[Forest->event->ncsi] = Hira->Telescope[itele]->Csi.Order[i].energy;
            Forest->event->csiER[Forest->event->ncsi] = Hira->Telescope[itele]->Csi.Order[i].energyR;
            Forest->event->csiT[Forest->event->ncsi] = Hira->Telescope[itele]->Csi.Order[i].time;
            Forest->event->csiID[Forest->event->ncsi] = CSIS_PER_TELESCOPE*itele + Hira->Telescope[itele]->Csi.Order[i].strip;
            Forest->event->ncsi++;
        }
    }

    Forest->tree->Fill();
}

void det::loadTree(Event *event)
{
    int itele = 0;
    int strip = 0;
    Hira->reset();

    for (int j=0; j<event->nfront; j++)
    {
        itele = event->frontID[j]/NUMBER_OF_STRIPS; //32 strips on the front
        strip = event->frontID[j]%NUMBER_OF_STRIPS;
        Hira->Telescope[itele]->Front.Add(strip, event->frontE[j], 0, event->frontT[j]);
    }

    for (int j=0; j<event->nback; j++)
    {
        itele = event->backID[j]/NUMBER_OF_STRIPS;
        strip = event->backID[j]%NUMBER_OF_STRIPS;
        Hira->Telescope[itele]->Back.Add(strip, event->frontE[j], 0, event->frontT[j]);
    }

    for (int j=0; j<event->ncsi; j++)
    {
        itele = event->csiID[j]/CSIS_PER_TELESCOPE; //CSIS_PER_TELESCOPE CsIs
        strip = event->csiID[j]%CSIS_PER_TELESCOPE;
        Hira->Telescope[itele]->Csi.Add(strip, event->csiE[j], event->csiER[j], event->csiT[j]);
    }
}
