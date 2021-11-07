# Reference를 Return하는 함수
```cpp
#include <iostream>

class A
{
    int x;

public:
    A(int c) : x(c) {}

    int &acces_x() { return x; }
    int get_x() { return x; }
    void show_x() { std::cout << x << std::endl; }
};

int main()
{
    A a(5);
    a.show_x();

    int &c = a.acces_x();
    c = 4;
    a.show_x();

    int d = a.acces_x();
    d = 3;
    a.show_x();

    // error
    // int& e = a.get_x();
    // e = 2;
    // a.show_x();

    int f = a.get_x();
    f = 1;
    a.show_x();

    return 0;
}
```

```
5
4
4
```
- 위와 같은 코드가 있다고 해보자.
- acces_x는 x의 **Reference**를 Return하게 된다.
- get_x는 x의 **값**을 Return 한다.
- c는 x의 Reference, 즉 별명을 받았다.

```cpp
int &c = x; // a의 x이다.
```
- Reference를 리턴하는 함수는 그 함수 부분을 원래 변수로 치환했다고 생각해도 상관없다.
---
- d는 int&가 아닌 int 변수에 x의 별명을 전달했다.
- d가 int&였다면 x의 별명을 받아 또다른 Reference가 되었겠지만, int 변수이므로 **값의 복사**가 일어나 d에는 x의 값이 들어간다.
  - d는 독립적인 변수이므로 x가아니다.
  - d=3; 을해도 x의 값은 바뀌지 않는 것이다.

```cpp
int& e = a.get_x();
e = 2;
a.show_x();
```

```
error C2440: 'initializing' : cannot convert from 'int' to 'int &'
```
- 위와 같은 에러가 나는 코드다.
- Reference가 아닌 Type을 Return하는 경우 **값의 복사**가 이루어져서 임시 객체가 생성된다.
- 임시 객체의 Reference를 가질 수 없기 때문에 오류가 발생한다.

![image](https://user-images.githubusercontent.com/69780812/140634370-ff13b0c9-4aff-4e6b-b469-4d20cef163b4.png)
- 값의 복사 시 생성되는 임시 객체는 문장이 끝나게 되면 소멸된다.
- get_x의 리턴으로 인해 임시로 **복사생성**된 int는 a.get_x()부분을 대체한다.
- x'은 문장이 끝날 때 자동으로 소멸되는 **임시 객체**이므로 Reference를 만들수 없다.
  - 만들어졌다해도 이미 존재하지 않는 것에 대한 Reference이므로 접근하는 것은 오류가 된다.
- 정확히 설명하면 int&은 l-value에 대한 reference, a.get_x()는 r-value 이므로 Reference를 만들 수 없다고한다.

```cpp
a.access_x() = 3;
```
- 잘 작동하는 코드다.
- **Reference를 Return하는 함수는 그 함수 부분을 Return하는 원래 변수로 치환해도 된다.** 라는 말이 명확히 들어 맞다는 것을 확인할 수 있다.

```cpp
a.get_x() = 3;
```
- 이는 오류가 나는 코드다.
- a.get_x()는 리턴하면서 생성되는 임시 객체 x'으로 치환되며 임시 객체에 대입하는 것은 모순적인 상황이기 때문이다.

```cpp
Marine& Marine::be_attacked(int damage_earn) {
  this->hp -= damage_earn;
  if (this->hp <= 0) this->is_dead = true;

  return *this;
}
```
- 이 경우에도 Marine& 타입을 리턴하게 되는데 *this를 Return 하게 된다.
- this : 이 함수를 호출한 객체
  - *this : 그 객체 자신

```cpp
marine2.be_attacked(marine1.attack()).be_attacked(marine1.attack());
```
- marine2.be_attacked(marine1.attack())이 먼저 실행되고 그 다음 똑같은 것이 실행된다고 생각할 수 있겠다.

```cpp
MarineMarine::be_attacked(int damage_earn) {
  this->hp -= damage_earn;
  if (this->hp <= 0) this->is_dead = true;

  return *this;
}
```
- Marin&을 리턴하는게 아니라 Marine을 리턴한다고 생각해보자.
- 이전 코드를 실행하면 marine2는 실제로 두번 공격이 아닌 1번 공격으로 감소한 HP만 보일 것이다.
- 임시 객체 Marine을 생성해서 *this의 내용으로 복사가 되고(**복사생성자 호출**) 이 임시 객체에 대한 be_acttacked 함수가 호출되는 것이기 떄문이다.
  - 따라서 marine2가 아닌 엉뚱한 임시 객체에 대해 호출하는 것이 되므로 marine2는 marine1의 공격을 1번만 받게 되는 것이다.