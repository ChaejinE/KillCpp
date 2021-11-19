# map
- map은 set과 거의 똑같은 자료구조다.
- set의 경우 key만 보관했지만, map의 경우 키에 대응되는 value까지도 같이 보관하는 것이다.

```cpp
#include <iostream>
#include <map>
#include <string>

template <typename K, typename V>
void print_map(std::map<K, V>& m)
{
    for (auto itr = m.begin(); itr != m.end(); ++itr)
    {
        std::cout << itr->first << " " << itr->second << std::endl;
    }
}

int main()
{
    std::map<std::string, double> pitcher_list;

    // map의 insert 함수는 pari 객체를 인자로 받는다.
    pitcher_list.insert(std::pair<std::string, double>("정채진", 2.23));
    pitcher_list.insert(std::pair<std::string, double>("김채진", 2.93));
    pitcher_list.insert(std::pair<std::string, double>("박채진", 2.95));

    pitcher_list.insert(std::pair<std::string, double>("김로또", 3.04));
    pitcher_list.insert(std::pair<std::string, double>("유로또", 3.05));
    pitcher_list.insert(std::pair<std::string, double>("진로또", 3.09));

    pitcher_list["유로뱅"] = 3.56;
    pitcher_list["로또"] = 3.76;
    pitcher_list["헨리"] = 3.90;

    print_map(pitcher_list);

    std::cout << "유로뱅 방어율 :: " << pitcher_list["유로뱅"] << std::endl;
}
```

```
김로또 3.04
김채진 2.93
로또 3.76
박채진 2.95
유로또 3.05
유로뱅 3.56
정채진 2.23
진로또 3.09
헨리 3.9
유로뱅 방어율 :: 3.56
```
- map의 경우 템플릿 인자로 2개를 가지는데, 첫번쨰는 Key, 두번쨰는 Value 타입이다.
- map에 원소를 넣기 위해서는 std::pair 객체를 전달해야 된다.
---
```cpp
template <class T1, class T2>
struct std::pair
{
    T1 first;
    T2 second;
}
```
- 단순히 2개의 객체를 가지는 객체다.
- std::pair 객체 사용 시 템플릿 **인자를 초기화** 해야하는 데 꽤나 귀찮다.

```cpp
pitcher_list.insert(std::make_pair("차우찬", 3.04));
pitcher_list.insert(std::make_pair("장원준 ", 3.05));
pitcher_list.insert(std::make_pair("헥터 ", 3.09));
```
- STL은 std::make_pair 함수를 제공한다.
- 인자로 들어오는 객체를 보고 타입을 추측해 std::pair객체를 만들어 리턴해준다.
  - 귀찮게 타입을 명시해줄 필요가 없다.
---
- map의 경우 operator[]를 통해 새로운 원소를 추가할 수 있다.
  - 해당 키가 맵에 없다면 새로운 원소로 추가되고
  - 해당 키가 맵에 있다면 **값이 대체된다.**
- map의 경우도 셋과 마찬가지로 반복자를 이용해 순차적으로 맵에 저장되어있는 원소들을 탐색할 수 있다.
  - set의 경우 **\*itr가 저장된 원소를 바로 가리켰는데,** 맵의 경우 반복자가 **맵에 저장되어있는 std::pair 객체를 가리키게**된다.
    - itr->first, itr->second
- map에 저장된 value를 찾고 싶다면 간단히 \[] 연산자를 이용하면 된다.

```cpp
std::cout << pitcher_list["없는애"] std::endl;
```
- 맵에서 없는 값을 참조한다면 오류가 발생해야되지만 오히려 값을 돌려준다.
- 맵에 없는 키를 참조하게 되면 자동으로 값의 **Default 생성자**를 호출해서 원소를 추가하기 때문이다.
- 따라서 되도록 **find함수로 원소가 키가 존재하는지 먼저 화긴하고, 값을 참조하는 것이 좋다.**

```cpp
#include <iostream>
#include <map>
#include <string>

template <typename K, typename V>
void print_map(const std::map<K, V>& m)
{
    for (const auto& kv : m)
    {
        std::cout << kv.first << " " << kv.second << std::endl;
    }
}

template <typename K, typename V>
void search_and_print(std::map<K, V>& m, K key)
{
    auto itr = m.find(key);
    if (itr != m.end())
    {
        std::cout << key << " --> " << itr->second << std::endl;
    } else {
        std::cout << key << "은 목록에 없다." << std::endl;
    }
}

int main()
{
    std::map<std::string, double> pitcher_list;

    pitcher_list["정채진"] = 3.58;

    print_map(pitcher_list);
    

    search_and_print(pitcher_list, std::string("정채진"));
    search_and_print(pitcher_list, std::string("류현진"));
}
```

```
정채진 3.58
정채진 --> 3.58
류현진은 목록에 없다.
```
- 위처럼 find 함수는 맵에서 해당하는 key를 찾아 이를 가리키는 반복자를 리턴한다.
  - 키가 존재하지 않으면 end()를 리턴한다.

```cpp
#include <iostream>
#include <map>
#include <string>

template <typename K, typename V>
void print_map(const std::map<K, V>& m) {
  // kv 에는 맵의 key 와 value 가 std::pair 로 들어갑니다.
  for (const auto& kv : m) {
    std::cout << kv.first << " " << kv.second << std::endl;
  }
}

int main() {
  std::map<std::string, double> pitcher_list;

  // 맵의 insert 함수는 std::pair 객체를 인자로 받습니다.
  pitcher_list.insert(std::pair<std::string, double>("박세웅", 2.23));
  pitcher_list.insert(std::pair<std::string, double>("박세웅", 2.93));

  print_map(pitcher_list);

  // 2.23 이 나올까 2.93 이 나올까?
  std::cout << "박세웅 방어율은? :: " << pitcher_list["박세웅"] << std::endl;
}
```

```
박세웅 2.23
박세웅 방어율은? :: 2.23
```
- 맵은 셋 처럼 **중복된 원소를 허락하지 않는다.**
  - 나중에 오는 insert는 무시된다.

# multimap
- 한개의 키에 여러개의 값이 대응될 수 있다.

```cpp
#include <iostream>
#include <map>
#include <string>

template <typename K, typename V>
void print_map(const std::multimap<K, V>& m)
{
    for (const auto& kv : m)
    {
        std::cout << kv.first << " " << kv.second << std::endl;
    }
}

int main()
{
    std::multimap<int, std::string> m;

    m.insert(std::make_pair(1, "hello"));
    m.insert(std::make_pair(1, "hi"));
    m.insert(std::make_pair(1, "Chaejin"));
    m.insert(std::make_pair(1, "Jeong"));

    print_map(m);

    std::cout << "-------------------" << std::endl;
    std::cout << m.find(1)->second << std::endl;
}
```

```
1 hello
1 hi
1 Chaejin
1 Jeong
-------------------
hello
```
- 한개의 키에 여러개의 값이 대응 될 수 있다는 것을 알 수 있다.
- \[] 연산자를 멀티맵의 경우 사용할 수 없다.
  - m\[1] -> "hello" ?? "hi" ?? 무엇을 return해야할지 모르기 때문이다.
- 위처럼 find()함수를 사용했을 때, 키가 없으면 m.end()를 Return한다.
- 그렇다면 위의 1 이라는 key에 3개 문자열에 대응되어있는데, 어떤 것을 return 해야 할까?
  - C++ 표준에서는 무엇을 리턴하라고 정해놓지 않았다.
  - 해당되는 값들 중 아무 거나 리턴해도 상관없다는 뜻이다.
  - 위의 경우 "hello"가 나왔지만 라이브러리에 따라 다른 것이 나올 수 있다.

```cpp
#include <iostream>
#include <map>
#include <string>

template <typename K, typename V>
void print_map(const std::multimap<K, V>& m) 
{
    // 맵의 모든 원소들을 출력하기
    for (const auto& kv : m) 
    {
    std::cout << kv.first << " " << kv.second << std::endl;
    }
}

int main() 
{
    std::multimap<int, std::string> m;
    m.insert(std::make_pair(1, "hello"));
    m.insert(std::make_pair(1, "hi"));
    m.insert(std::make_pair(1, "ahihi"));
    m.insert(std::make_pair(2, "bye"));
    m.insert(std::make_pair(2, "baba"));

    print_map(m);

    std::cout << "--------------------" << std::endl;

    auto range = m.equal_range(1);
    for (auto itr = range.first; itr != range.second; ++itr)
    {
        std::cout << itr->first << " : " << itr->second << " " << std::endl;
    }
}
```

```
1 hello
1 hi
1 ahihi
2 bye
2 baba
--------------------
1 : hello 
1 : hi 
1 : ahihi
```
- 1에 대응한 값을 알기위해서 Multimap은 위와 같은 함수를 제공하고있다.
- equal_range 함수의 경우 인자로 멀티탭의 key를 받고 이 key에 대응되는 원소들의 반복자들 중 시작과 끝 바로 다음을 가리키는 반복자를 std::pair 객체로 만들어서 리턴한다.
  - first : 시작점
  - second : 끝점 바로 뒤

# unordered_map
- 원소들이 정렬되어 있지 않다.
- set의 경우 원소들이 순서대로 정렬되어 내부에 저장되지만, 위의 경우 원소들이 순서대로 정렬되서 들어가지 않는다.
  - 출력해보면 매우 랜덤한 순서로 나오는 것을 알 수 있다.

```cpp
#include <iostream>
#include <string>
#include <unordered_set>

template <typename K>
void print_unordered_set(const std::unordered_set<K>& m)
{
    for (const auto& elem :m)
    {
        std::cout << elem << std::endl;
    }
}

int main()
{
    std::unordered_set<std::string> s;

    s.insert("hi");
    s.insert("my");
    s.insert("name");
    s.insert("is");
    s.insert("Chaejin");
    s.insert("Jeong");

    print_unordered_set(s);
}
```

```
Jeong
is
name
my
Chaejin
hi
```
- 랜덤한 순서로 출력된다.

![image](https://user-images.githubusercontent.com/69780812/142591018-50a1b40e-7065-4b36-a29f-d86b848dfb89.png)
- unordered_set은 insert, erase, find 모두가 O(1)로 수행된다.
  - set, map 의 경우 O(log N)이었다.
  - 상수시간에 원소를 삽입하고 검색이 가능하다.
- 원소를 삽입하거나 검색하기 위해 먼저 해시 함수라는 것을 사용한다.
  - 해시 함수 : 임의의 크기의 데이터를 고정된 크기의 데이터로 대응시켜주는 함수라고 볼 수 있다. (고정된 크기의 데이터 : 일정 범위의 정수값을 의미한다.)
- 해시 함수는 1부터 상자의 수(=위의 D)까지 값을 반환하고, 그 해시값을 원소를 저장할 상자의 번호로 삼게 된다.
- 해시 함수의 중요한 성질은 **같은 원소를 해시 함수에 전달한다면 같은 해시값을 리턴한다**는 점이다.
  - 이 덕분에 원소의 탐색을 빠르게 수행할 수 있는 것이다.
- 물론 위처럼 빨간색 공과 보라색 공처럼 다른 원소임에도 같은 해시값을 갖는 경우가 있다.
  - 이를 **hash collision**(해시 충돌)이라고한다.
  - 따라서 보라색 공이 이 set에 포함되어 있는지 확인하고 싶다면 보라색 공의 해시값을 계산한 뒤 해당ㅎ하는 상자에 있는 모든 원소들을 타맥해 보아야할 것이다.
    - 해시 함수는 최대한 1부터 N까지 고른 값을 반환하도록 설계되어있다. 그래서 상자의 수도 충분히 많아야 상수 시간 탐색을 보장할 수 있다.
    - 그럼에도 운이 매우매우 나쁘면 같은 원소들이 모두 1번 상자에 들어갈 수도 있다. 그럼 O(N)으로 실행되버리게된다.
- unordered_set은 평균적으로 O(1)으로 원소의 삽입/탐색을 수행할 수 있고, 최악의 경우에는 O(N)으로 수행될 수 있다.
  - set, map은 평균도, 최악도 O(logN)이다.
  - 위의 경우 때문에 보통의 경우 안전하게 맵이나 셋을 사용하고, 최적화가 매우 필요할 때만 unordered_set과 unordered_map을 사용하는 것이 좋다.
- 처음부터 많은 개수의 상자를 사용할 수 없으므로(메모리 낭비 때문) 상자의 개수는 삽입되늰 원소가 많아짐에 따라 점진적으로 늘어나게 된다.
  - 문제는 상자의 개수가 늘어나면 해시 함수를 바꿔야 하므로 모든 원소들을 처음부터 끝까지 다시 insert해야된다.
  - 이를 **rehash**라하며 O(N) 만큼의 시간이 걸리게된다.

```cpp
#include <iostream>
#include <string>
#include <unordered_set>

template <typename K>
void print_unordered_set(const std::unordered_set<K>& m)
{
    for (const auto& elem :m)
    {
        std::cout << elem << std::endl;
    }
}

template <typename K>
void is_exist(std::unordered_set<K>& s, K key)
{
    auto itr = s.find(key);
    if (itr != s.end())
    {
        std::cout << key << " 가 존재" << std::endl;
    } else {
        std::cout << key << " 가 없다" << std::endl;
    }
}

int main()
{
    std::unordered_set<std::string> s;

    s.insert("hi");
    s.insert("my");
    s.insert("name");
    s.insert("is");
    s.insert("Chaejin");
    s.insert("Jeong");
    s.insert("C++");

    print_unordered_set(s);
    std::cout << "----------------" << std::endl;
    is_exist(s, std::string("c++"));
    is_exist(s, std::string("c"));

    std::cout << "----------------" << std::endl;
    std::cout << "'hi' 를 삭제" << std::endl;
    s.erase(s.find("hi"));
    is_exist(s, std::string("hi"));
}
```

```
C++
Jeong
is
name
my
Chaejin
hi
----------------
c++ 가 없다
c 가 없다
----------------
'hi' 를 삭제
hi 가 없다
```
- unordered_set, unorder_map 모두 find 함수를 지원하며 사용법은 셋과 동일하다.
- find 함수의 경우 해당하는 원소가 존재하면 이를 가리키는 반복자를, 없다면 end를 return 한다.
- 제거하고 싶다면 find로 원소를 가리키는 반복자를 찾고, 이를 전달하면 된다.

## 내가 만든 클래스를 unordered_set/map의 원소로 넣고 싶다면 ?
- 안타깝게도 셋이나 맵에 넣는 것보다 훨씬 어렵다.
  - **hash function**을 직접 만들어 줘야하기 때문이다.
  - 그래서 set, map을 사용하는 것을 권장하는 것이다.
- set, map 처럼 순서대로 정렬하지는 않아서 operator< 가 필요하지는 않지만 operator==는 필요하다.
  - 해시 충돌 발생 시 상자안에 있는 원소들과 비교해야하기 때문이다.
- 한가지 다행인점은 C++에서 기본적 타입들에 대한 해시 함수들을 제공하고 있다.
- 기본적으로 unordered_set과 unordered_map은 해시 함수 계산을 위해 hash 함수 객체를 사용한다.

```cpp
hash<string> hash_fn;
size_t hash_val = hash_fn(str);
```
- 예를들어 string 함수의 해시값을 계산할 때 hash 함수 객체가 위와 같이 생겼다.
- string을 템플릿으로 이자로 받는 hash_fn 객체를 만들고, 마치 함수를 사용하는 것처럼 사용한다. (Functor)

```cpp
// hash 클래스의 Todo 템플릿 특수화 버전!
template <>
struct hash<Todo> {
  size_t operator()(const Todo& t) const {
    // 해시 계산
  }
};
```
- Todo 함수의 hash 함수를 계산하는 함수 객체를 만들기 위해 hash 클래스의 Todo 특수화 버전을 만들어 줘야한다.
- 해시 함수는 객체의 operator()를 오버로드 하면 되고 std::size_t 타입을 리턴하면 된다.
  - size_t = int (0 ~ 4294967295)

```cpp
#include <iostream>
#include <string>
#include <unordered_set>
#include <functional>


template <typename K>
void print_unordered_set(const std::unordered_set<K>& m) {
  // 셋의 모든 원소들을 출력하기
  for (const auto& elem : m) {
    std::cout << elem << std::endl;
  }
}

template <typename K>
void is_exist(std::unordered_set<K>& s, K key) {
  auto itr = s.find(key);
  if (itr != s.end()) {
    std::cout << key << " 가 존재!" << std::endl;
  } else {
    std::cout << key << " 가 없다" << std::endl;
  }
}

class Todo {
  int priority;  // 중요도. 높을 수록 급한것!
  std::string job_desc;

 public:
  Todo(int priority, std::string job_desc)
      : priority(priority), job_desc(job_desc) {}

  bool operator==(const Todo& t) const {
    if (priority == t.priority && job_desc == t.job_desc) return true;
    return false;
  }

  friend std::ostream& operator<<(std::ostream& o, const Todo& t);
  friend struct std::hash<Todo>;
};

// Todo 해시 함수를 위한 함수객체(Functor)
// 를 만들어줍니다!
namespace std {
    template <>
    struct hash<Todo> {
    size_t operator()(const Todo& t) const {
        hash<string> hash_func;

        return t.priority ^ (hash_func(t.job_desc));
    }
};

}  // namespace std
std::ostream& operator<<(std::ostream& o, const Todo& t) {
  o << "[중요도 : " << t.priority << " ] " << t.job_desc;
  return o;
}


int main()
{
    std::unordered_set<Todo> todos;

    todos.insert(Todo(1, "농구 하기"));
    todos.insert(Todo(2, "수학 숙제 하기"));
    todos.insert(Todo(1, "프로그래밍 프로젝트"));
    todos.insert(Todo(3, "친구 만나기"));
    todos.insert(Todo(2, "영화 보기"));

    print_unordered_set(todos);
    std::cout << "---------------" << std::endl;
}
```

```
[중요도 : 2 ] 영화 보기
[중요도 : 3 ] 친구 만나기
[중요도 : 1 ] 프로그래밍 프로젝트
[중요도 : 2 ] 수학 숙제 하기
[중요도 : 1 ] 농구 하기
```
- int , std::strnig 등 기본적 타입들에 대한 해시함수는 C++ STL이 제공하고있다.
- Todo 클래스의 해시 함슈는 이들을 잘 사용해 짬뽕 시켜보면 된다..
  - 짬뽕 시키기 위해 XOR 연산 이용
- operator== 오버 로딩도 필요하다.