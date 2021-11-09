# Const Pointer (상수 포인터)
- 어떠한 데이터를 상수로 만들기 위해서는 const 키워드를 붙여주면 된다.

```c
const int a = 3;
```
- int 변수 a를 상수로 정의한 것이다.
- **이 데이터의 내용은 절대로 바뀔 수 없다**는 의미의 키워드다.
- int 변수 a의 값은 절대로 바뀌면 안된다 !!를 코드로써 알리는 것이다.

```c
a = 4;
```
- 컴파일 시 바로 오류가 발생한다.
- 심지어 **값이 변경될 가능성이 있는 문장** 조차 허용되지 않는다.
  - a = 3이라 할지라도 같은 값이지만 "가능성"이 있으므로 컴파일 오류가 난다.

# 도대체 왜 const를 사용하는가 ?
- 프로그래머들의 실수를 줄여주기 위해
- 실수를 했더라도 실수를 잡아내는데 중요한 역할을 한다.
- **절대로 바뀌지 않을 것 같은 값에는 무조건 const 키워드를 붙여주는 습관**을 길러야한다.

```c
#include <stdio.h>

int main() {
    int a;
    int b;
    const int* pa = &a;

    *pa = 3;
    pa = &b;
    return 0;
}
```
- 위에서는 컴파일 오류가 발생할 것이다.

```c
const int* pa = &a;
```
- 포인터 pa는 const int형을 가리키는 포인터다.
- 어떻게 int 형 변수 a의 주소값이 대입될 수 있는 것인가 ?
  - const 라는 키워드는 **이 데이터의 값은 절대로 바뀌면 안된다**를 알려주는 키워드다.
  - const int*의 의미는 const int형 변수를 가리키는 것이 아니다.
  - int형 변수를 가리키는데, **그 값을 절대로 바꾸지 말라**라는 의미다.
- a라는 값 자체는 변수 이므로 값이 자유롭게 변경될 수 있다. 하지만 pa를 통해 a를 간접적으로 가리킬 때는 **내가 const를 변수를 가리키고 있다**는 것으로 알고 있기 때문에 값을 바꿀 수 없는 것이다.

```c
pa = &b;
```
- 올바르지 않은 문장이다. 이 것때문에 컴파일 에러가 나는 것이다.
- pa가 처음에 가리키는 것(a) 말고는 다른 것은 절대로 건드릴 수 없다는 것이다.
- 결론적으로 pa에 저장된 주소값을 바꾸는 것이므로 오류가 나게된다.

```c
*pa = 3;
```
- 올바른 문장이다.
- pa가 가리키는 값을 바꾸면 안된다는 말은 안했다.