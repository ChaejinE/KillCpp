# 가변 길이 템플릿
```cpp
print(1, 3.1, "abc")
```
- python 처럼 인자로 전달된 것들을 모두 출력할 수 있을까?

```cpp
#include <iostream>

template <typename T>
void print(T arg)
{
    std::cout << arg << std::endl;
}

template <typename T, typename... Types>
void print(T arg, Types... args)
{
    std::cout << arg << ", ";
    print(args...);
}

int main()
{
    print(1, 3.1, "abc");
}
```

```
1, 3.1, abc
```
- 위와 같이 C++ 템플릿을 사용하면 임의의 개수의 인자를 받는 함수를 구현할 수 있다.
- typename...으로 오는 것을 템플릿 파라미터 팩(**parameter pack**)이라고 부른다.
  - 템플릿 파라미터 팩의 경우 0개 이상의 템플릿 인자들을 나타낸다.
- 함수에 인자로 ... 오는 것을 함수 파라미터 팩이라고 부르며 0개 이상의 함수 인자를 나타낸다.
- 템플릿 파라미터 팩과 함수 파라미터 팩의 차이점은 템플릿의 경우 타입 앞에 ...이 오고, 함수의 경우 뒤에 ...이 온다는 점이다.
  - 파라미터 팩은 추론된 인자를 제외한 나머지 인자들을 나타내게된다.
- 위의 경우 C++ 컴파일러가 print 함수 정의를 살펴보면서 어느 것을 택할지 정한다. 인자를 여러개 받으면 두 번째 print가 택해지는 것이다.