# Overview
- C++11에 decltype처럼 추가된 std::declval 함수에 대해 알아보자.

```cpp
struct A
{
    int f() { return 0; }
};

decltype(A().f()) ret_val;
```
- 어떤 타입 T의 f 라는 함수의 리턴타입을 정의해본다고하자.
- 위는 int ret_val;이 된다.
- 참고로 위 과정에서 A의 객체가 생성되거나 함수 f가 호출되거나 하지는 않는다.
  - decltype 안에 들어가는 식은 **그냥 식의 형태로 존재할 뿐 컴파일 시 decltype() 전체 식이 타입으로 변환되므로 decltype안에 식은 런타임 시에 실행되는 것이 아니다.**

```cpp
struct B
{
    B(int x) {}
    int f() { return 0; }
};

int main()
{
    decltype(B().f()) ret_val;
}
```
- 위는 컴파일 오류가 발생한다.
- decltype안에 틀린 식을 전달할 수 있지는 않ㄴ다.
  - B의 클래스에서 디폴트 생성자가 없다.
- 단지 B의 멤버변수 f의 타입 참조만 필요할 뿐인데 실제 B 객체를 생성할게 아닌데도 생성자 규칙에 맞는 코드를 작성해야된다.
  - 디폴트 생성자를 정의하는 타입일 수 있다.

```cpp
#include <utility>

template <typename T>
decltype(std::declval<T>().f()) call_f_and_return(T& t)
{
    return t.f();
}

struct A
{
    int f() { return 0; }
};

struct B
{
    B(int x) {}
    int f() { return 0; }
};

int main()
{
    A a;
    B b(1);

    call_f_and_return(a);
    call_f_and_return(b);
}
```
- call_f_and_return 함수에 디폴트 생성자가 없어도 오류가 발생하지 않는다.
  - 보통 타입들의 생성자가 모두 같은 꼴이 아니므로 문제가 생긴다.
- std::declval에 type을 전달하면 T type의 생성자를 직접 호출 하지 않더라도 T가 생성된 객체를 나타낼 수 있다.

```cpp
std::declval<T>()
```
- 위를 통해 심지어 T에 생성자가 존재하지 않아도 마치 T()를 한 것과 같은 효과를 낼 수 있다.
  - 따라서 생성자 형태가 모두 달라서 발생하는 오류를 막을 수 있다.

```cpp
#include <utility>

struct B
{
    B(int x) {}
    int f() { return 0; }
};

int main() { B b = std::declval<B>(); }
```
- declval 함수를 타입 연산에서만 사용하고 런타임에 사용하면 오류가 발생하니 주의하자.

```cpp
template <typename T>
auto call_f_and_return(T& t)
{
    return t.f();
}
```
- C++14 부터는 함수의 리턴 타입을 컴파일러가 알아서 유추해주는 기능이 추가되었다.
- 함수 return 타입을 auto로 지정해주면된다.
---
- decltype 키워드를 통해 우리가 원하는 식의 타입을 알 수 있다.
  - 단순 식별자일 경우 (identifier expression) 그 식의 타입으로 치환된다.
  - 그 이외에는 식의 값 카테고리가 뭐냐에따라 decltype의 타입이 정해진다.
- Cpp의 모든 식에는 두 가지 꼬리표가 있으며 타입, 값 카테고리다.
  - lvalue, prvalue, xvalue
- std::declval 함수를 사용해서 원하는 타입의 생성자 호출을 우회해서 멤버 함수의 타입에 접근할 수 있다.