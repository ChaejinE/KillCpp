#include <iostream>

class A
{
    int x;

public:
    A(int c) : x(c) {}

    int &acces_x() { return x; }
    int get_x() { return x; }
    void show_x() { std::cout << x << std::endl; }
};

int main()
{
    A a(5);
    a.show_x();

    int &c = a.acces_x();
    c = 4;
    a.show_x();

    int d = a.acces_x();
    d = 3;
    a.show_x();

    // error
    // int& e = a.get_x();
    // e = 2;
    // a.show_x();

    int f = a.get_x();
    f = 1;
    a.show_x();

    return 0;
}