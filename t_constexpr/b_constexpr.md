# Overview
- constexpr는 어떠한 식이 **상수식**이라고 명시해주는 키워드다.
- 객체 정의에 constexpr이 오게되면 해당 객체는 어떤 상수식에도 사용될 수 있다.

```cpp
#include <iostream>

template <int N>
struct A
{
    int operator()() { return N; }
};

int main()
{
    constexpr int size = 3;
    int arr[size];

    constexpr int N = 10;
    A<N> a;
    std::cout << a() << std::endl;

    constexpr int number = 3;
    enum B{ x = number, y, z };
    std::cout << B::x << std::endl;
}
```

```
10
3
```
- constexpr은 const와 큰 차이가 없어보인다.
  - 위 키워드로 정의된 변수들도 마찬가지로 상수이므로 수정할 수 없기 때문이다.

## constexpr vs const
```cpp
int a;

...

const int b = a;
```
- const로 정의된 상수들은 컴파일 타임에 그 값을 알 필요가 없다.
- 위 코드를 볼 때, b의 값을 컴파일 타임에 알 수는 없으나 b의 값을 지정해주면 바꿀수 없다는 것은 확실하다.

```cpp
int a;

...

constexpr int b = a;
```
- 반면 constexpr의 경우 반드시 오른쪽에 다른 상수식이 와야한다.
- 컴파일러 입장에서 컴파일 타임에 a가 뭐가 올지 알 수 없다. 위 코드는 컴파일 오류가 되는 것이다.
- **constexpr은 항상 const이지만 const는 constexpr이 아닌 것이다.**

```cpp
const int i = 3;
```
- const 객체가 만일 상수식으로 초기화 되었더라도 컴파일러에 따라 이를 런타임에 초기화할지, 컴파일에 초기화할지 다를 수 있다.
- 예를들어 위의 경우 i는 컴파일 타임에 초기화 될 수도, 런타임에 초기화 될 수도 있다.
- 따라서 **컴파일 타임에 상수를 확실히 사용하고 싶다면 constexpr 키워드를 꼭 사용해야한다.**

# constexpr Function
- consexpr로 객체를 선언하면 해당 객체는 컴파일 타임 상수로 정의된다. 함수는 ?

```cpp
#include <iostream>

int factorial(int N)
{
    int total = 1;
    for (int i = 1; i <= N; i++)
        total *= 1;
    
    return total;
}

template <int N>
struct A
{
    int operator()() { return N; }
};

int main()
{
    A<factorial(5)> a;
}
```
- 위를 컴파일 하면 오류가 발생한다.
- factorial(5)는 컴파일 타임 상수가 아니기 때문이다.
  - A<120> a; 로 컴파일안된다. 우리 머리로만 될 뿐
- 함수의 리턴 타입에 Constexpr을 추가한다면 조건이 맞을 때, 해당 함수의 리턴 값을 컴파일 타임 상수로 만들어 버릴 수 있다.

```cpp
#include <iostream>

constexpr int Factorial(int n) {
  int total = 1;
  for (int i = 1; i <= n; i++) {
    total *= i;
  }
  return total;
}

template <int N>
struct A {
  int operator()() { return N; }
};

int main() {
  A<Factorial(10)> a;

  std::cout << a() << std::endl;
}
```

```
3628800
```
- Factorial(10)이 컴파일 타임에 계산되어 클래스 A의 템플릿 인자로 들어가게 된다.
- C++14 부터 몇가지 제약 조건들 빼고는 모두 constexpr 함수 내부에서 수행할 수 있게되었다.
  - 1. goto 사용
  - 2. 예외 처리 (C++20부터 가능)
  - 3. 리터럴 타입이 아닌 변수 정의
  - 4. 초기화 되지 않는 변수 정의
  - 5. 실행 중간에 constexpr이 아닌 함수를 호출
- 위의 작업을 하지 않는 이상 constexpr 키워드를 함수에 붙일 수 있게 된다.
  - 아니면 컴파일 오류

```cpp
#include <iostream>

constexpr int Factorial(int n) {
  int total = 1;
  for (int i = 1; i <= n; i++) {
    total *= i;
  }
  return total;
}

int main() {
  int num;
  std::cin >> num;
  std::cout << Factorial(num) << std::endl;
}
```

```
5
120
```
- constexpr 함수에 인자로 컴파일 타임 상수가 아닌 값을 전달했다면 일반 함수 처럼 동작하게 된다.
- 따라서 constexpr을 함수에 붙일 수 있다면 붙여주는 것이 좋다.
  - constexpr 처럼 동작하지 못하면 일반 함수처럼 동작 할 것이고 컴파일 타임 상수를 생성할 수 있는 상황이면 간단히 이용할 수 있기 때문이다.

## 리터럴 타입?
- constexpr 함수 내부에서 불가능한 작업으로 리터럴 타입이 아닌 변수의 정의가 있다.
- void 형
- char, int, bool, long, float, double 등
- reference type
- literal type array
- default 소멸자를 가지는 타입
- lambda function
- Aggregate type(사용자 정의 생성자 소멸자가 없고 모든 데이터 멤버들이 public) ex) pair
- constexpr 생성자를 가지며 복사 및 이동생성자가 없음.
- 위의 내용들이 리터럴 타입이라 의미한다.
- 해당 객체들만 constexpr로 선언되던지 constexpr 함수 내부에서 사용될 수 있다.

# constexpr 생성자
- constexpr 생성자 경우에도 constexpr 함수에서 적용되는 제약조건들이 모두 적용된다.
  - 생성자 인자들은 반드시 리터럴 type이어야만 하고, 해당 클래스는 가상 상속 받을 수 없다.

```cpp
class Vector 
{
    public:
        constexpr Vector(int x, int y) : x_(x), y_(y) {}
    
    constexpr int x() const { return x_; }
    constexpr int y() const { return y_; }

    private:
        int x_;
        int y_;
};
```
- Vector 생성자는 리터럴인 int 두 개를 인자로 받고 있다. 이는 적합한 constexpr 생성자가 될 것이다.
- 마찬가지로 두 멤버 변수 접근 함수도 constexpr로 정의해준다.
  - constexpr 함수 내부에서 x(), y()를 사용할 수 있게 된다.

```cpp
#include <iostream>

class Vector 
{
    public:
        constexpr Vector(int x, int y) : x_(x), y_(y) {}
    
    constexpr int x() const { return x_; }
    constexpr int y() const { return y_; }

    private:
        int x_;
        int y_;
};

constexpr Vector AddVec(const Vector& v1, const Vector& v2)
{
    return {v1.x() + v2.x(), v1.y() + v2.y()};
}

template <int N>
struct A
{
    int operator()() { return N; }
};

int main()
{
    constexpr Vector v1{1, 2};
    constexpr Vector v2{2, 3};

    A<v1.x()> a;
    std::cout << a() << std::endl;

    A<AddVec(v1, v2).x()> b;
    std::cout << b() << std::endl;
}
```

```
1
3
```
- consexpr로 생성자를 만들었기 때문에 Vector를 constexpr로 선언할 수 있다.
- v1의 constexpr 멤버 함수인 x()를 호출했는데 역시 x()도 constexpr 함수이므로 A<1> a와 다름이 없게된다.
  - 만일 v1이나 x()가 하나라도 constexpr이 아니라면 위 코드는 컴파일 되지 않는다.
- **constexpr 객체의 constexpr 멤버 함수만이 constexpr을 준다.**
- AddVec()함수 역시 v1, v2를 인자로 받아 constexpr 객체를 리턴하는데, 이것이 가능한 것은 constexpr 함수이면서 Vector가 리터럴 type이어서 그렇다.

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void show_value(T t)
{
    if constexpr (std::is_pointer<T>)
    {
        std::cout << "Pointer: " << *t << std::endl; 
    } else {
        std::cout << "No Pointer: " << t << std::endl;
    }
}

int main()
{
    int x = 3;
    show_value(x);

    int* p = &x;
    show_value(p);
}
```

```
No Pointer: 3
Pointer: 3
```
- type_traits에서는 여러 템플릿 함수를 제공하는데 해당 타입이 포인터인지 아닌지 확인하는 함수도 있다.
  - is_pointer : value가 포인터면 true, 아니면 false가 return되는 템플릿 메타함수
- if constexpr를 쓰지 않으면 컴파일 오류가 난다.
  - t에 * 연산자가 붙어있어 컴파일이 안된다. (int 타입이라고 되어있는데 int* 타입이아니어서 그런듯 하다.)
  - 템플릿이 인스턴스화 되면서 생성되는 코드에 컴파일이 불가능한 부분이 발생하는 것이다.
- if constexpr은 조건이 반드시 bool로 타입 변환이될 수 있어야 하는 컴파일 타임 상수식이어야만한다.
  - if constexpr이 참이면 else에 해당하는 문장은 컴파일 되지 않는다. (완전히 무시)
  - 거짓이면 else에 해당하는 부분만 컴파일 된다.
  - 따라서 위의 경우 포인터가아닌 것은 *t 자체가 컴파일 되지 않는다.