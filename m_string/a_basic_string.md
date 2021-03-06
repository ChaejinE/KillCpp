# Overview
```cpp
template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT> >
class basic_string;
```
- std::string은 basic_string 이라는 클래스 템플릿의 인스턴스화 버전이다.
- 위는 basic_string의 정의이다.
- CharT 타입의 객체들을 **메모리에 연속적으로 저장**하고, 여러가지 문자열 연산들을 지원해주는 클래스이다.
  - 만약 CharT 자리에 char가 오면 우리가 생각하는 std::string이 되는 것이다.

![image](https://user-images.githubusercontent.com/69780812/143241080-d1d4c6fd-26d9-4ff9-ab36-0784a5b105f5.png)
- string 말고도 총 5가지 종류의 인스턴스화 된 문자열들이 있다.
- 일단, 대충 이런게 있구나 하고 넘어가면된다.
- 다시 이전으로 넘어가보면, Traits는 주어진 CharT 문자들에 대해 기본적 문자열 연산들을 정의해 놓은 클래스를 의미한다.
  - 기본적 문자열 연산 : 문자열 대소 비교, 문자열 길이 어떻게 잴지 등등
  - basic_string에 정의된 문자열 연산들은 사실 전부 Traits의 기본적인 문자열 연산들을 가지고 정의된다.
  - 덕분에 **문자열들을 어떻게 보관하는지에 대한 로직**과 **문자열들을 어떻게 연산하는지에 대한 로직**을 분리시킬 수 있었다.
    - 전자는 basic_string, 후자는 Traits에서 담당하게 된다.
    - 로직을 분리한 이유는 basic_string의 User에게 좀 더 자유를 부여하기 위해서 이다.
    - 예를들어 string 처럼 char가 기본 타입인 문자열에서 문자열 비교시 대소 문자 구분을 하지 않는 버전을 만들고 싶다면 처음 부터 Traits 에서 문자열들을 비교하는 부분만 살짝 바꿔주면 된다. Traits가 없었다면, 문자열을 비교하는 부분을 일일이 찾아서 고쳐야할 것이다.
    - [참고 코드](https://en.cppreference.com/w/cpp/string/char_traits)
- Traits에는 \<string>에 정의된 std::char_traits 클래스의 인스턴스화 버전을 전달한다.
  - string의 경우 char_traits\<char>를 사용하게 된다.

# 숫자들의 순위가 알파벳 보다 낮은 문자열
- Traits 클래스를 오버로딩해 문자열 비교시 숫자들의 순위가 제일 낮은 문자열을 만들어보자.
  - 어떻게 활용되는지 살펴보기위해

```cpp
#include <cctype>
#include <iostream>
#include <string>

struct my_char_traits : public std::char_traits<char>
{
    static int get_real_rank(char c)
    {
        if(isdigit(c))
        {
            return c + 256;
        }

        return c;
    }

    static bool lt(char c1, char c2)
    {
        return get_real_rank(c1) < get_real_rank(c2);
    }

    static int compare(const char* s1, const char* s2, size_t n)
    {
        while(n-- != 0)
        {
            if (get_real_rank(*s1) < get_real_rank(*s2))
            {
                return -1;
            }

            if (get_real_rank(*s1) > get_real_rank(*s2))
            {
                return 1;
            }
            ++s1;
            ++s2;
        }
        return 0;
    }
};

int main()
{
    std::basic_string<char, my_char_traits> my_s1 = "1a";
    std::basic_string<char, my_char_traits> my_s2 = "a1";

    std::cout << "숫자의 우선순위가 더 낮은 문자열 : " << std::boolalpha
              << (my_s1 < my_s2) << std::endl;

    std::string s1 = "1a";
    std::string s2 = "a1";

    std::cout << "일반 문자열 : " << std::boolalpha << (s1 < s2) << std::endl;
}
```

```
숫자의 우선순위가 더 낮은 문자열 : false
일반 문자열 : true
```
- basic_string의 Traits에는 char_traits에서 제공하는 모든 멤버함수들이 구현된 클래스가 전달되어야한다. (꼭 char_traits를 상속받을 필요 없다.)
- 제일 간편한건 char_traits를 상속받고, 필요한 부분만 새로 구현하면 된다.
- char_traits에 정의되는 함수들은 모두 static 함수들이다.
  - char_traits 존재 이유를 생각해보면, Traits는 문자와 문자열들 간에 간단한 연산을 제공해주는 클래스이므로 굳이 데이터를 저장할 필요가 없다.
    - 이런 것을 **Stateless**하다고 한다.
- 일반적 char를 다루는 char_traits\<char> 에서 바꿔줘야할 부분은 대소 비교하는 부분 뿐이다.
  - 따라서 문자들 간 크기를 비교하는 lt 함수와 길이 n의 문자열의 크기를 비교하는 compare 함수를 새로 정의해줘야했다.
- get_real_rank 함수 : 문자를 받아서 숫자면 256을 더해 순위를 매우 떨어뜨린다.
  - 따라서, 숫자들이 모든 문자들 뒤에 오게될 것이다.
- 따라서 실제로 my_s1이 my_s2보다 뒤에 온다고 나타나게 된다.
  - my_s1 > my_s2
- string의 경우에는 s1이 s2 앞에 나올 것이다.

# 짧은 문자열 최적화 (SSO)
- 메모리를 할당하는 작업은 시간을 꽤나 잡아먹는다.
- basic_string이 저장하는 문자열의 길이는 천차 만별이다.
  - 한 두 문자 정도의 짧은 문자열 일 수도, 수십만 바이트의 거대 문자열일 수도 있다.
- 문제는 **거대 문자열은 매우 드물게 저장되는데 비해 길이가 짧은 문자열들은 굉장히 많이 생성되고 소멸된다는 점**이다.
- 매번 모든 문자열을 동적으로 메모리를 할당 받는다고 한다면, 길이가 짧은 문자열을 여러번 할당한다면 매번 메모리 할당이 이루어져야 하므로 굉장히 비효율적이다.
- 따라서 basic_string 의 제작자들은 **짧은 길이 문자열의 경우** 따로 문자 데이터를 위한 메모리를 할당하는 대신 그냥 객체 자체에 저장해 버린다.
  - 이를 **짧은 문자열 최적화**, Short String Optimization이라고 부른다.

```cpp
#include <iostream>
#include <string>

void* operator new(std::size_t count)
{
    std::cout << count << " bytes 할당 " << std::endl;
    return malloc(count);
}

int main()
{
    std::cout << "s1 생성 --- " << std::endl;
    std::string s1 = "this is a pretty long sentence!!!";
    std::cout << "s1 size : " << sizeof(s1) << std::endl;

    std::cout << "s2 생성 --- " << std::endl;
    std::string s2 = "short sentence";
    std::cout << "s2 size : " << sizeof(s2) << std::endl;
}
```

```
s1 생성 --- 
34 bytes 할당 
s1 size : 32
s2 생성 --- 
s2 size : 32
```
- 메모리가 할당되는지 안되는지 확인하기 위해 new 연산자를 새로 정의한 것이다.
  - **new의 경우 위와같이 클래스 외부 함수로 정의하게 되면 모든 new 연산자들이 위 함수를 사용하게 된다.**
  - 반면, 클래스 내 멤버 함수로 new를 정의하면 **해당 객체를 new로 생성할 때 해당 new 함수가 호출된다.**
- 결과는 긴 문자열 s1 생성 시 메모리 할당이 발생했고, 짧은 문자열인 s2 의 경우 메모리 할당이 발생하지 않았다.
- 문자열 객체의 크기를 확인했을 때 32바이트로 꽤나 크다.
- 만일 단순하게 문자열 라이브러리를 구현하면 문자열 길이를 저장한 변수 하나, 할당한 메모리 공간 크기 저장을 위한 변수 하나, 메모리 포인터 하나로 해 총 12바이트로 만들 수도 있을 것이다.
  - 하지만 라이브러리 제작자들은 메모리 사용량을 조금 희생한 대신 성능 향상을 꾀했다.
- 라이브러리마다 어느 길이 문자열 부터 따로 메모리 할당할지는 다르다.
  - 하지만 주류 C++ 라이브러리, gcc의 libstdc++, clang의 libc++ 들은 어떤 방식이든 SSO를 사용하고 있다.
- basic_string 구현에서는 Copy On Write 기법도 사용되었다.
  - 문자열 복사 시 바로 복사하는 것이 아닌 복사된 문자열이 바뀔 때 비로소 복사를 수행하는 방식이다. (C++ 11에서 개정된 표준에따라 불가능해 졌다.)

# 문자열 리터럴 정의
```cpp
#include <iostream>
#include <string>

int main()
{
    const char* s = "hello";

    char s2[] = "hello";

    auto str = "hello";

    std::string str = "hello";
}
```
- C++의 경우 auto str으로하면, str은 string으로 정의되지 않는다.
  - C와 마찬가지로 str 의 타입은 **const char\***로 정의된다.
- 문자열을 꼭 만들어야한다면 std::string 처럼 타입을 꼭 명시해줘야한다.
  - C++14 에 이 문제를 해결할 방법이 나왔다.

```cpp
auto str = "hello"s
```
- 위와 같이 ""뒤에 s를 붙여주면 auto가 string으로 추론된다.
  - **리터럴 연산자**

```cpp
std::string operator"" s(const char *str, std::size_t len);
```
- 참고로 리터럴 연산자는 위처럼 정의되어 있다.
- "hello"s 는 컴파일 과정에서 operator""s("hello", 5);로 바뀌게 된다.

```cpp
#include <iostream>
#include <string>
using namespace std::literals;

int main()
{
    const char* s = "hello";

    char s2[] = "hello";

    auto str = "hello";

    std::string st = "hello";

    auto s3 = "hello"s;
    std::cout << "s1의 길이 : " << s3.size() << std::endl;
}
```

```
s1의 길이 : 5
```
- 리터럴 연산자를 사용하기 위해서는 std::string_literals 네임스페이스를 사용해야한다.
- 제대로 string으로 auto가 추론된 것을 확인할 수 있다.
- 이외의 리터럴 정의 방법을 알아보자.

```cpp
std::string str = "hello"; // char[]
std::wstring wstr = L"hello"; // wchar_t[]
```
- wchar_t 문자열을 만들고 싶다면 앞에 그냥 L을 붙여주면 된다.
  - 컴파일러가 알아서 wchar_t 배열을 만들어 준다.
- 이외에 [여기](https://en.cppreference.com/w/cpp/language/string_literal)를 참고하자.

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string str = R"(asdfasdf//   어떤것들이 와도된다. \n\n)";

    std::cout << str;
}
```

```
asdfasdf//   어떤것들이 와도된다. \n\n
```
- **Raw string literal** 이라는 C++ 11의 유용한 기능이있다.
- R"()" 안에 오는 문자들은 모두 문자 그대로 char 배열 안에 들어가게 된다.
  - 입력 안되던 것들이 모두 다된다.
  - 한가지 문제는 **)"를 문자열 안에 넣을 수 없다는 점이다.**

```cpp
std::string str = R"foo(
)"; <-- 무시됨
)foo";
```
- 구분 문자를 추가함으로써 해결할 수 있다.

```
R"/* delimiter */( /* 문자열 */ )/* delimiter */"
```
- 문법을 정확히 살펴보자면 내가 원하는 문자열이 와도 되는데, delimiter와 뒤의 delimiter는 같아야한다.

# C++에서 한글 다루기
- 한글을 다루는 일은 생각보다 복잡하다.
- 대부분 초창기 컴퓨터는 영미권 국가에서 사용하여 문자를 표현하는 데 1바이트로도 충분했다.
- 점차, 전 세계적으로 사용이 확대되며 각국의 문자를 나타내는 데에는 한계를 느꼈다.
- 전 세계 문자들을 컴퓨터로 표현할 수 있도록 설계된 표준이 **Unicode**이다.
  - 유니코드는 모든 문자들에 고유의 값을 부여하게 된다.
  - 0xAC01, 0x7F 등
- 모든 문자들을 4바이트 씩 지정해서 표현하는 것은 매우 비효율적이다. 전체 텍스트가 모두 영어라면 영문자는 값의 범위가 0부터 127 사이 이기 때문에 1바이트 문자만 사용해도 전부 표현이 가능하기 때문이다.
  - 그래서 등장한 것이 **Encoding 방식**이다.
- 인코딩 방식에 따라 문자를 표현하기 위해 동일하게 4바이트를 사용하는 대신 어떤 문자는 1바이트, 어떤 건 2바이트 등등의 길이로 저장하게된다.
  - UTF-8 : 문자를 최소 1부터 최대 4바이트로 표현한다.
    - 문자마다 길이가 다르다.
  - UTF-16 : 문자를 2 혹은 4바이트로 표현한다.
  - UTF-32 : 문자를 4바이트로 표현한다.

```cpp
#include <iostream>
#include <string>

int main()
{
    std::u32string u32_str = U"이건 UTF-32 문자열 입니다";
    std::cout << u32_str.size() << std::endl;
}
```

```
17
```
- u32string은 C++에서 UTF-32로 인코딩 된 문자열을 보관하는 타입이다.
- U는 해당 문자열을 리터럴이 UTF-32로 인코딩하라는 의미이다.
  - UTF-32는 모든 문자들을 동일하게 4바이트로 나타내서 문자열 원소 개수와 실제 문자열의 크기가 일치한다.

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string str = u8"이건 UTF-8 문자열 입니다";
    std::cout << str.size() << std::endl;
}
```

```
32
```
- 문자열의 길이는 16인데, 실제 출력된 것은 32가 나왔다.
- 이는 UTF-8인코딩 방식이 문자들에 최소 1 바이트 부터 최대 4바이트 까지 지정하기 때문이다.
- 최소 단위가 1바이트 이므로 UTF-8의 인코딩 방식의 문자열은 char 원소들을 보고나하는데, 어떤 문자들은 char 1개로 충분하고, 어떤 원소는 2개, 3개, 4개까지도 필요하게된다.

![image](https://user-images.githubusercontent.com/69780812/143460328-b7358d12-2fea-40e4-a995-3efed5bf91c7.png)
- 위 표는 유니코드 별로 어떻게 UTF-8로 인코딩 되는지 설명하고 있다.
- 예를들어 0~0x7F 까지 문자들은 1바이트, 그 다음 0x80부터 0x7FF 까지 문자들은 2바이트, 0x800 ~ 0xFFFF까지는 3바이트, 그리고 나머지가 4바이트로 지정된다.
  - 한글의 경우 0XAC00 ~ 0XD7AF까지 걸쳐있어 전부 3바이트로 표현된다.
- "이건 UTF-8 문자열 입니다"에는 한글이 8개, 영어 알파벳, 공백 문자, - 가 8개 있다. 한글은 3바이트, 나머지 애들은 1바이트로 표현이 되므로 3x8+1x8 = 32가 되는 것이다. 그래서 32개의 char가 필요하다.
- std::string은 문자열이 어떤 인코딩으로 이루어져있는지는 관심이 없으며 단순히 Char의 나열로 이루어져있다고 생각한다. 그래서 str.size()시, 문자열의 실제 길이가 아니라 그냥 char가 몇 개가 있는지 알려준다.

```cpp
std::cout << str[1]
```
- '건' 이 나와야할 것 같지만 실제로 이상한 결과가 나온다.
  - string이 각각의 문자를 구분하지 못해 불편함이 이만저만이 아니다.
  - 아마 출력된 결과는 이의 UTF-8의 인코딩의 두번째 바이트일 것이다.
- 영문자와 섞인 경우, 한글은 3바이트로 해석되지만 영어는 1바이트이므로 반복자를 통해 문자들을 순차적으로 뽑아내기가 힘들다.

```cpp
#include <iostream>
#include <string>

int main()
{
    std::u16string u16_str = u"이건 UTF-16 문자열 입니다";
    std::cout << u16_str.size() << std::endl;
}
```

```
17
```
- UTF-16 인코딩은 최소 단위가 2바이트이다. 따라서 UTF-16으로 인코딩 된 문자열을 저장하는 클래스 u16string도 원소 타입이 2바이트이다.
  - char16_t
- 유니코드에서 0 ~ D7FF 번 까지, 그리고 E000 ~ FFFF 까지의 문자들을 2바이트로 인코딩한다.
  - FFFF 이상이면 4바이트로 인코딩
- 대부분의 문자들이 2바이트로 인코딩 된다. 이모지나 이집트상형문자 같은 경우만 유니코드 상 높은 번호로 매핑되어 4바이트로 인코딩 된다.
- 일반적인 문자들만 수록되어 있는 텍스트를 다룬다면 u16string을 사용하는 것만큼 좋은 것이 없다. 거의 대부분 문자들이 2바이트로 인코딩 되므로 모든 문자들이 원소 1개 만큼씩을 사용한다.

```cpp
#include <iostream>
#include <string>

int main()
{
    std::u16string u16_str = u"안녕하세요 채진이 집들이 오신 것을 환영합니다";
    std::string jaum[] = {"ㄱ", "ㄲ", "ㄴ", "ㄷ", "ㄸ", "ㄹ", "ㅁ",
                        "ㅂ", "ㅃ", "ㅅ", "ㅆ", "ㅇ", "ㅈ", "ㅉ",
                        "ㅊ", "ㅋ", "ㅌ", "ㅍ", "ㅎ"};
    
    for (char16_t c : u16_str)
    {
        if (!(0xAC00 <= c && c <= 0xD7A3))
            continue; // 유니코드 상에서 한글의 범위
        
        // 한글은 AC00부터 시작해 한 초성 당 총 0x24C개 씩있다.
        int offset = c - 0XAC00;
        int jaum_offset = offset / 0x24C;

        std::cout << jaum[jaum_offset];
    }
}
```

```
ㅇㄴㅎㅅㅇㅊㅈㅇㅈㄷㅇㅇㅅㄱㅇㅎㅇㅎㄴㄷ
```
- 한글의 초성만 분리해내는 코드를 작성한 것이다.
- 위와 같이 잘 분리가 된다.
- 한글은 유니코드 상 한 초성 당 588 개 씩 있다.
  - 가, 각, 갂, 간 순으로 진행

- UTF-16 역시 때론 4바이트로 문자를 인코딩 해야하므로 i번째 문자를 str[i]와 같이 접근할 수 있는 것은 아니다.
- UTF-32 인코딩으로 바꿔버리는 것이 간단하겠지만 이는 메모리 사용량을 매우 증가시킨다.
  - 다행히도 UTF8-CPP 라는 C++ 에서 여러 방식으로 인코딩된 문자열을 쉽게 다룰 수 있게 도와주는 라이브러리가 있다. (표준 라이브러리 아니다.)