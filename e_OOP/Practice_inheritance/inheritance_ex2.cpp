#include <iostream>
#include <string>

class Base
{
    std::string s;

    public:
        Base() : s("base") { std::cout << "base class" << std::endl; }

        void what() { std::cout << s << std::endl; }
};

class Derived : public Base
{
    std::string s;

    public:
        Derived() : Base(), s("derived") 
        { 
            std::cout << "derived class" << std::endl; 
            what();    
        }

        void what() { std::cout << s << std::endl; }
};

int main()
{
    std::cout << " === Base Class === " << std::endl;
    Base p;

    std::cout << " === Deri Class === " << std::endl;
    Derived c;

    return 0;
}