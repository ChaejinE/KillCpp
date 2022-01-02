# Overview
- enable_if는 SFINAE를 통해 조건에 맞지 않는 함수들을 오버로딩 후보군에서 쉽게 뺄 수 있게 도와주는 간단한 템플릿 메타 함수다.

# enable_if
```cpp
template <bool B, class T = void>
struct enable_if;
```
- enable_if는 위와 같이 정의되어 있다.
- B부분에 확인하고픈 조건ㅇ르 전달한다.
  - B가 참이라면 enalbe_if::value는 Type이 T가 되고, 아니라면 value가 존재하지 않게된다.

```cpp
std::enable_if<std::is_integral<T>::value>::type
```
- 어떤 함수의 인자 T가 정수 타입일 때만 오버로딩을 하고 싶다고 해보면 enable_if는 위와 같이 쓸 수 있다.
- B자리에 원하는 조건인ㅇ std:;is_integral<\T>::value를 전달한 것이다.

```cpp
#include <iostream>
#include <type_traits>

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
void test(const T& t)
{
    std::cout << "t : " << t << std::endl;
}

int main()
{
    test(1); // int
    test(false);  // bool
    test('c');  // char
}
```

```
t : 1
t : 0
t : c
```
- enable_if를 사용한 함수에 대한 코드다.
- 만약 정수 타입이 아닌 객체를 전달하면 컴파일 오류가 발생한다.
  - struct A() {};를 정의해보면된다.
  - 가능한 오버로딩이 없다고 나오게 되기 때문이다.
- typename = 부분은 템플릿에 디폴트 인자를 전달하는 부분이다.
  - 보통 typenaem U = 이런식으로 받지만 저 식자체만 필요한 경우에는 굳이 인자를 정의할 필요가 없다.
- std::enalbe_if앞에 추가적으로 typename이 또 붙은 이유는 std::enableif<>::type이 **의존 타입**이기 때문이다.

```cpp
template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;  // C++ 14 부터 사용 가능

template <class T>
inline constexpr bool is_integral_v =
  is_integral<T>::value;  // C++ 17
```
- 길게쓰면 복잡하니 enable_if 같이 자주 쓰이는 패턴에 대한 alias를 활용할 수 있다.

## enalbe_if 구현방식
```cpp
template <bool B, class T = void>
struct eanble_if {};

template <class T>
struct enable_if<true, T>
{
    typedef T type;
};
```
- B가 true일 때 특수화된 버전에서 type이 정의되어있고 나머지 경우에는 enalbe_if 내부에 아무것도 정의되어있지 않다.
- B가 참일 때만 std::enable_if<\B>::value 가 올바른 문장이 되어 오버로딩 후보군에서 제외되지 않고 남을 수 있게 된 것이다.

```cpp
template <typename T>
class vector
{
    public:
        vector(size_t num, const T& element);
    
        template <typename Iterator>
        vector(Iterator start, Iterator end);
};
```
- vector 클래스를 제작한다고 생각해보자. 생성자로 위 두가지 형태를 제공할 것이다.
- 첫 번째 생성자는 원소가 num개 들어있는 vector를 만드는 생성자다.
- 두 번째 생성자는 반복자 싲가과 끝을 받는 생성자다.

```cpp
vector<int> v(10, 3);
```
- 위는 어떤식으로 해석 도리까?
- 첫 번째 생성자 오버로드를 통해 3이 10개 들어있는 벡터를 생성하길 원했을 것이다.

```cpp
#include <iostream>

template <typename T>
class vector
{
    public:
        vector(size_t num, const T& element)
        {
            std::cout << element << " : " << num << std::endl;
        }

        template <typename Iterator>
        vector(Iterator start, Iterator end)
        {
            std::cout << "Constructor for Iterator" << std::endl;
        }
};

int main()
{
    vector<int> v(10, 3);
}
```

```
Constructor for Iterator
```
- 반복자를 이용한 생성자가 호출되는 것을 확인할 수 있다.
- 주목할 점은 num type이 size_t라는 것이다.(unsinged int)
- v(10, 3) 했을 때 **10은 부호가 있는 정수라는 점**이다.
- C++ 컴파일러가 똑똑하게 캐스팅해서 넘어갈 수도 있었지만 더 나은 후보가 없는 가정하에 일어나는 일이다.
- Iterator 생성자를 보면 Iterator가 int로 오버로딩 한다면 v(10, 3)을 완벽하게 매칭 시켜버릴 수 있다는 점이다.
- 따라서 이 경우 Iterator가 실제 반복자임을 강제할 필요성이 있다.

```cpp
template <typename Iterator,
          typename = std::enable_if_t<is_iterator<iterator>::value>>
vector(Iterator start, Iterator end)
{
    std::cout << "Constructor for Iterator" << std::endl;
}
```
- 위처럼 Iterator가 실제 반복자임을 강제할 수 있다.
- is_iterator라는 메타함수가 있다고 가정하면 위처럼 쓸 수 있게 된다.
  - 실제로는 없는 메타함수다.
- Iterator가 실제로 반복자일 경우에만 해당 vector 생성자가 오버로딩 후보군에 들어갈 것이다.