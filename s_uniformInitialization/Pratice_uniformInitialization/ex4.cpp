#include <iostream>
#include <map>
#include <string>
#include <vector>

template <typename T>
void print_vec(const std::vector<T>& vec)
{
    std::cout << "[";
    for (const auto& e : vec)
    {
        std::cout << e << " ";
    }
    std::cout << "]" << std::endl;
}

template <typename K, typename V>
void print_map(const std::map<K, V>& m)
{
    for (const auto& kv : m)
        std::cout << kv.first << " : " << kv.second << std::endl;
}

int main()
{
    std::vector<int> v = {1,2,3,4,5};
    print_vec(v);

    std::cout << "----------------------" << std::endl;
    std::map<std::string, int> m = {
        {"abc", 1}, {"hi", 3}, {"hello", 5}, {"C++", 2}
    };
    print_map(m);
}