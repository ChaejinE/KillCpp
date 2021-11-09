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