# 상속 (Inheritance)
- C++에서는 다른 클래스의 내용을 그대로 포함할 수 있는 작업을 가능하도록 해준다.
  - **Inheritance, 상속**이라한다.

```cpp
class Base
{
    std::string s;

    public:
        Base() : s("base") { std::cout << "기반 클래스" << std::endl; }

        void what() { std::cout << s << std::endl; }
};
```
- Base, 기반 Class이다.

```cpp
class Derived : public Base
{
    std::string s;

    public:
        Dervied() : Base(), s("derived")
        {
            std::cout << "파생 클래스" << std::endl;
            what();
        }
}
```
- Derived가 Base를 public 형식으로 상속 받는 것이다.

![image](https://user-images.githubusercontent.com/69780812/141084727-d3293329-2520-458c-88c3-8a4430a3c291.png)
- 상속 받은 후 Derived의 모습은 오른쪽과 같다.
- 마치 Base Class의 코드가 그대로 들어가 있는 듯한 모습이다.

```cpp
Dervied() : Base(), s("파생") {
    std::cout << "파생 클래스" << std::endl;
    what();
}
```
- 파생 클래스의 생성자 호출 부분을 집중해보자.
- Derived의 생성자는 Base의 생성자를 호출하여 먼저 처맇나 다음 Derived의 생성자를 실행한다.
- 위 처럼 초기화 리스트에서 Base를 통한 기반의 생성자를 먼저 호출하게 된다.
- 명시적으로 호출하지 않으면 Base Class의 default 생성자가 호출된다.

```cpp
#include <iostream>
#include <string>

class Base
{
    std::string s;

    public:
        Base() : s("base") { std::cout << "base class" << std::endl; }

        void what() { std::cout << s << std::endl; }
};

class Derived : public Base
{
    std::string s;

    public:
        Derived() : Base(), s("derived") 
        { 
            std::cout << "derived class" << std::endl; 
            what();    
        }
};

int main()
{
    std::cout << " === Base Class === " << std::endl;
    Base p;

    std::cout << " === Deri Class === " << std::endl;
    Derived c;

    return 0;
}
```
- 이렇게하면 c에서 호출한 what()에서 base라고 출력이 나올 것이다.
- 이는 c에서 호출되는 what이 부모의 method이며 이 method는 base의 s를 출력하는 역할을 하고 있기 때문이다.

```cpp
class Derived : public Base
{
    std::string s;

    public:
        Derived() : Base(), s("derived") 
        { 
            std::cout << "derived class" << std::endl; 
            what();    
        }

    void what() { std::cout << s << std::endl; }
};
```
- derived class에 what을 정의한다면 ? derived가 출력된다.
- 컴파일러는 **다른 클래스에 정의되어 있는 것이므로 다른 함수로 취급한다.**
  - 파생 클래스에 대해 what 함수가 정의되어 있어 호출할 때 Base 함수 까지는 뒤지지 않고, Derived class의 함수를 호출하게 된다.
  - 이를 **Overriding**(오버라이딩)이라고한다.