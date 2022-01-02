#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<int, std::string> m = { {3, "hi"}, {5, "hello"} };
    for (auto& [key, val] : m)
        std::cout << "Key : " << key << " Value : " << val << std::endl;
}