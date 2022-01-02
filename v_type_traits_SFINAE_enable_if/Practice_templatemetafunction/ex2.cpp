#include <iostream>

template <typename T>
struct is_void
{
    static constexpr bool value = false;
};

template <>
struct is_void<void>
{
    static constexpr bool value = true;
};

template <typename T>
void tell_type()
{
    if (is_void<T>::value)
    {
        std::cout << "T is void \n";
    } else {
        std::cout << "T is not void \n";
    }
}

int main()
{
    tell_type<int>();

    tell_type<void>();
}