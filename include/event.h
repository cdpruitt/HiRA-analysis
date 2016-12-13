#ifndef EVENT_H
#define EVENT_H

#include <fstream>

/******************************************************************************/
/* Event structure */

class Event
{
    protected:
        class Quantity // a physical quantity that is part an event
            // (e.g., time, charge, module address are all quantities)
        {
            public:
            Quantity(const std::string n,
                    unsigned const int bM,
                    unsigned const int bS)
                : name(n), bitMask(bM), bitShift(bS) {}

            Quantity(
                    const std::string n,
                    unsigned const int bM,
                    unsigned const int bS,
                    unsigned const int v)
                : name(n), bitMask(bM), bitShift(bS), value(v) {}

            const std::string name; // human-readable name of quantity
            unsigned const int bitMask; // mask for extracting value from buffer
            unsigned const int bitShift; // shit for extracting value from buffer

            unsigned int value; // numerical value, as read from the buffer

            void read(unsigned int word) // read the quantity from a buffer word
            {
                value = (word >> bitShift) & bitMask;
            }

            bool check(unsigned int word) // test to see if value matches buffer
            {
                return (value == ((word >> bitShift) & bitMask));
            }
        };
};

#endif
