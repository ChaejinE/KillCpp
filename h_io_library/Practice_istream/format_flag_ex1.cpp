#include <string>
#include <iostream>

int main()
{
    int t;

    while (true)
    {
        std::cin.setf(std::ios_base::hex, std::ios_base::basefield);
        std::cin >> t;
        std::cout << "t : " << t << std::endl;

        if (std::cin.fail())
        {
            std::cout << "제대로 입력" << std::endl;
            std::cin.clear();
        }

        if (t == 0) break;
    }
}