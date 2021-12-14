# Overview
- shared_ptr의 순환 참조의 경우 문제를 해결할 수 없었다.

# weak_ptr
![image](https://user-images.githubusercontent.com/69780812/145999006-f847b2a2-a637-4ced-b9cc-ae68e139d2cf.png)
- 위와 같은 형태를 자료 구조로 어떻게 나타 낼 수 있는가 ?
- shared_ptr로 하게 되면 순환 참조 문제가 생길 것이다.
  - refrence count가 절대로 0이 될 수 없다.
- weak_ptr은 일반 포인터와 shared_ptr 사이 위치한 스마트 포인터이다.
  - shared_ptr 처럼 객체를 안전하게 참조할 수 있게 해주지만 참조 개수를 늘리지는 않는다.
- weak_ptr 자체로는 원래 객체를 참조할 수 없으며 반드시 shared_ptr로 변환해서 사용해야 한다.
  - 가리키고 있는 객체가 이미 소멸되었다면 빈 shared_ptr로 변환 된다.
  - 아닐 경우 해당 객체를 가리키는 shared_ptr로 변환된다.

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class A
{
    std::string s;
    std::weak_ptr<A> other;

    public:
        A(const std::string& s) : s(s) { std::cout << "자원 획득" << std::endl; }

        ~A() { std::cout << "소멸자 호출" << std::endl; }

        void set_other(std::weak_ptr<A> o) { other = o; }
        void access_other()
        {
            std::shared_ptr<A> o = other.lock();
            if (o)
            {
                std::cout << "접근 : " << o->name() << std::endl;
            } else {
                std::cout << "이미 소멸" << std::endl;
            }
        }

        std::string name() { return s; }
};

int main()
{
    std::vector<std::shared_ptr<A>> vec;
    vec.push_back(std::make_shared<A>("자원 1"));
    vec.push_back(std::make_shared<A>("자원 2"));

    vec[0]->set_other(vec[1]);
    vec[1]->set_other(vec[0]);

    std::cout << "vec[0] ref count : " << vec[0].use_count() << std::endl;
    std::cout << "vec[1] ref count : " << vec[1].use_count() << std::endl;

    vec[0]->access_other();

    vec.pop_back();
    vec[0]->access_other(); // 접근 실패
}
```

```
자원 획득
자원 획득
vec[0] ref count : 1
vec[1] ref count : 1
접근 : 자원 2
소멸자 호출
이미 소멸
소멸자 호출
```
- set_other 함수는 weak_ptr<A>를 인자로 받고 있었지만 shared_ptr을 전달받았다.
  - 즉 weak_ptr은 생성자로 shared_ptr이나 weak_ptr을 받는 것이다.
  - 이미 제어 블록이 만들어진 객체만이 의미를 가져 평범한 포인터 주소값으로 weak_ptr을 생성할 수 없다.
- weak_ptr 자체로는 원소를 참조할 수없으므로 shared_ptr로 변환해야하며 이 작업은 lock() 함수를 통해 수행한다.
  - weak_ptr이 가리키는 객체가 아직 메모리에 살아있다면 (refcount != 0) 해당 객체를 가리키는 shared_ptr을 반환한다. 이미 해제 되었다면 아무것도 가리키지 않는 shared_ptr을 반환한다.
  - 아무것도 가리키지 않는 shared_ptr은 false로 형변환된다. (if문의 이유)
- 제어 블록은 자원을 사용하고 있는 객체가 소멸된다고해서 사라지지않는다. 해제 하기 위해서는 weak_ptr 역시 0개여야한다.
  - weak count를 제어 블록에서 참조 개수와 더불어 기록한다.