#include <iostream>
#include <type_traits>

// T 는 반드시 정수 타입을 리턴하는 멤버 함수 func 을 가지고 있어야 한다.
template <typename T, typename = std::enable_if_t<std::is_integral<decltype(std::declval<T>().func())>>>
void test(const T& t)
{
    std::cout << "t.func() : " << t.func() << std::endl;
}

struct A
{
    int func() const { return 1; }
};

struct B
{
    char func() const { return 'a'; }
};

int main()
{
    test(A{});
    test(B{});
}