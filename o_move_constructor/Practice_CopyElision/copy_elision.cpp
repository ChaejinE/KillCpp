#include <iostream>

class A
{
    int data_;

    public:
        A(int data) : data_(data) { std::cout << "일반 생성자 호출" << std::endl; }

        A(const A& a) : data_(a.data_)
        {
            std::cout << "복사 생성자 호출" << std::endl;
        }
};

int main()
{
    A a(1);
    A b(a);

    A c(A(2));
}