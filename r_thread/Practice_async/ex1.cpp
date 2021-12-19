#include <future>
#include <iostream>
#include <thread>
#include <vector>

int sum(const std::vector<int>& v, int start, int end)
{
    int total = 0;
    for (int i = start; i < end; ++i)
    {
        total += v[i];
    }

    return total;
}

int parallel_sum(const std::vector<int>& v)
{
    // 1 ~ 500 까지 비동기적으로 더한다.
    // lambda 사용하면 더 깔끔하게 표현할 수도 있다.
    // std::async([&v]() { return sum(v, 0, v.size() / 2); })
    std::future<int> lower_half_future =
        std::async(std::launch::async, sum, cref(v), 0, v.size() / 2);
    
    // 501 ~ 1000까지 더한다.
    int upper_half = sum(v, v.size() / 2, v.size());

    return lower_half_future.get() + upper_half;
}

int main()
{
    std::vector<int> v;
    v.reserve(1000);

    for (int i = 0; i < 1000; ++i)
        v.emplace_back(i + 1);
    
    std::cout << "1 부터 1000까지의 합 : " << parallel_sum(v) << std::endl;
}