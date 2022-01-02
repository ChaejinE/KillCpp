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

    int age = std::get<0>(student);
    std::string name = std::get<1>(student);
    bool is_male = std::get<2>(student);

    std::cout << "name : " << name << std::endl;
    std::cout << "age : " << age << std::endl;
    std::cout << "man ? : " << std::boolalpha << is_male << std::endl;
}