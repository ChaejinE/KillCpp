#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream in("test.txt");
    std::string s;

    if (in.is_open())
    {
        in >> s;
        std::cout << "입력 받은 문자열 :: " << s << std::endl;
    } else {
        std::cout << "file을 찾을 수 없다." << std::endl;
    }

    return 0;
}