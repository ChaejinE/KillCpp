#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./some_file");

    std::cout << "current path : " << fs::current_path() << std::endl;
    std::cout << "relative path : " << p.relative_path() << std::endl;
    std::cout << "absolute path : " << fs::absolute(p) << std::endl;
    std::cout << "official abs path : " << fs::canonical(p) << std::endl;
}