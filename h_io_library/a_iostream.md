# C++ I/O Library
![image](https://user-images.githubusercontent.com/69780812/141624789-24e1b46b-b5d7-4965-b573-e498291d21cb.png)
- C++ 입출력 라이브러리는 위와 같은 클래스들로 구성되어 있다.
- 모든 입출력 클래스는 ios_base를 기반 클래스로 한다.
  - ios_base : 많은 일은 하지 않고 stream의 입출력 형싱 관련 데이터를 처리
    - 예를들어 실수형 출력 시 정밀도를 어떤 식으로 할지, 정수형 출력 시 10진수로 할지, 16진수로 할지 등을 이 클래스에서 처리
  - ios : stream buffer 초기화
    - stream buffer : 데이터를 내보거나 받아들이기 전에 임시로 저장하는 곳
    - 사용자가 1byte 씩 읽는다고 했을 때, 실제로 프로그램은 1Byte씩 읽지 않는다. 512Byte 이런 식으로 한 뭉터기로 읽어서 Stream buffer에 저장하여 사용자 요청이 있을 때마다 1Byte 씩 꺼내는 것이다.
    - 만일 Buffer를 다 읽으면 다시 하드에서 512Byte를 읽는 식으로 작동한다.
    - 이는 하드에서 읽어오는 작업이 매우 느리기 때문에 1Byte씩 읽을 때의 딜레이 발생을 줄이기 위해서다.
    - 1문자 출력 시 하드에 바로 쓰는 것이 아니라 일단 버퍼에 보낸 후 어느 정도 모인 뒤 출력하게 되는 것이다.
    - 이 외에도 현재 입출력 작업의 상태를 처리한다.
      - 파일을 읽다가 끝에 도달했는지 안했는지 확인하기 위해 eof 함수를 호출한다.
      - 현재 입출력이 잘 수행할 수 있는지 확인하기 위해 good 함수를 호출한다.

# istream
- ios_base, ios 클래스들은 입출력 작업을 위해 바탕을 깔아주는 클래스다.
- isteam은 실제 입력을 수행하는 Class다.

```cpp
std::cin >> a;
```
- operator>>가 istream 클래스에 정의있는 연산자 인 것이다.
- cin은 istream 클래스의 객체중 하나다.
- 어떤 Type이든 처리 가능 한것은 >> 연산자 함수에서 기본 타입에대한 정의가 오버로딩되어 있기 때문이다.

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string s;

    // ABCD EFGH IJKL 입력
    while (true)
    {
        std::cin >> s;
        std::cout << "word : " << s << std::endl;
    }

    return 0;
}
```

```
ABCD EFGH IJKL
word : ABCD
word : EFGH
word : IJKL
```
- operator>>의 또다른 특징으로는 모든 공백문자 (띄어쓰기, 엔터, 탭 등)을 입력하면 무시해버린다는 점이다.

```cpp
#include <iostream>
#include <string>

int main()
{
    int n;

    // c 문자 입력
    while (true)
    {
        std::cin >> n;
        std::cout << "num : " << n << std::endl;
        if (n == 0)
            break;
    }

    return 0;
}
```

```
1
num : 1
2
num : 2
3
num : 3
c
num : 0
```
- c를 입력했는데 이상하게 0이된다.
- 무한루프에 빠지는 경우도 발생한다고 한다.
  - 이는 operator>>의 처리 방식을 이해해야 한다.
- ios 클래스에서는 stream 상태를 관리한다고 했다. 이 때 4개의 정의된 플래그를 이용한다.
  - goodbit : stream 입출력 작업 가능
  - badbit : stream에 복구 불가능한 오류 발생
  - failbit : stream에 복구 가능한 오류 발생
  - eofbit : 입력 작업이 EOF 도달
  - 위와같은 상황일 때 각각의 플래그 비트들이 켜지는 것이다.
- operator>>는 위와 같은 경우 Type이 맞지 않아 failbit가 켜지게 된다.
  - 입력값을 받지 않고 return 해버린다.
  - 이러면 buffer에 남아있는 "c\n" 문자열에 손대지 않는 것이다. 그래서 또 읽고 하는 경우가 발생한다.

```cpp
operator void*() const;
```
- ios 정의되어있는 함수인데, ios 객체를 void* 형태로 변환해 준다.
- NULL 포인터가 아닌 값을 리턴하는 조건이 failbit, badbit가 모두 off 일 때다.
  - 즉, stream에 정상적으로 입출력 작업을 수행할 수 있을 때다.
- 'c' 입력 시 operator>> 는 cin 객체의 failbit를 켜게 될테고, std::cin >> t 이후 cin이 return 되는데 return 값이 while의 조건식으로 들어가기 때문에 컴파일러는 적절한 타입 변환을 찾게된다.
  - 결국 ios object -> void* -> bool로 가는 2단 변환을 통해 while문을 잘 빠져나오게 된다.
  - NULL 포인터는 bool상 false이다.

```cpp
#include <iostream>
#include <string>

int main()
{
    int n;

    while (true)
    {   
        std::cin >> n;
        std::cout << "num : " << n << std::endl;
        if (std::cin.fail())
        {
            std::cout << "제대로 입력" << std::endl;
            std::cin.clear(); // 플래그를 초기화
            std::cin.ignore(100, '\n'); // 개행 문자가 나올 때 까지 무시
        }
        if (n == 1) break;
    }

    return 0;
}
```

```
a
num : 0
제대로 입력
x
num : 0
제대로 입력
d
num : 0
제대로 입력
2
num : 2
1
num : 1
```
- 위와 같이 해결할 수 있겠다.
- fail 함수는 ios에 정의되어 있다.
  - failbit가 true거나 badbit가 true면 true를 return 한다.
  - 위의 예에서는 failbit가 true이므로 fail() 함수가 true를 return 하는 것이다.
- clear() 역시 ios에 정의되어 있으며 인자를 주지 않을 경우 플래그를 goodbit로 초기화 시킨다.
  - fail() 상태를 지울 수 있는 것이다.
- ignore() 함수는 istream에 정의되어 있으며 최대 첫번때 인자(위에서 100)만큼 두번쨰 인자가 나올 때까지 (위에서 '\n') 두 번쨰 인자를 포함하여 버퍼에서 무시한다.
  - 최대 100자 까지 개행 문제가 나올 때까지 무시할 수 있게 되는 것이다.
  - this is a very bad input을 집어넣으면 buffer에는 "this is a very input\n"이 들어가있고 ignore함수를 통해 최대 100자까지 개행문자가 나올 때까지 무시하는 것이다.
  - 100자 이상 넣으면 ignore 함수가 여러번 호출되는 것을 확인할 수 있다.

# fomat flag
- ios_base 클래스에서 stream의 입출력 형식을 바꿀 수 있다.
  - 10진수 처리 -> 16진수 처리

```cpp
#include <string>
#include <iostream>

int main()
{
    int t;

    while (true)
    {
        std::cin.setf(std::ios_base::hex, std::ios_base::basefield);
        std::cin >> t;
        std::cout << "t : " << t << std::endl;

        if (std::cin.fail())
        {
            std::cout << "제대로 입력" << std::endl;
            std::cin.clear();
        }

        if (t == 0) break;
    }
}
```

```
ff
t : 255
0xFF
t : 255
123
t : 291
ABCDE
t : 703710
```
- 16진수 읿력을 잘 받는다.
- 출력형식은 바꾸지 않아 10진수로 출력된다.
- 위와 같은 기능이 가능한 것은 setf() 함수 덕분이다.

```cpp
std::cin.setf(ios_base::hex, ios_base::basefield);
```
- setf 함수 버전은 2개가 있다.
  - 하나는 인자를 1개만 : 인자로 준 형식 플래그를 적용
  - 다른 하나는 인자를 2개 받는 다. : 두 번째 인자(basefield)의 내용을 초기화하고 첫 번째 인자(hex)를 적용
- 위의 경우 수를 처리하는 방식은 1가지 진수만 한 번에 처리할 수 있으므로 몇 진법으로 수를 처리할지 보관하는 basefield의 값을 초기화 한후 hex flag를 적용시킨 것이다.
- 16진법 처리 함수를 만들어도 되지만 사용자자 0x를 붙일 수도 있고 안붙일 수 있고 여러 예외 경우의 수가 많으므로 마음편하게 IO라이브러리에서 지원하는 방식을 사용하는게 좋다.

```cpp
#include <string>
#include <iostream>

int main()
{
    int t;

    while (true)
    {
        std::cin >> std::hex >> t;
        std::cout << "t : " << t << std::endl;

        if (std::cin.fail())
        {
            std::cout << "제대로 입력" << std::endl;
            std::cin.clear();
            std::cin.ignore(100, '\n');
        }

        if (t == 0) break;
    }
}
```

```
ff
t : 255
0xFF
t : 255
123
t : 291
ABCDE
t : 703710
0
t : 0
```
- 위와 같은 방식으로도 16진수를 잘 입력 받을 수 있다.
- cin에서 수를 받는 방식을 바로 바꿔버린 것이다.
- std::hex 와 같이 스트림을 조작하여 입력 혹은 출력방식을 바꿔주는 함수를 **조작자**라고 부른다.
  - hex is function
  - format flag hex(ios_base에 선언)와 std::hex(ios에 정의)는 다른 것이다.
  - foramt flag는 단순한 상수 값이다.

```cpp
std::ios_base& hex(std::ios_base& str);
```
- 조작자 hex는 ios_base 객체를 reference로 받고, 그 객체를 Return 하도록 정의되어 있다.

```cpp
istream& operator>>(ios_base& (*pf)(ios_base&));
```
- operator>>에서 조작자를 받는 다면 많은 일을 하는 것이 아니라 단순히 pf(*this)를 호출하게 되는 것이다.

```cpp
str.setf(std::ios_base::hex, std::ios_base::basefield)
```
- hex 함수가 호출되더라도 단순히 위를 수행해주는 것이다.
- 위 처럼 setf를 사용하지 않고도 간단히 조작자를 사용하면 훨씬 쉽게 입력 형식을 바꿀 수 있게 된다.
---
- 이 외에도 많은 조작자들이 있다.
- boolalpha : ture나 false를 1, 0으로 처리하는 대신 문자열 그대로 입력 받는다.
- left, right : 출력 형식으로 왼쪽 혹은 오른쪽으로 정렬시킨다.
- std::endl : ostream(출력을 담당)에 정의되어있는 조작자로 한 줄 개행출력, buffer를 모두 내보내는 flush 역할도 수행한다.
- 문자 1개를 내보낸다고 해서 화면에 바로 출력되는 것이 아니라 buffer에 모은 다음 어느정도 쌓이면 비로소 출력한다.
- 하지만 정해진 시간에 딱딱 맞춰서 화면에 출력해야 한다면 ?
  - buffer에 저장할 필요ㅇ벗이 화면에 바로 내보내면된다.
  - **flush**함수
  - std::endl은 stream에 "\n" 출력할 뿐아니라 flush를 수행해주는 것을 알 수 있다.
---

# Stream Buffer에 대하여
- 모든 입출력 객체들은 대응하는 Stream Object를 가지고 있게 된다.
- 따라서 C++의 입출력 라이브러리에는 이에 대응되는 stream buffer Class도 존재한다.
  - **streambuf** Class 이다.
- stream : 문자들의 순차적인 나열이라고 쉽게 보면된다.
  - 화면에 입력하는 문자, 하드디스크에서 파일을 읽는 것, TCP/IP 통신도 모두 Stream을 통해 이루어진다.

![image](https://user-images.githubusercontent.com/69780812/141642931-61b9fe95-3f58-464e-a139-3aa57461b1a0.png)
- streambuf 클래스에서 stream을 어떤식으로 추상화하는지 보여주는 그림이다.
- streambuf는 stream에서 입력을 받던지, 출력을 하던지, 혹은 입력과 출력을 동시에(파일 입출력에서 rw옵션을 떠올려보면된다.) 수행할 수도 있다.
- stream의 상태를 나타내기 위해 **세 개의 포인터를 정의**하고 있다.
  - buffer 시작 부분 가리키는 시작 포인터
  - 다음으로 읽을 문자를 가리키고 있는 포인터(stream 위치 지정자)
  - buffer의 끝 부분을 가리키는 끝포인터
- streambuf 클래스에서는 입력 버퍼와 출력 버퍼를 구분해서 부른다.
  - Input buffer : get area
  - Output buffer : put area
  - 그래서 포인터도 g, p를 붙여서 표현한다.

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string s;
    std::cin >> s;

    // 위치 지정자 한 칸 옮기고, 그 다음 문자를 훔쳐본다. (이 때는 움직이지 않는다.)
    char peek = std::cin.rdbuf()->snextc();
    if (std::cin.fail()) std::cout << "Fail";

    std::cout << "두번쨰 단어 맨 앞글자 : " << peek << std::endl;
    std::cin >> s;
    std::cout << "다시 읽기 : " << s << std::endl;

    return 0;
}
```

```
hello world
두번쨰 단어 맨 앞글자 : w
다시 읽기 : world
```
- 입력 객체 cin의 rdbuf를 호출하게되면 cin 객체가 **입력을 수행하고 있던 streambuf 객체를 가리키는 포인터를 return**하게 된다.
- cin은 istream 객체 이므로 오직 입력만 수행하고 있으므로 streambuf 객체는 오직 get area만 있다.
- snextc() : Stream 위치 지정자를 한 칸 전진 시키고 그 자리에 해당하는 문자를 엿본다. (Read 하는 것이 아니다.)
  - Read와 엿보는 것의 차이점 : streambuf 객체에서 Read 하는 것은 stream 위치 지정자를 한 칸 전진시켜서 다음 읽기 때 다음 문자를 읽을 수 있도록 준비하는 것이다. 하지만 엿보는 것은 해당 문자를 읽고도 스트림 위치 지정자를 움직이지 않는다.
- peek의 결과가 w가 나오는 이유를 알아보자.

![image](https://user-images.githubusercontent.com/69780812/141643164-4082aed8-f413-4cd7-8a70-a05d699d6c99.png)
- hellow world를 치면 std::cin >> s 한 이후의 streambuf 상태이다.
- 문자열의 경우 공백 문자가 나오기 전까지 읽어들이므로 위와 같은 상태가 된다.

![image](https://user-images.githubusercontent.com/69780812/141643325-b14694fb-e1f8-459a-b09b-123735b5f2d0.png)
- snextc() 함수를 호출했을 때이다.
- 위 함수가 스트림 위치 지정자를 한칸 전진시키므로 공백 문자를 띄어넘어 'w'를 가리키게 되며 이를 return 한다.
- snext()는 stream 위치 지정자를 건드리지는 않기 때문에 다음에 "world" 전체가 나오는 것이다.

---
- streambuf에는 snextc 함수 말고도 많은 함수들이 정의 되어있다.
  - 직접 사용할 일을 많이 없겠지만, C++ 입출력 라이브러리는 Stream 버퍼도 추상화해서 클래스로 만들었다는 것 정도로 이해하면 좋을 것 같다.
- streambuf를 도입한 중요한 이유는 1Byte짜리 문자 뿐만 아니라 wchar_t(다중 바이트 문자들, Such as UTF-8)에 대한 처리도 용이하게 하기 위함이다.
- C++에서는 읽어들일 Byte에 대한 처리를 Stream buffer 객체 자체에서 수행하도록 하여 사용자가 입출력 처리를 이요한느데 훨씬 용이하게 했다.
