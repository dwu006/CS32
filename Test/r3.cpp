#include <iostream>
#include <stack>
#include <string>
#include <queue>
using namespace std;

bool balanced(string str, int depth)
{
    stack<char> charStack;
    queue<char> charQueue;
    for(int i = 0; i < str.size(); i++)
    {
        if(str[i] == '(' || str[i] == '{' || str[i] == '[')
        {
            charStack.push(str[i]);
        }
        if(str[i] == ')' || str[i] == '}' || str[i] == ']')
        {
            charQueue.push(str[i]);
        }
    }
    if(charStack.size() != charQueue.size())
    {
        return false;
    }
    int size = charStack.size();
    for(int i = 0; i < depth; i++)
    { 
        if(charStack.top() != charQueue.front()){return false;}
        charStack.pop();
        charQueue.pop();
    }
    return true;

}

int main()
{

}
