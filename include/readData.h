#ifndef READ_DATA_H
#define READ_DATA_H

#include <fstream>
#include <vector>

bool readWord(std::ifstream& evtfile, std::vector<char> variable);
bool readWord(std::ifstream& evtfile, std::vector<unsigned short> variable);
bool readWord(std::ifstream& evtfile, std::vector<unsigned int> variable);
bool readWord(std::ifstream& evtfile, std::vector<double> variable);

#endif
