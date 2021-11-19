#include <iostream>
#include <map>
#include <string>

template <typename K, typename V>
void print_map(const std::multimap<K, V>& m)
{
    for (const auto& kv : m)
    {
        std::cout << kv.first << " " << kv.second << std::endl;
    }
}

int main()
{
    std::multimap<int, std::string> m;

    m.insert(std::make_pair(1, "hello"));
    m.insert(std::make_pair(1, "hi"));
    m.insert(std::make_pair(1, "Chaejin"));
    m.insert(std::make_pair(1, "Jeong"));

    print_map(m);

    std::cout << "-------------------" << std::endl;
    std::cout << m.find(1)->second << std::endl;
}