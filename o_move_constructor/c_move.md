# Overview
```cpp
template <typename T>
void my_swap(T &a, T &b)
{
    T tmp(a);
    a = b;
    b = tmp;
}
```
- 위와 같이 객체를 받아서 Swap해주는 함수가 있다고 해보자.
- a는 좌측값이므로 tmp의 복사생성자가 호출된다.
- 따라서 1차적으로 a가 차지하는 공간 만큼 메모리 할당이 발생한 후 a의 데이터가 복사된다.
- 두 번째로 a = b;에서 2차적으로 복사가 발생한다.
- 또한, 마지막 b = tmp; 에서 또 한번 문자열 전체의 복사가 이루어 지게 된다.
- swap을 위해 무려 3번의 복사를 진행한 것이다.

![image](https://user-images.githubusercontent.com/69780812/144048865-a710afc0-942d-4a52-b552-1e036fffa1cd.png)
- 위와 같은 그림의 현상이 발생한 것이다.
- 굳이 문자열 내용을 복사할 필요 없이 각 T객체의 member variable 주소 값만 서로 바꿔주면 되는 것을 알고 있다.
  - 큰 용량의 데이터인 경우 복사가 영향을 준다.

![image](https://user-images.githubusercontent.com/69780812/144049093-be0db82f-c9e5-4212-a34a-35cda877f3ca.png)
- 위와 같은 Flow를 my_swap 함수에서 구현하기에는 여러 문제가 있다.
- 첫번째, 임의의 타입을 받는 함수 Generic 이다.
- 위 함수가 일반적 타입 T에 대해 작동해야 한다는 의미다.
  - member variable의 경우 의도한 객체에만 존재하는 필드이므로 일반적인 타입 T에 대해서는 작동하지 않는다.
  - 특수화를 통해서 해결할 수 있지만 member variable이 private 이면 의도한 클래스 내부에 swap 관련한 함수를 만들어야한다는 점이 문제다. 이러면 my_swap은 사실 필요가 없다.
- 다시 생각해보면 T tmp(a);가 복사생성자가 아닌 이동 생성자가 되기를 원한다.
  - 잠시 a의 값을 옮겨놓는 용도이기만 하면 되기 때문이다.
  - 문제는 a가 좌측값이라는 정미다.
  - 이 상태로는 뭘해도 이동 생성자는 오버로딩 되지 않는다.

# move function (move semantics)
- C++ 11 부터 <utility> 라이브러리에서 좌측값을 우측값로 바꿔주는 move 함수를 제공하고 있다.

```cpp
#include <iostream>
#include <utility>

class A
{
    public:
        A() { std::cout << "일반 생성자 호출 " << std::endl; }
        A(const A& a) { std::cout << "복사 생성자 호출" << std::endl; }
        A(A&& a) noexcept { std::cout << "이동 생성자 호출 " << std::endl; }
};

int main()
{
    A a;

    std::cout << "--------" << std::endl;
    A b(a);

    std::cout << "--------" << std::endl;
    A c(std::move(a));
}
```

```
일반 생성자 호출 
--------
복사 생성자 호출
--------
이동 생성자 호출 
```
- b(a) 했을 때, a가 이름이 있는 좌측값이므로 좌측값 레퍼런스가 참조한다.
- std::move 함수가 인자로 받은 객체를 우측값으로 변환해서 return 해준다.
  - 실제로는 단순한 타입 변환만 수행할 뿐이다.

```cpp
template <typename T>
void my_swap(T &a, T &b)
{
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}
```
- tmp라는 임시 객체를 a 로부터 이동생성하기 위해서 T 객체는 이동생성자를 선언해둔다.
- 그렇다면 위 함수를 이용하면 tmp라는 임시 객체를 a 로부터 이동생성한다.
- 이동생성이므로 기존 복사 생성하는 것 보다 훨씬 빠르게 수행된다.
- a, b에 대한 대입 연산 시에도 일반적 대입이 아니라 **이동*이 수행된다.
  - 이를 위해서는 객체에 우측값 레퍼런스에 대한 연산자 오버로딩을 해둔다.
- 실제로 데이터가 이동되는 과정은 이동 생성자나 이동 대입 연산자를 호출할 때 수행된다.
  - std::move 시점에 수행되는 것은 아니다.

# Perfect forwarding
```cpp
#include <iostream>
#include <vector>

template <typename T>
void wrapper(T u)
{
    g(u);
}

class A {};

void g(A& a) { std::cout << "좌측값 레퍼런스 호출" << std::endl; }
void g(const A& a) { std::cout << "좌측값 상수 레퍼런스 호출" << std::endl; }
void g(A&& a) { std::cout << "우측값 레퍼런스 호출 " << std::endl; }

int main()
{
    A a;
    const A ca;

    std::cout << "원본 ---------" << std::endl;
    g(a);
    g(ca);
    g(A());

    std::cout << "Wrapper --------" << std::endl;
    wrapper(a);
    wrapper(ca);
    wrapper(A());
}
```

```
원본 ---------
좌측값 레퍼런스 호출
좌측값 상수 레퍼런스 호출
우측값 레퍼런스 호출 
Wrapper --------
좌측값 레퍼런스 호출
좌측값 레퍼런스 호출
좌측값 레퍼런스 호
```
- 위의 경우 예상한대로 나온다.
- 반면, wrapper 함수를 거칠 경우 공교롭게 모두 좌측값 레퍼런스를 받는 함수가 호출되었다.
  - C++ 컴파일러가 템플릿 타입을 추론할 때, 템플릿 인자 T가 레퍼런스가 아닌 일반적 타입이면 const를 무시하기 때문이다.
  - T가 전부 다 class A 로 추론되는 것이다.
  - 따라서 wrapper는 모두 class A로 추론되는 것이다.

# Universal reference
```cpp
#include <iostream>

template <typename T>
void wrapper(T&& u)
{
    g(std::forward<T>(u));
}

class A {};

void g(A& a) { std::cout << "좌측값 레퍼런스 호출" << std::endl; }
void g(const A& a) { std::cout << "좌측값 상수 레퍼런스 호출" << std::endl; }
void g(A&& a) { std::cout << "우측값 레퍼런스 호출" << std::endl; }

int main() {
  A a;
  const A ca;

  std::cout << "원본 --------" << std::endl;
  g(a);
  g(ca);
  g(A());

  std::cout << "Wrapper -----" << std::endl;
  wrapper(a);
  wrapper(ca);
  wrapper(A());
}
```

```
원본 --------
좌측값 레퍼런스 호출
좌측값 상수 레퍼런스 호출
우측값 레퍼런스 호출
Wrapper -----
좌측값 레퍼런스 호출
좌측값 상수 레퍼런스 호출
우측값 레퍼런스 호출
```
- wrapper 함수가 인자로 아예 T&& 를받아버리고 있다.
- Template 인자 T에 대해 우측값 레퍼런스를 받는 형태를 **보편적 레퍼런스, Universal Reference**라고한다.
  - 우측값만 받는 레퍼런스와는 다르다.
  - 좌측값 역시 받아낸다.
- 좌측값이 왔을 때, 보편적 레퍼런스는 reference collapsing rule(레퍼런스 겹침 규칙)에 따라 T의 타입을 추론하게 된다.
  - 쉽게 생각하면 &는 1이고 && 0이라 둔뒤 OR 연산을 한다고 보면된다.

- wrapper(a);, wrapper(ca); 두개 호출의 경우 T가 각각 A&, const A&로 추론될 것이다.
- wrapper(A());는 T가 단순히 A로 추론될 것이다.
- 문제는 g에 이 인자를 전달하는 방법이다.
  - 왜 g(u)로 하지 않았을까?
- u는 l-value다. 따라서 우리는 int&&를 오버로딩 하는 g를 호출하고 싶었겠지만 실제로는 const int&를 오버로딩하는 g가 호출된다.
  - 따라서 이 경우 move를 통해 u를 다시 우측값으로 변환해야한다.
- 당연히 아무때나 move를 하면 안된다. 인자로 받은 u가 우측값 레퍼런스일 때만 move를 해줘야한다. 만일 좌측값 레퍼런스일 때 move를 해버리면 좌측값에 오버로딩 되는 g가 아닌 우측값에 오버로딩되는 g가 호출된다.

```cpp
g(forward<T>(u));
```
- 위 문제를 해결해주는 것이 forward 함수다.
- u가 우측값 레퍼런스일 때에만 마치 move를 적용한 것 처럼 작동한다.

```cpp
template <class S>
S&& forward(typename std::remove_reference<S>::type& a) noexcept
{
    return static_cast<S&&>(a);
}
```
- 실제 forward 함수는 위와 같이 생겼다.
  - std::remove_reference는 타입의 레퍼런스를 지워주는 템플릿 메타 함수다.
- S가 A& 라면

```cpp
A&&& forward(typename std::remove_reference<A&>::type& a) noexcept
{
    return static_cast<A&&&>(a);
}
```
- 위 처럼 되어 레퍼런스 겹침 규칙에 따라

```cpp
A& forward(A& a) noexcept { return static_cast<A&>(a); }
```
- 위 처럼되버리고 S가 A라면

```cpp
A&& forward(A& a) noexcept { return static_cast<A&&>(a); }
```
- 위 처럼 되어 성공적으로 우측값으로 캐스팅 해준다.
- 결과적으로 위 코드 처럼 원본과 Wrapper를 사용했을 때 모두 호출되는 함수가 동일해진다.