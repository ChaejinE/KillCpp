# Overview
- 범위지정(::), 멤버지정(.), .*(멤버 포인터로 멤버 지정)을 제외한 모든 연산자를 **사용자 정의 연산자**로 사용할 수 있다.
- 직접 사용자가 정의하는 것을 **연산자를 오버로딩한다**라고 부른다.

# "==" 연산자 오버로딩
```
(Return Type) operator(연산자) (연산자가 받는 인자)
```
- 위와 같이 정의하여 연산자를 오버로딩할 수 있다.

```cpp
bool operator(MyString& str);
```

```cpp
bool Mystring::operator==(MyString& str)
{
    return !compare(str);
}
```
- ==라는 것은 return이 bool type일 것이다.
- 이러한 연산자를 맏느다면 str1 == str2 이런 식으로 쉽게 string이 같은지, 다른지를 직관적으로 판별할 수 있게된다.

# 복소수(Complex)로 배우는 연산자 오버로딩
```cpp
#include <iostream>

class Complex
{
    private:
        double real, img;

    public:
        Complex(double real, double img) : real(real), img(img) {}
};
```

![image](https://user-images.githubusercontent.com/69780812/140931198-b05b86af-c4d4-4dd3-b96e-c79fadbbc5ce.png)

![image](https://user-images.githubusercontent.com/69780812/140931263-0e95e361-aff8-4603-a54c-73f456b6a580.png)

![image](https://user-images.githubusercontent.com/69780812/140931307-c25597a7-2551-4c40-b4b7-9fdeea59dc25.png)

- 복소수 사칙연산을 보여주는 그림들이다.
- 오버로딩을 모르고 구현하면 굉장히 복잡할 것이다.
  - 또한 가독성이 떨어지고 무슨 작업을 하려고하는지도 쉽게 알 수 없을 것이다.

```cpp
Complex operator+(const Complex& c) const;
Complex operator-(const Complex& c) const;
Complex operator*(const Complex& c) const;
Complex operator/(const Complex& c) const;
```
- 함수 내부에서 읽기만 수행되고 값이 바뀌지 않는 인자에 대해서는 const 키워드를 붙여주는게 바람직하다.
  - c의 값을 읽기만하지 어떠한 변화도 안주므로 const Complex& 타입으로 받는다.
- 값의 복사 대신 Reference 복사로 속도 저하를 방지해준다.

```cpp
Complex Complex::operator+(const Complex& c) const {
    Complex temp(real + c.real, img + c.img);
    return temp;
}

Complex Complex::operator-(const Complex& c) const {
    Complex temp(real - c.real, img - c.img);
    return temp;
}

Complex Complex::operator*(const Complex& c) const {
    Complex temp(real * c.real - img * c.img, real * c.img + img * c.real);
    return temp;
}

Complex Complex::operator/(const Complex& c) const {
    Complex temp(
        (real * c.real + img * c.img) / (c.real * c.real + c.img * c.img),
        (img * c.real - real * c.img) / (c.real * c.real + c.img * c.img));
    return temp;
}
```
- 위와 같이 Complex의 4칙연산을 구현할 수 있다.

```cpp
int main()
{
    Complex a(1.0, 2.0);
    Complex b(3.0, -2.0);

    Complex c = a * b;

    c.println();
}
```

# 대입연산자 함수
```cpp
Complex& Complex::operator=(const Complex& c) {
    real = c.real;
    img = c.img;
    
    return *this;
}
```
- 한가지 재밌는 사실은 위를 만들지 않더라도 정상 작동한다.
  - 컴파일러 차원에서 디폴트 대입 연산자를 지원하고 있기 때문이다.
  - 디폴트 대입연산자 역시 얕은 복사를 수행하므로 깊은 복사가 필요한 클래스의 경우 대입 연산자 함수를 꼭 만들어줄 필요가 있다.
- 대입 연산 이후 불필요한 복사를 방지하기 위해 Complex&타입을 Return한다.

# 여러 사칙연산 함수
```cpp
Complex& Complex::operator+=(const Complex& c) {
    (*this) = (*this) + c;
    return *this;
}

Complex& Complex::operator-=(const Complex& c) {
    (*this) = (*this) - c;
    return *this;
}
Complex& Complex::operator*=(const Complex& c) {
    (*this) = (*this) * c;
    return *this;
}

Complex& Complex::operator/=(const Complex& c) {
    (*this) = (*this) / c;
    return *this;
}
```
- 위와 같이 확장도 가능하다.
- operator+나 operator=를 정의해 놓았다고 해서 자동으로 적용되는게 아니다.
- **연산자 오버로딩을 하게 되면 생각하는 모든 연산자들에 대해 개별적 정의가 필요해진다.**

# 이항 연산자 오버로딩
## friend 키워드
```cpp
class A
{
    private:
        void private_func() {}
        int private_num;

    friend class B;

    friend void func();
};

class B
{
    public:
        void b() {
            A a;

            a.private_func();
            a.private_num = 2;
        }
};

void func() {
    A a;

    a.private_func();
    a.private_num = 2;
}

int main()
{
    return 0;
}
```
- 다른 클래스나 함수들을 firend로 정의할 수 있다.
  - friend로 정의된 클래스나 함수들은 원래 크래스의 private로 정의된 변수나 함수들에 접근할 수 있다.

```cpp
friend class B;

friend void func();
```
- B와 void 함수 func를 A의 친구라고 선언하고 있다.
- B와 func안에서는 A의 모든 private 멤버 함수들과 변수들에 대한 접근 권한을 부여하게 된다.
- **정말 친한 친구**, But **짝사랑**
  - B, func는 A의 모든것(private 까지)보지만, B안에서 A를 friend라고 지정하지 않는 이상 A는 B의 private에 접근할 수 없다.
## 이항 연산자
```cpp
a = a + "-1.1 + i3.923";  // ①
```

```cpp
a = "-1.1 + i3.923" + a;  // ②
```
- 1의 경우 a.operator+("i3.923");으로 변환 될 수 있다.
- 2의 경우는 그렇지 못하다.
- 원칙적으로 1이 되면 2도 되어야한다.

```cpp
*a.operator@(b);
*operator@(a, b);
```
- 컴파일러는 둘 중 가능한 녀석을 택해 처리한다.
  - 가능한 operator를 찾는 다는 말이다.
- a.operator@(b)에서 operator@는 a 클래스의 멤버 함수로써 사용된다.
- operator@(a, b)에서의 operator@는 클래스 외부에 정의되어 있는 일반적인 함수를 의미하게 된다.


```cpp
friend Complex operator+(const Complex& a, const Complex& b);
```
- Class에 friend keyword로 선언해주면 이 함수에서는 private 멤버 변수들에 접근할 수 있게된다.

```cpp
Complex operator+(const Complex& a, const Complex& b)
{
    Complex temp(a.real + b.real, a.img + b.img);
    return temp;
}
```
- 이런식으로 외부에 함수로 선언하여 사용하면 된다.

```cpp
"-1.1 + i3.923" + a;
```
- operator+(Complex("-1.1 + i3.923), a);로 되어 잘 실행된다.

```cpp
a + a
```
- a.operator+(a)와 operator+(a, a) 둘 중 선택하는지 몰라 에러가난다.
- 통상적으로 자기자신을 리턴하지 않는 이항 연산자들 +, -, *, / 들은 모두 **외부 함수로 선언하는 것이 원칙**이다.
- 반대로 +=, -= 등 자신을 리턴하는 이항연산자들은 **멤버 함수로 선언하는 것이 원칙**이다.

# 입출력 연산자 오버로딩
```cpp
std::cout << a;
```
- std::cout.operator<<(a)를 하는 것과 동일한 명령이다.
- std::cout 객체에 operator<<()가 정의되어 있기 때문이다.
- iostream은 **ostream** header를 include하고 있으며 ostream 클래스에 엄청난 수의 operator<<가 정의되어 있다.
  - 덕분에 편하게 인자 타입에 관계없이 손쉽게 출력을 사용할 수 있는 것이다.
- 실제 사용하고 있는 Class에서 사용하기 위해 ostream 클래스에 Complex객체에 해당하는 연산자 오버로딩을 하기는 어렵다.
  - ostream객체와 Complex 객체 두개를 인자로 받는 **전역 operator<< 함수를 정의**하면 된다.

```cpp
std::stream& operator<<(std::ostream& os, const Complex& c)
{
    os << "( " << c.real << " , " << c.img << " ) ";
    return os;
}
```

```cpp
std::cout << "a value : " << a << "이다." << std::endl;
```
- 위와 같이 선언할 수 있는데 한가지 문제가 있다.
  - operator<<에서 c.real, c.img 멤버에 접근할 수 없다. (Private이므로)
- Complex에 print(std::ostream& os)와 같은 멤버함수를 통해 operator<<에서 호출할 수 있다.

```cpp
friend ostream& operator<<(ostream& os, const Complex& c);
```
- 하지만 friend 키워드로도 해결이 가능하다.
- cin은 istream 객체이고 operator>>를 오버로딩한다.
---
- 무분별하게 friend 키워드를 남발하는 것은 권장하지 않는다.
- private의 **구현 디테일은 최대한 숨긴다.**는 원칙을 지키기 어려워진다.
- 테스트 코드를 작성할 때 friend 코드를 유용하게 사용하는 경우가 종종 있다.
---

# 첨자 연산자 (operator[])
- 만약 String Class를 만든다면 클래스에서 개개의 문자에 접근하기 위해 \[]를 지원해야한다.
- \[]는 몇번째 인지에 대한 첨자를 넣어주는 역할을 한다.

```cpp
char& operator[](const int index);
```
- index로 \[]안에 들어가는 값을 받는다.
- char&로 Return하는 이유는 str[10] = 'c';와 같은 명령을 수행하므로 그 원소의 Reference를 Return하는 것이다.

```cpp
char& MyString::operator[](const int index)
{
    return string_content[index];
}
```
- 이런 식으로 간단히 구현할 수 있겠다.
- index번째 string_content를 return 하여 operator[]를 사용하는 사용자가 이의 Reference를 갖을 수 있게 된다.

# Type 변환 연산자 (Wrapper Class)
```cpp
class Int
{
    int data;

    public:
        Int(int data) : data(data) {}
        Int(const int& i) : data(i.data) {}
};
```
- wrapper : 무언가를 포장하는 이라는 뜻이다.
- 어떤 경우 기본 자료형을 객체로써 다루어야할 때가 있다.
- Int는 int형의 Wrapper 클래스이므로 정확히 똑같이 동작하도록 만들고 싶을 것이다.

```cpp
Int x = 3;
int a = x + 4;
```
- 이를 잘 수행하기 위해 복잡한 연산자들이 생각나겠지만, 이 클래스는 단순히 int형 변수 끼리 하는 일과 정확히 똑같기 때문에 굳이 이미 제공하는 기능을 다시 만들필요가 없다.
- class 객체를 int 형 변수로 변환할 수 있다면 operator+ 등을 정의하지 않고도 컴파일러가 이 객체를 int 형 변수로 변환하여 수행할 수 있을 것이다.

```
operator (변환하고자 하는 Type) ()
```

```cpp
operator int()
```
- 한가지 주의할 점 : 생성자 처럼 함수의 Return Type을 써주면 안된다.

```cpp
operator int() {return data; }
```
- 단순히 data를 Return 해주면 된다.
- 컴파일러 입장에서 적절한 변환 연산자로 operator int를 찾아 int로 변환하여 처리해주기 때문이다.
- 대입시 문제가 발생할 것 같지만 디폴트 대입 연산자가 알아서 처리해준다.

# 전위/후위 증감 연산자
- ++, --로 사용하는 증감 연산자들이다.

```cpp
operator++();
operator--();
```
- 이는 ++x, --x 를 오버로딩한다.

```cpp
operator++(int x);
operator--(int x);
```
- 이는 x++, x--를 오버로딩한 것이다.
- 단순히 컴파일러에서 전위와 후위를 구분하기 위해 int 인자를 넣은 것뿐이다.

```cpp
operator++(int);
operator--(int);
```
- 사실 인자로 들어가는 값을 사용하지는 않는다 그래서 후위 증감연산자 오버로딩은 위와같이 하면된다.
- 주의할 점
  - 전위 증감 연산 : **값이 바뀐 자신을 Return**
  - 후위 증감 연산 : **값이 바뀌기 이전의 객체를 Return**

```cpp
A& operator++() {
    return *this;
}
```
- 전위 연산자는 ++에 해당하는 연산을 수행하고 자기 자시늘 반환해야한다.

```cpp
A operator++(int) {
    A temp(A);

    return temp;
}
```
- ++ 하기전에 객체를 반환해야 하므로 temp 객체를 만들어서 이전 상태 기록 후 ++을 수행한뒤 temp 객체를 반환하게 된다.
- 따라서 **후위 증감 연산의 경우 추가적으로 복사 생성자 호출하므로 전위 증감 연산자 보다 더 느리다.**
