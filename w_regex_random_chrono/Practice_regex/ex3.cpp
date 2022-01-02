#include <iostream>
#include <regex>
#include <vector>

int main()
{
    std::vector<std::string> phone_numbers = {"010-1234-5678", "010-123-4567",
                                            "011-1234-5567", "010-12345-6789",
                                            "123-4567-8901", "010-1234-567"};
    
    std::regex re("[01]{3}-(\\d{3,4})-\\d{4}", std::regex::optimize);
    std::smatch match; // matching 결과 string으로 보관
    for (const auto &number : phone_numbers)
    {
        if (std::regex_match(number, match, re))
        {
            for (size_t i = 0; i < match.size(); i++)
                std::cout << "Match : " << match[i].str() << std::endl;

            std::cout << "----------------------------\n";
        }
    }
}