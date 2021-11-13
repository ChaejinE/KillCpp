#include <string>
#include <iostream>

int main()
{
    int t;

    while (true)
    {
        std::cin >> std::hex >> t;
        std::cout << "t : " << t << std::endl;

        if (std::cin.fail())
        {
            std::cout << "제대로 입력" << std::endl;
            std::cin.clear();
            std::cin.ignore(100, '\n');
        }

        if (t == 0) break;
    }
}