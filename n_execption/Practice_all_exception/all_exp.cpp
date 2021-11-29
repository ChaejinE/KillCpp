#include <iostream>
#include <stdexcept>
#include <vector>

int func(int c)
{
    if (c == 1)
        throw 1;
    else if (c == 2)
        throw "hi";
    else if (c == 3)
        throw std::runtime_error("error");

    return 0;
}

int main()
{
    std::vector<int> vec;

    for (int i = 1; i < 4; ++i)
        vec.emplace_back(i);

    for (auto c : vec)
    {
        try {
            func(c);
        } catch (int e) {
            std::cout << "Catch int : " << e << std::endl;
        } catch (...) {
            std::cout << "Default Catch ! : " << std::endl;
        }
    } 
}