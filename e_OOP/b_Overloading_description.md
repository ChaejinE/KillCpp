# Overloading
## 사전적 의미
1. 과적하다.
2. 너무 많이 주다.
3. (컴퓨터 전기 시스템 등에) 과부하가 걸리게하다.
- C언어에서는 하나의 이름을 가지는 함수는 딱 1개만 존재할 수 밖에 없다. 하지만 CPP에서는 같은 이름을 가진 함수가 여러개 존재해도 된다.
  - C언어의 특징 때문에 함수 이름에 과부하가 걸린다고 표현하는 것 같다.
- 즉, 함수의 이름에 과부하가 걸려도 상관 없다는 것이다.

```cpp
#include <iostream>

void print(int x) { std::cout << "int " << x << std::endl; }
void print(char x) { std::cout << "char " << x << std::endl; }
void print(double x) { std::cout << "double " << x << std::endl; }

int main()
{
    int a = 1;
    char b = 'c';
    double c = 3.2f;

    print(a);
    print(b);
    print(c);

    return 0;
}
```
- 함수의 이름이 같더라도 **인자가 다르면 다른 함수**라고 판단하기때문에 오류가 나지 않는 것이다.
- 컴퍼일러가 알아서 적합한 인자를 가지는 함수를 찾아 호출해주는 것이다.

```cpp
#include <iostream>

void print(int x) { std::cout << "int " << x << std::endl; }
void print(double x) { std::cout << "double " << x << std::endl; }

int main()
{
    int a = 1;
    char b = 'c';
    double c = 3.2f;

    print(a);
    print(b);
    print(c);

    return 0;
}
```
- 조금 특이한 경우다.
- 자기를 인자로 정확하지 않은 함수인 경우에는 **자신과 최대로 근접한 함수**를 찾게된다.

# Compiler 에서 함수를 Overloading 하는 과정
1. 자신과 타입이 정확히 일치하는 함수를 찾는다.
2. 정확히 일치하는 Type이 없는 경우 형변환을 통해 일치하는 함수를 찾아본다.
  - char, unsinged char, short -> int
  - unsinged short 는 int 크기에 따라 int or unsinged int로 변환
  - float -> double
  - enum -> int
3. 2단계에서 변환해도 일치하는 것이 없다면 좀 더 포괄적인 형변환을 통해 함수를 찾는다.
  - 임의의 numeric type은 다른 numeric type으로 변환된다. (float -> int)
  - Enum도 numeric type으로 변환된다. (enum -> double)
  - 0 (??? 무슨말인지 모른다.)
  - 포인터는 void 포인터로 변환된다.
4. User 정의된 Type 변환으로 일치하는 것을 찾는다.
- 위 과정을 통하더라도 일치하는 함수를 찾을 수 없거나  같은 단계에서 두 개 이상 일치하는 경우 ambiguous라고 판단하여 오류를 발생시킨다.

```cpp
#include <iostream>

void print(int x) { std::cout << "int " << x << std::endl; }
void print(char x) { std::cout << "double " << x << std::endl; }

int main()
{
    int a = 1;
    char b = 'c';
    double c = 3.2f;

    print(a);
    print(b);
    print(c);

    return 0;
}
```
```
error C2668: 'print' : ambiguous call to overloaded function
 could be 'void print(char)'
 or       'void print(int)'
while trying to match the argument list '(double)'
```
- 말했다시피 모호한 오버로딩은 에러를 발생시킨다.
- 1단계에서는 명백히 일치하는 것이없다.
- 2단계에서는 double의 casting에 대한 내용이 없다.
- 3.단계에서는 임의의 numeric type이 다른 임의의 numeric type으로 변환되므로 doble은 char, int로 변환 될 수 있다. -> Ambiguos

