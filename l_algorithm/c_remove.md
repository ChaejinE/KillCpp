# Overview
- 원소를 제거하는 함수들에 대해 배울 것이다.
- 사실 이미 대부분의 컨테이너에서 원소를 제거하는 함수를 지원하고 있다.
  - vec.erase() 등
- 그러나 위 함수로는 많은 작업들을 하기 어렵다.

```cpp
std::vector<int>::iterator itr = vec.begin();

for (; itr != vec.end(); ++itr)
{
    if (*itr == 3)
    {
        vec.erase(itr);
        itr = vec.begin();
    }
}
```
- 3이라는 것을 지우기 위해 위와 같은 코드를 만들어서 사용해야한다.
- 반복자 무효화 문제도 있다.

# remove
```cpp
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

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
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    std::cout << "처음 상태 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << " 3 제거 ----" << std::endl;
    vec.erase(std::remove(vec.begin(), vec.end(), 3), vec.end());
    print(vec.begin(), vec.end());
}
```

```
처음 상태 ----
[5] [3] [1] [2] [3] [4] 
 3 제거 ----
[5] [1] [2] [4] 
```
- erase는 2 가지 형태가 있다.

```cpp
Iterator erase(Iterator pos);
Iterator erase(Iterator first, Iterator last);
```
- 전자의 경우 pos가 가리키는 원소를 벡터에서 지운다.
- 후자는 first부터 last 사이 **모든 원소를 지운다.**

![image](https://user-images.githubusercontent.com/69780812/142870567-e5ebbd0c-a4d1-4fb3-952b-867bfae1e67b.png)

![image](https://user-images.githubusercontent.com/69780812/142870616-d18654a0-8d13-4eb0-bf98-5c4ce2a604fb.png)
- 위와 같이 만일 3인 원소를 만나면 그 뒤에 있는 원소들로 쭈르륵 쉬프트 해주게 된다.
- 자연스럽게 알고리즘이 끝나면 해당 위치에서 전 까지 3이 제외된 원소들로 쭈르륵 채워지게 된다.

![image](https://user-images.githubusercontent.com/69780812/142870817-cdd718ee-357a-40b6-956b-e584a1efba50.png)
- 반복이 끝나는 위치부터 벡터 맨 뒤 까지 제거해 버리면 3이 싹 제거된 벡터만 남게 된다.
- remove 함수는 원소의 이동만을 수행하지 실제로 원소를 삭제하는 연산을 수행하지는 않는다.
- 따라서 벡터에서 실제로 원소를 지우기 위해서는 반드시 earse 함술르 호출하여 실제로 원소를 지워줘야한다.
- 따라서 remove 함수를 이용해 3인 원소들을 뒤로 보내버리고 그 원소들을 벡터에서 삭제할 수 있는 것이다.
- remove 함수의 경우 반복자 타입이 ForwardIterator이다.
  - vector, list, set, map에서도 모두 사용할 수 있다.

# remove_if
```cpp
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

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

// 홀수면 제거하도록하는 객체
struct is_odd
{
    bool operator()(const int& i) { return i % 2 == 1; }
};

int main()
{
    std::vector<int> vec;
    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    std::cout << "처음 상태 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << " 3 제거 ----" << std::endl;
    vec.erase(std::remove_if(vec.begin(), vec.end(), is_odd()), vec.end());
    print(vec.begin(), vec.end());
}
```

```
처음 상태 ----
[5] [3] [1] [2] [3] [4] 
 3 제거 ----
[2] [4]
```
- 위와 같이 is_odd 구조체에 operator()를 만들어서 함수 객체를 전달하면 된다.
- 특정 조건에 해당하는 원소들을 제거하는 것이다.

```cpp
template <typename Iter, typename Pred>
remove_if(Iter first, Iter last, Pred pred)
```
- 함수 객체로 실제 함수를 전달할 수도 있다.
  - Pred가 함수 포인터 타입이 될 것이다.

```cpp
bool odd(const int& i) { return i % 2 == 1; }
...
int main()
{
    ...

    vec.erase(std::remove_if(vec.begin(), vec.end(), odd), vec.end);
    ...
}
```
- 위와 같이 실제 함수를 전달한다면 함수 객체와 정확히 동일하게 동작하게 된다.

```cpp
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

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

// 홀수면 제거하도록하는 객체
struct is_odd
{
    int num_delete;

    is_odd() : num_delete(0) {}

    bool operator()(const int& i)
    {
        if (num_delete >= 2) return false;

        if (i % 2 == 1)
        {
            num_delete++;
            return true;
        }

        return false;
    }
};

int main()
{
    std::vector<int> vec;
    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    std::cout << "처음 상태 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << "벡터에서 홀수인 원소 앞의 2개 제거" << std::endl;
    vec.erase(std::remove_if(vec.begin(), vec.end(), is_odd()), vec.end());
    print(vec.begin(), vec.end());
}
```

```
처음 상태 ----
[5] [3] [1] [2] [3] [4] 
벡터에서 홀수인 원소 앞의 2개 제거
[2] [3] [4] 
```
- 홀수 원소 2개를 삭제하는 코드다.
- 결과는 예상밖으로 3개가 삭제가 된다.
  - 분명히 2개 이상이면 false를 return하도록 했다.
- C++ 표준에 따르면 remove_if에 전달되는 함수 객체의 경우 이전의 호출에 의해 내부 상태가 달라지면 안된다 한다.
  - 원칙상 num_delete를 넣는 것은 안된다는 것이다.
  - remove_if를 실제로 구현했을 때, 해당 함수 객체가 여러번 복사될 수 있기 때문이라고 한다. (C++ 표준은 remove_if 함수를 어떤 방식으로 구현하라고 정해놓지 않았다.)

```cpp
template <class ForwardIt, class UnaryPredicate>
ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate pred) {
  first = std::find_if(first, last, pred);  // <- pred 한 번 복사됨
  if (first != last)
    // 아래 for 문은 first + 1 부터 시작한다고 봐도 된다 (++i != last)
    for (ForwardIt i = first; ++i != last;)
      if (!pred(*i))  // <-- pred 호출 하는 부분
        *first++ = std::move(*i);
  return first;
}
```
- 어떤 라이브러리들의 경우 위와 같은 방식으로 구현됐다고 생각해보자.
  - 대부분 위와 비슷하다고 한다.
- find_if 함수의 경우 인자로 전달된 조건 pred가 참인 첫 번째 원소를 리턴한다.
  - 문제는 find_if가 함수 객체 pred의 reference를 받는 것이 아니라 복사 생성된 버전을 받는다는 점이다.
  - 따라서 **find_if 호출 후 아래 for 문에서 이미 한개 원소를 지웠다는 정보가 소멸**된다.
  - 후에 호출되는 pred 들은 이미 num_delete가 1인지 모른 채 0부처 다시 시작하게 되는 것이다.
- **함수 객체에는 절대로 특정 상태를 저잘해서 이에 따라 결과가 달라지는 루틴을 짜면 안된다.**
  - 위처럼 이해하기 어려운 오류가 발생할 수 있다.
- 해결 방법은 num_delete를 외부 변수로 빼면 된다.

```cpp
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

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

// 홀수면 제거하도록하는 객체
struct is_odd
{
    // int num_delete;
    int* num_delete;

    is_odd(int* num_delete) : num_delete(num_delete) {}

    bool operator()(const int& i)
    {
        if (*num_delete >= 2) return false;

        if (i % 2 == 1)
        {
            (*num_delete)++;
            return true;
        }

        return false;
    }
};

int main()
{
    std::vector<int> vec;
    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    std::cout << "처음 상태 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << "벡터에서 홀수인 원소 앞의 2개 제거" << std::endl;
    int num_delete = 0;
    vec.erase(std::remove_if(vec.begin(), vec.end(), is_odd(&num_delete)), vec.end());
    print(vec.begin(), vec.end());
}
```

```
처음 상태 ----
[5] [3] [1] [2] [3] [4] 
벡터에서 홀수인 원소 앞의 2개 제거
[1] [2] [3] [4] 
```
- 위와 같이 제대로 나온다.
- num_delete에 관한 정보를 아예 함수 객체 밖으로 빼서 보관해버린 것이다.
  - 프로젝트의 커진다면 이 클래스는 뭐하는 거지? 라는 궁금증이 발생할 수 있고, 잘못 사용할 수도 있다.

```cpp
vec.erase(std::remove_if(vec.begin(), vec.end(),
                        bool is_odd(inti) {return i % 2 == 1;}).
                        vec.end());
```
- STL 알고리즘 사용 시, 그 안에 직접 써놓는 것이 가장 이상적인 방법일 수 있겠다.
- 위 문법은 당연히 안된다. lambda를 공부하자.