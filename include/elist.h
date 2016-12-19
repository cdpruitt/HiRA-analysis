#include <string>
#include "detector/geometry.h"

using namespace std;

struct order
{
  float energy;
  float energyR;  // high gain channels
  float energyRlow;  //low channels
  float energylow;
  float energyMax;
  int strip;
  int overflow;
  int neighbours;
  float time;
};

/**
 * !\brief Energy ordered list
 *
 * This class creates an energy ordered list of the strips
 * read out from a strip detector, keeping track of the strip 
 * numbers that fired.
 */

class elist
{
 public:
  int Nstore = 0; //number stored in list
  order Order[NUMBER_OF_STRIPS];
  int mult;
  void Add(int,int,float,float);
  void Add(int,int,float,float,float);
  void Add(int,float,float,float,float);
  void Add(int, float, int, int);
  int  Reduce(char*);
  void reset();
  void Neighbours(string);
};
