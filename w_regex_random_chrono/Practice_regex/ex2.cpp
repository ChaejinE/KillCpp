#include <iostream>
#include <regex>
#include <vector>

int main()
{
    std::vector<std::string> phone_numbers = {"010-1234-5678", "010-123-4567",
                                            "011-1234-5567", "010-12345-6789",
                                            "123-4567-8901", "010-1234-567"};
    
    std::regex re("[01]{3}-\\d{3,4}-\\d{4}", std::regex::optimize);
    for (const auto &number : phone_numbers)
    {
        std::cout << number << ": " << std::boolalpha
                  << std::regex_match(number, re) << '\n';
    }
}