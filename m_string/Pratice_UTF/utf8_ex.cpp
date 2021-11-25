#include <iostream>
#include <string>

int main()
{
    std::string str = u8"이건 UTF-8 문자열 입니다";
    std::cout << str.size() << std::endl;
}