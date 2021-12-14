#include <iostream>
#include <memory>
#include <vector>

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

int main()
{
    std::vector<std::shared_ptr<A>> vec;

    vec.push_back(std::shared_ptr<A>(new A()));
    vec.push_back(std::shared_ptr<A>(vec[0]));
    vec.push_back(std::shared_ptr<A>(vec[1]));

    std::cout << "첫 번쨰 소멸" << std::endl;
    vec.erase(vec.begin());

    std::cout << "다음 원소 소멸" << std::endl;
    vec.erase(vec.begin());

    std::cout << "프로그램 종료" << std::endl;
}