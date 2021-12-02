#include <iostream>
#include <memory>

class Foo
{
    int a, b;

    public:
        Foo(int a, int b) : a(a), b(b) { std::cout << "생성자 호출" << std::endl; }
        void print() {std::cout << "a : " << a << ", b : " << b << std::endl; }
        ~Foo() { std::cout << "소멸자 호출" << std::endl; }
};

int main()
{
    auto ptr = std::make_unique<Foo>(3, 5);
    ptr->print();
}