#include "Sequence.h"
#include <iostream>

Sequence::Sequence():arr(), arrSize(0){}

bool Sequence::empty() const{return arrSize == 0;}

int Sequence::size() const {return arrSize;}

int Sequence::insert(int pos, const ItemType& value)
{
    if(pos < 0 || pos > arrSize || arrSize == DEFAULT_MAX_ITEMS){return -1;}
    for(int i = arrSize; i > pos; i--){arr[i] = arr[i-1];}
    arr[pos] = value;
    arrSize++;
    return pos;
}

int Sequence::insert(const ItemType& value)
{
    if(arrSize == DEFAULT_MAX_ITEMS){return -1;}
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
    ItemType temp1, temp2;
    int thisSize = size();
    int otherSize = other.size();
    int minSize = (thisSize < otherSize) ? thisSize : otherSize;

    for (int i = 0; i < minSize; i++)
    {
        get(i, temp1);
        other.get(i, temp2);
        set(i, temp2);
        other.set(i, temp1);
    }

    if (thisSize > otherSize)
    {
        for (int i = minSize; i < thisSize; i++)
        {
            get(minSize, temp1);
            erase(minSize);          
            other.insert(other.size(), temp1);  
        }
    }
    else if (otherSize > thisSize)
    {
        for (int i = minSize; i < otherSize; i++)
        {
            other.get(minSize, temp1);
            other.erase(minSize);
            insert(size(), temp1);
        }
    }
}
