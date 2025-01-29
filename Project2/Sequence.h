// Author: Daniel Wu, 1/28/2025

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string>
using ItemType = std::string;

class Sequence
{
    public:
    Sequence(int items = 0);
    Sequence(const Sequence& other);  
    Sequence& operator=(const Sequence& rhs);
    ~Sequence();

    bool empty() const;
    int size() const;  
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);

    private:
        struct Node
        {
            ItemType value;
            Node* next;
            Node* prev;
        };
        Node* head;        
        int sizeS;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void zipper(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif