# Overview
- 타입 관련 연산을 사용할 때 요긴하게 쓰인다.
- c++11에 추가된 키워드
- decltype 라는 이름의 함수처럼 사용된다.

```cpp
decltype(/* type을 알고자 하는 식 */)
```

# decltype keyword
```cpp
#include <iostream>

struct A
{
    double d;
};

int main()
{
    int a = 3;
    decltype(a) b = 2;

    int& r_a = a;
    decltype(r_a) r_b = b;

    int&& x = 3;
    decltype(x) y = 2;

    A* aa;
    decltype(aa->d) dd = 0.1;
}
```
- 함수와는 달리 타입을 알고자하는 식의 타입으로 치환되게 된다.
- 위의 경우 decltype이 각각 int, int&, int&&로 치환되서 컴파일 되게 된다.
- **괄호로 둘러쌓이지 않은 식별자 표현식(id-expression)**이라면 해당 식의 타입을 얻을 수 있다.
  - 식별자 표현식 : 변수의 이름, 함수의 이름, enum 이름, 클래스 멤버변수(a.b, a->b와 같은 꼴) 등을 의미한다.
    - 쉽게 생각하면 어떤 연산을 하지 않고 단순히 객체 하나만을 가리키는 식
- decltype에 식별자 표현이 아닌 식을 전달하면 해당 식의 값의 종류에 따라 달라진다.
  - xvlaue -> T&&
  - lvalue -> T&
  - prvalue -> T

## Value category
- 모든 C++ 식(expression)에는 두가지 정보가 항상 따라다닌다.
  - Type, Value Category
- Category는 좌측값, 우측값을 일컫는다.

![image](https://user-images.githubusercontent.com/69780812/147844568-fd5c7d97-4b30-4b90-bcb1-ab2d44268781.png)
- C++에서는 어떤 식의 값의 카테고리를 따질 때 크게 두가지 질문을 던질 수 있다.
  - 정체를 알 수 있나 ? : 해당 식이 어떤 식과 같은 것인지 아닌지 구분할 수 있다는 뜻이다. 일반적 변수라면 주소값을 취해서 구분할 수 있고, 함수라면 그냥 이름만 확인해보면 된다.
  - 이동 시킬 수 잇나 ? : 해당 식을 다른 곳으로 안전하게 이동할 수 있는지 여부를 묻는다. 즉 해당 식을 받는 이동생성자, 이동 대입연산자 등을 사용할 수 있어야만한다.
- 정체를 알 수 있는 모든 식 : gvalue (generalized lvalue)
- 이동 시킬 수 있는 모든 식 : rvalue
- 좌측값 : lvalue
- 순수 우측값 : prvalue (pure rvalue)
- 소멸하는 값 : xvalue (eXpiring value)

![image](https://user-images.githubusercontent.com/69780812/147844619-7526a274-fa47-4c24-be1a-dbfe4ded2237.png)

## lvalue
```cpp
int i;
i;
```
- i라는 식을 썼을 때, j라는 다른 식과 구분할 수 있다.
- i라는 식의 주소값은 실제 변수 i의 주소값이 될 것이다.
- i는 이동이 불가능하다. int&& x = i;는 컴파일이 되지 않기 때문이다.
- i는 ***lvalue***다.
- 이름을 가진 대부분의 객체들은 모두 lvalue다. 해당 객체의 주소값을 취할 수 있기 때문이다.
  - 변수, 함수의 이름, 어떤 Type의 데이터 멤버 등등
  - 좌측값 레퍼런스를 리턴하는 함수의 호출식
    - std::cout << 1 이나 ++it
  - a = b, a += b, a *= b 같이 복합 대입 연산자 식들
  - ++a, --a 같은 전위 증감 연산자 식들
  - a.m, p->m 과 같이 멤버를 참조할 때, m은 enum 값이거나 static이 아닌 멤버 함수인 경우를 제외한다.
  ```cpp
  class A
  {
      int f();
      static int g();
  }

  A a;
  a.g; // lvalue
  a.f; // lvalue가 아닌 prvalue
  ```
  - a\[n] 과 같은 배열 참조식들
  - 문자열 리터럴 "hi"
- 특히 이 lvalue들은 주소값 연산자 &을 통해 해당 식의 주소값을 알아낼 수 있다.
  - &++i, &std::endl 은 모두 올바른 작업이다.
- lvalue는 또한 좌측값 레퍼런스를 초기화하는데에 사용할 수 있다.

```cpp
void f(int&& a)
{
    a; // <-- ?
}

f(3);
```
- 위 코드에서 a는 우측값 레퍼런스이긴하지만 식 a의 경우는 lvalue이다. 왜냐하면 이름이 있기 때문이다.
  - 식 a의 type은 우측값 레퍼런스가 맞으나 식 a의 값 카테고리는 lvalue가 된다.

```cpp
#include <iostream>

void f(int&& a) { std::cout << &a; }
int main() { f(3); }
```
- a가 우측값 레퍼런스므로 a는 우측값이라고 생각했다면 타입과 값 카테고리가 다른 개념이란 사실을 헷갈린 경우이다.

## prvalue
```cpp
int f() { return 10; }

f(); // <-- ?
```
- f()는 어떤 카테고리에 들어가는가 ?
- f()의 주소값을 취할 수 있는가 ? 아니다.
- f()는 우측값 레퍼런스에 붙을 수 있다. 따라서 prvalue이다.
- 대표적으로 prvalue는 아래와 같다.
  - 1. 문자열 리터럴을 제외한 모둔 리터럴들 42, true, nullptr 등
  - 2. 레퍼런스가 아닌 것을 리턴하는 함수들의 호출식
    - str.substr(1, 2), str1+str2
  - 3. 후위 증갑 연산자 식 a++, a--
  - 4. 산술 연산자 논리연산자 식들
    - a + b, a && b, a < b (연산자 오버로딩이 아닌 디폴트로 제공되는 것들)
  - 5. 주소값 연산자 식 &a
  - 6. a.m, p->m 멤버 참조 시 m은 enum 값이거나 static이 아닌 멤버변수인 경우
  - 7. this
  - 8. enum 값
  - 9. lambda expression
    - []() { return 0;}
  - etc..
- prvalue는 **정체를 알 수 없는 녀석들**이어서 주소값을 취할 수 없다.
  - &a++, &42 모두 오류다.
- prvalue들은 식의 **좌측에 올 수 없다.**
- 우측값 레퍼런스와 **상수 좌측값 레퍼런스**초기화 하는데 사용할 수 있다.
  ```cpp
  const int& r = 42;
  int&& rr = 42; // int& rrr = 42; 불가능
  ```

## xvalue
- 값 카테고리가 lvalue, prvalue 두개만으로 구분되면 문제가 있다.
  - 좌측값으로 분류되는 식을 이동 시킬 방법이 없기 때문이다.
  - 정체가 있지만 이동도 시킬 수 있는 것을 생각해봐야한다.
- 크게 **우측값 레퍼런스를 리턴하는 함수의 호출식**을 들 수 있다.
  - std::move(x)
  ```cpp
  template <class T>
  constexpr typename std::remove_reference<T>::type&& move(T&& t) noexcept;
  ```
  - move의 리턴값은 우측값 레퍼런스임을 알 수 있다.
  - 따라서 move를 호출한 식은 lavalue처럼 좌측값 레퍼런스를 초기화하는 데 사용할 수 있고, prvalue처럼 우측값 레퍼런스에 붙이거나 이동 생성자에 전달해서 이동시킬 수 있다.
---
- decltype에 식별자 표현이 아닌 식이 전달되면 식의 타입이 T라 할 때 아래와 같은 방식으로 Type을 return 한다.
  - if expr's value category is ***xvalue*** : decltype T&&
  - if expr's value category is ***lvalue*** : T&
  - if expr's value category is ***prvalue*** : T

```cpp
int a, b;
decltype(a+b) c;
```
- c의 타입은 a + b가 prvalue이므로 실제 타입인 int로 추론된다.
- int c;를 한것과 똑같게 된다.

```cpp
int a;
deltype((a)) b;
```
- (a)는 식별자 표현식이 아니므로 어느 값 카테고리에 들어가는지 생각해봐야한다.
- &(a)와 같이 주소값 연산자를 적용할 수 있고 이동이 불가능하므로 lvalue가 된다.
- 따라서 b는 int가 될 것이라는 예상과 다르게 int&로 추론된다..
- 이는 C++에서 괄호의 유무로 인해 무언가 결과가 달라지는 첫 경우다.

# decltype의 쓰임새
- type 추론은 auto로 충분하지 않나 ?
- 하지만 auto는 엄밀히 정확한 타입을 표현하지 않는다.

```cpp
const int i = 4;
auto j = i; // int j = i
decltype(i) k = i; // const int k = i
```
- auto의 경우 const를 띄워버리지만 decltype의 경우 이를 그대로 보존한다.

```cpp
int arr[10];
auto arr2 = arr; // int* arr2 = arr;
decltype(arr) arr3; // int arr3[10];
```
- 그 외에도 배열의 경우 auto는 암시적으로 포인터로 변환하지만 decltype의 경우 배열 타입 그대로를 전달할 수 있다.
- 즉, delctype을 이용하면 **타입 그대로 정확하게 전달**할 수 있다.

```cpp
template <typename T, typename U>
void add(T t, U u, /* what is type ? */ result)
{
    *result = t + u;
}
```
- template 함수에서 어떤 객체의 타입이 템플릿 인자들에 의해 결정되는 경우가 있다.
- add 함수는 단순하지만 문제는 t + u로 result 타입이 결정된다는 사실이다.
  - 예로 t: double + u: int면 result type은 double*이 된다.

```cpp
template <typename T, typename U>
void add(T t, U u, decltype(t+u)* result)
{
    *result = t + u;
}
```
- 이런경우 decltype을 사용해주면된다.

```cpp
template <typename T, typename U>
decltype(t+u) add (T t, U u)
{
    return t + u;
}
```
- 이렇게하면 오류가 발생한다.
- t와 u의 정의가 decltype 나중에 나오기 때문이다.
  - 이 경우 **함수의 리턴값을 인자들 정의 부분 뒤에 써야한다**

```cpp
template <typename T, typename U>
auto add(T t, U u) -> decltype(t + u)
{
    return t + u
}
```
- C++14부터 추가된 문법으로 구현이 가능하다.
- 리턴에는 auto라 써놓고 -> 뒤에 함수의 실제 리턴 타입을 지정해주는 것이다. (람다와 매우 유사)