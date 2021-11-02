# Code Analysis
```cpp
#include <iostream>

int main() {
    std::cout << "Hello, World" << std::endl
    return 0;
}
```
## iostream
```cpp
#include <iostream>
```
- iostream이라는 header file을 include 하고있다.
- 이 hear file은 C++ 표준 입출력에 필요한 것들을 포함하고 있다.
  - std::cout
  - std::endl
  - C의 stdio.h 와 비슷하다.
  - Cpp 에서는 h가 붙지 않는다.
## main
```cpp
int main()
```
- 프로그램이 실행될 때 가장 먼저 실행되는 함수
## code
```cpp
std::cout << "Hello, World" << std::endl
return 0;
```
- std::cout : 화면에 무언가 출력시켜준다.
- std::endl : 한줄 개행, 한 줄 엔터쳐서 나타내라는 표시

# namespace
## std의 정체
- std는 C++ 표준라이브러리의 모든 함수, 객체 등이 정의된 이름 공간이다.
- namespace란 어떤 정의된 객체에 대해 **어디 소속**인지 지정해주는 것과 동일하다.
  - 코드 길이가 늘어나면 중복된 이름의 함수가 많아진다.
  - 이를 구분하기 위해 소속된 이름공간이 다르면 다른 것으로 취급할 수 있게 된다.

```cpp
std::cout
```
- std라는 namepace에 정의되어있는 cout을 의미한다.
  - 그냥 cout하면 컴파일러가 못찾는다.

## namespace definition way
```cpp
namespace header1 {
    int foo();
    void bar():
}

namespace header2 {
    int foo();
    void bar();
}
```
- header1에 있는 foo, bar와 header2에 있는 foo, bar는 다른 것이다.
- 자기 자신이 포함되어있는 namespace에서는 앞에 굳이 namespace를 명시하지 않아도 된다.

```cpp
#include "header1.h"

namespace header1 {
    int func() {
        foo();
    }
}
```
- header1 namespace 안에서 foo를 부르면 알아서 header1::foo()를 호출하게 된다.

```cpp
#include "header1.h"
#include "header2.h"

namespace header1 {
    int func() {
        foo();
        header2::foo();
    }
}
```
- header1 namespace안에서 다른 namespace의 함수를 호출할 수 있다.

```cpp
#include "header1.h"
#include "header2.h"

int func() {
    header1::foo();
}
```
- 어떤 namespace에도 소속되지 않는 경우 명시적으로 namespace를 지정해야한다.

```cpp
#include "header1.h"
#include "header2.h"

using header1::foo;

int func() {
    foo();
}
```
- foo를 여러번 호출하는 경우 매번 namespace 명시하기 귀찮다.
- 위와 같이 header1이란 namspace에 들어있는 foo만 쓰겠다고 명시할 수 있다.

```cpp
#include "header1.h"
#include "header2.h"

using namespace header1;

int func() {
    foo();
}
```
- header1 namespace안의 모든 것들을 header1:: 없이 쓰려면 위와 같이 선언한다.

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "Hello World" << endl;
    return 0;
}
```
- std를 붙이기 귀찮으면 그냥 저렇게 선언하면 된다.
- 보통 위처럼 하는 것은 **권장되지 않는다.**
  - 거대한 표준 라이브러리이므로 std에 이름이 겹치는 함수를 만들면 오류가 발생한다.
  - std라이브러리에서 구현된 것과 이름을 겹치게 사용해버리는 경우에도 오류가 발생한다.
  - 오류가 발생하면 고치느라 시간이 오래걸린다.
  - 그러므로 명시해주는 것이 좋다.

```cpp
#include <iostream>

namespace {
    int OnlyInThisFile() {}

    int only_in_this_file = 0;
}

int main() {
    OnlyInThisFile();
    only_in_this_file = 3;
}
```
- 이름이 없는 namespace는 해당 파일 안에서만 접근할 수 있다.
  - static 키워드를 사용한 것과 같은 효과