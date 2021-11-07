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

# Destructor
```cpp
#include <iostream>
#include <string.h>

class Marine
{
    int hp;
    int coord_x, coord_y;
    int damage;
    bool is_dead;
    char *name; // 마린 이름

public:
    Marine();                                      // 기본생성자
    Marine(int x, int y);                          // x, y 좌표에 마린 생성하는 생성자
    Marine(int x, int y, const char *marine_name); // 이름까지 지정하는 생성자

    int attack();                      // 데미지 return
    void be_attacked(int damage_earn); // 입은 데미지
    void move(int x, int y);
    void show_status();
};

Marine::Marine(int x, int y)
{
    coord_x = x;
    coord_y = y;
    hp = 50;
    damage = 5;
    is_dead = false;

    name = NULL;
}

Marine::Marine(int x, int y, const char *marine_name)
{
    name = new char[strlen(marine_name) + 1];
    strcpy(name, marine_name);

    coord_x = x;
    coord_y = y;
    hp = 50;
    damage = 5;
    is_dead = false;
}

void Marine::move(int x, int y)
{
    coord_x = x;
    coord_y = y;
}

int Marine::attack() { return damage; }
void Marine::be_attacked(int damage_earn)
{
    hp -= damage_earn;
    if (hp < 0)
        is_dead = true;
}

void Marine::show_status()
{
    std::cout << " Marine " << name << std::endl;
    std::cout << "Location : ( " << coord_x << " , " << coord_y << " )"
              << std::endl;

    std::cout << " HP : " << hp << std::endl;
}

int main()
{
    Marine *marines[100];

    marines[0] = new Marine(2, 3, "Marine 2");
    marines[1] = new Marine(1, 5, "Marine 1");

    marines[0]->show_status();
    marines[1]->show_status();

    std::cout << std::endl
              << "마린 1이 마린 2 공격" << std::endl;

    marines[0]->be_attacked(marines[1]->attack());

    marines[0]->show_status();
    marines[1]->show_status();

    delete marines[0];
    delete marines[1];

    return 0;
}
```
- new는 malloc과 동적으로 할당하는 것이 같지만 객체를 **동적으로 생성하면서 동시에 자동으로 생성자를 호출**해준다.

```cpp
marines[0]->show_status();
marines[1]->show_status();
```
- Marine들의 포인터를 가리키는 배열이므로 '.'이 아니라 '->'를 사용해야된다.

```cpp
delete marines[0];
delete marines[1];
```
- 동적할당한 메모리는 언제나 해제해줘야한다는 원칙을 잊으면 안된다.

```cpp
Marine::Marine(int x, int y, const char *marine_name)
{
    name = new char[strlen(marine_name) + 1];
    strcpy(name, marine_name);

    coord_x = x;
    coord_y = y;
    hp = 50;
    damage = 5;
    is_dead = false;
}
```
- name을 동적으로 생성해서 문자열을 복사했는데, 이렇게 동적할당된 char 배열에 대한 delete는 언제 이루어 지는 것일까?
  - 명확히 delete를 지정하지 않는 한 자동으로 delete가 되는 경우는 없다.
  - name은 영원히 메모리 공간 속에서 둥둥 떠다닌다는 의미다.
  - 이러한 것들이 쌓이면 **Meamory Leak**이 발생하게 된다.
- 객체가 소멸될 때 자동으로 객체를 delete해주느 함수가 **Destructor**인 것이다.

```cpp
Marine::~Marine() {
    std::cout << name << " 의 소멸자 호출 " << std::endl;
    if (name != NULL) {
        delete[] name;
    }
}
```
- 위와 같은 소멸자를 추가해준다.
  - public쪽에 ~Marine(); 추가하고 해야된다.
- 소멸자의 호출 방식은 ~(ClassName)인 것이다.
- name은 배열로 동적할당해서 delete역시 delete[]으로 해제해줘야한다.

```
...
Marine 2 의 소멸자 호출 
Marine 1 의 소멸자 호출 
```
- delete marines[idx]를 호출로 인해 객체가 소멸될 때 소멸자가 호출되는 것을 알 수 있다.

## Destructor의 호출 순서
```cpp
#include <iostream>
#include <string.h>

class Test
{
    char c;

public:
    Test(char _c)
    {
        c = _c;
        std::cout << "생성자 호출 " << c << std::endl;
    }

    ~Test() { std::cout << "소멸자 호출 " << c << std::endl; }
};

void simple_function() { Test b('b'); }
int main()
{
    Test a('a');
    simple_function();
}
```
- simple_fucntion안에서 b 객체를 생성하므로 b는 simple_function의 **지역 객체**이기 때문에 simple_function이 종료됨과 동시에 b의 소멸자가 호출된다.
- main 함수 종료 시 지역 객체였던 a가 소멸되면서 a의 소멸자가 호출된다.

```
생성자 호출 a
생성자 호출 b
소멸자 호출 b
소멸자 호출 a
```
- 그래서 위와 같은 순서로 호출되는 것이다.
---
- 소멸자의 역할은 상당히 중요하다.
- 객체가 다른 부분에 영향을 끼치지 않도록 깔끔하게 소멸시키는 일은 매우 중요하다.
- 가장 흔한 역할은 동적으로 할당받은 메모리를 해제하는 일이다.
- 그 외에도 Thread 사이에서 lock된 것을 푸는 역할 등을 수행한다.
- Default Destructor도 있다. 소멸자 내부에서 아무런 작업도 수행하지 않게 된다.
  - 소멸자가 필요없는 클래스에서 사용


