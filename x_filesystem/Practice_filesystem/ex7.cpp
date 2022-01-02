#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./c/d/e/f");
    std::cout << p << "exist ? [" << std::boolalpha << fs::exists(p)
              << "]" << std::endl;

    fs::create_directories(p);

    std::cout << p << "exist ? [" << fs::exists(p) << "]" << std::endl;
    std::cout << p << "directory ? [" << fs::is_directory(p) << "]" << std::endl;
}