#include "../include/einstein.h"
#include "../include/constants/physicalConstants.h"

using namespace std;

CEinstein::CEinstein():CKinematics(),scale(1.)
{
} 

//*********************************************************
float CEinstein::getMomentum(float eKin, float mass)
{
    float pc = sqrt(pow(eKin+mass,2) - pow(mass,2));
    return pc;
}

//*********************************************************
float CEinstein::getKE(float pc, float mass)
{
    float ek = sqrt(pow(pc,2) + pow(mass,2))-mass;
    return ek;
}

//********************************************************
/**
 *transform a momentum vector to new frame
 * and returns the new kinetic energy in MeV
 */
float CEinstein::transformMomentum(float* mom, float *Vreference, 
        float energyTot, float* momNew)
{
    //find momentum parallel and perpendicular to transfrom velocity
    float dot = 0.;
    float VVreference = 0.;
    float perpendicular[3];
    float parallel[3];
    float parallelOld = 0.;
    for (int i=0;i<3;i++) 
    {
        dot+= mom[i]*Vreference[i];
        VVreference += pow(Vreference[i],2);
    }

    for (int i=0;i<3;i++) 
    {
        parallel[i] = dot/VVreference*Vreference[i];
        perpendicular[i] = mom[i] - parallel[i];
        parallelOld += pow(parallel[i],2);
    }

    parallelOld = sqrt(parallelOld); // magnitude of parallel mometum
    VVreference = sqrt(VVreference); // magnitude of velocity shift

    //transform parallel component
    float gamma = 1./sqrt(1-pow(VVreference/SPEED_OF_LIGHT,2));
    float parallelNew = (parallelOld - energyTot*VVreference/SPEED_OF_LIGHT)*gamma;

    // add perpendicular and new parallel components
    for (int j=0;j<3;j++)
    {
        momNew[j] = perpendicular[j] + parallelNew/parallelOld*parallel[j];
    }

    float energyTotNew = gamma*(energyTot - VVreference*parallelOld/SPEED_OF_LIGHT);
    return energyTotNew;
}

//**************************************************
float CEinstein::gamma(float vel)
{
    return 1./sqrt(1-pow(vel/SPEED_OF_LIGHT,2));
}
