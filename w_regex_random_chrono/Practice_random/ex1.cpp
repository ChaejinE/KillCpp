#include <iostream>
#include <random>

int main()
{
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dis(0, 99);

    for (int i = 0; i < 5; i++)
        std::cout << "난수 : " << dis(gen) << std::endl;
}