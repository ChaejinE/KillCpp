#include <iostream>
#include <string>

template <typename T>
T max(T& a, T& b)
{
    return a > b ? a : b;
}

int main()
{
    int a = 1, b = 2;

    std::cout << a << ", " << b << " = " << max(a, b) << std::endl;
}