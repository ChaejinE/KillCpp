# Overview
- C++ 11에 처음으로 도입되었다.

# lambda
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
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    std::cout << "처음 vec ----------" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << "벡터에서 홀수인 원소 제거 ----" << std::endl;
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                            [](int i) -> bool { return i % 2 == 1; }),
              vec.end());
    print(vec.begin(), vec.end());
}
```

```
처음 vec ----------
[5] [3] [1] [2] [3] [4] 
벡터에서 홀수인 원소 제거 ----
[2] [4] 
```
- 특정 조건에 부합하는 원소 제거에 lambda 식을 적용한 사례다.

```
[capture list] (받는 인자) -> return type { 함수 본체 }
```
- 위와 같은 형태로 사용된다.
- 간단하게 함수 꼴을 살펴보자면 int i 를 받고, bool을 리턴하는 람다 함수를 정의한 것이다.

```cpp
[](int i) { return i % 2 == 1; }
```
- Return type을 생략하면 컴파일러가 알아서 함수 본체에서 return 문을 보고 type을 추측해준다.
  - return 경로가 여러 군데면 추측할 수 없어 컴파일 오류가 난다.

```cpp
auto func = [](int i) { return i % 2 == 1; };
```
위 처럼 func라는 함수 객체를 생성하고 호출할 수 도 있겠다.

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
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    std::cout << "처음 vec ----------" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << "벡터에서 홀수인 원소 2개 제거 ----" << std::endl;
    int num_erased = 0;
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                            [&num_erased](int i) -> bool
                            {
                                if (num_erased >= 2)
                                    return false;
                                else if (i % 2 == 1)
                                {
                                    num_erased++;
                                    return true;
                                }
                                return false;
                            }),
              vec.end());
    print(vec.begin(), vec.end());
}
```

```
처음 vec ----------
[5] [3] [1] [2] [3] [4] 
벡터에서 홀수인 원소 2개 제거 ----
[1] [2] [3] [4] 
```
- 위와 같이 **캡처 목록**에는 어떤 변수를 capture할지 써주면 된다.
  - 위 경우 num_erased를 캡쳐한 것이다.
- num_erased를 마치 같은 스포크안에 있는 것처럼 사용할 수 있게 된다.
  - lambda는 자기만의 스코프를 가진다.
- num_erased 앞에 reference &가 붙어있는데, 이 변수의 레퍼런스를 캡쳐한 것이다.
  - 함수 내부에서 num_erased의 값을 바꿀 수 있게되는 것이다.
- Reference를 붙이지 않으면 복사본을 얻는데, 그 복사본의 형태는 **const**이다.
  - 함수 내부에서 값을 바꿀 수 없게 된다.

```cpp
class SomeClass {
  std::vector<int> vec;

  int num_erased;

 public:
  SomeClass() {
    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    num_erased = 1;

    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [&num_erased](int i) {
                               if (num_erased >= 2)
                                 return false;
                               else if (i % 2 == 1) {
                                 num_erased++;
                                 return true;
                               }
                               return false;
                             }),
              vec.end());
  }
};
```
- class의 경우 &로 캡쳐해서 사용할 수 있을 것 같지만 실제로 컴파일이 되지 않는다.
- 객체에 종속되어 있는 멤버 변수이기 때문이다.
  - 그래서 num_erased가 이 객체의 멤버 변수가 아니라 그냥 일반 변수라고 생각하게 된다.

```cpp
num_erased = 0;

vec.erase(std::remove_if(vec.begin(), vec.end(),
                         [this](int i) {
                           if (this->num_erased >= 2)
                             return false;
                           else if (i % 2 == 1) {
                             this->num_erased++;
                             return true;
                           }
                           return false;
                         }),
          vec.end());
```
- 위와 같이 this를 전달해주는 것이 좋다. this를 복사본으로 전달해서 함수 안에서 this를 통해 멤버 변수들을 참조하여 사용하면된다.
  - **this는 레퍼런스로 전달할 수 없다.**

## Capture 리스트 사용법 간단정리
- \[] : 아무것도 캡처안함
- \[&a, b] : a는 레퍼런스로 캡쳐하고 b는 변경불가능한 복사본으로 캡쳐
- \[&] : 외부의 모든 변수들을 레퍼런스로 캡쳐
- \[=] : 외부 모든 변수들을 복사본으로 캡쳐

