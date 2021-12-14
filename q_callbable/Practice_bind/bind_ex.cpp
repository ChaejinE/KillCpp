#include <functional>
#include <iostream>

void add(int x, int y)
{
    std::cout << x << " + " << y << " = " << x + y << std::endl;
}

void substract(int x, int y)
{
    std::cout << x << " - " << y << " = " << x - y << std::endl;
}

int main()
{
    auto add_with_2 = std::bind(add, 2, std::placeholders::_1);
    add_with_2(3);

    add_with_2(3, 4);

    auto subtract_from_2 = std::bind(substract, std::placeholders::_1, 2);
    auto negate = 
        std::bind(substract, std::placeholders::_2, std::placeholders::_1);

    subtract_from_2(3); // 3 - 2
    negate(4, 2); // 2 - 4
}