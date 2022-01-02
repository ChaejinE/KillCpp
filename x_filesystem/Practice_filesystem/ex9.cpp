#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./a/b.txt");
    std::cout << p << " exist ? " << std::boolalpha << fs::exists(p) << std::endl;

    fs::remove(p);
    std::cout << p << " exist ? " << std::boolalpha << fs::exists(p) << std::endl;
}