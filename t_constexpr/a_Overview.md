# Overview
- constexpr 키워드는 객체나 함수 앞에 붙일 수 있는 키워드다.
- **해당 객체나 함수의 리턴값은 컴파일 타임에 값을 알 수 있다**는 의미를 전달하게 된다.
- 컴파일러가 컴파일 타임에 어떤 식의 값을 결정할 수 있다면 **Constant Expression**(상수식)으로 표현할 수 있다.
- 이런 상수식들 중 값이 정수인 것이 **정수 상수식(Integral constant expression)**이라고 한다. 이 정수 상수식들은 매우 쓰임새가 많다.

```cpp
int arr[size]
```
- 위 배열 선언식이 컴파일되려면 size가 정수 상수식이어야한다.

```cpp
template <int N>
struct A
{
    int operator()() { return N; }
};

A<number> a;
```
- 템플릿 타입 인자의 경우에도 마찬가지로 number가 정수 상수식 이어야한다.

```cpp
enum A= { a = number, b, c };
```
- enum에서 값을 지정해줄 때 오는 number도 역시 정수 상수식이어야만 한다.
- 이처럼 C++ 언어 상 정수 상수식이 등장하는 곳은 매우 많다.