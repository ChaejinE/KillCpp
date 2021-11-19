#include <iostream>
#include <set>

template <typename T>
void print_set(std::set<T>& s)
{
    std::cout << "[ ";
    for (typename std::set<T>::iterator itr = s.begin(); itr != s.end(); ++itr)
    {
        std::cout << *itr << " ";
    }

    std::cout << " }" << std::endl;
}

int main()
{
    std::set<int> s;
    s.insert(10);
    s.insert(50);
    s.insert(20);
    s.insert(40);
    s.insert(30);

    std::cout << "순서대로 정렬되어 나온다." << std::endl;
    print_set(s);

    std::cout << "20 이 s의 원소 ? :: ";
    auto itr = s.find(20);
    if (itr != s.end())
    {
        std::cout << "Y" << std::endl;
    } else {
        std::cout << 'N' << std::endl;
    }

    std::cout << "25 이 s의 원소 ? :: ";
    itr = s.find(25);
    if (itr != s.end())
    {
        std::cout << "Y" << std::endl;
    } else {
        std::cout << 'N' << std::endl;
    }
}