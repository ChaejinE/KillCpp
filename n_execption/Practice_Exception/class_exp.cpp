#include <iostream>
#include <exception>
#include <vector>

class Parent : public std::exception
{
    public:
        virtual const char* what() const noexcept override { return "Prent!\n"; }
};

class Child : public Parent
{
    public:
        const char* what() const noexcept override { return "Child!\n"; }
};

int func(int c)
{
    if (c == 1)
        throw Parent();
    else if (c == 2)
        throw Child();

    return 0;
}

int main()
{
    std::vector<int> vec;
    
    for (int i = 1; i < 3; ++i)
        vec.emplace_back(i);

    for (int c : vec)
    {
        std::cout << "val : " << c << std::endl;
        try
        {
            func(c);
        } catch (Parent& p)
        {
            std::cout << "Parent Catch" << std::endl;
            std::cout << p.what();
        } catch (Child& c)
        {
            std::cout << "Child Catch" << std::endl;
            std::cout << c.what();
        }
    }
}