#ifndef DATUM_H
#define DATUM_H

#include <string>
#include <vector>

class Datum
{
    public:
        Datum(std::string n, unsigned int bitM, unsigned int bitS) :
            name(n),
            bitMask(bitM),
            bitShift(bitS)
        {}

        void readValue(unsigned int word)
        {
            value = (word >> bitShift) & bitMask;
        }

        const std::string name;
        const unsigned int bitShift;
        const unsigned int bitMask;

        unsigned int value;
};

#endif