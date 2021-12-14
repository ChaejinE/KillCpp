# Overview
- Callabel이란, 호출(Call)할 수 있는 모든 것을 의미한다.
  - 대표적으로 function이 있겠다.
  - ()를 붙여 호출할 수 있는 모든 것을 Callable이라 정의한다.
- class에서 ()연산자를 오버로딩하거나 lambda도 ()로 호출되므로 Callable이다.

# std::function
- Callable 들을 객체 형태로 보관할 수 있는 클래스를 제공한다.
  - 함수 뿐 아니라 모든 Callable을 보관할 수 있는 객체다.

```cpp
#include <functional>
#include <iostream>
#include <string>

int some_func1(const std::string& a)
{
    std::cout << "Func1 호출" << a << std::endl;
    return 0;
}

struct S
{
    void operator()(char c) { std::cout << "Func2 호출" << c << std::endl; }
};

int main()
{
    std::function<int(const std::string&)> f1 = some_func1;
    std::function<void(char)> f2 = S();
    std::function<void()> f3 = []() { std::cout << "Func3 호출 " << std::endl; };

    f1("hello");
    f2('c');
    f3();
}
```

```
Func1 호출hello
Func2 호출c
Func3 호출
```
- function 객체는 template 인자로 전달 받은 함수의 type을 갖는다.
  - return 값, 인자 값

- Functor인 클래스 S의 경우 단순히 S의 객체를 전달해도 이를 마치 함수처럼 받아들인다.
  - char를 받고 void를 return하는 것으로 표현할 수 있다.
- std::function은 C++의 모든 Callable을 마음대로 보관할 수 있는 유용한 객체다.

# member 함수를 가지는 std::function
- 멤버 함수 내 this의 경우 자신을 호출한 객체를 의미하므로 만일 멤버함 수를 그냥 function에 넣게 되면 this가 무엇인지 알 수 없는 문제가 발생한다.

```cpp
#include <functional>
#include <iostream>
#include <string>

class A
{
    int c;

    public:
        A(int c) : c(c) {}
        int some_function() 
        {
            std::cout << "비상수 함수: " << ++c << std::endl;
            return c;
        }

        int some_const_function() const
        {
            std::cout << "상수 함수: " << c << std::endl;
        }

        static void st() {}
};

int main()
{
    A a(5);

    std::function<int(A&)> f1 = &A::some_function;
    std::function<int(const A&)> f2 = &A::some_const_function;

    f1(a);
    f2(a);
}
```

```
비상수 함수: 6
상수 함수: 6
```
- 위와 같이 원래 인자에 추가적으로 객체를 받는 인자를 전달해주면 된다.
- 멤버 함수들의 경우 암시적 변환이 발생하지 않으므로 & 연산자를 통해 명시적으로 주소값을 전달해줘야 한다.
- 따라서 호출하고자 하는 객체를 인자로 전달해주면 마치 해당 객체의 멤버함수를 호출한 것과 같은 효과를 낼 수 있다.