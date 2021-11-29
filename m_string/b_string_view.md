# Overview
- 문자열 읽기만 필요로 한다면 보통 const std::string& 으로 받거나 const char* 형태로 받게 된다.

```cpp
#include <iostream>
#include <string>

void* operator new(std::size_t count)
{
    std::cout << count << "bytes 할당" << std::endl;
    return malloc(count);
}

bool contains_very(const std::string& str)
{
    return str.find("very") != std::string::npos;
}

int main()
{
    std::cout << std::boolalpha << contains_very("c++ string is very easy to use")
              << std::endl;

    std::cout << contains_very("c++ string is not easy to use") << std::endl;
}
```

```
31bytes 할당
true
30bytes 할당
false
```
- const string& 의 형태 경우를 보자.
- 문제는 contains_very 함수에 문자열 리터럴을 전달하면 이는 (const char*) 인자는 string만 받을 수 있어서 암묵적으로 string 객체가 생성된다는 점이다.
  - 위 출력 결과 처럼 불필요한 메모리 할당이 발생한다.
- contains_very 함수를 const char* 형태의 인자로 바꾼다 ?
  - string을 함수에 직접 전달할 수 없고, c_str 함수를 통해 string에서 const char* 주소값을 뽑아내야된다.
  - const char* 로 변환하는 과정에서 문자열의 길이에 대한 정보를 잃어버리게 된다. 만일, 함수 내부에서 문자열 길이 정보가 필요하다면 매 번 다시 계산해야한다.
- 이러한 이유로 string&을 인자로 받는 오버로딩 하나, const char*를 이자로 받는 오버로딩 하나를 각각 준비해야하는 문제점이 있었다.
  - C++17에서 string_view가 도입되며 해결되었다.

```Cpp
#include <iostream>
#include <string>

void* operator new(std::size_t count) {
  std::cout << count << " bytes 할당 " << std::endl;
  return malloc(count);
}

// 문자열에 "very" 라는 단어가 있으면 true 를 리턴함
bool contains_very(std::string_view str) {
  return str.find("very") != std::string_view::npos;
}

int main() {
  // string_view 생성 시에는 메모리 할당이 필요 없다.
  std::cout << std::boolalpha << contains_very("c++ string is very easy to use")
            << std::endl;

  std::cout << contains_very("c++ string is not easy to use") << std::endl;

  std::string str = "some long long long long long string";
  std::cout << "--------------------" << std::endl;
  std::cout << contains_very(str) << std::endl;
}
```
- string_view는 문자열을 읽기만 하는 클래스다.
- 이때문에 string_view는 문자열을 소유하고 있지 않다.
  - 어딘가 존재하는 문자열을 참조해서 읽기만 하는 것
  - 따라서 string_view가 현재 보고 있는 문자열이 소멸되면 정의됮 ㅏㅇㄶ는 작업이 발생하게된다. (Undefined begavior)
- string_view는 **현재 읽고 있는 문자열이 소멸되지 않은 상태인지 주의해야한다.**
- 또한 읽기만한다는 특성 때문에 객체 생성 시 메모리 할당이 불필요하다.
  - 그냥 읽고 있는 문자열의 시작 주소값만 복사하면 되기 때문이다.
- string이나 const char*를 전달하더라도 메모리 할당이 발생하지 않는다.