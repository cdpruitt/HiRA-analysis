#ifndef CAEN
#define CAEN
#include <cstdlib>
#include <vector>

/**
 * !\brief handles the readout from a CAEN ADC,QDC,TDC
 *
 * This class deals with the read out from a number of 
 * CAEN VME modules
 */

class caen
{
 public:
  unsigned short channelsHit; // number of converted channels
  unsigned short crate; // crate number
  unsigned short geographic; // geographic address of module
  std::vector<ADCchannel> events; // event data

  unsigned long* read(unsigned long *);
};
#endif
