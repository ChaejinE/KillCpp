#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream in("test.txt", std::ios::binary);
    std::string s;

    int x;
    if (in.is_open())
    {
        in.read((char*)(&x), 4);
        std::cout << "입력 받은 수 :: " << std::hex << x << std::endl;
    } else {
        std::cout << "file을 찾을 수 없다." << std::endl;
    }

    return 0;
}