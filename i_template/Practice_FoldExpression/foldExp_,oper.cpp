#include <iostream>

class A
{
    public:
        void do_somthing(int x) const
        {
            std::cout << "Do something" << x << std:: endl;
        }
};

template <typename T, typename... Ints>
void do_may_things(const T& t, Ints... nums)
{
    (t.do_somthing(nums), ...);
}

int main()
{
    A a;
    do_may_things(a, 1, 2, 3, 4);
}

