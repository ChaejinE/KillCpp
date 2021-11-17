# Overview
- Template을 통해 타입이 마치 인자 인것 처럼 사용하는 것을 **일반화 프로그래밍**(**제네릭 프로그래밍**)이라고 부른다.
- 템플릿 인자로는 타입 뿐아니라 특정한 조건을 만족하는 값들도 올 수 있었다.

```cpp
Array<int ,5>
Array<int, 3>
```
- template을 통해 Array class를 만들었다고 해보자. 이 둘은 같은 클래스일까?
- typeid로 비교해보면 False가 나온다.
- 이는 **서로의 객체 생성을 위해 각기 다른 코드를 생성**하기 때문이다.

```cpp
#include <iostream>
#include <typeinfo>

template <int N>
struct Int
{
    static const int num = N;
};

template <typename T, typename U>
struct add
{
    typedef Int<T::num + U::num> result;
};

int main()
{
    typedef Int<1> one;
    typedef Int<2> two;

    typedef add<one, two>::result three;

    std::cout << "Add result : " << three::num << std::endl;
}
```

```
Add result : 3
```
- one과 two는 객체가 아니다.. type을 나타내는 데 one은 1 type을 two는 2 type을 나타내닌 것이다.
- 마치 숫자 연산을 한것 처럼 된다.
- add 클래스의 템플릿은 인자로 두 개 타입ㅇㄹ 받아 그 타입의 num 멤버를 더해서 새로운 타입인 result를 만들어 낸다.
- add 클래스를 함수라고 생각하면, 그 계산 결과를 내부 result 타입으로 반환한다고 보면된다.
- 흥미로운 점은 3이라는 값이 프로그램이 실행되면서 계산되는 것이 아니라 **컴파일 시 컴파일러가 three::num을 3으로 치환해버린다.**
  - 런타임 시, 단순히 그 결과를 보여주는 것이다.

# Template Meta Programming - TMP
- 여태까지 타입은 어떠한 객체에 무엇을 저장하느냐를 지정하는데 사용했지, 타입 자체가 어떠한 값을 가지지는 않았다.
- 위 예제에서 볼 수 있다 시피 템플릿을 사용하면 객체를 생성하지 않더라도 타입에 어떠한 값을 부여할 수 있고, 그 타입들을 가지고 연산을 수행할 수 있다.
- 또한, 타입은 반드시 컴파일 타임에 확정되어야해서 컴파일 타임에 모든 연산이 끝나게 된다.
- 이렇게 타입을 가지고 **컴파일 타임에 생성되는 코드로 프로그래밍을 하는 것**을 **메타프로그래밍**이라고 한다.
  - C++의 경우 템플릿을 가지고 이러한 작업을 해서 템플릿 메타프로그래밍이라고 하는 것이다.

```cpp
#include <iostream>

template <int N>
struct Factorial
{
    static const int result = N * Factorial<N - 1>::result;
};

template <>
struct Factorial<1>
{
    static const int result = 1;
};

int main()
{
    std::cout << "6! = 1*2*3*4*5*6 = " << Factorial<6>::result << std::endl;
}
```

```
6! = 1*2*3*4*5*6 = 720
```
- Factorial을 일반적으로 함수로 구성했다면 재귀함수 형태가 됬을 것이다.
- 재귀함수의 호출이 끝나기 위해서는 n = 1 일 때, 처리를 해줘야한다.
- 템플릿 역시 마찬가지로 n = 1일 때 따로 처리할 수 있는데, 템플릿 특수화를 이용한다.
- 위 예제에서는 실질적으로 값을 가지는 객체는 아무 것도 없다.
  - 720 이라는 값을 가지고 있는 변수는 메모리 상에서 없다는 뜻이다. (출력 시 제외)
  - 단순히 컴파일러가 만들어낸 Factorial<6> 이라는 타입을 나타내고 있을 뿐이다.
- for문으로 구현할 수 있는 모든 코드는 Template Meta Programming을 이용해서 구현할 수 있다고한다.
- if 문 역시 템플릿 특수화로 TMP로 구현할 수 있다.

# TMP는 왜 쓰는가 ?
- C++ 코드도 템플릿 메타프로그래밍 코드로 변환할 수 있으나 코드가 엄청 길어질 것이다.
- 모두 컴파일 타임에 모든 연산이 끝나므로 프로그램 실행 속도를 향상 시킬 수 있다. 당연히 컴파일 시간은 엄청 늘어나게될 것이다.
- 템플릿 메타 프로그래밍으로 프로그램 전체를 구현하는 일은 없다.
- 템플릿 메타 프로그래밍으로 작성된 코드는 버그를 찾기 매우 힘들다.
  - 컴파일 타임에 연산하므로 디버깅이 불가능
  - 템플릿 오류 시 엄청난 길이의 오류를 내뿜는다.
- TMP를 사용하는 경우 꽤나 제한 적이지만 많은 C++ 라이브러리들이 TMP를 이용해서 구현되어있고, TMP를 통해 컴파일 타임에 여러 오류들을 잡아 낼 수 있으며 속도가 매우 중요한 프로그램의 경우 TMP를 통해 런타임 속도도 향상 시킬 수 있다.

```cpp
#include <iostream>

template <int X, int Y>
struct GCD
{
    static const int value = GCD<Y, X % Y>::value;
};

template <int X>
struct GCD<X, 0>
{
    static const int value = X;
};

template <int N, int D = 1>
struct Ratio
{
    typedef Ratio<N, D> type;
    static const int num = N; // 분자
    static const int den = D; // 분모
};

template <class R1, class R2>
struct _Ratio_add
{
    typedef Ratio<R1::num * R2::den + R2::num * R1::den, R1::den * R2::den> type;
};

template <class R1, class R2>
struct Ratio_add : _Ratio_add<R1, R2>::type {};

int main()
{
    std::cout << "gcd (36, 24) :: " << GCD<36, 24>::value << std::endl;

    // typedef Ratio<2, 3> rat;
    // typedef Ratio<3, 2> rat2;
    // typedef Ratio_add<rat, rat2> rat3;
    using rat = Ratio<2, 3>;
    using rat2 = Ratio<3, 2>;
    using rat3 = Ratio_add<rat, rat2>;

    std::cout << rat3::num << " / " << rat3::den << std::endl;

    return 0;
}
```

```
gcd (36, 24) :: 12
13 / 6
```
- 간단하고 직관적으로 using 키워드를 통해 표현할 수 있다.
- Ratio 클래스의 객체를 생성한듯 보이지만, 실제로 생성된 객체는 한 개도 없고, 단순히 타입들을 컴파일러가 만들어낸 것 뿐이다.

```cpp
bool is_prime(int N)
{
    if (N == 2) return true;
    if (N == 3) return true;

    for (int i = 2; i <= N / 2; i++)
    {
        if (N % i == 0) return false;
    }

    return true;
}
```
- 소수 판별함수를 작성하면 위와 같을 것이다.

# 의존 타입 (dependent type)
= 템플릿 인자에 따라 어떠한 타입이 달라질 수 있는 것을 의존 타입이라고 부른다.
  - T::t* p 가 어떤 것을 특수화 하느냐에 따라 t x p 곱셈인지, p의 포인터가 될지 달라진다.
  - 컴파일러가 성공적으로 해석하기 위해서 무조건 Type이라고 알려줘야한다.
  - **typename**을 붙여주면된다.

```cpp
#include <iostream>

template <int N>
struct INT
{
    static const int num = N;
};

template <typename a, typename b>
struct add
{
    typedef INT<a::num + b::num> result;
};

template <typename a, typename b>
struct divide
{
    typedef INT<a::num / b::num> result;
};

using one = INT<1>;
using two = INT<2>;
using three = INT<3>;

template <typename N, typename d>
struct check_div
{
    // RESULT 중 하나라도 TRUE면 전체가 TRUE다.
    static const bool result = (N::num % d::num == 0) ||
                                check_div<N, typename add<d, one>::result>::result;
};

template <typename N>
struct check_div<N, typename divide<N, two>::result>
{
    static const bool result = (N::num & (N::num / 2) == 0);
};

template <typename N>
struct _is_prime
{
    static const bool result = !check_div<N, two>::result;
};

template <>
struct _is_prime<two>
{
    static const bool result = true;
};

template <>
struct _is_prime<three>
{
    static const bool result = true;
};

template <int N>
struct is_prime
{
    static const bool result = _is_prime<INT<N>>::result;
};

int main()
{
    std::cout << std::boolalpha;
    std::cout << "Is 2 prime ? " << is_prime<2>::result << std::endl;
    std::cout << "Is 10 prime ? " << is_prime<10>::result << std::endl;
    std::cout << "Is 11 prime ? " << is_prime<11>::result << std::endl;
    std::cout << "Is 61 prime ? " << is_prime<61>::result << std::endl;
}
```

```
Is 2 prime ? true
Is 10 prime ? false
Is 11 prime ? true
Is 61 prime ? true
```
- 소수 판별하는 코드를 템플릿 메타 프로그래밍을 한것이다.
- 위에서는 static const int result는 static const int 의 값이 된다.
- 그러므로 check_div에서 두번째 인자에 typename을 붙여 줌으로써 Type임을 명시하고 전달하여 그에 해당하는 값을 얻게된다.

## 단위 라이브러리 (Unit)
- TMP의 진짜 파워에 대해 알아본다.
- 물리적 수치 계산 시에 꼭 필요한 것은 단위이다.
  - kg, m, s
- 먼저, 변수에 넣을 경우 프로그램적으로 어떤 실수가 일어 날 수 있다.
  - NASA에서는 단위를 잘못 처리해 1조원 화성 탐사선을 날려먹었다고 한다.
- 그렇다면 Class를 만들어서 객체에 보관하던지 operator+ 등으로 연산자 오버로딩으로 단위가 맞는 것끼리 체크하면서 연산을 처리하면 될 것이다.
  - 만일 틀린 단위를 연산하는 코드가 **매우 드물게** 일어난다면 ?
  - 런 타임에서 그 문제를 발견하지 못한 채 넘어 갈 수 있는 문제가 있다.
  - 가장 이상적 상황은 단위가 맞지 않는 연산 수행하는 코드가 있다면 컴파일 시 오류를 발생시켜 버리는 것이다.

```cpp
#include <iostream>

template <typename U, typename V, typename W>
struct Dim
{
    using M = U; // Kg
    using L = V; // m
    using T = W; // s

    using type = Dim<M, L, T>
};
```
- 어떤 데이터의 단위를 나타내는 Dim 클래스를 정의한다.
- 물리량 단위를 나타내기 위해서는 무게, 길이, 시간 3개로 나타낼 수 있다.
  - 속도의 경우 m/s이므로 Dim<0, 1, -1>
  - 힘의 경우 kg m / s^2 이므로 Dim<1, 1, -2>가 될 것이다.
  - 물론 숫자로 받는 것은 안되고 타입으로 받아야 된다.

```cpp
Dim<Ratio<1, 1>, Ratio<1, 1>, Ratio<-2, 1>>
```
- 위와 같이 정의되야 할 것이다.

```cpp
kg + meter
```
- Unit_ex1.cpp 파일에서 단위가 다르면 연산이 안된다.

```cpp
  template <typename D2>
  quantity<T, typename add_dim_<D, D2>::type> operator*(quantity<T, D2> quant) {
    return quantity<T, typename add_dim_<D, D2>::type>(q * quant.q);
  }

  template <typename D2>
  quantity<T, typename subtract_dim_<D, D2>::type> operator/(quantity<T, D2> quant) {
    return quantity<T, typename subtract_dim_<D, D2>::type>(q / quant.q);
  }
```
- qunatity 클래스 안에 위와 같이 정의하여 *, /를 정의한다.
- meter / (second * second) = 가속도를 표현하기 위함이다.
  - 다만, 새로운 차원의 데이터가 나올뿐이다.
- 새로 마들어지는 타입의 차원은 add_dim<D, D2>::type이 될것이다.
  - 값은 그냥 실제 값을 곱해주면 된다.

```cpp
quantity<T, D> operator*(T scalar) { return quantity<T, D>(q * scalar); }
quantity<T, D> operator/(T scalar) { return quantity<T, D>(q / scalar); }
```
- 스칼라를 처리해주기 위한 연산도 만들어줘야한다.
- 3 * kg 과 같은 연산을 위함이다.

## auto keyword
- F (힘)의 차원은 F = kg * meter / (second * second) 이므로 표현하기가 아주 빡세다..
```cpp
quantity<double, Dim<one, one, Ratio<-2, 1>>> F = kg * meter / (second * second)
```
- 휴..
- 많은 경우 굳이 타입을 쓰지 않아도 알아서 추측할 수 있는 경우들이 많다. (C++)

```cpp
(??) a = 3;
```
- int 를 의도한 것일 것이다.

```cpp
some_class a;
(??) b = a;
```
- (??)는 아마 some_class일 것이다.
- 복사하는 대상의 타입을 확실히 알면 굳이 타입을 명시하지 않아도 컴파일러가 알아 낼 수 있다.

```cpp
auto F = kg * meter / (second * second);
```
- 정확히 타입을 추론할 수 있는 경우 auto 키워드를 이용하면 되겠다.
- auto keyword는 템플릿의 사용으로 복잡해진 타입 이름들을 간단히 나타낼 수 있는 획기적인 방법이다.
- 간단한건 써주면 좋지만 추측하기 쉬운 경우 auto 키워드를 활용하는 것도 좋다.

---
TMP는 특성상 복잡하고 버그 발생 시 찾는 것이 매우 어렵다.
하지만 적절하게 사용하면 런타임에서 찾아야하는 오류를 컴파일 타임에서 미리 다 잡아낼 수 있다.
런타임 시 수행해야 하는 연산들도 일부 컴파일 타임으로 옮길 수 있다.

boost:MPL 라이브러리가 있는데 비교적 쉬운 TMP를 할 수 있다고한다.
```
