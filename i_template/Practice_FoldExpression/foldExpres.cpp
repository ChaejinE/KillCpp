#include <iostream>

template <typename... Ints>
int sum_all(Ints... nums)
{
    return (... + nums);
}

int main()
{
    std::cout << sum_all(1, 4, 2, 3, 10) << std::endl;
}
