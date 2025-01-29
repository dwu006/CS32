#ifndef TURNOUTLIST_H
#define TURNOUTLIST_H

#include "Sequence.h"
#include <limits>

const unsigned long NO_TURNOUTS = std::numeric_limits<unsigned long>::max();

class TurnoutList
{
    public:

    TurnoutList();
    bool add(unsigned long turnout);
    bool remove(unsigned long turnout);
    int size() const;
    unsigned long minimum() const;
    unsigned long maximum() const;

    private:
        Sequence s;
};

#endif