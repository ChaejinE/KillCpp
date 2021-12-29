#include <iostream>

class A
{
    public:
        A(int x, double y) { std::cout << "Constr: " << x << ',' << y << std::endl; }
};

A func()
{
    return {1, 2.3};
}

int main()
{
    func();
}