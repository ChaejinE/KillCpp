#include <iostream>
#include <regex>
#include <vector>

int main()
{
    std::vector<std::string> file_names = {"db-123-log.txt", "db-124-log.txt",
                                           "not_db-log.txt", "db-12-log.txt",
                                           "db-12-log.jpg"};

    std::regex re("db-\\d*-log\\.txt");
    for (const auto &file_name : file_names)
    {
        std::cout << file_name << " : " << std::boolalpha
                  << std::regex_match(file_name, re) << "\n";
    }
}