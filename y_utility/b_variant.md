# Overview
- One-of를 구현한 클래스라고 보면된다.
- 컴파일 타임에 정해진 여러가지 타입들 중 한 가지 타입의 객체를 보관할 수 있는 클래스다.
- union으로 해결할 수 도 있겠지만 union이 현재 어떤 타입의 객체를 보관하고 있는지 알 수 없으므로 실제로 사용하기에는 매우 위험하다.

# std::variant
```cpp
std::variant<int, std::string, double> v = 1;

v = "abc";

v = 3.14;
```
- variant 정의 시 포함하고자 하는 타입들을 명시해줘야한다.
  - 위에서는 int, string, double 셋 중 하나의 타입을 가질 수 있다.
- variant는 **반드시 값을 들고 있어야한다.**
  ```cpp
  std::variant<int, std::string, double> v;
  ```
  - 이 처럼 선언하더라도 int의 디폴트 생성자가 호출되면서 0이들어가게된다. 비어있는 것은 불가능하다는 뜻이다.
- 객체 대입 시 어떤 동적할당도 발생하지 않는다.
  - 작은 오버헤드로 객체들을 보괂라 수 있다.
  - 객체 자체 크기는 나열된 가능한 타입들 중 가장 큰 타입의 크기를 따라간다.
- variant는 이러이러한 타입들중 하나, One-of를 표현하기 매우 적합한 도구다.

```cpp
#include <iostream>
#include <memory>
#include <variant>

class A
{
    public:
        void a() { std::cout << " A " << std::endl; }
};

class B
{
    public:
        void b() { std::cout << " B " << std::endl; }
};

std::variant<A, B> GetDatafromDB(bool is_a)
{
    if (is_a)
        return A();
    return B();
}

int main()
{
    auto v = GetDatafromDB(true);

    std::cout << v.index() << std::endl;
    std::get<A>(v).a();
}
```

```
0
 A
```
- variant 역시 optional과 마찬가지로 각각의 타입의 객체를 받는 생성자가 정의되어있다.
- A를 return 하면 A를 가지는 variant, B면 B를 가지는 variant가 생성된다.
- 현재 variant에 몇 번째 타입이 들어가있는지 알려면 index()를 사용하면 된다.
- 실제 원하는 값을 뽑으려면 std::get<\T>를 이용하면 된다.
  - 뽑아내고자하는 Type or type의 index 번호를 넣어주면 된다.
- variant가 보관하는 객체들은 타입으로 구분 되므로 같은 타입을 중복하면 안된다.
  - 컴파일 오류

# std::monostate
- variant에 아무 것도 들고 있지 않은 상태를 표현하려면 해당 타입으로 std::monostate를 사용함녀 된다.
  - 마치 std::optional과 같은 효과를 낼 수 있다.

```cpp
std::variant<std::monostate, A, B> v;
```
- 이렇게 정의하면 v에는 아무것도 안들어있거나 A, B가 들어가있을 수 있다.
- 디폴트 생성자가 없더라도 monostate 덕에 오류가 안난다.