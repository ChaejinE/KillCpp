# copy eilision
```cpp
#include <iostream>

class A
{
    int data_;

    public:
        A(int data) : data_(data) { std::cout << "일반 생성자 호출" << std::endl; }

        A(const A& a) : data_(a.data_)
        {
            std::cout << "복사 생성자 호출" << std::endl;
        }
};

int main()
{
    A a(1);
    A b(a);

    A c(A(2));
}
```

```
일반 생성자 호출
복사 생성자 호출
일반 생성자 호출
```
- 마지막은 예상과 다를 수 있다.
- 복사 생성자가 왜 불리지 않았을까?
- 굳이 임시객체를 한 번만들고 이를 복사 생성할 필요가 없다.
  - 똑똑한 컴파일러는 복사 생성을 굳이 수행하지 않고, 임시로 만들어진 A(2) 자체를 c로 만들어 버린다.
- 컴파일러 자체에서 복사를 생략해 버리는 작업을 복사 생략 (copy elision)이라고 한다.
- 하지만, 컴파일러가 복사 생략을 해도 되는 경우에도 복사 생략을 하지 않을 수도 있다.
