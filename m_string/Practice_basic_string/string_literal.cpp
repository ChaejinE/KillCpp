#include <iostream>
#include <string>
using namespace std::literals;

int main()
{
    const char* s = "hello";

    char s2[] = "hello";

    auto str = "hello";

    std::string st = "hello";

    auto s3 = "hello"s;
    std::cout << "s1의 길이 : " << s3.size() << std::endl;
}