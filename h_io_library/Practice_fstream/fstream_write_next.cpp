#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ofstream out("write.txt", std::ios::app);

    std::string s;
    if (out.is_open())
    {
        out << "덧붙여서 쓸 내용";
    }

    return 0;
}