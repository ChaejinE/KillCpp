#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream in("test2.txt");
    char buf[100];

    if (!in.is_open()) {
        std::cout << "file 찾을 수 없다." << std::endl;

        return 0;
    }

    while (in) {
        in.getline(buf, 100);
        std::cout << buf << std::endl;
    }

    return 0;
}