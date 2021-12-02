# Resource 관리의 중요성
- C++ 이후 많은 언어들은 대부분 Garbage Collector가 기본적으로 내장되어 있다.
  - Garbage Collector : 더 이상 쓰이지 않는 자원을 자동으로 해제해주는 역할을 한다.
  - 자원을 해제하는 일에 크게 신경쓰지 않는다.
- C++은 한 번 획득한 자원은 직접 해제해주지 않는 이상 프로그램이 종료되기 전 까지 영원히 남아있게 된다.
  - 해제하지 않고 프로그램이 종료되면 운영체제가 알아서 해제한다.

```cpp
#include <iostream>

class A 
{
    int *data;

    public:
        A()
        {
            data = new int[100];
            std::cout << "자원 획득" << std::endl;
        }

        ~A()
        {
            std::cout << "소멸자 호출" << std::endl;
            delete[] data;
        }
};

void do_somthing() { A *pa = new A(); }

int main()
{
    do_somthing();
}
```

```
자원 획득
```
- 자원을 획득하기만 하고 소멸자는 호출되지 않는 것을 확인할 수 있다.
  - 그 이유는 delete pa;를 하지 않았기 떄문이다.
- delete는 메모리를 해제하기 직전 가리키는 객체의 소멸자를 호출해준다.
- 만약 delete를 do_something 함수 안에서 해주지 않으면 생성된 객체를 가리키던 pa는 메모리에서 사라지게 된다.
  - Heap 어딘가에 Class A 객체가 남아있지만 그 주소값을 가지고 있는 포인터는 메모리 상에 존재하지 않게된다.
  - 영원히 해제되지 못한 채 힙에서 자리만 차지하고 있게 되는 것이다.
- 위 경우 400 바이트의 메모리 누수가 발생할 것이다.
- 프로그램의 커질 수록 자원을 해제하는 위치가 애매한 경우가 많아 놓치기 십상이다.
  - 예를들어 예외가 발생하여 delete가 실행되지 않아버리는 상황 (메모리 누수를 피할 수가 없게된다.)

# Resoucre Acquisition Is Initialization - RAII
- RAII : 자원 획득은 초기화다. (디자인 패턴)
- 자원 관리를 스택에 할당한 객체를 통해 수행하는 것이다.
- 함수의 스택에 정의되어 있는 모든 객체들은 빠짐없이 소멸자가 호출된다는 것을 알고있다.
  - stack unwinding
  - 예외가 발생하지 않는다면 소멸자가 당연히 호출될 것이다.
- 소멸자들 안에 다 사용한 자원을 해제하는 루틴을 넣으면 어떨까?
  - 위의 경우 pa가 객체가 아니기 때문에 소멸자가 호출되지 않는다. 그렇다면 pa를 일반적인 포인터가 아닌 **포인터 객체**로 만들어 자신이 소멸될 때, 자신이 가리키고있던 데이터도 같이 delete 하게 하면 된다.
  - 즉, 자원 관리를 스택의 객체인 포인터 객체를 통해 수행하게 되는 것이다.
- 이렇게 작동하는 포인터 객체를 **Smart Pointer**라고 부른다.

# 객체의 유일한 소유권 unique_ptr
- C++에서 메모리를 방식으로 관리하면 크게 두가지 문제점이 발생할 수 있다.
  - 1. 메모리를 사용한 후 해제하지 않는 경우 (**memory leak**)
    - 서버처럼 장시간 작동하는 프로그램의 경우 시간이 지남에 따라 점점 사용하는 메모리의 양이 늘어나서 결과적으로 나중에 시스템 메모리가 부족해 서버가 죽어버리는 상황이 발생한다.
    - 이는 RAII 패턴을 사용해 사용이 끝난 메모리는 항상 해제시켜버리면 막을 수 있다.
  - 2. 이미 해제된 메모리를 다시 참조하는 경우
  ```cpp
  Data* data = new Data();
  Data* data2 = data;
  ...
  delete data;
  ...
  delete data2;
  ```
  - 위 경우 data와 data2가 동시에 한 객체를 가리키고 있다.
  - data2는 이미 소멸된 객체를 다시 소멸시키려하게 될 것이다.
    - 이럴 경우 메모리 오류가 나면서 프로그램이 죽는다.
    - 이러한 버그를 double free 버그라고 부른다.
  - 위와 같은 문제의 원인은 만들어진 객체의 소유권이 명확하지 않기 때문이다.
  - 어떤 포인터가 객체의 유일한 소유권을 부여받아 이 포인터 말고는 객체를 소멸시킬수 없다면, 두 번 소멸시켜버리는 일은 없게될 것이다.
- 특정 개체에 유일한 소유권을 부여하는 포인터 객체를 unique_ptr이라고 한다.

```cpp
#include <iostream>
#include <memory>

class A
{
    int *data;

    public:
        A()
        {
            std::cout << "자원 획득" << std::endl;
            data = new int[100];
        }

        void some() { std::cout << "일반 포인터와 동일하게 사용가능" << std::endl; }

        ~A()
        {
            std::cout << "자원을 해제" << std::endl;
            delete[] data;
        }
};

void do_something()
{
    std::unique_ptr<A> pa(new A());
    pa->some();
}

int main()
{
    do_something();
}
```

```
자원 획득
일반 포인터와 동일하게 사용가능
자원을 해제
```
- unique_ptr을 정의하기 위해서 템플릿에 인자로 포인터가 가리킬 클래스를 전달한다.
  - 위의 경우 pa는 A 클래스의 객체를 가리키는 포인터가 된다.
- pa->some();로 포인터처럼 사용하면 된다.
  - unique_ptr은 -> 연산자를 오버로드해서 마치 포인터를 다루는 것과 같이 사용할 수 있게 했다.
- pa는 스택에 정의된 객체이므로 do_something() 함수가 종료될 때 자동으로 소멸자가 호출된다. 그리고 unique_ptr은 소멸자 안에서 자신이 가리키고 있는 자원을 해주므로 자원이 잘 해제된다.

```cpp
...

void do_something()
{
    std::unique_ptr<A> pa(new A());

    std::unique_ptr<A> pb = pa;
}

int main()
{
    do_somthing();
}
```
- 위와 같이하면 삭제된 함수를 사용했다는 에러가 발생한다.

## 삭제된 함수
- C++ 11에 추가된 기능이다.

```cpp
#include <iostream>

class A
{
    public:
        A(int a) {};
        A(const A& a) = delete;
};

int main()
{
    A a(3); // 가능
    A b(a); // 불가능
}
```
- 이를 컴파일 하면 복사 생성자 호출하는 부분에서 오류가 발생한다.
  - 복사 생성자를 명시적으로 삭제했기 때문이다.
  - Class A의 복사 생성자는 존재하지 않는다.
  - = delete;를 사용하면 명시적으로 **이 함수는 쓰지마**라고 표현할 수 있다.
- unqiue_ptr도 마찬가지로 복사 생성자가 명시적으로 삭제되어 있다.
  - 어떠한 객체를 유일하게 소유해야하기 때문이다.
  - 복사 생성할 수 있다면 특정 객체를 여러 개의 unqieu_ptr들이 소유하게 되는 문제가 발생한다.
- 각각의 unique_ptr 들이 소멸될 때 전부 객체를 delete하려 해서 double free버그가 발생한다.

## 소유권 이전
- 복사는 되지 않지만 소유권을 이전할 수 있다.

```cpp
#include <iostream>
#include <memory>

class A
{
    int *data;

    public:
        A()
        {
            std::cout << "자원 획득" << std::endl;
            data = new int[100];
        }

        void some() { std::cout << "일반 포인터와 동일하게 사용가능" << std::endl; }

        ~A()
        {
            std::cout << "자원을 해제" << std::endl;
            delete[] data;
        }
};

void do_something()
{
    std::unique_ptr<A> pa(new A());
    std::cout << "pa : ";
    pa->some();

    std::unique_ptr<A> pb = std::move(pa);
    std::cout << "pb : ";
    pb->some();
}

int main()
{
    do_something();
}
```

```
자원 획득
pa : 일반 포인터와 동일하게 사용가능
pb : 일반 포인터와 동일하게 사용가능
자원을 해제
```
- unique_ptr은 복사 생성자는 정의되어 있지 않지만 이동 생성자는 가능하다.
  - 마치 소유권을 이동시킨다는 개념으로 생각하면 되기때문이다.
- pa를 pb에 강제로 이동시켜버리는 것이다.
  - pb가 new A로 생성된 객체의 소유권을 갖게되고 pa는 아무 것도 가리키고 있지 않게 된다.

```cpp
pa.get()
```
- pa가 가리키고 있는 실제 주소값을 확인해보면 0 (nullptr)이 나오게될 것이다.
- pa를 이동시킨 후 pa->some() 하면 문제가 발생하게 될 것이다.
- 소유권을 이동 시킨 이후 기존의 unique_ptr을 접근하지 않도록 조심해야한다.
- 소유권이 이전된 unique_ptr을 **dangling pointer**라고 하며 이를 재 참조하면 런타임 오류가 발생하도록 한다.
  - 소유권 이전은 **댕글링 포인터를 절대 다시 참조하지 않겠다는 확인 하에 이동해야한다.**

# unique_pointer 함수 인자로 전달
- 복사 생성자가 없다. Reference로 전달하면 잘 전달 된다.

```cpp
#include <iostream>
#include <memory>

class A
{
    int *data;

    public:
        A()
        {
            std::cout << "자원 획득" << std::endl;
            data = new int[100];
        }

        void some() { std::cout << "일반 포인터와 동일하게 사용가능" << std::endl; }

        void do_sth(int a)
        {
            std::cout << "무언가를 한다" << std::endl;
            data[0] = a;
        }

        ~A()
        {
            std::cout << "자원을 해제" << std::endl;
            delete[] data;
        }
};

void do_something(std::unique_ptr<A>& ptr)
{
    ptr->do_sth(3);
}

int main()
{
    std::unique_ptr<A> pa(new A());
    do_something(pa);
}
```

```
자원 획득
무언가를 한다
자원을 해제
```
- 함수에 unique_ptr을 전달하는 것이 문맥 상 맞는 코드일까?
- unique_ptr은 어떤 객체의 소유권을 의미한다.
- 위처럼 Reference로 unique_ptr을 전달하면 do_something 함수 내부에서  ptr은 더이상 유일한 소유권을 의미하지 않는다.
  - 물론 ptr은 레퍼런스라 함수가 종료되면서 pa가 가리키는 객체를 파괴하지는 않는다.
  - 단지 pa가 **유일하게 소유하고 있던 객체**는 적어도 do_something 함수 내부에서는 ptr을 통해서도 소유할 수 있게 되는 것이다. (원칙 위배)
  - 단순히 포인터의 Wrapper로 사용하는 것에 불과하게 된다.

```cpp
#include <iostream>
#include <memory>

class A
{
    int *data;

    public:
        A()
        {
            std::cout << "자원 획득" << std::endl;
            data = new int[100];
        }

        void some() { std::cout << "일반 포인터와 동일하게 사용가능" << std::endl; }

        void do_sth(int a)
        {
            std::cout << "무언가를 한다" << std::endl;
            data[0] = a;
        }

        ~A()
        {
            std::cout << "자원을 해제" << std::endl;
            delete[] data;
        }
};

void do_something(A* ptr)
{
    ptr->do_sth(3);
}

int main()
{
    std::unique_ptr<A> pa(new A());
    do_something(pa.get());
}
```

```
자원 획득
무언가를 한다
자원을 해제
```
- get함수 호출 시 실제 객체의 주소값을 리턴해준다.
- do_something 함수가 일반적인 포인터를 받고 있다.
  - 소유권이라는 의미는 버린채로 함수 내부에서 객체에 접근할 수 있는 권한을 주는 것이다.
- unique_ptr이 소유한 객체에 일시적으로 접근하고 싶다면 get을 통해 해당 객체의 포인터를 전달하면 된다.

# unique_ptr 쉽게 생성하기
- C++14 부터 unique_ptr을 간단히 만들 수 있는 std::make_unique 함수를 제공한다.

```cpp
#include <iostream>
#include <memory>

class Foo
{
    int a, b;

    public:
        Foo(int a, int b) : a(a), b(b) { std::cout << "생성자 호출" << std::endl; }
        void print() {std::cout << "a : " << a << ", b : " << b << std::endl; }
        ~Foo() { std::cout << "소멸자 호출" << std::endl; }
};

int main()
{
    auto ptr = std::make_unique<Foo>(3, 5);
    ptr->print();
}
```

```
생성자 호출
a : 3, b : 5
소멸자 호출
```
- make_unique 함수는 아예 템플릿 인자로 전달된 클래스의 생성자의 인자들에 직접 완벽한 전달을 수행한다.
- 기존 처럼 불필요하게 std::unique_ptr<Foo> ptr(new Foo(3, 5)); 할 필요없이 만들 수 있다.

# unique_ptr을 원소로 가지는 컨테이너
- unique_ptr은 다른 타입과 큰 차이는 없지만 **복사 생성자가 없다**는 특성을 염두하자.

```cpp
#include <iostream>
#include <memory>
#include <vector>

class A
{
    int *data;

    public:
        A(int i)
        {
            std::cout << "자원 획득" << std::endl;
            data = new int[100];
            data[0] = i;
        }

        void some() { std::cout << "값 : " << data[0] << std::endl; }

        ~A()
        {
            std::cout << "자원 해제" << std::endl;
            delete[] data;
        }
};

int main()
{
    std::vector<std::unique_ptr<A>> vec;

    vec.emplace_back(new A(1));

    vec.back()->some();
}
```

```
자원 획득
값 : 1
자원 해제
```
- 기존 vector의 push_back은 복사하여 인자를 집어넣는 방식으로 복사생성자가 없는 unqiue_ptr 타입을 사용하면 에러가 발생한다.
  - std::move로 소유권을 넘겨주면 되기는 한다.
- emplace_back() 함수를 사용하면 직접 unique_ptr을 생성하면서 집어넣을 수 있다.
  - 불필요한 이동 과정(std::move)를 생략할 수 있다.
- emplace_back 함수는 전달된 인자를 완벽한 전달(**perfect forwarding**)을 통해 직접 unique_ptr<A> 의 생성자에 전달해서 vector 맨 뒤에 unqiue_ptr<A> 객체를 생성하게 된다.
