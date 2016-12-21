#include "../../include/unpacker/Datum.h"

#include <iostream>

using namespace std;

void Datum::print(ofstream& outFile)
{
    outFile << name << " = " << value << endl;
}
