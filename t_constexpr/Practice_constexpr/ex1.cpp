#include <iostream>

template <int N>
struct A
{
    int operator()() { return N; }
};

int main()
{
    constexpr int size = 3;
    int arr[size];

    constexpr int N = 10;
    A<N> a;
    std::cout << a() << std::endl;

    constexpr int number = 3;
    enum B{ x = number, y, z };
    std::cout << B::x << std::endl;
}
