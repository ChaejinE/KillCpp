#include <iostream>
#include <type_traits>

class A
{
    public:
        int n;
    
    A(int n) : n(n) {}
};

int main()
{
    int A::*p_n = &A::n;

    A a(3);
    std::cout << "a.n : " << a.n << std::endl;
    std::cout << "a.*p_n : " << a.*p_n << std::endl;
}