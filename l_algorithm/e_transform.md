# Overview
- 원소들을 수정하는 함수들이다.
- 많은 경우 컨테이너 전체 혹은 일부를 순회하며 값들을 수정하는 작업을 많이한다.
  - 예를들어 벡터의 모든 원소에 1씩 더한다는 작업들 등

# transform
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
    vec.emplace_back(4);

    std::vector<int> vec2(6, 0); // 6개의 0으로 초기화

    std::cout << "처음 vec, vec2 -------_" << std::endl;

    print(vec.begin(), vec.end());
    print(vec2.begin(), vec2.end());

    std::cout << "vec 전체에 1을 더한 것을 vec2에 저장" << std::endl;

    std::transform(vec.begin(), vec.end(), vec2.begin(),
                    [](int i) { return i +1; });
    
    print(vec.begin(), vec.end());
    print(vec2.begin(), vec2.end());
}
```

```
처음 vec, vec2 -------_
[5] [3] [1] [2] [3] [4] 
[0] [0] [0] [0] [0] [0] 
vec 전체에 1을 더한 것을 vec2에 저장
[5] [3] [1] [2] [3] [4] 
[6] [4] [2] [3] [4] [5]
```
- vector에 1씩 더해서 다음 버전에 저장한 코드다.

```
transform(시작 반복자, 끝반복자, 결과를 저장할 컨테이너의 시작 반복자, Pred)
```
- 위와같은 꼴로 사용하여 vec의 시작부터 끝까지 각 원소에 람다 함수를 적용시킨 결과를 vec2.begin()부터 저장하게 된것이다.
- 한가지 주의할 점은 값을 저장하는 컨테이너의 크기가 원래 컨테이너보다 최소한 같거나 커야한다는 점이다.
- 귀찮게 for문을 쓸 필요도 없어지면서 이 코드에서 무슨일을 하는지 더 간단 명료하게 나타낼 수 있게된다.
