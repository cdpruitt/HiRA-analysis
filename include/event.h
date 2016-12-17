#ifndef EVENT_H
#define EVENT_H

#include <fstream>

/******************************************************************************/
/* Event structure */

struct Event
{
    Event(int s) : size(s) {}

    struct Quantity // the smallest granularity of event data
        // (e.g., time, charge, module address are all Quantities)
    {
        Quantity(
                const std::string n,
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
    };

    struct Section // a portion of an event (e.g., header, body, footer)
    {
        Section(Quantity ID, unsigned int s) : identifier(ID), size(s) {}

        Quantity identifier; 
        unsigned int size; // size of section, in words

        bool checkID(unsigned int word) // test to see if a word has this
            // Section's unique ID
        {
            return (identifier.value == ((word >> identifier.bitShift) & identifier.bitMask));
        }

        virtual void extractData(unsigned int word);

    };

    /*class QuantityGroup // allows grouping of Quantities
    // (e.g., each channel of an event may have a group
    // of quantities associated with it, like time,
    // integrated charge, etc)
    {
    void read(unsigned int word) // read the quantity group from a buffer
    }*/

    virtual bool readEvent(std::ifstream& evtfile) = 0;

    unsigned int size; // the number of total words in the event
};

#endif
