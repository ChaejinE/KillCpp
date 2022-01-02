#include <iostream>

template <typename T>
class vector
{
    public:
        vector(size_t num, const T& element)
        {
            std::cout << element << " : " << num << std::endl;
        }

        template <typename Iterator>
        vector(Iterator start, Iterator end)
        {
            std::cout << "Constructor for Iterator" << std::endl;
        }
};

int main()
{
    vector<int> v(10, 3);
}