#include <iostream>
#include <string>

int main()
{
    int n;

    // c 문자 입력
    while (std::cin >> n)
    {
        std::cout << "num : " << n << std::endl;
        if (n == 0)
            break;
    }

    return 0;
}