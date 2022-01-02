#include <functional>
#include <iostream>
#include <optional>
#include <utility>
#include <optional>

class A
{
    public:
        int data;
};

int main()
{
    A a;
    a.data = 5;

    std::optional<std::reference_wrapper<A>> maybe_a = std::ref(a);

    maybe_a->get().data = 3;

    std::cout << "a.data : " << a.data << std::endl;
}