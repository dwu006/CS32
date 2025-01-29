// Author: Daniel Wu, 1/28/2025

#include "Sequence.h"
#include <iostream>

Sequence::Sequence(int items) // main constructor to initialize class
{
    head = new Node;
    head->next = nullptr;
    head->prev = nullptr;
    sizeS = items;
}

Sequence::Sequence(const Sequence& other) // copy constructor
{
    head = new Node;
    head->next = nullptr;
    head->prev = nullptr;
    sizeS = 0;

    for (int i = 0; i < other.sizeS; i++) 
    {
        ItemType value;
        other.get(i, value); 
        insert(i, value);  
    }
}

Sequence& Sequence::operator=(const Sequence& rhs) // assignment constructor
{
    if(this == &rhs){return *this;}
    Sequence temp;
    for(int i = 0; i < rhs.sizeS; i++)
    {
        ItemType tempValue;
        rhs.get(i, tempValue);
        temp.insert(i, tempValue);
    }
    swap(temp);
    return *this;
}

Sequence::~Sequence() //destructor
{
    for(int i = 0; i < sizeS;){erase(i);}
    delete head;
}

bool Sequence::empty() const{return sizeS == 0;} // returns true if sequence is empty

int Sequence::size() const{return sizeS;} // returns the number of items in sequence

int Sequence::insert(int pos, const ItemType& value) // inserts value at pos in insert
{
    if(pos < 0 || pos > sizeS){return -1;}
    Node *temp = new Node;
    Node *current = head;
    for(int i = 0; i < pos; i++){current = current->next;}
    if(current->next != nullptr)
    {
        temp->next = current->next;
        current->next->prev = temp;
    } else {
        temp->next = nullptr;
    }
    current->next = temp;
    temp->value = value;
    temp->prev = current;
    sizeS++;
    return pos;
}

/*
Let p be the smallest integer such that value <= the item at
position p in the sequence. reuturn pos inserted 
*/
int Sequence::insert(const ItemType& value)
{
    Node *temp = new Node;
    Node *current = head;
    int pos = 0;
    while(current->next != nullptr && value >= current->next->value)
    {
        current = current->next;
        pos++;
    }
    if(current->next != nullptr)
    {
        temp->next = current->next;
        current->next->prev = temp;
    } else {temp->next = nullptr;}
    current->next = temp;
    temp->value = value;
    temp->prev = current;
    sizeS++;
    return pos;
}

/*
ensures pos is greater than 0 and not larger than size.
removes node at pos and adjust other pointers
*/
bool Sequence::erase(int pos)
{
    if (pos < 0 || pos >= sizeS){return false;}
    Node *temp = head;
    for(int i = 0; i < pos; i++){temp = temp->next;}
    Node *del = temp->next;
    if(del->next != nullptr)
    {
        temp->next = del->next;
        del->next->prev = temp;
    } else {head->next = nullptr;}
    delete del;
    sizeS--;
    return true;
}

/*
erase items from the sequence that are equal to value
return num of items removed
*/
int Sequence::remove(const ItemType& value)
{
    int count = 0;
    Node *temp = head;
    int i = 0;
    while(i < sizeS)
    {
        if(temp->next->value == value){erase(i); count++;}
        else{temp = temp->next; i++;}
    }
    return count;
}

/*
ensures pos is greater than 0 and not larger than size.
copies item value at pos into value
*/
bool Sequence::get(int pos, ItemType& value) const
{
    if (pos < 0 || pos >= sizeS){return false;}
    Node* temp = head->next;
    for(int i = 0; i < pos; i++){temp = temp->next;}
    value = temp->value;
    return true;
}
/*
ensures pos is greater than 0 and not larger than size.
replaces item at pos with value and return true else return false
*/
bool Sequence::set(int pos, const ItemType& value)
{
    if (pos < 0 || pos >= sizeS){return false;}
    Node *temp = head->next;
    for(int i = 0; i < pos; i++){temp = temp->next;}
    temp->value = value;
    return true;
}

/*
find the smallest pos where item at pos is equal to value
*/
int Sequence::find(const ItemType& value) const
{
    Node *temp = head->next;
    for(int i = 0; i < sizeS; i++)
    {
        if(temp->value == value){return i;}
        temp = temp->next;
    }
    return -1;
}

// swaps nodes
void Sequence::swap(Sequence& other)
{
    if(this == &other){return;}
    std::swap(sizeS, other.sizeS);
    std::swap(head, other.head);
}

/*
Consider all the items in seq2; let's call them seq20, seq21, ..., seq2n. 
If there exists at least one k such that seq1k == seq20 and seq1k+1 == 
seq21 and ... and seq1k+n == seq2n, and k+n < seq1.size(), then this 
function returns the smallest such k. 
*/
int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    if(seq2.size() == 0 || seq1.size() < seq2.size()){return -1;}
    for(int i = 0; i <= seq1.size() - seq2.size(); i++)
    {
        bool start = true;        
        for(int j = 0; j < seq2.size(); j++)
        {
            ItemType temp1, temp2;
            seq1.get(i + j, temp1);
            seq2.get(j, temp2);

            if(temp1 != temp2)
            {
                start = false;
                break;
            }
        }
        
        if(start){return i;}
    }
    return -1;
}

/*
This function produces as a result a sequence that consists of the first 
item in seq1, then the first in seq2, then the second in seq1, then the
 second in seq2, etc., rather like the way the teeth of a zipper interlock.
*/
void zipper(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    result = Sequence();
    int s1 = seq1.size();
    int s2 = seq2.size();
    int minS = (s1 > s2) ? s2 : s1;

    for(int i = 0; i < minS; i++)
    {
        ItemType temp;
        seq1.get(i, temp);
        result.insert(result.size(), temp);
        seq2.get(i, temp);
        result.insert(result.size(), temp);
    }

    if(s1 > s2)
    {
        for (int i = minS; i < s1; i++) {
            ItemType temp;
            seq1.get(i,temp);
            result.insert(result.size(), temp);
        }
    } else if (s1 < s2) {
        for (int i = minS; i < s2; i++) {
            ItemType temp;
            seq2.get(i,temp);
            result.insert(result.size(), temp);
        }
    } 
}