#ifndef einstein_
#define einstein_
#include "kinematics.h"
#include <cmath>
#include <iostream>

/**
 * This class performs relativistics kinematics
 */

class CEinstein : public CKinematics
{
    public:
        float const scale;

        CEinstein();
        float getMomentum(float eKin,float mass);
        float getKE(float pc,float mass);
        float transformMomentum(float* mom,float* vreference,float energyTot,
                float*momNew);
        float gamma(float vel);
};
#endif 
