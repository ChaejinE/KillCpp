#include <iostream>
#include <string>

class Base
{
    std::string s;

    public:
        Base() : s("기반") { std::cout << "기반 클래스" << std::endl; }
        
        virtual void what() { std::cout << s << std::endl; }
};

class Derived : public Base
{
    std::string s;

    public:
        Derived() : s("파생"), Base() { std::cout << "파생 클래스" << std::endl; }

        void what() { std::cout << s << std::endl; }
};

int main()
{
    Base p;
    Derived c;

    Base* p_c = &c;
    Base* p_p = &p;

    std::cout << "Real Object : Base" << std::endl;
    p_p->what();

    std::cout << "Real Object : Derived " << std::endl;
    p_c->what();

    return 0;
}