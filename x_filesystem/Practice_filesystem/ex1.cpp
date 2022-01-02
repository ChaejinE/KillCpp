#include <filesystem>
#include <iostream>

int main()
{
    std::filesystem::path p("./some_file");

    std::cout << "Does " << p << " exist? [" << std::boolalpha
              << std::filesystem::exists(p) << "]" << std::endl;
    
    std::cout << "Is " << p << " file? [" << std::filesystem::is_regular_file(p)
              << "]" << std::endl;

    std::cout << "Is " << p << " directory ? [" << std::filesystem::is_directory(p)
              << "]" << std::endl;
}