# Static
## Marine 수 세기
***방법 1.*** 어떤 배열에 Marine 을 보관하고 생성된 마린 개수를 모두 센다.
***방법 2.*** 어떤 변수를 만들어서 Marine의 생성시 1을 추가하고 소멸 시 1을 뺀다.
- 방법 1. 의 경우, 크기가 자유자재로 변할 수 있는 배열을 따로 만들어야하는 문제점이있다.
- 방법 2.의 경우 어떤 함수 내 이런 변수를 정의헀다면 다른 함수에서도 그 값을 이용하기 위해 **인자로 계속 전달해줘야 하는**귀찮음이 있다.
- 전역 변수로 ?
  - 프로젝트의 크기가 커질 수록 프로그래머의 실수로 인해 서로 겹쳐 오류날 가능성이 다분해서 필요한 경우가 아니면 사용하지 않는다.
- CPP는 static 멤버 변수를 제공함으로써 위같은 문제를 해결할 수 있다.
## static
- C언어에서 static은 함수가 종료될 때 소멸되는 것이 아니라 프로그램이 종료될 때 소멸되는 것처럼 어떤 클래스의 static 멤버 변수는 멤버 변수들 처럼 객체가 소멸될 때 소멸되는 것이 아닌, **프로그램이 종료될 때 소멸되는 것**이다.
---
- Static 멤버 변수의 경우 클래스의 모든 객체들이 **공유 하는 변수**라고 생각하면 된다.
- 모든 객체들이 하나의 static 멤버 변수를 사용하게 된다.

```cpp
#include <iostream>

class Marine
{
    static int total_marine_num;

    int hp;               // 마린 체력
    int coord_x, coord_y; // 마린 위치
    bool is_dead;

    const int default_damage; // 기본 공격력

public:
    Marine();             // 기본 생성자
    Marine(int x, int y); // x, y 좌표에 마린 생성
    Marine(int x, int y, int default_damage);

    int attack();                      // 데미지를 리턴한다.
    void be_attacked(int damage_earn); // 입는 데미지
    void move(int x, int y);           // 새로운 위치

    void show_status(); // 상태를 보여준다.
    ~Marine() { --total_marine_num; }
};
int Marine::total_marine_num = 0;

Marine::Marine()
    : hp(50), coord_x(0), coord_y(0), default_damage(5), is_dead(false) { ++total_marine_num; }

Marine::Marine(int x, int y)
    : coord_x(x), coord_y(y), hp(50), default_damage(5), is_dead(false) { ++total_marine_num; }

Marine::Marine(int x, int y, int default_damage)
    : coord_x(x),
      coord_y(y),
      hp(50),
      default_damage(default_damage),
      is_dead(false) { ++total_marine_num; }

void Marine::move(int x, int y)
{
    coord_x = x;
    coord_y = y;
}
int Marine::attack() { return default_damage; }
void Marine::be_attacked(int damage_earn)
{
    hp -= damage_earn;
    if (hp <= 0)
        is_dead = true;
}
void Marine::show_status()
{
    std::cout << " *** Marine *** " << std::endl;
    std::cout << " Location : ( " << coord_x << " , " << coord_y << " ) "
              << std::endl;
    std::cout << " HP : " << hp << std::endl;
    std::cout << " 현재 총 마린 수 : " << total_marine_num << std::endl;
}

void create_marine()
{
    Marine marine3(10, 10, 4);
    marine3.show_status();
}
int main()
{
    Marine marine1(2, 3, 10);
    Marine marine2(3, 5, 10);

    marine1.show_status();
    marine2.show_status();

    create_marine();

    std::cout << std::endl
              << "마린 1 이 마린 2 를 공격! " << std::endl;
    marine2.be_attacked(marine1.attack());

    marine1.show_status();
    marine2.show_status();
}
```
- total_marine_num이라는 static 변수를 만든 코드다.

```
*** Marine *** 
 Location : ( 2 , 3 ) 
 HP : 50
 현재 총 마린 수 : 2
 *** Marine *** 
 Location : ( 3 , 5 ) 
 HP : 50
 현재 총 마린 수 : 2
 *** Marine *** 
 Location : ( 10 , 10 ) 
 HP : 50
 현재 총 마린 수 : 3

마린 1 이 마린 2 를 공격! 
 *** Marine *** 
 Location : ( 2 , 3 ) 
 HP : 50
 현재 총 마린 수 : 2
 *** Marine *** 
 Location : ( 3 , 5 ) 
 HP : 40
 현재 총 마린 수 : 2
```
- marine3 은 지역 객체여서 create_marine 함수가 끝나면 사라진다. 그래서 2명이다.

```cpp
int Marine::total_marine_num = 0;
```
- static 변수들은 정의와 동시에 값이 자동으로 0으로 초기화된다.
  - 굳이 따로 초기화 안해도 된다.

```cpp
class Marine{
    static int total_marine_num = 0; // 불가능
}
```

```cpp
class Marine{
    const static int total_marine_num = 0; // 가능
}
```
- static 변수는 클래스 내부에서 초기화하는 것은 불가능하다.
  - const static 변수일 때만 가능하다.

## static method
```cpp
Marine::show_total_marine();
```
- (Class)::(Static Method) 형식으로 호출하게 된다.
  - 어떠한 객체도 이 함수를 소유하고 있지 않기 때문이다.
- static 함수내에서는 Class의 **static 변수 만 이용**할 수밖에 없다.
  - 다른 멤버변수들을 사용한다면, 이 멤버 변수가 어떤 객체의 변수인지 알 수가 없다.
  - static 함수는 어떤 객체에도 속해있지 않기 떄문이다.

# this
```cpp
Marine& Marine::be_attacked(int damage_earn) {
  hp -= damage_earn;
  if (hp <= 0) is_dead = true;

  return *this;
}
```
- this : 객체 자신을 가리키는 Pointer 역할
  - C++ 언어 차원에서 정의되어있는 키워드

```cpp
Marine& Marine::be_attacked(int damage_earn) {
  this->hp -= damage_earn;
  if (this->hp <= 0) this->is_dead = true;

  return *this;
}
```
- 사실상 위와 동일한 의미가 된다.
  - 구조체 포인터 변수에서 ->를 통해 구조체 원소들에 접근했던 것과 같다.
- 실제 모든 멤버 함수 내에서는 this 키워드가 정의되어 있으며 this 키워드가 없는 함수는 static 함수 뿐이다.
- Marin&의 경우에 대한 설명은 reference_return.md를 참고하자.

# const 함수
```cpp
int attck() const;
```
- 상수 함수는 위와 같은 형태로 선언하게 된다.

```cpp
int Marine::attack() const { return default_damage; }
```
- 함수 정의 역시 const 키워드를 꼭 넣어줘야 한다.
- 이 함수는 다른 변수의 값을 바꾸지 않는 함수라고 다른 프로그래머에게 명시 시킬 수 있다.
- 당연히 상수 함수 내에서는 객체들의 **읽기만 수행**이 되며 **상수 함수 내에서 호출할 수 있는 함수로는 다른 상수 함수 밖에 없다.**
- 많은 경우 클래스 설계시, 멤버 변수들은 private에 넣고 이 변수에 접근하는 방법으로 get_x 함수를 public에 넣어 리턴받는 형식을 많이 사용한다.
