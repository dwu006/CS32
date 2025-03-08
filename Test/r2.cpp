#include <iostream>
using namespace std;

class SeaAnimal
{
    public:
    SeaAnimal():m_weight(0){}
    virtual void makeNoise();
    int weight() const {return m_weight;}
    virtual void eat(SeaAnimal *s)
    {
        m_weight += s->weight();
        delete s;
    }

    private:
    int m_weight;


};

class Otters : public SeaAnimal
{
    public:
    Otters();
    void makeNoise()
    {
        cout << "Bark" << endl;
    }

};

class Squid : public SeaAnimal
{
    public:
    Squid():m_weight(1000){}
    void makeNoise()
    {
        cout << "Squeak" << endl;
    }
    private:
        int m_weight;
};

class GiantSquid : public SeaAnimal
{
    public:
    void eat(SeaAnimal *s){eat(s); cout << "Burp" << endl;}
};