# protected
- 상속을 받더라도 사실 private 멤버 변수들은 그 어떤 경우에서도 자기 class 말고는 접근할 수 없다.
- 종종 파생 클래스에서 원래 기반의 클래스의 데이터에 직접 접근할 필요성이 있다.
- **상속 받는 클래스에서는 접근 가능하고 그 외의 기타 정보는 접근 불가능** 이라는 키워드가 있다.
  - **protected**
```cpp
#include <iostream>
#include <string>

class Base {
 protected:
  std::string parent_string;

 public:
  Base() : parent_string("기반") {  std::cout << "기반 클래스" <<  std::endl; }

  void what() {  std::cout << parent_string <<  std::endl; }
};
class Derived : public Base {
  std::string child_string;

 public:
  Derived() : Base(), child_string("파생") {
     std::cout << "파생 클래스" <<  std::endl;

    // 그렇다면 현재 private 인 Base 의
    // parent_string 에 접근할 수 있을까?
    parent_string = "바꾸기";
  }

  void what() {  std::cout << child_string <<  std::endl; }
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
- public : 형태로 상속했다면 접근 지시자들에 영향없이 그대로 작동
- 파생 클래스 입장에서 public은 그대로 public이고, protected, private도 그대로
- protected로 상속하면 파생 클래스 입장에서 public은 protected로 바뀌고 나머지는 유지된다.
- private : 모든 접근 지시자들이 private이 된다.