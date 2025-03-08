void listAll(string path, const Class* c)
{
    if(c == nullptr){return;}
    string newPath = path.empty() ? c->name() : path + "=>" + c->name();  
    cout << newPath << endl;
    for (const Class* subclass : c->subclasses()) {
        listAll(newPath, subclass);
    }
}