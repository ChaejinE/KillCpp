#include <iostream>
#include <string>

int main()
{
    int n;

    while (true)
    {   
        std::cin >> n;
        std::cout << "num : " << n << std::endl;
        if (std::cin.fail())
        {
            std::cout << "제대로 입력" << std::endl;
            std::cin.clear(); // 플래그를 초기화
            std::cin.ignore(100, '\n'); // 개행 문자가 나올 때 까지 무시
        }
        if (n == 1) break;
    }

    return 0;
}