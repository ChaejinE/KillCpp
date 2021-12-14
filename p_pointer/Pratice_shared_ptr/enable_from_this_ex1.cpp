#include <iostream>
#include <memory>

class A : public std::enable_shared_from_this<A>
{
    int *data;

    public:
        A()
        {
            data = new int[100];
            std::cout << "자원 획득" << std::endl;
        }

        ~A()
        {
            std::cout << "소멸자 호출" << std::endl;
        }

        std::shared_ptr<A> get_shared_ptr() { return shared_from_this(); }
};

int main()
{
    std::shared_ptr<A> pa1 = std::make_shared<A>();
    std::shared_ptr<A> pa2 = pa1->get_shared_ptr();

    std::cout << pa1.use_count() << std::endl;
    std::cout << pa2.use_count() << std::endl;
}