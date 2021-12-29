# 한번 쯤 하는 실수
```cpp
#include <iostream>

class A
{
    public:
        A() { std::cout << "생성자" << std::endl; }
};

int main()
{
    A a();
}
```
- 실행 결과는 아무 것도 안나올 것이다.
- A의 객체 a를 만든 것이 아니라 A를 return하고 인자를 받지 않는 함수 a를 정의한 것이기 때문이다.
  - C++ 컴파일러는 함수의 정의처럼 보이는 것은 모두 함수의 정의로 해석한다.

```cpp
#include <iostream>

class A
{
    public:
        A() { std::cout << "생성자 호출" << std::endl; }
};

class B
{
    public:
        B(A a) { std::cout << "B의 생성자 호출" << std::endl; }
}

int main()
{
    B b(A());
}
```
- 실행 결과가 아무것도 출력되지 않을 것이다.
- b라는 class B 객체를 생성하는 것처럼 보인다. 사실은 인자로 A를 Return 하고 인자가 없는 함수를 받으며 Return Type이 B인 함수 b를 정의한 것이다.
- ()가 함수의 인자를 정의하는 데 사용되고 그냥 일반적인 객체의 생성자를 호출하는 데에도 사용되어 골치아픈 일이다.
- 이를 해결하게 위해 C++11에서는 Uniform Initialization이라는 것을 도입했다.

# Uniform Initialization
```cpp
#include <iostream>

class A
{
    public:
        A() { std::cout << "생성자 호출" << std::endl; }
};

int main()
{
    A a{};
}
```

```
생성자 호출
```
- () 대신 {}를 사용하면 끝이다.
- ()와 {}를 이용한 생성의 경우는 한 가지 큰 차이가 있는데 암시적 타입 변환들을 불허하고 있다는 점이다.

```cpp
class A
{
    public:
        A(int x) { ... }
}

int main()
{
    A a(3.5);
    A b{3.5};
}
```
- ()인 경우 정수 캐스팅 버전인 3이 전달된다.
- {} 경우에는 double을 int로 변환할 수 없다는 오류가 발생한다.
  - {} 중괄호를 이용한 생성자 호출의 경우 암시적 타입변환들이 불가능해진다.
  - Narrowing(데이터 손실이 있는) 변환인 경우들이다.
  - long double -> double or float
  - double -> float
  - int -> double, float types
- {}를 사용한다면 원하지 않는 타입 캐스팅을 방지해 미연에 오류를 잡아 낼 수 있다.

```cpp
#include <iostream>

class A
{
    public:
        A(int x, double y) { std::cout << "Constr: " << x << ',' << y << std::endl; }
};

A func()
{
    return {1, 2.3};
}

int main()
{
    func();
}
```

```
Constr: 1,2.3
```
- {}를 이용한 생성의 또다른 쓰임새로 함수 리턴시 굳이 생성하는 객체의 타입을 다시 명시하지 않아도 된다.
- {}를 사용하지 않았으면 A(1, 2.3)과 같이 클래스를 명시해줘야하는데 {} 이용할 경우 컴파일러가 알아서 함수의 리턴타입을 보고 추론해준다.

# Initializer list
```cpp
#include <iostream>

class A
{
    public:
        A(std::initializer_list<int> l)
        {
            for (auto itr = l.begin(); itr != l.end(); ++itr)
            {
                std::cout << *itr << std::endl;
            }
        }
};

int main()
{
    A a = {1, 2, 3, 4, 5};
}
```

```
1
2
3
4
5
```
- 배열, vector처럼 {}를 써서 같은 효과를 낼 수 있다.
- initializer_list는 {}를 이용해 생성자를 호출할 때 클래스 생성자들 중 initializer_list를 인자로 받는 생성자로 전달된다.
  - ()를 사용한 생성자 호출은 initializer_list가 생성되지 않는다.

```cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>

template <typename T>
void print_vec(const std::vector<T>& vec)
{
    std::cout << "[";
    for (const auto& e : vec)
    {
        std::cout << e << " ";
    }
    std::cout << "]" << std::endl;
}

template <typename K, typename V>
void print_map(const std::map<K, V>& m)
{
    for (const auto& kv : m)
        std::cout << kv.first << " : " << kv.second << std::endl;
}

int main()
{
    std::vector<int> v = {1,2,3,4,5};
    print_vec(v);

    std::cout << "----------------------" << std::endl;
    std::map<std::string, int> m = {
        {"abc", 1}, {"hi", 3}, {"hello", 5}, {"C++", 2}
    };
    print_map(m);
}
```

```
[1 2 3 4 5 ]
----------------------
C++ : 2
abc : 1
hello : 5
hi : 3
```

- initializer_list를 이용하면 컨테이너들을 간단히 정의할 수 있다.
- map의 경우는 vector와 다르게 pair<Key, Value> 원소들을 초기화 리스트의 원소들로 받는다.

## initializer_list 주의할 점
- initializer_list 받는 생성자가 있는 경우
  - {}를 이용해서 객체를 생성할 경우 생성자 오버로딩 시 해당 함수가 **최우선으로 고려**된다.

```cpp
vector(size_type count)
```
- vector의 경우 위와 같은 형태의 생성자가 존재한다.
- count 개수 만큼 원소 자리를 미리 생성해 놓는다.

```cpp
vector v{10};
```
- 위는 해당 생성자를 호출할까?
  - No. 그냥 원소 1개짜리 initializer_list라고 생각해서 10을 보관하고 있는 벡터를 생성하게 될 것이다.
- 이러한 불상사를 막기 위해서 {}fh todtjdgkrl qheksms ()를 이용해서 생성자를 호출한다. vector v(10)

```cpp
#include <iostream>
#include <initializer_list>
#include <string>

class A
{
    public:
        A(int x, double y) { std::cout << "일반 생성자" << std::endl; }

        A(std::initializer_list<std::string> lst)
        {
            std::cout << "초기화 생성자" << std::endl;
        }
};

int main()
{
    A a(1, 1.5);
    A b{1, 1.5};

    A c{"abc", "def"};
}
```

```
일반 생성자
일반 생성자
초기화 생성자
```
- initializer_list는 컴파일러가 최대한 매칭시키고 고려하는 대상이므로 initializer_list의 원소 타입으로 타입 변환 자체가 불가능한 경우인 것이 좋다.
- 위의 경우 int나 double은 string으로 변환 될 수 없으므로 initializer_list를 받는 생성자는 아예 고려대상에서 제외된다.

# initializer_list와 auto
- {}를 이용해서 생성 시 타입으로 auto를 지정한다면 initializer_list 객체가 생성된다.

```cpp
auto list = {1, 2, 3}
```
- list는 initializer<\int>가 될 것이다.

```cpp
auto b{1};
```
- 상식적으로 b는 1로 추론되어야할 것 같지만 어림도 없다. initializer_list<\int>다.
  - C++17에서 auto는 두 가지 형태로 구분해서 타입을 추론하게되었다.
    - auto x = {arg1, arg2...} : arg 끼리 모두 같은 타입이라면 std::initializer_list<\T>로 추론
    - auto x {arg1, arg2 ...} : 인자가 1개인 경우 인자의 type으로 추론되고 여러개일 경우는 오류 발생시킨다.

```cpp
auto list = {"a", "b", "cc"};
```
- 문자열의 경우는 initializer<\std::string>이 아닌 initializer_list<\const char*>가 된다.

```cpp
using namespace std::literals;

auto list = {"a"s, "b"s, "c"s};
```
- C++14에 추가된 리터럴 연산자를 통해 string으로 추론될 수 있도록 할 수 있다.
---
- {}를 통한 생성자 호출 : Uniform initialization을 통해 생성자가 함수의 정의로 오해되는 일을 막을 수 있다.
- initializer_list를 만들어 전달할 수도 있게 된다.