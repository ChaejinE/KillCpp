#include <iostream>
#include <type_traits>

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
void test(const T& t)
{
    std::cout << "t : " << t << std::endl;
}

int main()
{
    test(1); // int
    test(false);  // bool
    test('c');  // char
}