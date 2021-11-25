#include <iostream>
#include <string>

void* operator new(std::size_t count)
{
    std::cout << count << "bytes 할당" << std::endl;
    return malloc(count);
}

bool contains_very(const std::string& str)
{
    return str.find("very") != std::string::npos;
}

int main()
{
    std::cout << std::boolalpha << contains_very("c++ string is very easy to use")
              << std::endl;

    std::cout << contains_very("c++ string is not easy to use") << std::endl;
}