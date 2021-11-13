#include <iostream>
#include <sstream>
#include <string>

double to_number(std::string s)
{
    std::istringstream ss(s);
    double x;

    ss >> x;
    return x;
}

int main()
{
    std::cout << "변환 1 + 2 = " << to_number("1") + to_number("2") << std::endl;
}