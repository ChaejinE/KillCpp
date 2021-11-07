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

# Constructor Initialzer list
```cpp
Marine::Marine() : hp(50), coord_x(0), coord_y(0), damage(5), is_dead(false) {}
```
- 함수 본체에는 아무 것도 없어도 된다.
- 추가된 이상한 것들이 기존 생성자가 하던 일과 동일한 작업을 하고 있을 뿐이다.

```cpp
:: hp(50), coord_x(0), coord_y(0), damage(5), is_dead(false) {}
```
- 생성자 이름 뒤에 오는 것을 Initializer List(초기화 리스트)라고 부른다.
- 생성자 호출과 동시에 멤버 변수들을 초기화해주게 된다.

```cpp
(생성자 이름) : var1(arg1), var2(arg2) {}
```
- 일반적인 꼴은 위와같이 이해하면 될거 같다.
- 흥미로운 점은 var1과 arg1의 이름이 같아도 된다.
  - (arg1)은 무조건 Class가 인자로 받은 값을, var1은 Class 내의 member variable을 지칭하는 것이기 때문이다.
  - 이전에는 이렇게하면 오류가 난다.
## Initializer List, 왜 사용하는 가 ?
- **생성과 초기화를 동시에 하게 된다.**
- 반면 사용하지 않았을 시 **생성을 먼저하고 그 다음에 대입**을 수행한다.

```cpp
int a = 10;
```

```cpp
int a;
a = 10;
```
- 위와 동일하다고 볼 수 있는데, 전자의 경우 복사 생성자가 호출된다.
- 후자의 경우 디폴트 생성자가 호출된 뒤 대입이 수행된다.
- 딱 보아도 후자의 경우가 하는 일이 더 많아 보인다.
- **생성과 동시에 초기화 되어야하는 것들** : Reference, Constant
  - 전자의 경우 처럼 선언하면 사용이 불가하게될 것이다. (컴파일 오류)
  - Reference, Const 변수는 무조건 초기화 리스트를 사용해서 초기화 시켜줘야한다.


# Constructor Overloading
- 생성자 역시 함수이므로 함수의 Overloading이 적용될 수 있다.
- Class 객체를 여러가지 방식으로 생성할 수 있게 되는 것이다.

```cpp
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

# 복사 생성자
```cpp
// 포토캐논
#include <string.h>
#include <iostream>

class Photon_Cannon
{
    int hp, shield;
    int coord_x, coord_y;
    int damage;

public:
    Photon_Cannon(int x, int y);
    Photon_Cannon(const Photon_Cannon &pc);

    void show_status();
};
Photon_Cannon::Photon_Cannon(const Photon_Cannon &pc)
{
    std::cout << "복사 생성자 호출 !" << std::endl;
    hp = pc.hp;
    shield = pc.shield;
    coord_x = pc.coord_x;
    coord_y = pc.coord_y;
    damage = pc.damage;
}
Photon_Cannon::Photon_Cannon(int x, int y)
{
    std::cout << "생성자 호출 !" << std::endl;
    hp = shield = 100;
    coord_x = x;
    coord_y = y;
    damage = 20;
}
void Photon_Cannon::show_status()
{
    std::cout << "Photon Cannon " << std::endl;
    std::cout << " Location : ( " << coord_x << " , " << coord_y << " ) "
              << std::endl;
    std::cout << " HP : " << hp << std::endl;
}
int main()
{
    Photon_Cannon pc1(3, 3);
    Photon_Cannon pc2(pc1);
    Photon_Cannon pc3 = pc2;

    pc1.show_status();
    pc2.show_status();
}
```
- 포토케논 예제다.

```cpp
Photon_Cannon(const Photon_Cannon& pc);
```
- 복사 생성자의 표준적 정의다.

```cpp
T(const T& a);
```
- T의 객체 a를 상수 레퍼런스로 받는 다는 의미로 정의하면된다.
- a가 const 이므로 복사 생성자 내부에서 a의 데이터를 변경할 수 없고, 오직 새롭게 초기화되는 Instance 변수들에게 **복사만 할 수 있게 된다.**

```cpp
Photon_Cannon::Photon_Cannon(const Photon_Cannon& pc) {
  std::cout << "복사 생성자 호출 !" << std::endl;
  hp = pc.hp;
  shield = pc.shield;
  coord_x = pc.coord_x;
  coord_y = pc.coord_y;
  damage = pc.damage;
}
```
- 이 부분에서 pc.coord_x=3; 이런식으로 **pc의 값 자체는 변경할 수 없다**는 말이다.
- const가 헷갈리면 ConstPoimter(Ref).md를 보자.

## 복사 생성자 사용법
```cpp
Photon_Cannon pc1(3, 3);
Photon_Cannon pc2(pc1);
```
- pc1은 int x, int y를 인자로 가지는 생성자가 오버로딩되었다.
- pc2는 pc1을 넘겨서 **복사 생성자가 호출된다.**

```cpp
Phton_Cannon pc3 = pc2;
```
- 위 코드 역시 **복사 생성자가 호출된다.**
- 그 위의 코드와 동일한 형태로 동작하게 된다.
  - Photon_Cannon pc3(pc2);
- 생성자가 **1번 호출**된다.
- 사용자가 직관적이고 깔끔한 프로그래밍을 할 수 있다.

```cpp
Photon_Cannon pc3;
pc3 = pc2;
```
- 생성자가 1번 호출되고, pc3 = pc2; 라는 명령이 실행되는 것이다.
  - 평범한 대입 연산이다.
- 복사 생성자는 오직 **생성 시에 호출**된다는 것을 명심해야된다.
- 그 위의 코드와 엄연히 다른 코드다.
---
- 그런데 사실 Deafult Copy Constructor를 지원해주고 있다.
- 본 코드에서 복사 생성자를 지워보고 실행해보면 정확히 이전과 동일한 결과가 나타난다.
- 실제로 **복사**를 해주는 일이 일어나는 것이다.
  - 추정해보면, 위의 복사 생성자에서 정의한 것처럼 일이 작동할 것이다.
  - 간단한 클래스의 경우 귀찮게 복사생성자를 써주지 않고, 디폴트 생성자를 이용한다.
---
## 디폴트 복사 생성자의 한계
```cpp
// 포토캐논
#include <string.h>
#include <iostream>

class Photon_Cannon
{
    int hp, shield;
    int coord_x, coord_y;
    int damage;

    char *name;

public:
    Photon_Cannon(int x, int y);
    Photon_Cannon(const Photon_Cannon &pc);
    Photon_Cannon(int x, int y, const char *cannon_name);
    ~Photon_Cannon();

    void show_status();
};
Photon_Cannon::Photon_Cannon(const Photon_Cannon &pc)
{
    std::cout << "복사 생성자 호출 !" << std::endl;
    hp = pc.hp;
    shield = pc.shield;
    coord_x = pc.coord_x;
    coord_y = pc.coord_y;
    damage = pc.damage;

    name = NULL;
}
Photon_Cannon::Photon_Cannon(int x, int y)
{
    std::cout << "생성자 호출 !" << std::endl;
    hp = shield = 100;
    coord_x = x;
    coord_y = y;
    damage = 20;

    name = NULL;
}
Photon_Cannon::Photon_Cannon(int x, int y, const char *cannon_name)
{
    std::cout << "name 생성자 호출 !" << std::endl;
    hp = shield = 100;
    coord_x = x;
    coord_y = y;
    damage = 20;

    name = new char[strlen(cannon_name)];
    strcpy(name, cannon_name);
}
Photon_Cannon::~Photon_Cannon()
{
    if (name) delete[] name;
}
void Photon_Cannon::show_status()
{
    std::cout << "Photon Cannon " << std::endl;
    std::cout << " Location : ( " << coord_x << " , " << coord_y << " ) "
              << std::endl;
    std::cout << " HP : " << hp << std::endl;
}
int main()
{
    Photon_Cannon pc1(3, 3);
    Photon_Cannon pc2(pc1);
    Photon_Cannon pc3 = pc2;

    pc1.show_status();
    pc2.show_status();
}
```
- char *name을 추가한 코드다.
- 위 처럼 만들어서 실행했을 경우, 런타임에러가 발생한다.
- 디폴트 복사 생성자는 1 대 1로 원소들 간의 정확한 복사를 수행해준다.

```cpp
Photo_Cannon::Photo_Cannon(const Photo_Cannon& pc) {
    hp = pc.hp;
    shield = pc.shield;
    coord_x = pc.coord_x;
    coord_y = pc.coord_y;
    damage = pc.damage;
    name = pc.name;
}
```
- 추정해보자면 위와 같이 작동한다고 생각할 수 있다.

![image](https://user-images.githubusercontent.com/69780812/140632329-0d80099e-9285-4dee-82a6-38066377dfe7.png)
- 이 그림은 복사 생성자 호출한 뒤에 pc1과 pc2가 어떻게 되었는지에대한 그림이다.
- pc1의 name이 동적으로 할당 받아 가리키고 있던 메모리 (Cannon 이라는 문자열이 저장된 메모리)를 **pc2의 name도 같이 가리키게 된다.**
  - 이 상태에서는 문제가 될건 없어보인다.
- 문제는 **소멸자에서 일어난다.**

![image](https://user-images.githubusercontent.com/69780812/140632354-ad5fa723-8f10-462b-a663-b7670ad87199.png)
- main 함수 종료 직전에 객체들이 파괴되면서 소멸자를 호출한다.
- pc1이 먼저 파괴되었다고 생각해보자.
- 0x125ADD3에 할당한 메모리가 delete 될 것이다.
- 문제는 pc2의 name이 **해제된 메모리인 0x125ADD3을 가리키고 있다는 것이다.**
  - 이미 해제된 메모리에 접근해서 다시 해제하려고 했기에 런타임 오류가 발생한다.
  - 사실, 접근한 것에서 부터 오류다.
- 해결책은 다른 메모리에 동적 할당해서 그 내용만 복사하면 될 것이다.
  - **Deep Copy**
  - 아까처럼 단순 대입만 해주는 것을 **Shallow Copy**라고한다.
- Compiler가 생성하는 Default Copy Constructor는 Shallow Copy만 수행한다.
- Deep Copy를 수행하기 위해서는 사용자가 직접 복사 생성자를 만들어야한다.

![image](https://user-images.githubusercontent.com/69780812/140632406-81ada124-a7c4-4a9d-8b68-c0ee7c48bc02.png)
- 다른 변수들은 얕은 복사를 한다.
- name의 경우 따로 메모리를 할당해서 그 내용만 복사하는 DeepCopy를 수행하는 것이다.
- 그러면 소멸자에서도 메모리 해제 시 각기 다른 메모리를 해제하므로 전혀 문제가 발생하지 않는다.

```cpp
Photon_Cannon::Photon_Cannon(const Photon_Cannon &pc) {
    std::cout << "복사 생성자 호출 " << std::endl;
    hp = pc.hp;
    shield = pc.shield;
    coord_x = pc.coord_x;
    coord_y = pc.coord_y;
    damage = pc.damage;

    name = new char[strlen(pc.name) + 1];
    strcpy(name, pc.name);
}
```
- 위와 같이 복사 생성자를 DeepCopy하도록 정의할 수 있는 것이다.
- 사실, C언어 처럼 char 배열로 문자열을 다르는 것은 **매우 비추**한다고한다.
  - C++의 std::string이라는 문자열 클래스를 사용하여 숙지하자 !
