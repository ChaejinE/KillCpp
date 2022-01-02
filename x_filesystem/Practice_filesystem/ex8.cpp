#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    fs::path from("./a");
    fs::path to("./c");

    fs::copy(from, to, fs::copy_options::recursive);
}