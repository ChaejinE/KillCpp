# Overview
- 치환 오류는 컴파일 오류가 아니다.
  - Substitution failure is not an error (SFINAE)

```cpp
#include <iostream>

template <typename T>
void test(typename T::x a)
{
    std::cout << "T::x \n";
}

template <typename T>
void test(typename T::y b)
{
    std::cout << "T::y \n";
}

struct A
{
    using x = int;
};

struct B
{
    using y = int;
};

int main()
{
    test<A>(33);

    test<B>(22);
}
```

```
T::x
T::y
```
- 컴파일러는 템플릿 인자의 타입들을 유추한 후 템플릿 인자들을 해당 타입으로 치환한다.
- 문제는 템플릿 인자들을 유추한 타입으로 치환을 할 때 문법적으로 말이 안되는 경우들이 있기 마련이다.
- test<\A>(33); 함수를 컴파일 시 void test(A::y b) ...로 치환될 것이다. 이는 문법ㅂ적으로 올바르지 않은 식이다.
  - A는 y 라는 타입이 없다.
- 하지만 컴파일러는 오류를 발생시키지 않는다.
  - SFINAE 원칙. 치환 오류는 컴파일 오류가 아니다.
- 치환해서 만들어진 식이 문법적으로 맞지 않으면 오류를 발생시키는 대신 단순히 함수 오버로딩 후보군에서 제외만 시키게 된다. 따라서 test(A::y b)의 경우 오버로딩 후보군에서 제외되게된다.
- SFINAE(스피내)를 호라용하게되면 원하지 않는 타입들에 대해 오버로딩 후보군에서 제외할 수 있다.
  - type_traits에는 해당 작업을 손쉽게 할 수 잇는 메타함수를 하나 제공한다. (enable_if)