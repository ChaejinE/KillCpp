# Overview
- C++ 표현식의 경우 두 가지 카테고리로 구분할 수 있다.
  - 1. 이 구문이 어떤 타입을 가지는가
  - 2. 어떠한 종류의 값을 가지는가

```cpp
int a = 3;
```
- 'a'는 메모리 상에서 존재하는 변수이다. 즉, 'a'의 주소값을 & 연산자를 통해 알아낼 수 있다.
- 보통 이렇게 주소값을 취할 수 있는 값을 **l-value**라고 부른다.
  - 좌측값은 어떠한 표현식의 왼쪽 오른쪽 모두 올 수 있다.
- '3'은 주소값을 취할 수 없다. 연산할 때만 잠깐 존재할 뿐 위 식이 연산되고 나면 사라지는 값이다.
  - 즉, '3'은 실체가 없는 값인 것이다.
- 보통 주소값을 취할 수 없는 값을 **r-value**라고 부른다.
  - 우측값은 항상 식의 오른쪽에만 와야한다.

# l-value
```cpp
int a;
int& l_a = a;
int& r_b = 3;
```
- 우리가 다뤄왔던 레퍼런스는 "좌측 값"에만 레퍼런스를 가질 수 있다.
- 'a'의 경우 좌측 값이기 떄문에 'a'의 좌측값 레퍼런스인 l_a를 만들 수 있다.
- 반면, 3의 경우 우측값이므로 우측값 레퍼런스를 만들 수 없다.
- & 하나를 이용해 정의하는 Reference를 **l-value reference**라고 부른다.
  - 좌측값 레퍼런스 자체도 좌측값이 된다.

# Reference
```cpp
int& func1(int& a) { return a; }
int func2(int b) { return b; }

int main() {
  int a = 3;
  func1(a) = 4;
  std::cout << &func1(a) << std::endl;

  int b = 2;
  a = func2(b);               // 가능
  func2(b) = 5;               // 오류 1
  std::cout << &func2(b) << std::endl;  // 오류 2
}
```
- func1의 경우 레퍼런스의 값을 4로 해라는 으미ㅣ로 실제 변수 값이 바뀌게 된다.
- func2의 경우 레퍼런스가 아닌 일반적인 int 값을 리턴하고 있다.
- func2(a) = 5 문장 실행시 int 값이 잠깐 존재할 뿐 그 문장 실행이 끝나면 사라진다.
  - 우측값이 왼쪽에 오는 경우는 가능하지 않다.
- 또한 우측값의 주소값을 취할 수 없어 마지막 문장은 허용되지 않는다.
- 다만, const T& 의 타입의 한해서만 우측값도 레퍼런스로 받을 수 있다.
  - const reference는 임시로 존재하는 객체의 값을 참조만 할 뿐 이를 변경할 수 없기 때문이다.

# move
![image](https://user-images.githubusercontent.com/69780812/143879250-2f40177e-46cc-4fc3-a227-7f1a4f34177a.png)
- 기존 문자와 다른 문자가 합쳐지는 operator+가 정의된 클래스가 있다고 해보자.
- Mystring str3 = str1 + str2를 하고, 새로운 str을 return한다고 해보자.
  - 새로운 string 객체가 return 되므로 str3의 복사생성자를 호출하게된다.
  - 이는 필요가 없는 복사생성자 호출의 경우다.
- 위처럼 str3이 생성 시 임의로 생성된 객체의 string_content를 가리키는 문자열 주소값을 str3의 string_content로 해주면 된다.
- 문제는 위처럼하면 임시 객체 소멸 시 메모리 해제가 되는데, str3이 가리키던 문자열에 메모리에서 소멸되게 된다.
  - 따라서 임시 생성된 객체의 string_content를 nullptr로 바꿔주고, 소멸자에서 string_content가 nullptr이면 소멸하지 않도록 해주면 된다.
  - 하지만 const로 받았다면 그 값을 바꿔줄 수 없어 이것도 해결책이 안된다.
- 이와 같은 문제가 발생한 이유는 const MyString&이 좌측값 우측값 모두 받을 수 있다는 점에서 비롯된다.
  - C++은 우측값만 특이적으로 받을 수 있는 방법을 제시한다.
  - r-value reference

```cpp
MyString::MyString(MyString&& str) {
  std::cout << "이동 생성자 호출 !" << std::endl;
  string_length = str.string_length;
  string_content = str.string_content;
  memory_capacity = str.memory_capacity;

  // 임시 객체 소멸 시에 메모리를 해제하지
  // 못하게 한다.
  str.string_content = nullptr;
}
```
- r-value reference는 & 두 개를 사용해서 정의한다.
- str 자체는 좌측값이다. 실체가 있기 때문이다.
  - str은 타입이 MyString의 우측값 레퍼런스인 좌측값이라고 보면된다.
  - 따라서 표현식의 좌측에 올 수도 있다. (마지막 줄)
- 기존 복사 생성자의 경우 문자열 전체를 새로 복사해야 했지만 이동 생성자의 경우 단순히 주소값 하나만 달랑 복사해주면 끝나 매우 간단하다.
  - 한가지 중요한 부분은 인자로 받은 임시객체가 소멸되면서 자신이 가리키고 있던 문자열을 delete 하지 못하게 해야한다.
  - 따라서 str의 string_content를 nullptr로 바꿔준다.

```cpp
Mystring::Mystring()
{
    if (string_content) delete[] string_content;
}
```
- 소멸자 역시 string_content가 nullptr이 아닐 때에만 delete 하도록 바꿔주면 된다.

```cpp
int a;
int& l_a = a;
int& ll_a = 3; // 불가능

int&& r_b = 3;
int&& rr_b = a; // 불가능
```
- 우측값 레퍼런스의 경우 반드시 우측값 레퍼런스만 가능하다.
- 따라서, r_b의 경우 우측값 '3'의 레퍼런스가 될 수 있겠지만 rr_b의 경우 a가 좌측값이므로 컴파일 되지 않는다.
- 한 가지 특징은 임시객체가 소멸되지 않도록 부들고 있다는 점이다.

```cpp
MyString&& str3 = str1 + str2;
str3.println();
```
- str3이 str1 + str2 에서 리턴되는 임시 객체의 레퍼런스가 되면서 그 임시 객체가 소멸되지 않도록 해야된다.