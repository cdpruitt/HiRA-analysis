#ifndef UNPACKER_CONSTANTS_H
#define UNPACKER_CONSTANTS_H

const vector<unsigned short> XLM_MARKER = {0x1ff3, 0x2ff3, 0x3ff3};

// HINP4 constants
const unsigned int STRIPS_PER_CHIP = 16;

const unsigned int CHIPS_PER_CHIPBOARD = 2;
const unsigned int STRIPS_PER_CHIPBOARD = STRIPS_PER_CHIP*CHIPS_PER_CHIPBOARD;

const unsigned int NUMBER_OF_MBS = 2;
const unsigned int CHIPBOARDS_PER_MB = 12;
const unsigned int STRIPS_PER_MB = STRIPS_PER_CHIPBOARD*CHIPBOARDS_PER_MB;

// CsI constants
const unsigned int NUMBER_OF_ADCS = 2;

#endif
