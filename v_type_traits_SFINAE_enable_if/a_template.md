# Overview
- type 관련 연산을 위한 템플릿 메타함수들을 제공해주는 type_traits 라이브러리에 대해 알아본다.

# 공포의 Template
```cpp
template <class _CharT, class _Traits, class _Yp, class _Dp>
typename enable_if<
  is_same<void, typename __void_t<decltype(
                  (declval<basic_ostream<_CharT, _Traits>&>() << declval<
                     typename unique_ptr<_Yp, _Dp>::pointer>()))>::type>::value,
  basic_ostream<_CharT, _Traits>&>::type
operator<<(basic_ostream<_CharT, _Traits>& __os,
           unique_ptr<_Yp, _Dp> const& __p) {
  return __os << __p.get();
}
```
- 위 코드는 libc++ 라이브러리에서 가져온 것인데 unique_ptr의 주소값을 출력해주는 basic-ostream의 operator<< 연산자를 구현한 것이다. 정말 혐오스럽다.
- type_traits 라이브러리들은 template meta function들을 잘 이해만 한다면 위와 같은 코드는 무리 없이 해석이 가능해진다.

# Template meta function
- 함수는 아니지만 마치 함수처럼 동작하는 템플릿 클래스들을 이야기한다.
- Meta function인 이유는 보통 함수들은 값에대해 연산을 수행하지만 ***Type***에 대해 연산을 수행한다는 점때문이다.

```cpp
if (is_negative(x))
{
    ...
}
```
- 음수인지 아닌지 판별하는 함수는 보통 위처럼 사용된다.

```cpp
if (is_void<T>::value)
{
    ...
}
```
- 템플릿 메타 함수도 매우 비슷하다.
- 어떤 타입이 void인지 아닌지 판단하는 함수의 예시다.

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void tell_type()
{
    if (std::is_void<T>::value)
    {
        std::cout << "T는 void ! \n";
    } else {
        std::cout << "T is not void \n";
    }
}

int main()
{
    tell_type<int>();

    tell_type<void>();
}
```

```
T is not void 
T는 void !
```
- 일반적 함수와 몇가지 차이점이 있다.
- 가장 중요한 것은 템플릿 메타 함수는 **실제론 함수가 아니라는 점이다.**
  - 함수였다면 ()로 호출할 것이다.
  - is_void는 그렇지 않다. \<>인자를 통해 함수 인자가 아닌 **템플릿 인자를 전달**하고 있다.
  - is_void는 클래스로 구현되어 있다.

## is_void
- if 문은 template meta programming에서는 템플릿 특수화를 통해 구현된다.
  - is_void도 마찬가지다.

```cpp
#include <iostream>

template <typename T>
struct is_void
{
    static constexpr bool value = false;
};

template <>
struct is_void<void>
{
    static constexpr bool value = true;
};

template <typename T>
void tell_type()
{
    if (is_void<T>::value)
    {
        std::cout << "T is void \n";
    } else {
        std::cout << "T is not void \n";
    }
}

int main()
{
    tell_type<int>();

    tell_type<void>();
}
```

```
T is not void 
T is void 
```
- std::is_void와 매우 비슷한 코드를 구현한 것이다.
- template<> 부분은 void에 대해 특수화된 클래스이다.
  - is_void<\void>를 하면 위 특수화 된 템플릿이 매칭되어 value가 true가 될 것이다.
- C++ 표준라이브러리중 하나인 type_traits에서는 is_void 처럼 타입들에 대해서 여러 연산을 수행할 수 있는 메타함수들을 제공하고 있다.

```cpp
#include <iostream>
#include <type_traits>

class A {};

template <typename T>
void only_integer(const T& t)
{
    static_assert(std::is_integral<T>::value);
    std::cout << "T is an integer \n";
}

int main()
{
    int n = 3;
    only_integer(n);

    A a;
    only_integer(a);
}
```
- 컴파일 시 오류가 날 것이다.
- static_assert는 C++11에 추가된 키워드로 함수는 아니다. 인자로 전달된 식이 참인지 아닌지를 컴파일 타임에 확인한다.
  - bool 타입의 constexpr만 static_assert로 확인할 수 있고 그 외의 경우에는 컴파일 오류가 발생한다.
- static_assert가 참이라면 컴파일러에 의해 해당 식은 무시되고, 거짓이라면 컴파일 오류를 발생시키게 된다.
- 이처럼 static_assert와 type_traits의 메타함수들을 잘 사용하면 특정 타입만 받는 함수를 간단하게 작성할 수 있다.