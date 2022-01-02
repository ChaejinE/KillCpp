# std::optional (C++17 이상)
```cpp
#include <iostream>
#include <map>
#include <string>

std::string GetValueFromMap(const std::map<int, std::string>& m, int key)
{
    auto itr = m.find(key);
    if (itr != m.end())
    {
        return itr->second;
    }

    return std::string();
}

int main()
{
    std::map<int, std::string> data = {{1, "hi"}, {2, "hello"}, {3, "hirroo"}};
    std::cout << "map에서 2에 대응되는 값" << GetValueFromMap(data, 2) << std::endl;
    std::cout << "map에서 4에 대응되는 값" << GetValueFromMap(data, 4) << std::endl;
}
```

```
map에서 2에 대응되는 값hello
map에서 4에 대응되는 값
```
- 잘 동작하는 것처럼 보이지만 실제 어떤 키에 대응하는 값이 빈 문자열이라면 문제가 될 것이다.

```cpp
#include <iostream>
#include <map>
#include <string>

std::pair<std::string, bool> GetValueFromMap(
    const std::map<int, std::string>& m, int key)
{
    auto itr = m.find(key);
    if (itr != m.end())
    {
        return std::make_pair(itr->second, true);
    }

    return std::make_pair(std::string(), false);
}

int main()
{
    std::map<int, std::string> data = {{1, "a"}, {2, "b"}, {3, "c"}};
    std::cout << "map [2] : " << GetValueFromMap(data, 2).first << std::endl;
    std::cout << "map [4] exist? " << std::boolalpha
              << GetValueFromMap(data, 4).second << std::endl;
}
```

```
map [2] : b
map [4] exist? false
```
- 실제 맵에 존재하는지 유무도 같이 전달한 방법이다.
- 하지만 한가지 문제가 있다.
  - 맵에 키가 존재하지 않을 때 디폴트 객체를 리턴해야 한다는 점이다.
  - 1. 객체의 디폴트 생성자가 정의되어있지 않을 수 있다.
  - 2. 객체를 디폴트 생성하는 것이 매우 오래 걸릴 수 있다.
  - 위의 2가지 문제로 원하는 값을 보관할 수도, 안할수도있는 클래스를 도입하는 것이다.

```cpp
#include <iostream>
#include <map>
#include <string>
#include <utility>

std::optional<std::string> GetValueFromMap(const std::map<int, std::string>& m,
                                           int key) {
  auto itr = m.find(key);
  if (itr != m.end()) {
    return itr->second;
  }

  // nullopt 는 <optional> 에 정의된 객체로 비어있는 optional 을 의미한다.
  return std::nullopt;
}

int main() {
  std::map<int, std::string> data = {{1, "hi"}, {2, "hello"}, {3, "hiroo"}};
  std::cout << "맵에서 2 에 대응되는 값은? " << GetValueFromMap(data, 2).value()
            << std::endl;
  std::cout << "맵에 4 는 존재하나요 " << std::boolalpha
            << GetValueFromMap(data, 4).has_value() << std::endl;
}
```

```
맵에서 2 에 대응되는 값은? hello
맵에 4 는 존재하나요 false
```
- template 인자로 optional이 보관하고자 하는 객체의 타입을 써준다.
  - 이러면 해당 optional 객체는 std::string을 보고나하던지 아니면 안하던지 둘중 하나의 상태만 가지게 된다.
- 키에 대응하는 값이 존재한담녀 해당 값을 리턴했다. optional에는 보관하고자 하는 타입을 받는 생성자가 정의되어 있어 위와 같이 리턴하더라도 optional 객체로 알아서 만들어져서 리턴된다.
- optional의 큰 장점
  - 객체를 보관하는 과정 중 **동적 할당이 발생하지 않는다.** (불필요한 오버헤드 X)
  - optional 자체에 객체가 포함되어 있다고 보면된다.
- nullopt는 아무런 객체도 가지고 있지 않은 빈 optional 객체다.
  - 미리 정의되어있는 객체다.
- optional 객체가 가지고있는 객체를 접근하고 싶으면 value()를, 가지고 있는 객체가 없다면 std:;base_optional_access예외를 던진다.
  - 따라서 optional이 들고 있는 객체에 접근하기 전에 실제로 값을 가지고 있는지 확인하는게 좋다.
  - has_value()를 사용하면된다.
  - 그런데 보통 optional 객체 자체에 bool로 변환하는 캐스팅 연산자가 있다.
  ```cpp
  if (GetvalueFromMap(data, 4))
  or
  if (GetvalueFromMap(data, 4).has_value())
  ```
  - 위처럼 쓰면된다.
  - 마찬가지로 value() 대신 역참조 연산자 *를 써도 된다.
- 이 전의 std::pair<\bool, T>와의 차이점은 **아무 것도 들고 있지 않는 상태에서 디폴트 객체를 가질 필요가 없다**는 점이다.

```cpp
#include <iostream>
#include <utility>
#include <optional>

class A
{
    public:
        A() { std::cout << "default constructor" << std::endl; }

        A(const A& a) { std::cout << "copy constructor" << std::endl; }
};

int main()
{
    A a;

    std::cout << "optional Making -----" << std::endl;
    std::optional<A> maybe_a;

    std::cout << "maybe_a는 A객체를 포함하고 있지 않아 디폴트 생성할 필요 없음" << std::endl;

    maybe_a = a;
}
```

```
default constructor
optional Making -----
maybe_a는 A객체를 포함하고 있지 않아 디폴트 생성할 필요 없음
copy constructor
```
- optional객체에 a 객체를 전달하기 직전까지 디폴트 생성되었다는 메세지가 뜨지 않는다. (처음 a 만들때 빼고)
- optional은 빈 객체 상태에서는 해당 객체를 가지고 있다는 사실을 알 수 있다.
- **std::optional을 이용해서 어떤 객체를 보관하거나 말거나**라는 의미를 쉽게 전달할 수 있다.

# Reference를 갖는 optional
- optional은 reference를 받을 수는 없다. 하지만 std::reference_wrapper를 사용해 wrapper객체를 정의하여 사용하면된다.

```cpp
#include <functional>
#include <iostream>
#include <optional>
#include <utility>
#include <optional>

class A
{
    public:
        int data;
};

int main()
{
    A a;
    a.data = 5;

    std::optional<std::reference_wrapper<A>> maybe_a = std::ref(a);

    maybe_a->get().data = 3;

    std::cout << "a.data : " << a.data << std::endl;
}
```

```
a.data : 3
```
- reference_wrapper는 레퍼런스가 아닌 일반적인 객체이므로 optional에 전닳ㄹ 수 있다.
- get() 함수를 통해 레퍼런스 하고 있는 객체를 얻어올 수 있다.
- reference_wrapper 객체를 생성하기 위해서  std::ref 함수를 사용한다.
