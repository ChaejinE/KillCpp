# explicit keyword
```cpp
void DoSomethingWithString(MyString s)
{
    // Doing Something..
}
```
- MyString 타입을 받는 함수를 생각해보자.

```cpp
DoSomethingWithString(MyString("abc"))
```
- 컴파일 된다.
- MyString 객체 생성 후 이를 인자로 전달된다.

```cpp
DoSomethingWithString("abc")
```
- "abc"는 MyString Type이 아니다.
- C++ 컴파일러는 꽤 똑똑해서 "abc"를 어떻게 하면 MyString으로 바꿀 수 있는지 생각한다..
  - MyString 구현 중 생성자 중에 MyString(const char* str);이 있었다.
  - 이로 변환되어 버린다.
  - 위와 같은 변환을 **implicit conversion**이라고 부른다.
- implicit conversion이 무조건 편리한 것만은 아니다. 예상치 못한 경우가 발생할 수 있다.

```cpp
DoSomethingWithString(3)
```
- 이건 사용자가 아예 잘 못 사용한 경우다.
- 그런데 MyString 생성자에는 MyString(int capacity)가 있었다..
  - 컴파일 에러가 안난다.
  - implicit conversion
- C++에서는 원하지 않는 implicit conversion을 할 수 없도록 컴파일러에게 명시할 수 있다.
  - **explicit**

```cpp
explicit Mystring(int capacity);
```
- explicit은 명시적이라는 의미를 갖고있다.
- explicit의 경우 생성자를 이용한 암시적 변환을 수행하지 못하게 막을 수 있다.

```cpp
MyString s = 5;
```
- 컴파일 오류가 나며
- 명시적으로 생성자를 부를 때에만 허용할 수 있게 된다.

# mutable
- const 함수 내부에서는 멤버 변수들 값을 바꾸는 것은 불가능 하다.
- mutable로 선언하면 const 함수에서도 이들 값을 바꿀 수 있게 된다.
- mutable : 변이 가능한

```cpp
#include <iostream>

class A
{
    mutable int data_;

public:
    A(int data) : data_(data) {}
    void DoSomething(int x) const
    {
        data_ = x;
    }

    void PrintData() const
    {
        std::cout << "data : " << data_ << std::endl;
    }
};

int main()
{
    A a(10);
    a.DoSomething(3);
    a.PrintData();
}
```
```
data : 3
```
- 컴파일 및 실행이 가능하다.
- data_ 값이 const 함수 안에서 바뀐 것을 알 수 있다.
## 왜 mutable이 필요한가 ?
- 멤버 함수를 왜 const로 선언하는지 부터 생각해 보자.
  - 멤버함수는 이 객체가 이러이러한 일을 할 수 있다는 의미를 나타내고 있다.
- const로 선언하자는 의미는 **이 함수는 객체의 내부 상태에 영향을 주지 않는다.**를 표현하는 방법이다.
  - 일반적으로 **읽기 작업**을 수행하는 함수들이다.

```cpp
class Server
{
    ...

    User GetUserInfo(const int user_id) const
    {
        Data user_data = Database.find(user_id);

        return User(user_data);
    }
};
```
- Server에 GetUserInfo 라는 함수가 있는데 입력받은 user_id로 해당 유저를 조회하여 정보를 리턴하는 함수다.
- 당연히 데이터베이스를 업데이트 하지도 않고, 수정하는 작업도 없어 const로 선언되어 있다.
- 하지만, 대개 데이터베이스에 요청한 후 받아오는 작업이 오래걸리 메모리에 **cache**를 만들어서 자주 요청되는 데이터를 빠르게 조회할 수 있도록 한다.
  - cache는 크기가 데이터베이스 만큼 크지 않아 일부 유저 정보만 포함하고 있다.
  - 해당 유저가 없으면 cache miss가 나므로 database에 직접 요청해야된다.

```cpp
class Server
{
    ...
    Cache cache;

    User GetUserInfo(const int user_id) const
    {
        Data user_data = Database.find(user_id);

        if (!user_data)
        {
            user_data = Database.find(user_id);

            cache.update(user_id, user_data); // 불가능한 작업
        }
        return User(user_data);
    }
};
```
- 문제는 GetUserInfo가 const 함수라는 점이다.
- 따라서 캐쉬 업데이트하는 작업을 할 수 가 없다.
  - 캐쉬를 업데이트 하는 것은 cache 내부 정보를 바꿔야하는 뜻이기 떄문이다.
  - update 함수는 const 함수가 아닐것이다.
- GetUserInfo 에서 const를 떼기도 뭐한 것이 이 함수를 사용하는 사람 입장에선 당연히 const로 정의되어야하는 함수이기 때문이다.
  - Cache를 mutable로 선언해버리면되는 것이다.

```cpp
mutable Cache cache;
```
- mutable 키워드는 const 함수 안에서 해당 멤버 변수에 const가 아닌 작업을 할 수 있게 해준다.
