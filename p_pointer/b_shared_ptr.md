# Overview
- 때에 따라서는 여러 개의 스마트 포인터가 하나의 객체를 같이 소유해야 하는 경우가 발생한다.
- 이 자원을 사용하는 모든 객체들이 소멸되어야 하는데, 어떤 객체가 먼저 소멸되는지 알 수 없어 자원 역시 어느 타이밍에 해제 시켜야 할지 알 수 없게 된다.
- 이러한 경우 특정 자원을 **몇 개의 객체에서 가리키는지**추적한 다음 그 수가 0이 되면 비로소 해제시켜주는 방식의 포인터가 필요하다.

# shared_ptr
![image](https://user-images.githubusercontent.com/69780812/145993705-b03cb69c-9506-43c4-98a4-e559d74a884e.png)
- unique_ptr과 다르게 shared_ptr로 객체를 가리킬 경우, 다른 shared_ptr 역시 그 객체를 가리킬 수 있다.

![image](https://user-images.githubusercontent.com/69780812/145994019-f1289c23-9d7e-4c2a-9c7d-0dd2d0763321.png)
- shared_ptr은 몇개가 원래 객체를 가리키는지를 알아야한다.
  - 이를 reference count라고 한다.
- reference count = 0이면 객체를 해제할 수 있다.

```cpp
#include <iostream>
#include <memory>
#include <vector>

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

int main()
{
    std::vector<std::shared_ptr<A>> vec;

    vec.push_back(std::shared_ptr<A>(new A()));
    vec.push_back(std::shared_ptr<A>(vec[0]));
    vec.push_back(std::shared_ptr<A>(vec[1]));

    std::cout << "첫 번쨰 소멸" << std::endl;
    vec.erase(vec.begin());

    std::cout << "다음 원소 소멸" << std::endl;
    vec.erase(vec.begin());

    std::cout << "프로그램 종료" << std::endl;
}
```
- shapred_ptr을 원소로 가지는 vec을 정의하고 vec[0], vec[1], vec[2]가 모두 같은 객체를 가리키도록 한 것이다.
- 차례대로 첫번쨰 원소를 지워나가는데, 모든 스마트 포인터들이 소멸되어야만 객체를 파괴하므로 두 번의 erase에서는 아무것도 하지 않다가 마지막의 erase에서 A의 소멸자를 호출한다.
  - reference count가 3 -> 2 -> 1 -> 0이되는 것이다.

```cpp
std::shared_ptr<A> p1(new A());
std::shared_ptr<A> p2(p1);

std::cout << p1.use_count(); // 2
std::cout << p2.use_count(); // 2
```
- shared_ptr의 참조 개수를 알고 싶으면 use_count 함수를 사용하면 된다.

![image](https://user-images.githubusercontent.com/69780812/145995528-64f47f47-9f6f-4c1c-b221-3852f6a640b2.png)
- 만약 p3와 같이 다른 shared_ptr이 추가적으로 해당 객체를 가리킨다면 어떻게 shared_ptr끼리 동기화하는지 알아보자.
- 위 처럼 shared_ptr이 control_bolck을 동적으로 할당해서 이 제어 블록에 필요한 정보를 공유하는 방식으로 구현된다.
- shared_ptr은 복사 생성 마다 해당 제어 블록의 위치만 공유하면 되고, 소멸할 떄마다 제어 블록의 참조 개수르 하나 줄이고, 생성할 때마다 늘리는 방식으로 작동한다.

# make_shared
```cpp
std::shared_ptr<A> p1(new A());
```
- 사실 위와 같은 방식은 바람직한 shared_ptr의 생성 방법은 아니다.
- 동적 할당은 비싼 연산이지만 위에서는 두번 발생하게 된다.
  - A 생성 시, shared_ptr 제어블록 생성 시
- 아예 두 개 합친 크기로 한 번 할당하는 것이 훨씬 빠르다.

```cpp
std::shared_ptr<A> p1 = std::make_shared<A>();
```
- make_shared 함수는 A의 생성자의 인자들을 받아 A, shared_ptr의 제어 블록까지 한번에 동적 할당 한 후 만들어진 shared_ptr을 return 한다.

# 주의할 점
```cpp
A* a = new A();
std::shared_ptr<A> pa1(a);
std::sahred_ptr<A> pa2(a);
```

![image](https://user-images.githubusercontent.com/69780812/145996097-369a3a90-3920-4baf-90bf-235e984931b5.png)
- 위 코드를 실행하면 그림과 같이 두 개의 제어 블록이 따로 생성된다.
- 둘 중 하나는 자신이 가리키고 있는 객체가 이미 해제되어있음에도 소멸시키면서 오류가 발생하게 된다.
- 이와 같은 상황을 방지하려면 shared_ptr을 주소값을 통해서 생성하는 것을 지양해야한다.

## enalbe_shared_from_this
- this를 통해 shared_ptr을 만들고 싶은 클래스가 있으면 이를 상속받으면 된다.

```cpp
#include <iostream>
#include <memory>

class A : public std::enable_shared_from_this<A>
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
        }

        std::shared_ptr<A> get_shared_ptr() { return shared_from_this(); }
};

int main()
{
    std::shared_ptr<A> pa1 = std::make_shared<A>();
    std::shared_ptr<A> pa2 = pa1->get_shared_ptr();

    std::cout << pa1.use_count() << std::endl;
    std::cout << pa2.use_count() << std::endl;
}
```

```
자원 획득
2
2
소멸자 호출
```
- enable_shared_from_this 클래스에서는 shared_from_this라는 멤버함수를 정의하고 있다.
  - 이미 정의되어 있는 제어 블록을 통해 shared_ptr을 생성한다.
  - 따라서 두 개의 다른 제어 블록이 생성되는 일을 막을 수 있다.
- 한 가지 중요한 점은 shared_from_this가 잘 작동하기 위해서는 shared_ptr이 반드시 먼저 정의도어 있어야한다.
  - 제어 블록을 확인할 뿐 없는 제어 블록을 만들지는 않는다.

# 순환 참조의 경우
![image](https://user-images.githubusercontent.com/69780812/145998760-bf3e600b-2972-477f-b6a9-479c2add6159.png)
- 위와 같은 상황에서는 reference count가 0이 될 수 없다.
  - 소멸자가 정상적으로 호출되지 않는다.
- shared_ptr 자체에 내제되어 있는 문제로 이를 해결할 수는 없다.