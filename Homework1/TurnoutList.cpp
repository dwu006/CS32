#include "TurnoutList.h"

#include <iostream>
using namespace std;

TurnoutList::TurnoutList():s(){}

bool TurnoutList::add(unsigned long turnout)
{
    if(turnout > 1800){return false;}
    return s.insert(turnout) != -1;
}

bool TurnoutList::remove(unsigned long turnout)
{
    int pos = s.find(turnout);
    if(pos < 0){return false;}
    s.erase(pos);
    return true;
}

int TurnoutList::size() const {return s.size();}

unsigned long TurnoutList::minimum() const
{
    if(!size()){return NO_TURNOUTS;}
    unsigned long min = 0;
    s.get(0, min);
    for(int i = 0; i < size(); i++)
    {
        unsigned long temp = 0;
        s.get(i, temp);
        if(temp < min){min = temp;}
    }
    return min;
}

unsigned long TurnoutList::maximum() const
{
    if(!size()){return NO_TURNOUTS;}
    unsigned long max = 0;
    s.get(0, max);
    for(int i = 0; i < size(); i++)
    {
        unsigned long temp = 0;
        s.get(i, temp);
        if(temp > max){max = temp;}
    }
    return max;
}