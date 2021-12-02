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

        void do_sth(int a)
        {
            std::cout << "무언가를 한다" << std::endl;
            data[0] = a;
        }

        ~A()
        {
            std::cout << "자원을 해제" << std::endl;
            delete[] data;
        }
};

void do_something(A* ptr)
{
    ptr->do_sth(3);
}

int main()
{
    std::unique_ptr<A> pa(new A());
    do_something(pa.get());
}