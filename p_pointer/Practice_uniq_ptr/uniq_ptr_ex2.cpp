#include <iostream>
#include <memory>

class A
{
    int *data;

    public:
        A()
        {
            std::cout << "자원 획득" << std::endl;
            data = new int[100];
        }

        void some() { std::cout << "일반 포인터와 동일하게 사용가능" << std::endl; }

        ~A()
        {
            std::cout << "자원을 해제" << std::endl;
            delete[] data;
        }
};

void do_something()
{
    std::unique_ptr<A> pa(new A());
    pa->some();
}

int main()
{
    do_something();
}