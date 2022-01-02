# Overview
- 서로 다른 타입들의 묶음을 간단하게 다룰 수 있도록 제공되는 라이브러리다.

```cpp
struct Collection
{
    int a;
    std::string s;
    double d;
};
```
- 매번 이렇게 의미없는 구조체를 생성하게 되면 코드 읽는 사람 입장에서 상당히 골치아프다.

# tuple
```cpp
#include <iostream>
#include <string>
#include <tuple>

int main()
{
    std::tuple<int, double, std::string> tp;
    tp = std::make_tuple(1, 3.14, "hi");

    std::cout << std::get<0>(tp) << ", " << std::get<1>(tp) << ", "
              << std::get<2>(tp) << std::endl;
}
```

```
1, 3.14, hi
```
- tuple이 보관하고자 하는 type들을 나열해주면 된다.
- variant와 다르게 같은 타입이 들어있어도 전혀 문제가 될 것이 없다.
- tuple 생성을 위해서는 make_tuple()을 사용하면된다.
- 각 원소에 접근하려면 std::get을 이용하면 된다.
  - 원소 번호로 접근

## Structed binding
```cpp
#include <iostream>
#include <string>
#include <tuple>

std::tuple<int, std::string, bool> GetStudent(int id)
{
    if (id == 0)
        return std::make_tuple(30, "cj", true);
    else
        return std::make_tuple(29, "ej", false);
}

int main()
{
    auto student = GetStudent(1);

    int age = std::get<0>(student);
    std::string name = std::get<1>(student);
    bool is_male = std::get<2>(student);

    std::cout << "name : " << name << std::endl;
    std::cout << "age : " << age << std::endl;
    std::cout << "man ? : " << std::boolalpha << is_male << std::endl;
}
```

```
name : ej
age : 29
man ? : false
```
- 이렇게 값을 꺼내면 조금 귀찮다.

```cpp
#include <iostream>
#include <string>
#include <tuple>

std::tuple<int, std::string, bool> GetStudent(int id)
{
    if (id == 0)
        return std::make_tuple(30, "cj", true);
    else
        return std::make_tuple(29, "ej", false);
}

int main()
{
    auto student = GetStudent(1);

    auto [age, name, is_male] = student;

    std::cout << "name : " << name << std::endl;
    std::cout << "age : " << age << std::endl;
    std::cout << "man ? : " << std::boolalpha << is_male << std::endl;
}
```
- structed binding은 마치 파이썬을 생각나게 하는 문법이다.
- 한가지 중요한 점은 tuple의 모든 원소를 받아야한다는 점이다.
- Data 멤버들이 정의되어있는 구조체의 데이터필드를 받을 때에도 사용할 수 있다.
  ```cpp
  struct Data
  {
      int i;
      std::string s;
      bool b;
  };

  Data d;
  auto [i, s, b] = d;
  ```
  - 각각의 데이터 필드를 받을 수 있다.

```cpp
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<int, std::string> m = { {3, "hi"}, {5, "hello"} };
    for (auto& [key, val] : m)
        std::cout << "Key : " << key << " Value : " << val << std::endl;
}
```

```
Key : 3 Value : hi
Key : 5 Value : hello
```
- 기존 iterator로 받아서 first, second로 키와 대응되는 값ㅇ르 나타내야 하는데, structed binding을 사용해서 훨씬 깔끔하게 나타낼 수 있다.