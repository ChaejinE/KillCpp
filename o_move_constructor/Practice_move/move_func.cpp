#include <iostream>
#include <utility>

class A
{
    public:
        A() { std::cout << "일반 생성자 호출 " << std::endl; }
        A(const A& a) { std::cout << "복사 생성자 호출" << std::endl; }
        A(A&& a) noexcept { std::cout << "이동 생성자 호출 " << std::endl; }
};

int main()
{
    A a;

    std::cout << "--------" << std::endl;
    A b(a);

    std::cout << "--------" << std::endl;
    A c(std::move(a));
}