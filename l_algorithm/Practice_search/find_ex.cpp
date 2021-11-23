#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end)
{
    while (begin != end)
    {
        std::cout << "[" << *begin << "] ";
        begin++;
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<int> vec;

    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    vec.emplace_back(4);

    auto result = std::find(vec.begin(), vec.end(), 3);
    std::cout << "3 은 " <<  std::distance(vec.begin(), result) + 1 << " 번째 원소"
              << std::endl;
}