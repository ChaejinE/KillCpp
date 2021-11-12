# is a 관계
- C++의 상속을 도입한 이유는 단순히 똑같은 코드를 또 쓰는 것을 막기위한 용도만은 아니다.
- 실제이유는 객체지향 프로그래밍에서 추구하는 **객체 추상화**를 좀 더 효과적으로하기 위함이다.

```cpp
class Manager : public Employee
```
- C++에서 상속을 도입함으로써 클래스 사이 관계를 표현할 수 있게 되었다.
- Manager Class는 Employee의 모든 기능을 포함한다.
- Manager Class는 Employee의 모든 기능을 할 수 있으므로 Manager를 Employee라고 칭해도 무방하다.
  - 즉, **Manager is Employee**
- "A is a B"관계는 A와 B가 바뀌었을 때 성립하지 않는다.
  - A=B, B!=A

![image](https://user-images.githubusercontent.com/69780812/141396101-8407cc1e-9cee-46ac-8858-3523a4febcf4.png)
- "is a"관계로 이루어진 것들은 매우 많다.
- 이런 경우 클래스들 간의 상속 관계를 도표로 나타내는 경우가 많은데 파생 클래스가 기반 클래스를 화살표로 가리키게 그린다.
  - BankAccount : 은행 계좌
  - CheckingAccount : 자유롭게 입출금 가능. 이자 없음
  - SavingAccount : 비교적 자유롭게 입출금 가능. 매달 이자 붙음
  - CheckingAccount와 SavingAccount처럼 기능이 약간씩 다른 두 계좌 클래스들이 좀더 **일반적인** BankAccount 클래스를 상속받고있다.
- 클래스가 파생되면 파생될 수록 좀 더 **특수화, 구체화**된다.
  - Employee 클래스가 일반적인 사원을 위한 것이라면
  - Manager 클래스는 일반적 사원들 중 좀 더 특수한 부류의 사원을 의미한다.
- 기반 클래스로 거슬러 올라가면 올라갈 수록 **일반화(generalize)**된다고 말한다.

```cpp
#include <iostream>
#include <string>

class Base
{
    std::string s;

    public:
        Base() : s("기반") { std::cout << "기반 클래스" << std::endl; }
        
        void what() { std::cout << s << std::endl; }
};

class Derived : public Base
{
    std::string s;

    public:
        Derived() : s("파생"), Base() { std::cout << "파생 클래스" << std::endl; }

        void what() { std::cout << s << std::endl; }
};

int main()
{
    Base p;
    Derived c;

    std::cout << "Base" << std::endl;
    p.what();

    std::cout << "Derived " << std::endl;
    c.what();

    std::cout << "pointer ver" << std::endl;
    Base* p_c = &c;
    p_c -> what();

    return 0;
}
```

```
기반 클래스
기반 클래스
파생 클래스
Base
기반
Derived 
파생
pointer ver
기반
```
- Derived 객체 c를 Base객체를 가리키는 포인터에 넣었더니 동작한다.
- 이는 Derived is a Base를 의미한다.
  - Derived는 Base를 상속받아 어떻게 보면 Base 객체이므로 포인터가 c를 가리킬 수 있는 것이다.

![image](https://user-images.githubusercontent.com/69780812/141397644-e25c9693-3ee4-4c57-9002-e7f4b970f3e2.png)
- p는 엄연히 Base 객체를 가리키는 포인터다.
- p의 what을 실행한다면 당연히 Base의 what을 실행해주도록 동작한다.
  - 따라서 "기반"이 출력된다.

![image](https://user-images.githubusercontent.com/69780812/141397896-cd1cc11f-711b-45d3-acde-3579eaa2b841.png)
- 이러한 형태의 casting을 Up Casting이라고 부른다.
  - 파생 클래스에서 기반 클래스로 캐스팅하는 것

---
- 그렇다면 다운 캐스팅은 가능할까?

![image](https://user-images.githubusercontent.com/69780812/141398005-f956a219-acb5-4cad-beb0-8917f8d4ddad.png)
- Derived* 포인터가 Base 객체를 가리킨다고 해보자.
- Derived의 what 함수가 호출되어야하는데 가리키는 객체가 Base이면 Derived에 대한 정보가 없게되어 에러가난다.
  - 이와 같은 문제를 막기 위해 컴파일러 상에서 **함부로 DownCasting을 하는 것을 금지**하고 있다.

```cpp
Base p;
Dervied c;

Base* p_p = &c;

Derived* p_c = p_p;
p_c -> what();

return0;
```
- Derived* 포인터에 Base* 대입 시 똑같이 컴파일 에러가 난다.
- p_p가 가리키는 것이 최종적으로 Derived 객체를 가리킨다는 것을 알기 때문에 문제는 없어 보인다.

```cpp
Derived* p_c = static_cast<Derived*>(p_p);
```
- 위 처럼 강제적으로 타입 변환을 해주면 된다.
- 약간 위험하지만 오류를 발생시키지 않으며 성공적 컴파일을 할 수 있는 방법이다.
  - p_p가 Derived 객체를 가리키는 포인터가 아니라 Base 객체를 가리키는 포인터였으면 충분히 위험하다. 이때는 런타임 에러가 발생한다.
- 강제적으로 다운 캐스팅을 하는 경우에는 컴파일 타임에서 오류를 찾아내기 매우 힘들어 DownCasting은 작동이 보장되지 않는 한 매우매우 권장되지 않는다.

```cpp
Derived* p_c = dynamic_cast<Derived*>(p_p);
```
- C++에서는 상속 관계의 두 포인터들 간에 Casting을 해주는 dynamic_cast를 지원해준다.
- 위처럼해도 p_p가 사실 Base 객체를 가리킨다면 컴파일 오류가 발생할 것이다.

# has a 관계
```cpp
Class Car:
{
    private:
        Engine e;
        Brake b;
        ...
};
```
- 자동차는 브레이크와 엔진을 가지고있다.

```cpp
class EmployeeList
{
    int alloc_employee;
    int current_employee;
    Employee **employee_list // 직원 데이터
    ...
}
```
- EmployeeList 클래스를 보더라도 Employee를 포함하고있다.
- 이와 같이 어떤 클래스가 또다른 클래스를 포함할 수 있는 관계를 **has a 관계**라고한다.

# virtual keyword
```cpp
class Base
{
    std::string s;

    public:
        Base() : s("기반") { std::cout << "기반 클래스" << std::endl; }
        
        virtual void what() { std::cout << s << std::endl; }
};
```
- 위와 같이 virtual로 선언하면 어떻게 되는지 알아보자.

```cpp
int main()
{
    Base p;
    Derived c;

    Base* p_c = &c;
    Base* p_p = &p;

    std::cout << "Real Object : Base" << std::endl;
    p_p->what();

    std::cout << "Real Object : Derived " << std::endl;
    p_c->what();

    return 0;
}
```
- 업캐스팅으로 각 Derived c 객체를 Base* 포인터로 가리킨후 what()을 호출한다.

```
기반 클래스
기반 클래스
파생 클래스
Real Object : Base
기반
Real Object : Derived 
파생
```
- p_c는 Base* 포인터임에도 Dervied의 what()함수를 호출했다 !
  - 적절한 함수를 호출해줬다 !
- 컴파일 시에는 어떤 함수가 실행될지 정해지지 않는다.
- 런타임 시에 정해지게된다.
  - 이러한 일을 가리켜서 **dynamic binding**이라고 부른다.
- Derived의 what을 실행할지, Base의 what을 실행할지 결정언 Runtime에 이루어진다.

```cpp
if (i == 1)
{
    p_p = &c;
} else {
    p_p = &p;
}

p_p -> what();
```
- 좀 더 와닿은 예이다.
- p_p -> what()이 어떤 what일지 런타임에 정해질 것이다.
- 물론, 컴파일 타임에 어떤 함수가 호출될지 정해지는 것은 **static binding**이라 부른다.
  - 우리가 알고 사용해 오던 함수다.
---
- virtual keyword가 붙은 함수를 **virtual function**이라고 부른다.
  - **파생 클래스의 함수가 기반 클래스의 함수를 오버라이드 하기 위해서는 두 함수의 꼴이 정확히 같아야한다.**

# Override keyword
```cpp
void what() override { std::cout << s << std::endl; }
```
- C++ 11에서 파생 클래스에서 Base Class이 virtual function을 Override하는 경우 위 키워드로 명시적으로 나타낼 수 있다.
- 이 keyword를 통해 실수로 오버라이드 하지 않는 경우를 막을 수 있다.

```cpp
#include <iostream>
#include <string>

class Base {
  std::string s;

 public:
  Base() : s("기반") { std::cout << "기반 클래스" << std::endl; }

  virtual void what() { std::cout << s << std::endl; }
};
class Derived : public Base {
  std::string s;

 public:
  Derived() : s("파생"), Base() { std::cout << "파생 클래스" << std::endl; }

  void what() const { std::cout << s << std::endl; }
};

int main()
{
    Base p;
    Derived c;

    Base* p_c = &c;
    Base* p_p = &p;

    std::cout << " Real : Base " << std::endl;
    p_p->what();

    std::cout << " Real : Derived " << std::endl;
    p_c->what();

    return 0;
}
```

```
기반 클래스
기반 클래스
파생 클래스
 Real : Base 
기반
 Real : Derived 
기반
```
- what() 함수가 제대로 override 되지 않음을 알 수 있다.
- Base의 what 함수와 Derived의 what 함수는 거의 똑같이 생기긴 했지만 사실 다르다.
  - Derived의 what 함수는 const 함수이고, Base는 아니기 때문이다.
  - 따라서 Base의 what()을 호출하는 셈이 되는 것이다.
- 만약 위와 같은 상황이었다면 이 같은 버그는 컴파일 타임에 잡을 수 없어 어려움에 처하게 된다.
- 함수를 오버라이드 하기 위해서 만들었다면 override 키워드를 쓰자.
  - override를 키워드를 쓰고 override하고 있는게 없다면 에러나니 주의

# 다형성
```cpp
employee_list[i]->print_info();
total_pay += employee_list[i]->calculate_pay();
```
- Employee, Manager class가 구현되어 있어 employee list에 각각 저장되어 있다고 해보자.
- 이렇게 print_info(), calculate_play()라는 같은 기능을 호출해도 각 Instance에 맞게 호출되면서 다른 작업들을 하는 것을 바로 **다형성**(polymorphism)이라고 부른다.

---
- virtual keyword를 통해 동적 바인딩이라는 것을 이뤄낼 수 있었다.

```cpp
Parent* p = new Parent();
Parent* p = new Child();
```
- p, c 모두 Parent를 가리키는 포인터이다.

```cpp
p->f();
c->f();
```
- Praent의 f()가 호출되어야 하겠지만 실제 f()가 virtual function이라면 p와 c가 가리키는 개체에 맞게 호출될 것이다.
  - c->f()는 Child의 f()가 될 것이다.
  - 이는 f()가 vitual function으로 구현되었기 때문에 가능한 것이다.

# virtual 소멸자
- 상속 시 소멸자를 가상함수로 만들어야한다는 점
  - **클래스의 상속을 사용할 때 중요하게 처리되어야 하는 부분이다.**

```cpp
#include <iostream>

class Parent
{
    public:
        Parent() { std::cout << "Parent 생성자" << std::endl; }
        ~Parent() { std::cout << "Parent 소멸자" << std::endl; }
};

class Child : public Parent
{
    public:
        Child() { std::cout << "Child 생성자" << std::endl; }
        ~Child() { std::cout << "Child 소멸자" << std::endl; }
};

int main()
{
    std::cout << " 평범한 child 생성 " << std::endl;
    { Child c; }

    std::cout << "Parent 포인터로  Child 가리킬 때" << std::endl;
    {
        Parent *p = new Child();
        delete p;
    }
}
```
```
 평범한 child 생성 
Parent 생성자
Child 생성자
Child 소멸자
Parent 소멸자
Parent 포인터로  Child 가리킬 때
Parent 생성자
Child 생성자
Parent 소멸자
```
- 생성자 및 소멸자 호출 순서
  - Parent() -> Child() -> ~Child() -> ~Parent()
- 큰틀(Parent())를 짓고 집 내부 공사(Child())하는 것과 집을 철거할 때 내용물 제거(~Child())하고 집 구조물 철거(~Parent())와 같다.
- 문제는 Parent 포인터가 Child를 가리킬 때이다.
- delete p를 했음에도 p가 가리키는 것은 Parent 객체가 아닌 Child 객체이다. 보통의 객체가 소멸되는 것과 같은 순서로 호출되지않는다.
  - 실제로 ***Child 소멸자가 호출되지 않는다.***
- 소멸자가 호출되지 않을 시 메모리를 동적으로 할당했기 때문에 **메모리 누수**(Memory Leak)이 생긴다.
  - **Parent(Base Class)의 소멸자를 virtual로 만들어버려야한다.**

```cpp
class Parent
{
    public:
        Parent() { std::cout << "Parent 생성자" << std::endl; }
        virtual ~Parent() { std::cout << "Parent 소멸자" << std::endl; }
};
```
- 이와 같이 선언하면 Child 소멸자가 호출된다.
- 그렇다면 왜 Parent 소멸자가 호출되었을까?
  - Child 소멸자를 호출하면서 **알아서** Parent 소멸자도 호출해주기 때문이다.
  - 반면 Parent 소멸자 호출 시 Child가 있는지 없는지도 몰라서 소멸자를 호출해 줄 수 없다.
- 이와 같은 이유로 상속될 여자기 있는 Base Class는 반드시 소멸자를 virtual로 만들어줘야 나중에 문제가 생기지 않는다.

# Reference로 함수 접근 가능 !
- 기반 클래스에서 파생 클래스의 함수에 접근할 때 항상 기반 클래스의 포인터를 통해 접근했었다.
- 하지만, 사실 Class의 Reference여도 문제없이 작동한다.

```cpp
#include <iostream>

class A
{
    public:
        virtual void show() { std::cout << "Parent !" << std::endl; }
};

class B : public A
{
    public:
        void show() override { std::cout << "Child !" << std::endl; }
};

void test(A& a) { a.show(); }

int main()
{
    A a;
    B b;

    test(a);
    test(b);

    return 0;
}
```

```
Parent !
Child !
```
- test()함수를 통해 B 클래스의 객체를 전달해도 잘 작동한다.
  - **이는 B 클래스가 A클래스를 상속받고 있기 때문이다.**
- 함수에 Type이 Base Class여도 그 파생 클래스는 타입 변환되어 전달할 수 있다.
- show 함수가 virtual로 정의되어 있으므로 알아서 B(Derived Class)의 show 함수를 찾아내서 호출하게 되는 것이다.

# 가상함수 구현 원리
- 모든 함수를 virtual을 붙이면 안되는가 ?
  - 사실 virtual이더라도 실제 존재하는 함수이고 정상적으로 호출할 수 있다.
  - 모든 함수를 동적 바인딩이 제대로 동작하게 만들면 안되나 ?
- 실제로 JAVA는 모든 함수들이 Default로 virtual 함수로 선언된다.
- C++이 제약을 둔것은 virtual 함수 사용시 약간의 Overhead가 존재하기 떄문이다.
  - 보통 함수 호출 보다 가상 함수 호출 시간이 좀 더 오래 걸린다.

```cpp
class Parent
{
    public:
        virtual void func1();
        virtual void func2();
};

class Child : public Parent
{
    public:
        virtual void func1();
        void func3();
};
```
- C++ 컴파일러는 가상함수가 하나라도 존재하는 클래스에 대해서 **virtual function table**을 만든다.

![image](https://user-images.githubusercontent.com/69780812/141415072-ea29835f-1c9f-4802-a0c2-37f9e0416df0.png)
- 위 코드의 v-table 도식도이다.
- 가상함수와 가상함수가 아닌 함수의 차이점을 보면 Child의 func3() 같이 비 가상함수들은 단수히 특벼한 단계 없이 func3()이 호출된다.
- 가상 함수 호출 시에는 가상 함수 테이블을 한 단계 더 거쳐 실제로 어떤 함수를 고를지 결정한다.

```cpp
Parent *p = Parent();
p->func1();
```
- 위를 호출했을 시
- 1. p가 Parent 포인터 이므로 func1()의 정의를 Parent 클래스에서 찾아본다.
- 2. func1()이 가상함수다 ? 그렇다면 func1()을 직접 실행하지 않고, 가상 함수 테이블에서 func1()에 해당하는 함수를 실행한다.
  - 실제 프로그램 실행 시 가상 함수 테이블에서 func1()에 해당하는 함수, Parent::func1()을 호출하게 된다.
  - 그러므로 일반적인 함수 보다 호출하는 데 시간이 더 오래걸리게된다.
- 이 차이는 극히 미미하나 최적화가 매우 중요한 분야에서는 이를 감안할 필요가 있다.

# pure virtual function
```cpp
#include <iostream>

class Animal
{
    public:
        Animal() {}
        virtual ~Animal() {}
        virtual void speak() = 0;
};

class Dog : public Animal
{
    public:
        Dog() : Animal() {}
        void speak() override { std::cout << "왈왈" << std::endl; }
};

class Cat : public Animal
{
    public:
        Cat() : Animal() {}
        void speak() override { std::cout << "야옹 야옹" << std::endl; }
};

int main()
{
    Animal* dog = new Dog();
    Animal* cat = new Cat();

    dog->speak();
    cat->speak();
}
```
```
왈왈
야옹 야옹
```
- Animal Class의 speak 함수가 =0;으로 처리되어있다.
  - "무엇을 하는지 정의되어 있지 않은 함수"를 말한다.
  - 즉, **반드시 오버라이딩 되어야만 하는 함수**를 의미한다.
- =0; 을 붙여서 반드시 오버라이딩 되도록 만든 함수를 **pure virtual function**이라고 한다.
  - 본체가 없어 이 함수를 호출하는 것은 불가능하다.
  - **그래서 Animal 객체를 생성하는 것 또한 불가능하다.**
- pure virtual function을 최소 한개 이상 포함하고 있는 Class는 객체를 생성할 수 없고 instance 화 시키기 위해서는 이 Class를 상속받는 Class를 만들어 모든 순수 가상함수를 Override 해줘야만 한다.
  - 이러한 반드시 상속되어야 하는 Class를 **Abstract Class**라고 부른다.
> private에 pure virtual function을 구현해도 된다. private에 있다고 override안된다는 의미는 아니기 때문이다. 다만, 파생 클래스에서 접근을 못할 뿐이다.

## 왜 Abstract Class를 사용하는가?
- 추숭 클래스는 **설계도**라고 생각하면 좋다.
- 상속받아 사용하는 사람에게 이 기능은 일반적 상황에서 만들기 힘드니 개발자가 직접 특수화 되는 클래스에 맞춰서 만들어 쓰라는 의미다.
- Animal Class의 speak 함수는 동물마다 내는 소리가 다르므로 가상 함수로 만들기는 불가능하다. 따라서 순수 가상함수 만들면 Animal을 상속받은 Class 들은 speak()한다는 의미와 함께 상황에 맞게 구현하면된다.

```cpp
Animal* dog = new Dog();
Animal* cat = new Cat();

dog->speak();
cat->speak();
```
- 이는 모두 각자의 Class Type으로 Override되어 있으므로 각 Class의 Speak 함수로 대체되어 실행될 수 있는 것이다.

# Multiple inheritance(다중 상속)
- C++에서는 한 클래스가 여러 개의 클래스들을 상속받는 것을 허용한다.
```cpp
class A
{
    public:
        int a;
};

class B
{
    public:
        int b;
};

class C : public A, public B
{
    public:
        int c;
};
```
- C class가 A, B class를 동시에 같이 상속받고 있다.

![image](https://user-images.githubusercontent.com/69780812/141416747-d7e31416-4e01-4a9f-ab83-1cb95c05b6fc.png)
- 단순히 A와 B의 내용이 C에 들어간다고 생각하면 된다.

```cpp
C c;
c.a = 3;
c.b = 2;
c.c = 4;
```
- 이런게 된다는 뜻이다.

## 생성자 소멸자 호출 순서
```cpp
#include <iostream>

class A
{
    public:
        int a;
        A() { std:: cout << "A 생성자 " << std::endl; }
        virtual ~A() { std:: cout << "A 소멸자 " << std::endl; }
};

class B
{
    public:
        int b;
        B() { std:: cout << "B 생성자 " << std::endl; }
        virtual ~B() { std:: cout << "B 소멸자 " << std::endl; }
};

class C : public A, public B
{
    public:
        int c;
        C() { std:: cout << "C 생성자 " << std::endl; }
        ~C() { std:: cout << "C 소멸자 " << std::endl; }
};

int main()
{
    C c;
}
```

```
A 생성자 
B 생성자 
C 생성자 
C 소멸자 
B 소멸자 
A 소멸자
```
- A -> B -> C 순서이다.

```cpp
class C : public B, public A
```
- 상속받는 순서를 바꿔보자

```
B 생성자 
A 생성자 
C 생성자 
C 소멸자 
A 소멸자 
B 소멸자
```
- B -> A -> C 순서로 생성자가 호출된다.
- 상속의 순서에 좌우된다.

## 다중 상속 시 주의할 점
```cpp
class A {
 public:
  int a;
};

class B {
 public:
  int a;
};

class C : public B, public A {
 public:
  int c;
};

int main() {
  C c;
  c.a = 3;
}
```
- A, B 모두 a라는 변수이름을 사용했을 시 Compile 에러가 발생한다.

```cpp
class Human {
  // ...
};
class HandsomeHuman : public Human {
  // ...
};
class SmartHuman : public Human {
  // ...
};
class Me : public HandsomeHuman, public SmartHuman {
  // ...
};
```
- dreadful diamond of derivation(공포의 다이아몬드 상속)
  - **diamod inheritance**라고 부른다.

![image](https://user-images.githubusercontent.com/69780812/141417584-9b3526bc-7c06-4bc1-be65-1bb6285ed39c.png)
- 위와 같은 형태가 되기 때문이다.
- 이 같은 경우 Human에 name이라는 멤버 변수가 있을 것이고, HandsomeHuman, SmartHuman 모두 name을 가지고 있게 된다.
  - Me에서 name이라는 변수가 겹치게 되는 것이다.
  - 아무리 안겹치게 만든다고 해도 Human의 모든 내용이 중복되는 문제가 발생한다.

```cpp
class Human {
  // ...
};
class HandsomeHuman : virtual public Human {
  // ...
};
class SmartHuman : virtual public Human {
  // ...
};
class Me : public HandsomeHuman, public SmartHuman {
  // ...
};
```
- 따라서 virtual로 Human을 상속 받으면 Me에서 다중 상속 시에도 컴파일러가 언제나 Human을 한 번만 포함하도록 지정할 수 있게 된다.
- 참고로 가상 상속 시 Me의 생성자에서 HansomeHuman, SmartHuman의 생성자 호출은 당연하고, Human의 생성자 또한 호출해줘야 한다.

## Multi inheritance는 언제 사용해야하는가 ?
- 공식 웹사이트에 따르면 가이드라인을 제시한다.
- [Reference](https://isocpp.org/wiki/faq/multiple-inheritance#virtual-inheritance-where)