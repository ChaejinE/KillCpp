# Overview
- 생성한 객체를 초기화하는 과정을 항상 숙지해야한다.
- C++에서는 언어 차원에서 도와주는 장치가 있는데, 그것은 생성자다.

# Constructor
- **객체 생성시 자동으로 호출되는 함수**라고 볼 수 있다.
- 자동으로 호출하면서 객체를 초기화 해주는 역할을 담당하게 된다.
```
ClassName (Variable) {}
```
- 위와같은 컨벤션으로 만들며 리턴값이 없다.

```cpp
Date(int year, int month, int day)
```
- 위 함수에서 정의한 인자에 맞게 마치 함수를 호출하듯이 써주면 위 생성자를 호출하며 객체를 생성할 수 있다.

```cpp
Date day(2011, 3, 1)
```
- 예시
- Date 클래스의 day 객체를 만들면서 생성자 Date(int year, month, int day)를 호출한다는 의미다.
  - year = 2011, moth = 3, day = 1을 전달하며 객체를 생성한다.

```cpp
Date day = Date(2012, 3, 1);
```
- 위와 같이 생성자를 호출할 수 있는데, 이는 explicit한 방법이라고한다. (명시적)
- 그 이전은 implicit (암시적) 방법
- 많은 경우 암시적 방법으로 축약해서 쓸 수 있어 이를 선호하는 편이다.

# Deafult Constructor
```
Date day;
```
- 생성자를 명시하지 않았는데 생성자가 호출이 될까?
  - 호출된다.
- 생성자를 정의하지 않으면 **Default Constuctor**가 호출된다.
- Default Constructor는 인자를 하나도 가지지 않는다.
- 사용자가 어떠한 생성자도 명시적으로 정의하지 않았을 경우 **컴파일러가 자동으로 추가**해주는 생성자다.
  - 다른 생성자를 추가한 순간 컴파일러는 자동으로 Default Constructor를 삽입하지는 않는다. (명심)
- default constructor는 직접 명시해서 만들 수도 있다. (Practice에 있다.)

```cpp
Date day3();
```
- 위는 day3 객체를 디폴트 생성자를 이용해 초기화하지 않는다.
- **리턴 값이 Date이고 인자가 없는 함수 day3을 정의한 것으로 인식한다.**
  - 객체를 선언할 때 반드시 주의해두어야할 사항
- C++ 11 이전 : 디폴트 생성자 사용하고 싶을 때, 생성자를 정의하지 않는 방법 밖에 없었다.
```cpp
class Test{
    public:
        Test() = default;
}
```
- C++ 11 이후 : 개발자가 깜빡 잊고 생성자를 정의하지 않은 것인지 아니면 정말 디폴트 생성자를 사용하고 파서 인지 알 길이 없으므로 명시적으로 deafult constructor를 사용하도록 명시할 수 있게 되었다.

# Constructor Overloading
- 생성자 역시 함수이므로 함수의 Overloading이 적용될 수 있다.
- Class 객체를 여러가지 방식으로 생성할 수 있게 되는 것이다.

```
#include <iostream>

class Date
{
    int year_;
    int month_; // 1 부터 12 까지.
    int day_;   // 1 부터 31 까지.

public:
    void ShowDate();

    Date()
    {
        std::cout << "기본 생성자 호출!" << std::endl;
        year_ = 2012;
        month_ = 7;
        day_ = 12;
    }

    Date(int year, int month, int day)
    {
        std::cout << "인자 3 개인 생성자 호출!" << std::endl;
        year_ = year;
        month_ = month;
        day_ = day;
    }
};

class Date2
{
    int year_;
    int month_; // 1 부터 12 까지.
    int day_;   // 1 부터 31 까지.

public:
    void ShowDate();

    Date2() = default;

    Date2(int year, int month, int day)
    {
        std::cout << "인자 3 개인 생성자 호출!" << std::endl;
        year_ = year;
        month_ = month;
        day_ = day;
    }
};

void Date::ShowDate()
{
    std::cout << "오늘은 " << year_ << " 년 " << month_ << " 월 " << day_
              << " 일 입니다 " << std::endl;
}

void Date2::ShowDate()
{
    std::cout << "오늘은 " << year_ << " 년 " << month_ << " 월 " << day_
              << " 일 입니다 " << std::endl;
}

int main()
{
    Date day = Date();
    Date day2(2012, 10, 31);
    Date day3;
    Date2 day4;

    day.ShowDate();
    day2.ShowDate();
    day3.ShowDate();
    day4.ShowDate();

    return 0;
}
```
```
기본 생성자 호출!
인자 3 개인 생성자 호출!
기본 생성자 호출!
오늘은 2012 년 7 월 12 일 입니다 
오늘은 2012 년 10 월 31 일 입니다 
오늘은 2012 년 7 월 12 일 입니다 
오늘은 8 년 0 월 4199705 일 입니다 
```
- Constructor Overloading에 대해 여러 테스트를 해본 코드다.