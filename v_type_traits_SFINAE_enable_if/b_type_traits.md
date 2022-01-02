# is_class
- 이 메타함수는 인자로 전달된 타입이 **클래스인지 아닌지 확인하는 메타함수**이다.

```cpp
namespace detail
{
    template <class T>
    char test(int T::*);

    struct two
    {
        char c[2];
    };

    template <class T>
    two test(...);
}

template <class T>
struct is_class
    : std::intergral_constant<bool, sizeof(detail::test<T>(0)) == 1 &&
    !std::is_union<T>::value> {};
```
- ????
- std::intergral_const는 std::intergral_constant<\T, T v>로 정의되어 있는데, v를 static 인자로 가지는 클래스이다.
  - **어떤 값을 static 객체로 가지고 있는 클래스를 만들어주는 템플릿**이라고 생각하면된다.
  - integral_constant<\bool, fasle>는 value가 false인 클래스다.
  - sizeof(detail::test<\T>(0)) == 1 && !std::is_union<\T>::value 부분이 false면 is_class는 false가 되고 true면 ture가 되는 것이다.
- sizeof(detail::test<\T>(0)) == 1은 T가 클래스일 때만 1이 될까?

## Pointer to Data member
- 데이터 멤버를 가리키는 포인터

```cpp
template <class T>
char test(int T::*)
```
- int T::* 문법은 매우 생소하다. 이는 T의 int 멤버를 가리키는 포인터라는 의미다.

```cpp
#include <iostream>
#include <type_traits>

class A
{
    public:
        int n;
    
    A(int n) : n(n) {}
};

int main()
{
    int A::*p_n = &A::n;

    A a(3);
    std::cout << "a.n : " << a.n << std::endl;
    std::cout << "a.*p_n : " << a.*p_n << std::endl;
}
```

```
a.n : 3
a.*p_n : 3
```
- p_n은 A의 int 멤버를 가리킬 수 있는 포인터를 의미한다.
  - 이 때 실제하는 어떤 객체의 int 멤버를 가리키는 것이아니다.
  - p_n을 역참조하면 A의 n을 참조하는 식으로 사용할 수 있다.
  - 그래서 a.n이나 a.*p_n이나 같은 문장이 된다.
  - 이와같은 형태의 포인터를 데이터 멤버를 가리키는 포인터라고 한다.
- 데이터 멤버를 가리키는 포인터는 한가지 제한점이 있는데 클래스에만 사용할 수 있다는 것이다.

```cpp
template <class T>
char test(int T::*);
```
- 위 문장은 T가 클래스가 아니면 불가능한 문장이다.
  - T가 클래스라면 해당 클래스에 int 데이터 멤버가 없어도 유효하다.
- int 멤버가 없ㅅ으면 아무것도 가맄리 수 없다. 하지만 이전 코드에서는 T가 클래스인지 판별하는데 사용하므로 상관없다.

```cpp
struct two
{
    char c[2]
};

template <class T>
two test(...);
```
- 위 test 함수의 경우 사실 T가 무엇이냐에 관계없이 항상 인스턴스화 될 수 있다.
- 가변 길이 템플릿 함수처럼 임의의 개수ㄹ의 인자를 받는 함수다.
---
- detail::test<\T>(0)를 컴파일할 때 위 둘 중 어떤 것으로 오버로딩 할지 결정해보자.
- 첫 번째의 경우가 좀 더 구체적(인자가 명시)이어서 우선순위가 높아 1번으로 오버로딩 된다.
  - test<\T>(0)의 return type은 char가 되고 sizeof(char)는 1이므로 통과가된다.
  - 하지만 역시 클래스가 아니면 불가능한 문법이다.
  - 컴파일 오류가 아니라 오버로딩 후보군에서 제외될 것이다.
- 클래스가 아니라면 두 번째가 유일한 후보군이므로 detail::test<\T>(0)의 return type은 two가 될 것이다. 이 때 two는 char c\[2]dlamfh sizeof가 2가 된다. 그래서 is_class의 value는 false로 연산된다.

```cpp
sizeof(detail::test<T>(0)) == 1 && !std::is_union<T>::value
```
- 위에서 앞부분은 T가 클래스일 때 참이 되고 클래스가 아니라면 거짓이 됨을 알 수 있다.
- c++에서 데이터 멤버를 가리키는 포인터가 허용되는 것은 클래스와 공용체(union)이 두가지가 있다.
- 따라서 앞의 문장은 T가 공용체일 때도 성립하므로 확실히 클래스임을 보이기 위해 추가적으로 is_union을 통해 공용체가 아님을 확인해야한다.