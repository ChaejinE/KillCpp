#include <iostream>

template <typename Int, typename... Ints>
Int diff_from(Int start, Ints... nums)
{
    return (start - ... - nums);
}

int main()
{
    std::cout << diff_from(100, 1, 4, 2, 3, 10) << std::endl;
}