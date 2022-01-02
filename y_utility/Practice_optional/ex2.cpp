#include <iostream>
#include <map>
#include <string>

std::pair<std::string, bool> GetValueFromMap(
    const std::map<int, std::string>& m, int key)
{
    auto itr = m.find(key);
    if (itr != m.end())
    {
        return std::make_pair(itr->second, true);
    }

    return std::make_pair(std::string(), false);
}

int main()
{
    std::map<int, std::string> data = {{1, "a"}, {2, "b"}, {3, "c"}};
    std::cout << "map [2] : " << GetValueFromMap(data, 2).first << std::endl;
    std::cout << "map [4] exist? " << std::boolalpha
              << GetValueFromMap(data, 4).second << std::endl;
}