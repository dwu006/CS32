#include "newSequence.h"
#include <iostream>

Sequence::Sequence(int items)
{
    if(items < 0){std::cout << "Error:Negative Size" << std::endl; exit(1);}
    maxSize = items;
    arrSize = 0;
    arr = new ItemType[maxSize];
}

Sequence::Sequence(const Sequence& other)
{
    maxSize = other.maxSize;
    arrSize = other.arrSize;
    arr = new ItemType[maxSize];
    for(int i = 0; i < arrSize; i++){arr[i] = other.arr[i];}
}

Sequence& Sequence::operator=(const Sequence& rhs)
{
    if(this == &rhs){return *this;}
    delete[] arr;
    maxSize = rhs.maxSize;
    arrSize = rhs.arrSize;
    arr = new ItemType[maxSize];
    for(int i = 0; i < arrSize; i++){arr[i] = rhs.arr[i];}
    return *this;
}

Sequence::~Sequence()
{
    delete[] arr;
}

bool Sequence::empty() const{return arrSize == 0;}

int Sequence::size() const {return arrSize;}

int Sequence::insert(int pos, const ItemType& value)
{
    if(pos < 0 || pos > arrSize || arrSize == maxSize){return -1;}
    for(int i = arrSize; i > pos; i--){arr[i] = arr[i-1];}
    arr[pos] = value;
    arrSize++;
    return pos;
}

int Sequence::insert(const ItemType& value)
{
    if(arrSize == maxSize){return -1;}
    int p = arrSize;
    for(int i = arrSize-1; i >= 0; i--){if(arr[i] >= value){p = i;}}
    return insert(p, value);
}

bool Sequence::erase(int pos)
{
    if (pos < 0 || pos >= arrSize){return false;}
    for(int i = pos; i < arrSize-1; i++){arr[i] = arr[i + 1];}
    arrSize--;
    return true;
}

int Sequence::remove(const ItemType& value)
{
    int count = 0;
    for(int i = 0; i < arrSize;){if(arr[i] == value){erase(i); count++;}else{i++;}}
    return count;
}

bool Sequence::get(int pos, ItemType& value) const
{
    if (pos < 0 || pos >= arrSize){return false;}
    value = arr[pos];
    return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
    if (pos < 0 || pos >= arrSize){return false;}
    arr[pos] = value;
    return true;
}

int Sequence::find(const ItemType& value) const
{
    int p = -1;
    for(int i = arrSize-1; i >= 0; i--){if(arr[i] == value){p = i;}}
    return p;
}

void Sequence::swap(Sequence& other)
{
    ItemType* tempArr = arr;
    arr = other.arr;
    other.arr = tempArr;

    int tempSize = arrSize;
    arrSize = other.arrSize;
    other.arrSize = tempSize;

    int tempMax = maxSize;
    maxSize = other.maxSize;
    other.maxSize = tempMax;
}