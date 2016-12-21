#ifndef COUNTER_H
#define COUNTER_H

#include <string>

class Counter
{
    public:
        Counter(std::string n);
        unsigned long getCount();
        void print();

    private:
        std::string name;
        unsigned long count;
};

#endif
