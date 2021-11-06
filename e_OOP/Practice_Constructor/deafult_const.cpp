#include <iostream>

class Date
{
    int year_;
    int month_;
    int day_;

public:
    void ShowDate();

    Date()
    {
        year_ = 2012;
        month_ = 7;
        day_ = 12;
    }
};

void Date::ShowDate()
{
    std::cout << "오늘은 " << year_ << " 년 " << month_ << " 월" << day_
              << " z일 입니다 " << std::endl;
}

int main()
{
    Date day = Date();
    Date day2;

    day.ShowDate();
    day2.ShowDate();

    return 0;
}