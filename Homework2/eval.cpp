#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool validInfix(string &infix)
{
    if(infix.size()==0){return false;}
    string n_infix;
    for(int i = 0; i < infix.size(); i++)
    {
        if(infix[i] != ' '){n_infix += infix[i];}
    }

    int paran = 0;
    bool op = true;
    bool lastD = false;
    for(int i = 0; i < n_infix.size(); i++)
    {
        char ch = n_infix[i];
        if(isdigit(ch))
        {
            if(i > 0 && n_infix[i-1] == ')'){return false;}
            if(lastD){return false;} op = false; lastD = true;
        }
        else if (ch == '(')
        {
            if(i > 0 && (isdigit(n_infix[i-1]) || n_infix[i-1] == ')')){return false;}
            paran++; op = true; lastD = false;
        }
        else if (ch == ')')
        {
            if(paran == 0){return false;}
            paran--; if(op){return false;} op = false; lastD= false;
        }
        else if(ch == '!' || ch == '&' || ch == '|')
        {
            if(op == true && ch != '!'){return false;}
            op = true;
            lastD = false;
        }else {return false;}
    }
    if(paran != 0){return false;}
    return !op;
}

int order(char op) {
    if (op == '!') return 3; 
    if (op == '&') return 2;
    if (op == '|') return 1; 
    return 0; 
}

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    // Evaluates a boolean expression
    //   If infix is a syntactically valid infix boolean expression,
    //   then set postfix to the postfix form of that expression, set
    //   result to the value of the expression (where in that expression,
    //   each digit k represents element k of the values array), and
    //   return zero.  If infix is not a syntactically valid expression,
    //   return 1.  (In that case, postfix may or may not be changed, but
    //   result must be unchanged.)
    string n_infix = infix;
    if(!validInfix(infix)){return 1;}

    stack<char> opStack;
    postfix = "";

    for(int i = 0; i < n_infix.size(); i++)
    {
        char ch = n_infix[i];
        if(isdigit(ch)){postfix += ch;}
        else{
            switch(ch)
            {
                case '(':
                    opStack.push(ch);
                    break;
                case ')':
                    while(!opStack.empty() && opStack.top() != '(')
                    {
                        postfix += opStack.top();
                        opStack.pop();
                    }
                    opStack.pop();
                    break;
                case '!':
                case '&':
                case '|':
                    while(!opStack.empty() && order(opStack.top()) >= order(ch))
                    {
                        postfix += opStack.top();
                        opStack.pop();
                    }
                    opStack.push(ch);
                    break;
            }
        }
    }
    while(!opStack.empty())
    {
        postfix += opStack.top();  
        opStack.pop();
    }

    stack<bool> oppStack;
    for(int i = 0; i < postfix.size(); i++)
    {
        char ch = postfix[i];
        if(isdigit(ch))
        {
            int index = ch - '0';
            bool opVal = values[index];
            oppStack.push(opVal);
        } else if (ch == '!') {
            bool temp = oppStack.top();
            oppStack.pop();
            oppStack.push(!temp);
        } else {
            bool temp = oppStack.top();
            oppStack.pop();
            bool temp2 = oppStack.top();
            oppStack.pop();
            bool result;
            if(ch == '&'){result = temp && temp2;}
            else if(ch == '|'){result = temp || temp2;}
            oppStack.push(result);
        }
    }
    result = oppStack.top();
    return 0;
}

int main(){
   bool ba[10] = {
       //  0      1      2      3      4      5      6      7      8      9
       true,  true,  true,  false, false, false, true,  false, true,  false
   };
   string pf;
   bool answer;
   assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
   assert(evaluate("", ba, pf, answer) == 1);
   assert(evaluate("8|", ba, pf, answer) == 1);
   assert(evaluate(" &6", ba, pf, answer) == 1);
   assert(evaluate("4 5", ba, pf, answer) == 1);
   assert(evaluate("01", ba, pf, answer) == 1);
   assert(evaluate("()", ba, pf, answer) == 1);
   assert(evaluate("()4", ba, pf, answer) == 1);
   assert(evaluate("2(9|8)", ba, pf, answer) == 1);
   assert(evaluate("2(&8)", ba, pf, answer) == 1);
   assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
   assert(evaluate("x+5", ba, pf, answer) == 1);
   assert(evaluate("2|3|4", ba, pf, answer) == 0
          &&  pf == "23|4|"  &&  answer);
   assert(evaluate("2|(3|4)", ba, pf, answer) == 0
          &&  pf == "234||"  &&  answer);
   assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
          &&  pf == "43!03&&|"  &&  !answer);
   assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
   assert(evaluate("((6))", ba, pf, answer) == 0  &&  pf == "6"  &&  answer);
   ba[2] = false;
   ba[9] = true;
   assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
   assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
   cout << "Passed all tests" << endl;
   
   return 0;
}