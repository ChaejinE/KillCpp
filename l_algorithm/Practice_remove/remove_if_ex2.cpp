#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

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

// 홀수면 제거하도록하는 객체
struct is_odd
{
    // int num_delete;
    int* num_delete;

    is_odd(int* num_delete) : num_delete(num_delete) {}

    bool operator()(const int& i)
    {
        if (*num_delete >= 2) return false;

        if (i % 2 == 1)
        {
            (*num_delete)++;
            return true;
        }

        return false;
    }
};

int main()
{
    std::vector<int> vec;
    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    std::cout << "처음 상태 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << "벡터에서 홀수인 원소 앞의 2개 제거" << std::endl;
    int num_delete = 0;
    vec.erase(std::remove_if(vec.begin(), vec.end(), is_odd(&num_delete)), vec.end());
    print(vec.begin(), vec.end());
}