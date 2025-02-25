class Landmark
{
    public:
        Landmark(std::string name):m_name(name){}
        virtual ~Landmark() {}
        std::string name() const {return m_name;}
        virtual std::string icon() const = 0;
        virtual std::string color() const {return "yellow";}
    private:
        std::string m_name;
};

class Hotel : public Landmark
{
    public:
        Hotel(std::string name):Landmark(name){}
        virtual ~Hotel() {std::cout << "Destroying the hotel " << name() << "." << std::endl;}
        virtual std::string icon() const {return "bed";}
};

class Restaurant : public Landmark
{
    public:
        Restaurant(std::string name, int capacity):Landmark(name), m_capacity(capacity) {}
        virtual ~Restaurant(){std::cout << "Destroying the restaurant " << name() << "." << std::endl;};
        virtual std::string icon() const 
        {
            if(m_capacity >= 40){return "large knife/fork";}
            else{return "small knife/fork";}
        }
    private:
        int m_capacity;
};

class Hospital : public Landmark
{
    public:
        Hospital(std::string name):Landmark(name){}
        virtual ~Hospital() {std::cout << "Destroying the hospital " << name() << "." << std::endl;}
        virtual std::string icon() const {return "H";}
        virtual std::string color() const {return "blue";}        
};