#include <iostream>
#include <sstream>
#include <string>

std::string to_string(double n)
{
    std::ostringstream ss;

    ss << n;
    
    return ss.str();
}

int main()
{
    double n = 2.;

    std::cout << "to_string : " << to_string(n) << std::endl;
    std::cout << "type : " << typeid(to_string(n)).name() << std::endl;
}