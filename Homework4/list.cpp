#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Class
{
  public:
    Class(string nm) : m_name(nm) {}
    string name() const { return m_name; }
    const vector<Class*>& subclasses() const { return m_subclasses; }
    void add(Class* d) { m_subclasses.push_back(d); }
    ~Class();
  private:
    string m_name;
    vector<Class*> m_subclasses;
};

Class::~Class()
{
    for (size_t k = 0; k < m_subclasses.size(); k++)
        delete m_subclasses[k];
}

void listAll(string path, const Class* c)  // two-parameter overload
{
    if(c == nullptr){return;}
    string newPath = path.empty() ? c->name() : path + "=>" + c->name();  
    cout << newPath << endl;
    for (const Class* subclass : c->subclasses()) {
        listAll(newPath, subclass);
    }
}

void listAll(const Class* c)  // one-parameter overload
{
    if (c != nullptr)
        listAll("", c);
}

int main()
{
    Class* d1 = new Class("Koopa");
    listAll(d1);
    cout << "====" << endl;

    d1->add(new Class("SoopaKoopa"));
    Class* d2 = new Class("Enemy");
    d2->add(new Class("Fireball"));
    d2->add(d1);
    Class* d3 = new Class("Goodie");
    d3->add(new Class("Garlic"));
    d3->add(new Class("Beans"));
    d3->add(new Class("ExtraLife"));
    listAll(d3);
    cout << "====" << endl;

    Class* d4 = new Class("Actor");
    d4->add(d2);
    d4->add(new Class("Player"));
    d4->add(d3);
    listAll(d4);
    delete d4;
}