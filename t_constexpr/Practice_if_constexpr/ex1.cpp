#include <iostream>
#include <type_traits>

template <typename T>
void show_value(T t)
{
    if constexpr (std::is_pointer<T>::value)
    {
        std::cout << "Pointer: " << *t << std::endl; 
    } else {
        std::cout << "No Pointer: " << t << std::endl;
    }
}

int main()
{
    int x = 3;
    show_value(x);

    int* p = &x;
    show_value(p);
}