#include <iostream>
#include <map>
#include <string>

template <typename K, typename V>
void print_map(std::map<K, V>& m)
{
    for (auto itr = m.begin(); itr != m.end(); ++itr)
    {
        std::cout << itr->first << " " << itr->second << std::endl;
    }
}

int main()
{
    std::map<std::string, double> pitcher_list;

    // map의 insert 함수는 pari 객체를 인자로 받는다.
    pitcher_list.insert(std::pair<std::string, double>("정채진", 2.23));
    pitcher_list.insert(std::pair<std::string, double>("김채진", 2.93));
    pitcher_list.insert(std::pair<std::string, double>("박채진", 2.95));

    pitcher_list.insert(std::pair<std::string, double>("김로또", 3.04));
    pitcher_list.insert(std::pair<std::string, double>("유로또", 3.05));
    pitcher_list.insert(std::pair<std::string, double>("진로또", 3.09));

    pitcher_list["유로뱅"] = 3.56;
    pitcher_list["로또"] = 3.76;
    pitcher_list["헨리"] = 3.90;

    print_map(pitcher_list);

    std::cout << "유로뱅 방어율 :: " << pitcher_list["유로뱅"] << std::endl;
}
