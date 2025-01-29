#include "Sequence.h"
#include <iostream>
using namespace std;

void testSequence() {
    Sequence s;
    cout << "Testing empty: " << (s.empty() ? "PASS" : "FAIL") << endl;
    cout << "Testing size (expect 0): " << (s.size() == 0 ? "PASS" : "FAIL") << endl;
    
    s.insert(0, "A");
    cout << "Testing insert at 0: " << (s.size() == 1 ? "PASS" : "FAIL") << endl;
    
    s.insert(1, "B");
    s.insert(2, "C");
    cout << "Testing insert at multiple positions: " << (s.size() == 3 ? "PASS" : "FAIL") << endl;
    
    ItemType value;
    s.get(1, value);
    cout << "Testing get (expect B): " << (value == "B" ? "PASS" : "FAIL") << endl;
    
    s.set(1, "X");
    s.get(1, value);
    cout << "Testing set (expect X): " << (value == "X" ? "PASS" : "FAIL") << endl;
    
    s.erase(1);
    cout << "Testing erase (size should be 2): " << (s.size() == 2 ? "PASS" : "FAIL") << endl;
    
    s.insert("D");
    s.insert("E");
    cout << "Testing insert sorted order: " << (s.size() == 4 ? "PASS" : "FAIL") << endl;
    
    cout << "Testing find D: " << (s.find("D") != -1 ? "PASS" : "FAIL") << endl;
    cout << "Testing find Z (not found case): " << (s.find("Z") == -1 ? "PASS" : "FAIL") << endl;
    
    cout << "Testing remove D: " << (s.remove("D") == 1 ? "PASS" : "FAIL") << endl;
    cout << "Testing remove non-existent: " << (s.remove("Z") == 0 ? "PASS" : "FAIL") << endl;
    
    Sequence s2;
    s2.insert("X");
    s2.insert("Y");
    s2.insert("Z");
    
    s.swap(s2);
    cout << "Testing swap (size check): " << (s.size() == 3 && s2.size() == 3 ? "PASS" : "FAIL") << endl;
}

void testSubsequence() {
    Sequence s1, s2;
    s1.insert(0, "A");
    s1.insert(1, "B");
    s1.insert(2, "C");
    s1.insert(3, "D");
    
    s2.insert(0, "B");
    s2.insert(1, "C");
    
    cout << "Testing subsequence (expect 1): " << (subsequence(s1, s2) == 1 ? "PASS" : "FAIL") << endl;
}

void testZipper() {
    Sequence s1, s2, result;
    s1.insert(0, "A");
    s1.insert(1, "C");
    s1.insert(2, "E");
    
    s2.insert(0, "B");
    s2.insert(1, "D");
    s2.insert(2, "F");
    
    zipper(s1, s2, result);
    
    ItemType value;
    bool pass = true;
    string expected[] = {"A", "B", "C", "D", "E", "F"};
    for (int i = 0; i < 6; i++) {
        result.get(i, value);
        if (value != expected[i]) {
            pass = false;
            break;
        }
    }
    cout << "Testing zipper: " << (pass ? "PASS" : "FAIL") << endl;
}

int main() {
    testSequence();
    testSubsequence();
    testZipper();
    return 0;
}
