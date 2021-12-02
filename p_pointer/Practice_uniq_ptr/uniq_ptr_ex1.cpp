#include <iostream>

class A 
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
            delete[] data;
        }
};

void do_somthing() { A *pa = new A(); }

int main()
{
    do_somthing();
}