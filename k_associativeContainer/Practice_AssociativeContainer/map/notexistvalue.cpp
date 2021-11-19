#include <iostream>
#include <map>
#include <string>

template <typename K, typename V>
void print_map(const std::map<K, V>& m)
{
    for (const auto& kv : m)
    {
        std::cout << kv.first << " " << kv.second << std::endl;
    }
}

template <typename K, typename V>
void search_and_print(std::map<K, V>& m, K key)
{
    auto itr = m.find(key);
    if (itr != m.end())
    {
        std::cout << key << " --> " << itr->second << std::endl;
    } else {
        std::cout << key << "은 목록에 없다." << std::endl;
    }
}

int main()
{
    std::map<std::string, double> pitcher_list;

    pitcher_list["정채진"] = 3.58;

    print_map(pitcher_list);
    

    search_and_print(pitcher_list, std::string("정채진"));
    search_and_print(pitcher_list, std::string("류현진"));
}