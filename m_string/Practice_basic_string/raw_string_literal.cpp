#include <iostream>
#include <string>

int main()
{
    std::string str = R"(asdfasdf//   어떤것들이 와도된다. \n\n)";

    std::cout << str;
}