#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ofstream out("write.txt");

    std::string s;
    if (out.is_open())
    {
        out << "쓰여질 내용";
    }

    return 0;
}