#include <iostream>
#include <string>
#include <unordered_set>

template <typename K>
void print_unordered_set(const std::unordered_set<K>& m)
{
    for (const auto& elem :m)
    {
        std::cout << elem << std::endl;
    }
}

int main()
{
    std::unordered_set<std::string> s;

    s.insert("hi");
    s.insert("my");
    s.insert("name");
    s.insert("is");
    s.insert("Chaejin");
    s.insert("Jeong");

    print_unordered_set(s);
}