#include <iostream>

struct A
{
    double d;
};

int main()
{
    int a = 3;
    decltype(a) b = 2;

    int& r_a = a;
    decltype(r_a) r_b = b;

    int&& x = 3;
    decltype(x) y = 2;

    A* aa;
    decltype(aa->d) dd = 0.1;
}