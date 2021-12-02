#include <iostream>
#include <memory>
#include <vector>

class A
{
    int *data;

    public:
        A(int i)
        {
            std::cout << "자원 획득" << std::endl;
            data = new int[100];
            data[0] = i;
        }

        void some() { std::cout << "값 : " << data[0] << std::endl; }

        ~A()
        {
            std::cout << "자원 해제" << std::endl;
            delete[] data;
        }
};

int main()
{
    std::vector<std::unique_ptr<A>> vec;

    vec.emplace_back(new A(1));

    vec.back()->some();
}