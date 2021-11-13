#include <iostream>
#include <sstream>

int main()
{
    std::istringstream ss("123");
    int x;

    ss >> x;

    std::cout << "input data : " << x << std::endl;

    return 0;
}