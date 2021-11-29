# Overview
- 사람들은 실수를 하기 마련이고, 컴퓨터 역시 언제나 프로그램에 필요한 자원을 제공할 수 있는 것이 아니다.

```cpp
std::vector<int> v(3);
std::cout << v.at(4);
```
- 크기가 3인 벡터를 만들었다. 그런데 4번쨰 원소를 요청한다 ??
- 코드는 문법상 아무 문제가 없지만 막상 실행하면 오류가 발생한다.

```cpp
std::vector<int> v(1000000000000000000000);
```
- 위와 같이 큰 메모리를 할당할 수는 없다.
- 문법상 틀리진 않았지만 실제 실행 시 오류가 발생하게 된다.
- 이렇게 정상적인 상황에서 벗어난 모든 예외적인 상황들을 Exception이라고 부른다.

# 기존의 예외 처리 방식
- C에서는 딱히 예외 처리 방식이 존재하지 않는다.
  - 어떤 작업 실행 뒤 그 결과값을 확인하는 방식으로 처리했다.

```cpp
char *c = (char *)malloc(100000);
if (c == NULL)
{
    printf("메모리 할당 오류");
    return;
}
```
- malloc의 경우 메모리 할당 실패 시 NULL을 return 하므로 위와 같이 예외적인 상황을 처리할 수 있었다.
- 함수가 깊어지면 깊어질 수록 꽤나 귀찮은 작업이 된다.

# throw
- C++에서는 예외가 발생핬다는 사실을 명시적으로 나타낼 수 있다.

```cpp
#include <iostream>

template <typename T>
class Vector
{
    public:
        Vector(size_t size) : size_(size)
        {
            data_ = new T[size_];
            for (int i = 0; i < size_; i++)
            {
                data_[i] = 3;
            }
        }

        const T& at(size_t index) const
        {
            if (index >= size_)
            {
                throw std::out_of_range("vector의 index가 범위를 초과");
            }

            return data_[index];
        }

        ~Vector() { delete[] data_; }

    private:
        T* data_;
        size_t size_;
};
```
- 인자로 전달된 index가 범위 이내라면 간단히 data[index]를 리턴하겠지만 const T&를 리턴하기 때문에 따로 오류메시지를 리턴할 수 없다.
  - 하지만 C++에서는 명시적으로 알릴 수 있다 throw!
- 예외를 던지고 싶다면 **throw로 예외로 전달하고 싶은 객체를 써주면 된다.**
  - 위의 경우 out_of_range 객체를 throw 한다.
  - overflow_error, length_error, runtime_error 등 여러 가지의 정의된 표준 라이브러리에서 활용되고 있다.
- throw 한 위치에서 즉시 함수가 종료되고 예외 처리 부분까지 점프하게 된다.
  - throw 밑의 모든 문장은 실행되지 않는다.
- 한가지 중요한 점은 **stack에 생성되었던 객체들을 빠짐없이 소멸시켜준다는 점**이다.
  - 따라서 사용하고 있는 자원들을 제대로 소멸시킬 수 있다.
  - 소멸자만 제대로 작성했다면..

# try & catch
```cpp
#include <iostream>
#include <stdexcept>

template <typename T>
class Vector
{
    public:
        Vector(size_t size) : size_(size)
        {
            data_ = new T[size_];
            for (int i = 0; i < size_; i++)
            {
                data_[i] = 3;
            }
        }

        const T& at(size_t index) const
        {
            if (index >= size_)
            {
                throw std::out_of_range("vector의 index가 범위를 초과");
            }

            return data_[index];
        }

        ~Vector() { delete[] data_; }

    private:
        T* data_;
        size_t size_;
};

int main()
{
    Vector<int> vec(3);

    int index, data = 0;
    std::cin >> index;

    try
    {
        data = vec.at(index);
    }
    catch (std::out_of_range& e)
    {
        std::cout << "예외 발생 " << e.what() << std::endl;
    }

    std::cout << "읽은 데이터 : " << data << std::endl;
}
```

```
4
예외 발생 vector의 index가 범위를 초과
읽은 데이터 : 0
```
- 범위에 벗어난 값을 입력했더니 예외가 처리되었다.
- try는 무언가 예외가 발생할만한 코드가 실행된다.
- 예외가 발생할 경우 stack에 생성된 모든 객체들을의 소멸자들이 호출되고, 가장 가까운 catch 문으로 jump 한다.
- catch 문은 throw된 예외를 받는 부분으로 정의된 예외의 꼴에 맞는 객체를 받게 된다.

# stack unwinding
- catch로 점프하면서 스택 상에서 정의된 객체들을 소멸시키는 과정을 **스택 풀기, stack unwinding)**이라 부른다.
- 한가지 주의할 점은 **생성자에서 예외가 발생 시 소멸자가 호출되지 않는다**는 점이다.
- 예외를 던지기 이전에 획득한 자원이 있다면 catch 에서 잘 해제시켜줘야한다.

# 여러 종류의 예외
```cpp
#include <iostream>
#include <stdexcept>
#include <vector>

int func(int c)
{
    if (c == 1)
        throw 10;
    else if (c == 2)
        throw std::string("hi");
    else if (c == 3)
        throw 'a';
    else if (c == 4)
        throw "hello";

    return 0;
}

int main()
{
    int c = 1;
    std::vector<int> vec(4);
    for (int i = 4; i > 0; --i)
        vec.emplace_back(i);

    for (auto c : vec)
    {
        try
        {
            func(c);
        } 
        catch (char x)
        {
            std::cout << "Char : " << x << std::endl;
        }
        catch (int x)
        {
            std::cout << "Int : " << x << std::endl;
        }
        catch (std::string& s)
        {
            std::cout << "String : " << s << std::endl;
        }
        catch (const char* s)
        {
            std::cout << "String Literal : " << s << std::endl;
        }
    }
}
```

```
String Literal : hello
Char : a
String : hi
Int : 10
```
- 각기 다른 값들을 throw 했을 때 작동하는 catch가 달라지는 것을 확인할 수 있다.

```cpp
#include <iostream>
#include <exception>
#include <vector>

class Parent : public std::exception
{
    public:
        virtual const char* what() const noexcept override { return "Prent!\n"; }
};

class Child : public Parent
{
    public:
        const char* what() const noexcept override { return "Child!\n"; }
};

int func(int c)
{
    if (c == 1)
        throw Parent();
    else if (c == 2)
        throw Child();

    return 0;
}

int main()
{
    std::vector<int> vec;
    
    for (int i = 1; i < 3; ++i)
        vec.emplace_back(i);

    for (int c : vec)
    {
        std::cout << "val : " << c << std::endl;
        try
        {
            func(c);
        } catch (Parent& p)
        {
            std::cout << "Parent Catch" << std::endl;
            std::cout << p.what();
        } catch (Child& c)
        {
            std::cout << "Child Catch" << std::endl;
            std::cout << c.what();
        }
    }
}
```

```
val : 1
Parent Catch
Prent!
val : 2
Parent Catch
Child!
```
- Parent나 Chlid 클래스 객체를 리턴하게한 코드다.
- Child 객체 throw 시 Parent 객체가 리턴되었다.
- 이는 catch 문의 경우 가장 먼저 대입될 수 있는 객체를 받기때문이다.
- 위와 같은 문제를 방지하기 위해서는 Parent catch를 Child catch 보다 뒤에 써주는 것이 좋다.
- 덧붙여 일반적으로 예외 객체는 std::exception을 상속 받는 것이 좋다.
  - 표준 라이브러리의 유용한 함수들을 사용할 수 있기 때문이다.

# 모든 예외 받기
- 어떤 예외를 throw 했는데 받는 catch가 없을 수 있다.
- runtime_error 예외 등을 발생시키며 프로그램이 비정상적으로 종료되었다고 뜨게 된다.
  - 예외를 던지는 코드가 있다면 적절하게 받아내는 것으 중요하다.

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>

int func(int c)
{
    if (c == 1)
        throw 1;
    else if (c == 2)
        throw "hi";
    else if (c == 3)
        throw std::runtime_error("error");

    return 0;
}

int main()
{
    std::vector<int> vec;

    for (int i = 1; i < 4; ++i)
        vec.emplace_back(i);

    for (auto c : vec)
    {
        try {
            func(c);
        } catch (int e) {
            std::cout << "Catch int : " << e << std::endl;
        } catch (...) {
            std::cout << "Default Catch ! : " << std::endl;
        }
    } 
}
```

```
Catch int : 1
Default Catch ! : 
Default Catch ! : 
```
- catch(...)에서 try안에서 발생한 모든 예외들을 받게 된다.
  - 어떠한 예외도 다 받을 수 있기 때문에 특정 타깁을 찝어서 객체에 대입 시킬 수 없는 것이다.

# noexcept
- 예외를 발생시키지 않는 함수

```cpp
#include <iostream>

int foo() noexcept {}

int bar() noexcept { throw 1; }

int main()
{
    foo();

    try {
        bar();
    } catch (int x) {
        std::cout << "Error : " << x << std::endl;
    }
}
```

```
terminate called after throwing an instance of 'int'
```
- noexcept로 명시된 함수가 예외를 발생시키게 된다면 예외가 제대로 처리되지 않고 프로그램이 종료된다.
- catch 문에서 예외가 제대로 처리되지 않고, 프로그램이 종료되는 것을 알 수 있다.
  - 왜 붙이는 것일까?
- 단순히 프로그래머가 컴파일러에게 주는 힌트라고 생각하면 된다.
  - 어떤 함수가 절대로 예외를 발생시키지 않는 다는 사실을 알려주면 여러 추가적인 최적화를 수행할 수 있다.
- 소멸자들은 기본적으로 noexcept이다. 절대로 소멸자에서 예외를 던지면 안된다.