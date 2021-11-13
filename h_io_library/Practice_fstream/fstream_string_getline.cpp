#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream in("test2.txt");

    if (!in.is_open())
    {
        std::cout << "file 못 찾음" << std::endl;

        return 0;
    } else {

        std::string s;
        while (in)
        {
            getline(in, s);
            std::cout << s << std::endl;
        }
    }

    return 0;
}