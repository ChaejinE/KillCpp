#include <iostream>
#include <type_traits>

class A {};

template <typename T>
void only_integer(const T& t)
{
    static_assert(std::is_integral<T>::value);
    std::cout << "T is an integer \n";
}

int main()
{
    int n = 3;
    only_integer(n);

    A a;
    only_integer(a);
}