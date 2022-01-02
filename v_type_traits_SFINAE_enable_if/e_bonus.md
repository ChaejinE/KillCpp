# 특정 멤버 함수가 존재하는 타입만 받는 함수
- enable_if와 여러 메타 함수로 할 수 있던 것은 이러 저러한 조건을 만족하는 타입을 인자로 받는 함수를 만드는 것이었다.
- 하지만 만약 **이러 이러한 멤버 함수가 있는 타입을 인자로 받는 함수를 만들고 싶다**는 어떨까?

```cpp
#include <iostream>
#include <type_traits>

template <typename T, typename = decltype(std::declval<T>().func())>
void test(const T& t)
{
    std::cout << "t.func() : " << t.func() << std::endl;
}

struct A
{
    int func() const { return 1; }
};

int main()
{
    test(A{});
}
```

```
t.func() : 1
```
- Wow..
- 만약 struct B{};와 같이 정의되어있지 않은 클래스의 객체를 전달한다면 컴파일 오류가 발생한다.
  - test(B{})를 오버로딩 함수가 없다고 나온다.
  - 왜냐하면 decltype(std::declval<\T>().func())가 올바르지 않은 문장이어서 오버로딩 후보군에서 제외되었기 때문이다.

```cpp
#include <iostream>
#include <set>
#include <type_traits>
#include <vector>

template <typename Cont, typename = decltype(std::declval<Cont>().begin()),
          typename = decltype(std::declval<Cont>().end())>
void print(const Cont& container)
{
    std::cout << "[ ";
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "]\n";
}

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    print(v);

    std::set<char> s = {'a', 'b', 'f', 'i'};
    print(s);
}
```

```
[ 1 2 3 4 5 ]
[ a b f i ]
```
- 여러개의 함수를 확인하는 방법도 있다.
- 예를들어 컨테이너의 모든 원소들을 출력하는 print 함수를 작성한다고 해보면 주어진 타입 T가 컨테이너인지 쉽게 알 수 있는 방법은 없지만 begin, end가 정의되어 있다는 사실을 알기 때문에 T에 begin, end가 정의되어있는지 확인하면된다.
- struct Bad {void begin(); }; 와 같이 begin, end 정의가 되어있지 않는 클래스의 경우에는 오버로딩이 실패해서 컴파일 에러가 난다.
- 한가지 별로인점은 디폴트 템플릿인자 typename = 이 너무 많아진다는 점이다.
  - 가독성 매우 떨어짐

# void_t
```cpp
template <class ...>
using void_t = void;
```
- void_t는 정의가 단순하다.
- 가변 길이 템플릿을 이용해 void_t에 템플릿 인자로 임의의 개수의 타입들을 전달할 수 있다. 또한 void_t는 void와 동일하다.

```cpp
void_t<A, B, C, D>
```
- void_t에 전달된 템플렛 인자중 문법적으로 올바르지 못한 템플릿 인자가 있다면 void_t를 사용한 템플릿 함수의 경우 void 대신 SFINAE에 의해 오버로딩 목록에서 제외된다.

```cpp
template <typename Cont, typename = decltype(std::declval<Cont>().begin()),
          typename = decltype(std::declval<Cont>().end())>
```

```cpp
template <typename Cont,
          typename = std::void_t<decltype(std::declval<Cont>().begin(),
                                 decltype(std::declval<Cont>().end())>>
```
- 이전보다 훨씬 깔끔하게 다시 쓸 수 있다.
- void_t에 전달된 begin, end 중 하나라도 문법적으로 올바르지 않으면 SFINAE에 의해 해당 print 함수는 오버로딩 후보군에서 제외된다.

```cpp
template <typename Cont>
std::void_t<decltype(std::declval<Cont>().begin()),
            decltype(std::declval<Cont>().end())>
print(const Cont& container)
```
- 사실 사용자가 실수로 타입을 두개 전달해버릴 수있다.
  ```cpp
  print<Bad, void>(Bad{});
  ```
  - 이런 트롤짓을 막기 위해 사실상 위의 코드가 완성된 코드라고 볼 수 있다.
  - void가 전달되면 이전에는 그대로 적용되어버려 오버로딩 후보군에서 제외되지않게된다.
  - 디폴트 인자가 들어가있지 않게되어 사용자의 실수로부터 안전해진다.