#include <iostream>
#include <string>
#include <tuple>

std::tuple<int, std::string, bool> GetStudent(int id)
{
    if (id == 0)
        return std::make_tuple(30, "cj", true);
    else
        return std::make_tuple(29, "ej", false);
}

int main()
{
    auto student = GetStudent(1);

    auto [age, name, is_male] = student;

    std::cout << "name : " << name << std::endl;
    std::cout << "age : " << age << std::endl;
    std::cout << "man ? : " << std::boolalpha << is_male << std::endl;
}