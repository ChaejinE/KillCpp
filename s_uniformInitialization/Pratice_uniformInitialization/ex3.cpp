#include <iostream>

class A
{
    public:
        A(std::initializer_list<int> l)
        {
            for (auto itr = l.begin(); itr != l.end(); ++itr)
            {
                std::cout << *itr << std::endl;
            }
        }
};

int main()
{
    A a = {1, 2, 3, 4, 5};
}