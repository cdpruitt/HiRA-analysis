#ifndef READ_DATA_H
#define READ_DATA_H

#include <fstream>

bool readNextWord(std::ifstream& evtfile, char* variable);
bool readNextWord(std::ifstream& evtfile, unsigned short* variable);
bool readNextWord(std::ifstream& evtfile, unsigned int* variable);
bool readNextWord(std::ifstream& evtfile, double* variable);

#endif
