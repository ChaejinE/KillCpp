# sizeof... 연산자
```cpp
#include <iostream>

int sum_all() {return 0;}

template <typename... Ints>
int sum_all(int num, Ints... nums)
{
    return num + sum_all(nums...);
}

template <typename... Ints>
double average(Ints... nums)
{
    return static_cast<double>(sum_all(nums...)) / sizeof...(nums);
}

int main()
{
    std::cout << average(1, 4, 2, 3, 10) << std::endl;

    return 0;
}
```

```
4
```
- sizeof...연산자를 통해 파라미터 팩(nums)를 전달하면 실제 인자의 개수를 리턴해준다.

# Fold Expression
- 가변 길이 템플릿은 매우 편리하지만 재귀 함수 형태로 구성해야하는 것이 단점이다.
  - 재귀 호출 종료를 위한 함수를 따로 만들어야한다.
  - 가변길이 템플릿은 C++11 일 때, 도입되었다.

```cpp
int sum_all() { return 0; }
```
- 이것이 재귀 함수 호출을 종료하기 위한 것이다.
- 이는 코드의 복잡도를 쓸데 없이 늘리는 꼴이다.
- 새로 도입된 Fold 형식을 사용해서 훨씬 간단히 표현해보자!

```cpp
#include <iostream>

template <typename... Ints>
int sum_all(Ints... nums)
{
    return (... + nums);
}

int main()
{
    std::cout << sum_all(1, 4, 2, 3, 10) << std::endl;
}
```
- 이는 C++17에서 추가된 Fold형식이다.

```cpp
return ((((1+4) + 2) + 3 ) + 10);
```
- 컴파일러에서는 위와 같이 해석됟는 것이다.
- 위와 같은 형태를 단황 자측 Fold, **Unary left fold**라고 부른다.

![image](https://user-images.githubusercontent.com/69780812/141986796-84d67439-daff-4956-ba6b-02f0a7554df3.png)
- 위는 Fold방식의 종류이며 4가지가 있다.
- I는 초기값을 의미한다. (파라미터 팩 X)
- op자리에는 이항 연산자들이 포함된다. (+, -, < ..., > 등)
- 한가지 중요한 점은 Fold식을 쓸 때 꼭 ()로 감싸줘야 한다.
  - 안하면 컴파일 오류가난다.

```cpp
#include <iostream>

class A
{
    public:
        void do_somthing(int x) const
        {
            std::cout << "Do something" << x << std:: endl;
        }
};

template <typename T, typename... Ints>
void do_may_things(const T& t, Ints... nums)
{
    (t.do_somthing(nums), ...);
}

int main()
{
    A a;
    do_may_things(a, 1, 2, 3, 4);
}
```

```
Do something1
Do something2
Do something3
Do something4
```
- ',' 연산자를 사용하면 각각의 인자들에 대해 원하는 식을 실행할 수 있다.
- 모든 인자들에 대해 t.do_somthing(arg)를 실행한 것고 ㅏ같다.
- 지린다.
- **가변 길이 템플릿을 잘 활용하면 작성해야 하는 코드의 양을 줄일 수 있다.**
