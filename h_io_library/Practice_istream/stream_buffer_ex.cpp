#include <iostream>
#include <string>

int main()
{
    std::string s;
    std::cin >> s;

    // 위치 지정자 한 칸 옮기고, 그 다음 문자를 훔쳐본다. (이 때는 움직이지 않는다.)
    char peek = std::cin.rdbuf()->snextc();
    if (std::cin.fail()) std::cout << "Fail";

    std::cout << "두번쨰 단어 맨 앞글자 : " << peek << std::endl;
    std::cin >> s;
    std::cout << "다시 읽기 : " << s << std::endl;

    return 0;
}