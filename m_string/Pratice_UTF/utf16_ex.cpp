#include <iostream>
#include <string>

int main()
{
    std::u16string u16_str = u"이건 UTF-16 문자열 입니다";
    std::cout << u16_str.size() << std::endl;
}