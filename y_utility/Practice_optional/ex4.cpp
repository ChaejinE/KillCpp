#include <iostream>
#include <utility>
#include <optional>

class A
{
    public:
        A() { std::cout << "default constructor" << std::endl; }

        A(const A& a) { std::cout << "copy constructor" << std::endl; }
};

int main()
{
    A a;

    std::cout << "optional Making -----" << std::endl;
    std::optional<A> maybe_a;

    std::cout << "maybe_a는 A객체를 포함하고 있지 않아 디폴트 생성할 필요 없음" << std::endl;

    maybe_a = a;
}