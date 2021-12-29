#include <iostream>

class A
{
    public:
        A() { std::cout << "생성자 호출" << std::endl; }
};

int main()
{
    A a{};
}