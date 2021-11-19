# Set
```cpp
#include <iostream>
#include <set>

template <typename T>
void print_set(std::set<T>& s)
{
    std::cout << "[ ";
    for (typename std::set<T>::iterator itr = s.begin(); itr != s.end(); ++itr)
    {
        std::cout << *itr << " ";
    }

    std::cout << " }" << std::endl;
}

int main()
{
    std::set<int> s;
    s.insert(10);
    s.insert(50);
    s.insert(20);
    s.insert(40);
    s.insert(30);

    std::cout << "순서대로 정렬되어 나온다." << std::endl;
    print_set(s);

    std::cout << "20 이 s의 원소 ? :: ";
    auto itr = s.find(20);
    if (itr != s.end())
    {
        std::cout << "Y" << std::endl;
    } else {
        std::cout << 'N' << std::endl;
    }

    std::cout << "25 이 s의 원소 ? :: ";
    itr = s.find(25);
    if (itr != s.end())
    {
        std::cout << "Y" << std::endl;
    } else {
        std::cout << 'N' << std::endl;
    }
}
```

```
순서대로 정렬되어 나온다.
[ 10 20 30 40 50  }
20 이 s의 원소 ? :: Y
25 이 s의 원소 ? :: N
```
- set 원소 추가 : insert()
- Sequence container 처럼 **어디에 추가할지**에 대한 정보는 없다.
- 컨테이너 안에 원소가 어디에 있는지 중요한게 아니라 원소가 있냐 없냐만 중요한 정보인 것이다.
- 셋의 원소를 추가하거나 지우는 작업은 O(logN)에 처리된다.
  - 시퀀스 컨테이너의 경우 임의의 원ㅅ호를 지우는 작업이 O(N)인 것을 보면 훨씬 빠르다는 것을 알 수 있다.
- set의 반복자(iterator)는 BidirectionalIterator 타입이다.
  - 리스트 처럼 임의의 위치에 있는 원소에 접근하는 것은 불가능하고 순차적으로 하나 씩 접근하는 것밖에 안된다.
- **출력했을 때 내부에 원소를 추가하면 정렬된 상태를 유지하면서 추가**한다.
  - 이 때문에 O(logN)으로 진행되는 것이다.

![image](https://user-images.githubusercontent.com/69780812/142551149-6f2be997-15a2-498e-a89b-8da96fc71b46.png)
- set의 진가는 find 함수를 통해 원소가 존재하냐 안하냐를 확인하는 것이다.
  - find는 원소가 존재하면 **가리키는 반복자를 return**한다.
  - 존재하지 않는다면 s.end()를 return하게 된다.
  - vector의 경우 find는 O(N)의 시간복잡도를 가진다.
- set은 O(logN)으로 원소가 존재하는지 확인할 수 있다. 놀랍다.
  - 이것이 가능한 이유는 셋 내뷰적으로 원소들이 정렬된 상태를 유지하기 때문에 비교적 빠르게 원소 존재 유무를 확인할 수 있는 것이다.
- Set은 내부적으로 트리구조로 구성되어있다.
  - 각 원소들은 트리의 노드에 저장되며 규칙을 지키고 있다.
    - 1. 왼쪽에 오는 노드는 나보다 작다.
    - 2. 오른ㅉㄱ에 오는 노드는 나보다 크다.
- 25를 찾는다면 ?
  - 최상위 노드와 비교 25 > 20 - 오른쪽으로간다.
  - 30과 비교 25 < 30 - 왼쪽으로 간다.
  - 25 == 25 -> True
  - 원소는 8개지만 3번만에 찾는 것을 알 수 있다.
- 12를 찾는다 ?
  - 루트 노드와 비교 12 < 20 - 왼쪽
  - 15와 비교 12 < 15 - 왼쪽
  - 10과 비교 12 > 10 - 오른쪽 노드, But 아무것도 없다 ? False(존재하지 않는다.)
  - 단 3번의 비교로 셋에 찾으려는 원소가 존재하는지 안하는지를 알 수 있게 된다.

![image](https://user-images.githubusercontent.com/69780812/142551463-53269965-e815-48cc-85c7-db822c041a83.png)
- 원소 검색 시 필요한 횟수는 트리의 높이와 정확히 일치한다.

![image](https://user-images.githubusercontent.com/69780812/142551510-4484ad6b-f9fe-4a25-9073-0bceae812ca5.png)
- 위처럼 되버리면 시퀀스 컨테이너와 검색속도가 동일해진다.
  - unbalanced tree : 한쪽으로 아예치우쳐진 트리
- 실제 셋의 구현을 보면 위와 같은 상황이 발생하지 않도록 위의 단순한 두개 규칙ㅂ다 더 많은 규칙들을 도입해 트리를 항상 균형 잡히도록 유지하고있다.
  - O(logN) 보장
  - [set의 실제 트리구조](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree)

```cpp
#include <iostream>
#include <set>

template <typename T>
void print_set(std::set<T>& s) {
  // 셋의 모든 원소들을 출력하기
  std::cout << "[ ";
  for (const auto& elem : s) {
    std::cout << elem << " ";
  }
  std::cout << " ] " << std::endl;
}

int main() {
  std::set<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(30);
  s.insert(20);
  s.insert(10);

  print_set(s);
}
```

```
[ 10 20 30  ] 
```
- set의 중요한 특징은 **중복된 원소들이 없다는 것이다.**
- set 자체적을 이미 같은 워소가 있으면 insert하지 않기 때문이다.
  - 마지막 두 insert()는 무시되었을 것이다.
- set은 ranged-for문을 지원하고, 접근 순서는 반복자를 이용해 접근했을 때와 동일하다.
---
- set에 객체를 둘 때, 주의할 점은 클래스를 **operator<** 함수를 설계해야한다는 것이다.
   - A < A : 거짓
   - A < B != B < A
   - A < B && B < C == A < C
   - A == B -> A < B, B< A가 거짓
   - A == B && B == C -> A == C
   - 엄밀히따지면 위와 같은 조건들을 만족해야한다. (A, B는 Object)
   - 위와 같은 조건을 만족하는 < 연산자를 strict weak ordering을 만족한다고 한다.
   - 상식적으로 설계했다면 위 조건들은 모두 만족할 수 있다. (조건이 안맞으면 런타임 시 버그찾기 힘듬)
- 외부라이브러리를 통한 객체는 ..?
  - operator <가 정의되어있지 않을 가능성이 높다.

```cpp
#include <iostream>
#include <set>
#include <string>

template <typename T, typename C>
void print_set(std::set<T, C>& s)
{
    for (const auto& elem : s)
    {
        std::cout << elem << " " << std::endl;
    }
}

class Todo
{
    int priority;
    std::string job_desc;

    public:
        Todo(int priority, std::string job_desc)
            : priority(priority), job_desc(job_desc) {}
        
        friend struct TodoCmp;
        friend std::ostream& operator<<(std::ostream& o, const Todo& td);
};

struct TodoCmp
{
    bool operator()(const Todo& t1, const Todo& t2) const
    {
        if (t1.priority == t2.priority)
        {
            return t1.job_desc < t2.job_desc;
        }
        return t1.priority > t2.priority;
    }
};

std::ostream& operator<<(std::ostream& o, const Todo& td)
{
    o << "[ 중요도: " << td.priority << "] " << td.job_desc;
    return o;
}

int main()
{
    std::set<Todo, TodoCmp> todos;

    todos.insert(Todo(1, "농구하기"));
    todos.insert(Todo(2, "수학숙제"));
    todos.insert(Todo(1, "프로그래밍"));
    todos.insert(Todo(3, "친구"));
    todos.insert(Todo(2, "영화보기"));

    print_set(todos);

    std::cout << "숙제를 끝냈다면 " << std::endl;
    todos.erase(todos.find((Todo(2, "수학숙제"))));
    print_set(todos);
}
```

```
[ 중요도: 3] 친구 
[ 중요도: 2] 수학숙제 
[ 중요도: 2] 영화보기 
[ 중요도: 1] 농구하기 
[ 중요도: 1] 프로그래밍 
숙제를 끝냈다면 
[ 중요도: 3] 친구 
[ 중요도: 2] 영화보기 
[ 중요도: 1] 농구하기 
[ 중요도: 1] 프로그래밍
```
- operator< 를 쓰지 않고도 비교를 수행할 수 있다.
- 객체간 비교를 위해 TodoCmp라는 클래스를 만들었다.
- TodoCmp 타입을 set의 **두번째 인자**로 넘겨주면 set은 이를 받아서 TodoCmp에 정의된 함수 객체를 바탕으로 모든 비교를 수행하게 된다.

```cpp
template <class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>
class set;
```
- set의 클래스 정의는 위와 같으며 Compare를 받는 다는 것을 알 수 있다.
  - Default가 std::less\<Key>다.
  - Compare 타입을 전달하지 않으면 Key 클래스의 operator<로 비교를 수행한다.

# Multiset
- **중복된 원소를 허락한다.**
```cpp
#include <iostream>
#include <set>
#include <string>

template <typename K>
void print_set(const std::multiset<K>& s)
{
    for (const auto& elem : s)
    {
        std::cout << elem << std::endl;
    }
}

int main()
{
    std::multiset<std::string> s;

    s.insert("a");
    s.insert("a");
    s.insert("a");
    s.insert("b");
    s.insert("b");
    s.insert("a");

    print_set(s);
}
```

```
a
a
a
b
b
c
```
- set이었다면 a,b,c로 나왔어야 하지만 중복된 원소를 허락해 모든 원소들이 나오게 된다.

# unordered_set
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