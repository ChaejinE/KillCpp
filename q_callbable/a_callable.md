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

# mem_fn : 멤버 함수들을 함수 객체로
```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

using std::vector;

int main()
{
    vector<int> a(1);
    vector<int> b(2);
    vector<int> c(3);
    vector<int> d(4);

    vector<vector<int>> container;
    container.push_back(a);
    container.push_back(b);
    container.push_back(c);
    container.push_back(d);

    vector<int> size_vec(4);
    transform(container.begin(), container.end(), size_vec.begin(),
              std::mem_fn(&vector<int>::size));
    for (auto itr = size_vec.begin(); itr != size_vec.end(); ++itr)
    {
        std::cout << "vector size :: " << *itr << std::endl;
    }
}
```

```
vector size :: 1
vector size :: 2
vector size :: 3
vector size :: 4
```
- vector들을 가지는 vector가 있을 때 vector들의 크기를 벡터로 만들어주는 코드다.
- size 함수는 멤버함수다.
- mem_fn 함수는 전달된 멤버함수를 function 객체로 만들어 return해주는 역할을 해준다.
  - 그리 자주 쓰이지는 않고 람다 함수로도 동일한 작업을 수행할 수 있다. 좀 더 편리한 람다를 사용한다. 단지, 코드 길이 면에서는 mem_fn을 사용하는게 깔끔하다.
- transform 함수는 algorithm 라이브러리에 있는 함수이며 각 원소들에 대해 인자로 전달된 함수를 실행한 다음 그 결과를 전달된 컨테이너에 넣어준다.

# std::bind
- 함수 객체 생성 시 인자를 특정한 것으로 지정할 수 있다.

```cpp
#include <functional>
#include <iostream>

void add(int x, int y)
{
    std::cout << x << " + " << y << " = " << x + y << std::endl;
}

void substract(int x, int y)
{
    std::cout << x << " - " << y << " = " << x - y << std::endl;
}

int main()
{
    auto add_with_2 = std::bind(add, 2, std::placeholders::_1);
    add_with_2(3);

    // 두 번째 인자 무시
    add_with_2(3, 4);

    auto subtract_from_2 = std::bind(substract, std::placeholders::_1, 2);
    auto negate = 
        std::bind(substract, std::placeholders::_2, std::placeholders::_1);

    subtract_from_2(3); // 3 - 2
    negate(4, 2); // 2 - 4
}
```

```
2 + 3 = 5
2 + 3 = 5
3 - 2 = 1
2 - 4 = -2
```
- bind 함수는 원래 함수에 특정 인자를 붙여준다. (bind)
- add라는 함수에 첫 번째 인자로 2를 bind 시켜주고 두 번째 인자로는 새롭게 만들어진 함수 객체의 첫 번째 인자를 전달해 준다.
- placeholders의 _1, _2는 일일이 정의된 객체들이다. 그 개수는 라이브러리마다 다른데 libstdc++의 경우 _1 ~ _29까지 정의도어 있다.

## 주의할 점
- 레퍼런스를 인자로 받는 함수들의 경우 주의해야된다.

```cpp
#include <functional>
#include <iostream>

struct S
{
    int data;
    S(int data) : data(data) { std::cout << "일반 생성자 호출" << std::endl; }
    S(const S& s)
    {
        std::cout << "복사 생성자 " << std::endl;
        data = s.data;
    }

    S(S&& s)
    {
        std::cout << "이동 생성자 " << std::endl;
        data = s.data;
    }
};

void do_something(S& s1, const S& s2) { s1.data = s2.data + 3; }

int main()
{
    S s1(1), s2(2);

    std::cout << "Before : " << s1.data << std::endl;

    auto do_something_with_s1 = std::bind(do_something, s1, std::placeholders::_1);
    do_something_with_s1(s2);

    std::cout << "After : " << s1.data << std::endl;
}
```

```
일반 생성자 호출
일반 생성자 호출
Before : 1
복사 생성자 
After : 1
```
- do_something 함수의 경우 첫 인자의 data를 두 번째 인자의 data + 3으로 만들어 주지만 실제로 do_something_with_s1 함수를 실행하면 s1을 전달했음에도 불그하고 s1의 data가 바뀌지 않는다.
- 생성자 호출 메세지에서 확인할 수 있듯이 bind 함수로 인자가 **복사**되어 전달되기 때문이다.
  - 명시적으로 s1의 레퍼런스를 전달해줘야 한다.

```cpp
#include <functional>
#include <iostream>

struct S
{
    int data;
    S(int data) : data(data) { std::cout << "일반 생성자 호출" << std::endl; }
    S(const S& s)
    {
        std::cout << "복사 생성자 " << std::endl;
        data = s.data;
    }

    S(S&& s)
    {
        std::cout << "이동 생성자 " << std::endl;
        data = s.data;
    }
};

void do_something(S& s1, const S& s2) { s1.data = s2.data + 3; }

int main()
{
    S s1(1), s2(2);

    std::cout << "Before : " << s1.data << std::endl;

    auto do_something_with_s1 = std::bind(do_something, std::ref(s1), std::placeholders::_1);
    do_something_with_s1(s2);

    std::cout << "After : " << s1.data << std::endl;
}
```

```
일반 생성자 호출
일반 생성자 호출
Before : 1
After : 5
```
- ref 함수는 전달 받은 인자를 **복사 가능한 Reference**로 변환해 준다.
  - const reference의 경우 cref 함수를 호출하면 된다.