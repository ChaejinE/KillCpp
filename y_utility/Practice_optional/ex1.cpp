#include <iostream>
#include <map>
#include <string>

std::string GetValueFromMap(const std::map<int, std::string>& m, int key)
{
    auto itr = m.find(key);
    if (itr != m.end())
    {
        return itr->second;
    }

    return std::string();
}

int main()
{
    std::map<int, std::string> data = {{1, "hi"}, {2, "hello"}, {3, "hirroo"}};
    std::cout << "map에서 2에 대응되는 값" << GetValueFromMap(data, 2) << std::endl;
    std::cout << "map에서 4에 대응되는 값" << GetValueFromMap(data, 4) << std::endl;
}