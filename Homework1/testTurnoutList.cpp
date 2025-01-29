#include "TurnoutList.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    TurnoutList t;

    // Test adding valid turnouts
    assert(t.add(1000)); // Valid turnout
    assert(t.add(1800)); // Valid turnout at upper boundary
    assert(t.add(0));    // Valid turnout at lower boundary
    assert(!t.add(2000)); // Invalid turnout (out of range)
    assert(!t.add(-1));   // Invalid turnout (negative value)

    // Test size
    assert(t.size() == 3); // 3 valid turnouts added

    // Test minimum
    assert(t.minimum() == 0); // Minimum should be 0

    // Test maximum
    assert(t.maximum() == 1800); // Maximum should be 1800

    // Test removing turnouts
    assert(t.remove(1000));   // Remove 1000
    assert(!t.remove(2000));  // 2000 doesn't exist
    assert(t.size() == 2);    // Size should be 2 after removal
    assert(t.minimum() == 0); // Minimum still 0
    assert(t.maximum() == 1800); // Maximum still 1800

    // Test removing all items
    assert(t.remove(0));       // Remove 0
    assert(t.remove(1800));    // Remove 1800
    assert(t.size() == 0);     // Now the list is empty
    assert(t.minimum() == NO_TURNOUTS); // Minimum should return NO_TURNOUTS
    assert(t.maximum() == NO_TURNOUTS); // Maximum should return NO_TURNOUTS

    // Edge case: adding and removing repeatedly
    for (unsigned long i = 0; i < 100; i++)
        assert(t.add(i)); // Add numbers 0 to 99
    for (unsigned long i = 0; i < 100; i++)
        assert(t.remove(i)); // Remove numbers 0 to 99
    assert(t.size() == 0); // List should be empty again

}