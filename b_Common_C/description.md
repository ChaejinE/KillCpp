# Overview
- C++은 C 언어의 문법을 거의 완전하게 포함하고 있다.

# 변수 정의
- 전혀 달라진 것이 없다.
- [이름 정의할 때 Reference](https://google.github.io/styleguide/cppguide.html#Variable_Names)

# Pointer
```cpp
int arr[10];
int *parr = arr;

int i;
int *pi = &i;
```
- C와 차이가 없다.

# for
```cpp
/* 변수는 변수 사용 직전에 선언해도 된다.*/
#include <iostream>

int main() {
  int sum = 0;

  for (int i = 1; i <= 10; i++) {
    sum += i;
  }

  std::cout << "합은 : " << sum << std::endl;
  return 0;
}
```
- C++에서도 문법이 바뀌지 않았다.
- 한 가지 달라진 점은 변수의 선언이 반드시 최상단에 있을 필요가 없다는 것이다.
- **while문도 동일하다.**

# if-else
- 동일하다.

# 입력
```cpp
std:: cin >> variable;
```
- C에서는 scanf로 &를 붙였었는데 필요가 없어졌다.
  - 자료 형도 따지지 않는다.
  - 편리해졌다.

# switch 문
```cpp
switch (user_input) {
  case 1:
    cout << "Psi ! " << endl;
    break;

  case 2:
    cout << "99 살" << endl;
    break;

  case 3:
    cout << "남자" << endl;
    break;

  default:
    cout << "궁금한게 없군요~" << endl;
    break;
}
```
- 동일하다.
- continue, break 사용하는 것 동일