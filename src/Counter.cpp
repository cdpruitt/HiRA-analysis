#include "../include/Counter.h"

#include <iostream>

using namespace std;

Counter::Counter(string n) : name(n), count(0) {}

unsigned long Counter::getCount()
{
    return count;
}

void Counter::print()
{
    cout << name << " = " << count << endl;
}
