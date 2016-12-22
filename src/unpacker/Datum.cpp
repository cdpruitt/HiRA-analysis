#include "../../include/unpacker/Datum.h"

#include <iostream>

using namespace std;

void Datum::readValue(unsigned int word)
{
    value = (word >> bitShift) & bitMask;
}

void Datum::print(ofstream& outFile)
{
    outFile << name << " = " << value << endl;
}
