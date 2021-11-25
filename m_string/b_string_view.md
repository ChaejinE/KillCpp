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