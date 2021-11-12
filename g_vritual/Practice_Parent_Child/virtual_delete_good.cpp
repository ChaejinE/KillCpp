#include <iostream>

class Parent
{
    public:
        Parent() { std::cout << "Parent 생성자" << std::endl; }
        virtual ~Parent() { std::cout << "Parent 소멸자" << std::endl; }
};

class Child : public Parent
{
    public:
        Child() { std::cout << "Child 생성자" << std::endl; }
        ~Child() { std::cout << "Child 소멸자" << std::endl; }
};

int main()
{
    std::cout << " 평범한 child 생성 " << std::endl;
    { Child c; }

    std::cout << "Parent 포인터로  Child 가리킬 때" << std::endl;
    {
        Parent *p = new Child();
        delete p;
    }
}