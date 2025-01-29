#ifndef NEW_SEQUENCE_H
#define NEW_SEQUENCE_H

#include <string>
typedef unsigned long ItemType;

const int DEFAULT_MAX_ITEMS = 140;

class Sequence
{
    public:
    Sequence(int items = DEFAULT_MAX_ITEMS);
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
        ItemType* arr;
        int arrSize;
        int maxSize;
};

#endif