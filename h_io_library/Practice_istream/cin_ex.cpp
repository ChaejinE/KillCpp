#include <iostream>
#include <string>

int main()
{
    std::string s;

    // ABCD EFGH IJKL 입력
    while (true)
    {
        std::cin >> s;
        std::cout << "word : " << s << std::endl;
    }

    return 0;
}