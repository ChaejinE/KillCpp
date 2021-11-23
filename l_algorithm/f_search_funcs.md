# Intro
- 원소들을 탐색하는 계열의 함수들이다.

# find
```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end)
{
    while (begin != end)
    {
        std::cout << "[" << *begin << "] ";
        begin++;
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<int> vec;

    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    vec.emplace_back(4);

    auto result = std::find(vec.begin(), vec.end(), 3);
    std::cout << "3 은 " <<  std::distance(vec.begin(), result) + 1 << " 번째 원소"
              << std::endl;
}
```

```
3은 2 번째 원소
```
- find 함수로 원소 위치를 찾는 코드이다.

```cpp
template <class InputIt, class T>
InputIt find(InputIt first, InputIt last, const T& value)
```
- find 함수는 단순히 위와 같이 생겼으며 first부터 last까지 순회하면서 value와 같은 원소가 있는지 확인하고 있으며 이를 가리키는 반복자를 return 한다.
- forward_iterator면 앞에서부터 찾고, reverse_iterator 면 뒤에서 부터 거꾸로 찾게된다.
- 컨테이너에 **중복되는 값이 있더라도 가장 먼저 찾은 것을 return**한다.

```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end)
{
    while (begin != end)
    {
        std::cout << "[" << *begin << "] ";
        begin++;
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<int> vec;

    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    vec.emplace_back(4);

    auto current = vec.begin();
    while (true)
    {
        current = std::find(current, vec.end(), 3);
        if (current == vec.end()) break;
        std::cout << "3 은" << std::distance(vec.begin(), current) + 1
                  << " 번째 원소" << std::endl;

        current++;
    }
}
```

```
3 은2 번째 원소
3 은5 번째 원소
```
- 3인 원소를 모두 찾는 코드다.
- 위처럼 마지막 찾은 위치 바로 다음부터 계속 순차적으로 탐색해 나가면 컨테이너에서 값이 3인 원소들을 모두 찾을 수 있게 된다.
- find 계열 함수들을 사욯알 때 한가지 주의할 점
  - 만약 컨테이너에서 기본적으로 find를 지원하면 이를 사용하는 것이 훨씬 빠르다.
  - find 함수는 그 컨테이너가 어떤 구조를 가지고 있는지에 대한 정보가 없기 때문이다.
  - 예를들어 set의 find 함수의 경우 O(logn)이다. 내부 원소들이 정렬되어 있기 때문이다.
  - unordered_set은 find 함수가 O(1)이다. 해시 테이블을 이용하기 때문이다.
  - find는 이러한 추가정보를 모르고 하나씩 확인해 나가므로 O(n)으로 처리된다.
  - **vector와 같이 기본적으로 find를 지원하지 않는 컨테이너에 사용하는 것이 권장**된다.

# find_if
```Cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end)
{
    while (begin != end)
    {
        std::cout << "[" << *begin << "] ";
        begin++;
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<int> vec;

    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    vec.emplace_back(4);

    auto current = vec.begin();
    while (true)
    {
        current = std::find_if(current, vec.end(), [](int i) {return i % 3 == 2; });
        if (current == vec.end()) break;
        std::cout << "3 으로 나눈 나머지가 2인 원소는 : " << *current << " 이다 "
                  << std::endl;

        current++;
    }
}
```

```
3 으로 나눈 나머지가 2인 원소는 : 5 이다 
3 으로 나눈 나머지가 2인 원소는 : 2 이다 
```
- find가 단순한 값을 받았다면, find_if는 함수 객체를 인자로 받아 그 결과가 참인 원소들을 찾게 된다.
  - lambda를 사용하니 매우 간단하다. 공부하자.

# any_if, all_of
```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end) {
  while (begin != end) {
    std::cout << "[" << *begin << "] ";
    begin++;
  }
  std::cout << std::endl;
}

struct User
{
    std::string name;
    int level;

    User(std::string name, int level) : name(name), level(level) {}
    bool operator==(const User& user) const
    {
        if (name == user.name && level == user.level) return true;
    }
};


class Party
{
    std::vector<User> users;

    public:
        bool add_user(std::string name, int level)
        {
            User new_user(name, level);
            if (std::find(users.begin(), users.end(), new_user) != users.end())
            {
                return false;
            }

            users.push_back(new_user);
            return true;
        }

        // 파티원 모두가 15레벨 이상이어야 입장가능
        bool can_join_dungeon()
        {
            return std::all_of(users.begin(), users.end(),
                                [](User& user) { return user.level >= 15; });
        }

        // 파티원 중 한명이라도 19렙 이상이면 특별 아이템 사용가능
        bool can_use_special_item()
        {
            return std::any_of(users.begin(), users.end(),
                                 [](User& user) { return user.level >= 19; });
        }
};

int main()
{
    Party party;
    party.add_user("철수", 15);
    party.add_user("영희", 18);
    party.add_user("민수", 12);
    party.add_user("채진", 19);

    std::cout << std::boolalpha;
    std::cout << "던전 입장 가능? " << party.can_join_dungeon() << std::endl;
    std::cout << "특별 아이템 사용 가능 ? " << party.can_use_special_item() << std::endl;
}
```

```
던전 입장 가능? false
특별 아이템 사용 가능 ? true
```
- any_of : 인자로 받은 범위안의 모든 원소들 중 조건을 하나라도 충족하는 것이 있다면 true를 return 한다. OR 연산과 비슷
- all_of : 모든 원소들이 전부 충족해야 true를 return 한다. AND 연산과 비슷
