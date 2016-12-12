#include "../include/solution.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

void solution::SetTargetDistanceBe()
{
  distTarget  = 14.85;
}

void solution::SetTargetDistanceAlpha()
{
  distTarget  = 13.45;
}

void solution::SetTargetDistanceC()
{
  distTarget  = 14.85 - .355;
}

float solution::angle()
{
  float XYZ2 = pow(Xpos , 2) + pow(Ypos, 2) + pow(distTarget,2);
  theta = acos(distTarget/sqrt(XYZ2));
  phi = atan2(Ypos , Xpos);

  return theta;
}
//********************************************************
void solution::getMomentum()
{

  if (ipid == 1) A = 1;
  else if (ipid == 2)A = 4; 
  else if (ipid == 3)A = 2;
  else if (ipid == 4)A = 3;  
  else if (ipid == 5)A = 7;
  else if (ipid == 6)A = 6;  
  else if (ipid == 7)A = 6;
  else if (ipid == 8)A = 7;
  else if (ipid == 9)A = 9;
  else if (ipid == 10) A = 8;
  else if (ipid == 11) A = 3;
  else if (ipid == 0) return;

  mass = Kinematics.nMass * (float)A;
  momentum = Kinematics.getMomentum(Ekin,mass);
  Mvect[0] = momentum*sin(theta)*cos(phi);
  Mvect[1] = momentum*sin(theta)*sin(phi);
  Mvect[2] = momentum*cos(theta);
  

  //scale = 1 einstein, 0 for newton
  energyTot = Ekin*Kinematics.scale + mass;

}
