#include <iostream>
#include <set>
#include <type_traits>
#include <vector>

template <typename Cont, typename = decltype(std::declval<Cont>().begin()),
          typename = decltype(std::declval<Cont>().end())>
void print(const Cont& container)
{
    std::cout << "[ ";
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "]\n";
}

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    print(v);

    std::set<char> s = {'a', 'b', 'f', 'i'};
    print(s);
}