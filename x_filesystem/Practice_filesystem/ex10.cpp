#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./c/b");

    std::error_code err;
    fs::remove(p, err);
    std::cout << err.message() << std::endl;

    fs::remove_all(p);
}