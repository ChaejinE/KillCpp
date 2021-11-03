# Reference의 도입
```cpp
#include <iostream>

int change_val(int *p) {
    *p = 3;

    return 0;
}

int main() {
    int number = 5;

    std::cout << number << std::endl;
    change_val(&number);
    std::cout << number << std::endl;
}
```
- change_val 함수 인자 p에 number의 주소 값을 전달하여 *p를 통해 number를 참조한 후 number의 값을 3으로 바꾸는 Code
- C++에서는 다른 변수나 상수를 가리키는 방법으로 또다른 방식을 제공한다.

```cpp
#include <iostream>

int main() {
    int a = 3;
    int& another_a = a;

    another_a = 5;
    std::cout << "a : " << a << std::endl;
    std::cout << "another_a :" << another_a << std::endl;

    return 0;
}
```
- int형 변수 a를 정의하고 3이란 값을 넣어줬다.

```cpp
int& another_a = a;
```
- anotehr_a를 정의하였다.
- 참조자를 정하는 방법은 타입 뒤에 &를 붙이면 된다.
- anaother_a는 a의 **또다른 이름이라고 컴파일러에게 알려주는 것이다.**
- another_a에 어떠한 작업을 수행하던 간에 사실상 a에 그 작업을 하는 것과 마찬가지다.
- another를 바꿨음에도 a의 값을 확인해보면 5로 바뀌어있을 것이다.
- 참조자와 포인터는 상당히 유사한 개념이다.
  - 하지만 몇가지 중요한 차이점이있다.
- 1. 레퍼런스는 반드시 처음에 누구의 별명이 될 것인지 지정해야한다.
  - int& another_a;와 같은 문장은 불가능 하다.
  - int* p; 포인터의 경우 전혀 문제가 없다.

```cpp
int a = 10;
int &another_a = a;

int b = 3;
another_a = b;
```

- 2. 레퍼런스가 한 번 별명이 되면 절대로 다른 이의 별명이 될 수 없다.
  - a에  b의 값을 대입하라는 의미의 Code가 되버린다.
  - a = b가 되는 것이다.
  - if) &another_a = b; -> &a = b; 이 것 또한 말이안되는 code가 된다.
  
```cpp
int a = 10;
int* p = &a;

int b = 3;
p = &b
```
- 포인터는 누구를 가리키는지 자유롭게 바꿀 수 있다.

```cpp
int a = 10;
int *p = &a; // p, 메모리 상에서 8바이트 차지

int a = 10;
int &another_a = a; // 자리를 차지할 필요 없을 수 도..
```
- 3. 레퍼런스는 메모리 상에 존재하지 않을 수도 있다.
  - another_a를 위해 메모리 상에 공간을 할당할 필요가 있는 코드인가 ?
  - 아니라면 컴파일러는 another_a가 쓰이는 자리는 모두 a로 바꿔치기가 가능해진다는 얘기가 된다. 즉, 레퍼런스는 메모리상에 존재하지 않게 된다.
  - 하지만 항상 존재하지 않는 것은 아니다.

```cpp
#include <iostream>

int change_val(int &p) {
    p = 3;

    return 0;
}

int main() {
    int number = 3;

    std::cout << number << std::endl;
    change_val(number);
    std::cout << number << std::endl;
}
```
- change_val(number)로 호출할 때가 p가 정의되는 순간이다.
- 사실상 int& p = number가 실행된다.
- 여기서 중요한 점은 포인터가 인자일 때와 다르게 number 앞에 &를 붙일 필요가 없다.
- p = 3;이라는 작업은 main의 number = 3;을 하라는 것과 정확히 같은 작업을 하게 된다.

# 여러 참조자 예시
```cpp
// 참조자 이해하기

#include <iostream>

int main() {
  int x;
  int& y = x;
  int& z = y;

  x = 1;
  std::cout << "x : " << x << " y : " << y << " z : " << z << std::endl;

  y = 2;
  std::cout << "x : " << x << " y : " << y << " z : " << z << std::endl;

  z = 3;
  std::cout << "x : " << x << " y : " << y << " z : " << z << std::endl;
}
```

```
x : 1 y : 1 z : 1
x : 2 y : 2 z : 2
x : 3 y : 3 z : 3
```
- 참조자는 Type& 인데 y가 int&니까 y의 참조자 Type은 int&&가 되어야하지 않을까?
  - 참조자의 참조자라는 말의 의미를 생각해보자.
  - 별명의 별명을 굳이 만들 필요가 있는가 ?
  - C++ 문법 상 실제로 참조자의 참조자를 만드는 것은 금지되어 있다.
  - 즉, int&z = y;는 **x의 참조자를 선언해라와 같은 의미가 되서 z역시 x의 참조자가 되는 것일 뿐이다.**

# Reference가 편한 이유
```cpp
scanf("%d", &user_input);

std::cin >> user_input;
```
- 참조자를 사용하게 되면 불필요한 &와 *가 필요 없으므로 코드를 훨씬 간결하게 나타낼 수 있다는 장점이있다.
- cin은 reference로 user_input을 받는다.

# 상수에 대한 Reference
```cpp
#include <iostream>

int main() {
    int &ref = 4;

    std::cout << ref << std::endl;
}
```

```
error C2440: 'initializing' : cannot convert from 'int' to 'int &'
```
- 상수 값 자체는 리터럴이므로 에러가 난다.
- 리터럴의 값을 바꾸는 행위는 말도 안되는 것이다.
- Cpp 문법 상 상수 리터럴을 일반적으로 레퍼런스가 참조하는 것은 불가능하게 되어있다.

```cpp
const int &ref = 4;
int a = ref;
```
- 대신 상수 참조자로 선언한다면 리터럴도 참조할 수 있다.
- a = 4;와 동일한 문장으로 처리할 수 있게 된다.

# Ref's Array와 Array's Ref
- (명심)***레퍼런스는 반드시 정의와 함께 초기화해줘야한다.***
```cpp
int a, b;
int& arr[2] = {a, b};
```
- 컴파일 오류난다. (illegal error)
- arr[0]은 a를, arr[1]은 b를 의미하는 것으로 만들면 안되나 ?
  - arr[1] = *(arr + 1)로 바뀌어 처리할 수 있다.
  - arr은 주소값으로 변환이 되어야 처리할 수 있다.
  - **주소값이 존재한다는 의미는 해당 원소가 메모리상에 존재한다**라는 의미와 같다.
  - Reference는 특별한 경우가 아닌 이상 메모리 상 공간을 차지하지 않는다.
  - 이러한 모순 때문에 레퍼런스들의 배열을 정의하는 것은 언어 차원에서 금지하고 있다.

```cpp
#include <iostream>

int main() {
    int arr[3] = {1, 2, 3};
    int (&ref)[3] = arr;

    ref[0] = 2;
    ref[1] = 3;
    ref[2] = 1;

    std::cout << arr[0] << arr[1] << arr[2] << std::endl;

    return 0;
}
```
- 배열들의 레퍼런스는 불가능하지 않다.
- ref가 arr을 참조하게 만든 것이다.
  - ref[0] ~ ref[2]는 arr[0] ~ arr[2]의 reference가 된다.
- 포인터와 다르게 배열의 레퍼런스의 경우 참조하기 위해서는 **반드시 배열의 크기를 명시해야한다.**

# Reference를 Return하는 Function
```cpp
int function() {
    int a = 2;
    return a;
}

int main() {
    int b = function();
    return 0;
}
```
- 주목할 부분은 int b = function();이다.
  - a라는 변수의 값이 b에 **복사**된다.
  - function이 종료되면 a는 메모리상에서 사라진다.
  - 따라서 main안에서는 a를 만날일이 없어진다.

```cpp
int& function() {
    int a = 2;
    return a;
}

int main() {
    int b = function(); // a라는 ref가 사라진다..
    return 0;
}
```
- 컴파일 시 경고가 나오고, 런타임 오류가 발생하게 된다.
- int& 이므로 참조자를 Return하게 된다.
- 문제는 function안에 정으된 a는 리턴과 함께 사라진다는 점이다.
  - function이 refrence를 return하면서 원래 참조하던 변수가 사라져버려서 오류가 난것이다.
  - 본체는 사라졌지만 별명만 남아있는 상황..
  - 레퍼런스는 있는데 참조하던 것이 사라진 레퍼런스를 **Dangling reference**라고 부른다.
  - 따라서 Reference를 return하는 함수에서 **지역 변수의 레퍼런스를 리턴하지 않도록 조심해야한다.**

# 외부 변수의 Reference를 Return
```cpp
int& function(int& a) {
    a = 5
    return a;
}

int main() {
    int b = 2;
    int c = function(b);
    return 0;
}
```
- 인자로 받은 레퍼런스를 그대로 리턴하고 있다.
- function(b)를 실행한 시점에서 a는 main의 b를 참조하고 있다. 따라서 Function이 리턴한 참조자는 아직 살아있는 변수인 b를 계속 참조한다.
  - c에 b의 값을 대입하는 것과 동일한 문장인 것이다.

- C언어에서 엄청나게 큰 구조체가 있을 때 해당 구조체 변수를 그냥 리턴하면 **전체 복사가 발생**하여 시간이 오래걸린다.
  - 해당 구조체를 가리키는 포인터를 리턴하면 그냥 포인터 주소 한번 복사로 매우 빠르게 끝난다.
  - 마찬가지로 Reference를 Return하면, **Reference가 참조하는 타입의 크기와 상관없이 딱 한번의 주소값 복사로 전달이 끝나게 된다.**(효율적)

# 참조자가 아닌 값을 리턴하는 함수를 참조로 받기
```cpp
int function() {
    int a = 2;
    return a;
}

int main() {
    int& c = function();
    return 0;
}
```
- function 함수의 리턴값을 참조할 수 없다는 의미로 컴파일 오류가 난다.
- 이전과 마찬가지로 함수의 Return 값은 문장이 끝나고 바로 사라지는 값이어서 참조자를 만들게 되면 Dangling Reference가 되어 버리기 때문이다.

```cpp
int function() {
    int a = 2;
    return a;
}

int main() {
    const int& c = function();
    std::cout << "c : " << c << std::endl;
    return 0;
}
```

```
c : 5
```
- function()의 리턴값을 똑같이 참조자라 받았다.
- const를 쓰니 문제가 없이 컴파일이 되고, 실행도 잘된다.
- 원칙상 함수의 리턴값은 해당 문장이 끝나면 소멸되는 것이 정상이다.
- (예외적 상황) **상수 레퍼런스로 리턴값을 받게 되면 해당 리턴값의 생명이 연장된다.**
  - 연장되는 기간은 레퍼런스가 사라질 때 까지이다.

![image](https://user-images.githubusercontent.com/69780812/140068238-e77803a8-3b15-4201-a054-22ee83d2632f.png)