#include <iostream>
#include <type_traits>

template <typename T>
void tell_type()
{
    if (std::is_void<T>::value)
    {
        std::cout << "T는 void ! \n";
    } else {
        std::cout << "T is not void \n";
    }
}

int main()
{
    tell_type<int>();

    tell_type<void>();
}