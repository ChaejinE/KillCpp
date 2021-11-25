#include <iostream>
#include <string>

int main()
{
    std::u32string u32_str = U"이건 UTF-32 문자열 입니다";
    std::cout << u32_str.size() << std::endl;
}