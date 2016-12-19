#ifndef IDENTIFIER_H
#define IDENTIFIER_H

class Identifier : public Datum
{
    public:
        Identifier(std::string n, unsigned int bitM, unsigned int bitS, unsigned int v) :
            Datum(n, bitM, bitS),
            ID(v)
        {}

        bool checkID(unsigned int word)
        {
            return ((word >> bitShift) & bitMask) == ID;
        }

        const unsigned int ID;
};

#endif
