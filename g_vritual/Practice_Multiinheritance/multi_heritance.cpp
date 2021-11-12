#include <iostream>

class A
{
    public:
        int a;
        A() { std:: cout << "A 생성자 " << std::endl; }
        virtual ~A() { std:: cout << "A 소멸자 " << std::endl; }
};

class B
{
    public:
        int b;
        B() { std:: cout << "B 생성자 " << std::endl; }
        virtual ~B() { std:: cout << "B 소멸자 " << std::endl; }
};

class C : public B, public A
{
    public:
        int c;
        C() { std:: cout << "C 생성자 " << std::endl; }
        ~C() { std:: cout << "C 소멸자 " << std::endl; }
};

int main()
{
    C c;
}