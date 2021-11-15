# Overview
- Vector 클래스를 예로 들어보자.
- 한 가지 문제가 있는데, 우리가 담으려는 데이터 타입이 바뀔 때마다 다른 벡터를 만들어 주어야 한다.
  - 생성자를 계속 만들어 주거나 바꿔줘야 할 것이다.
- 이렇게 다른건 동일한데, 타입만 바꿔줘야하는 경우 T라는 임의의 변수로 설정해 제 떄에 맞게 작동하게 하고 싶을 수 있다.
  - T라는 임의 변수를 쓴다면 편리할 것이다 !

# C++ Template
- Template은 쉽게 말해 어떤 물건을 찍어내는 틀이라고 생각하면 된다.
- 사용자(프로그래머)가 원하는 타입을 넣어주면 알아서 코드를 찍어내는 틀이다.

```cpp
#include <iostream>
#include <string>

template <typename T>
class Vector
{
    T* data;
    int capacity;
    int length;

    public:
        Vector(int n = 1) : data(new T[n]), capacity(n), length(0) {}

        void push_back(T s)
        {
            if (capacity <= length)
            {
                T* temp = new T[capacity * 2];
                for (int i = 0; i < length; i++)
                    temp[i] = data[i];
            
                delete[] data;
                data = temp;
                capacity *= 2;
            }

            data[length] = s;
            length++;
        }

        T operator[](int i) { return data[i]; }

        void remove(int x)
        {
            for (int i = x + 1; i < length; i++)
            {
                data[i - 1] = data[i];
            }
            length--;
        }

        int size() { return length; }

        ~Vector()
        {
            if (data)
                delete[] data;
        }
};

int main()
{
    Vector<int> int_vec;
    int_vec.push_back(3);
    int_vec.push_back(2);

    std::cout << " int vector " << std::endl;
    std::cout << " first elem : " << int_vec[0] << std::endl;
    std::cout << " second elem : " << int_vec[1] << std::endl;

    Vector<std::string> str_vec;
    str_vec.push_back("hello");
    str_vec.push_back("world");
    std::cout << "string vector" << std::endl;
    std::cout << " first elem : " << str_vec[0] << std::endl;
    std::cout << " second elem : " << str_vec[1] << std::endl;

    return 0;
}
```

```
 int vector 
 first elem : 3
 second elem : 2
string vector
 first elem : hello
 second elem : world
```
- 클래스 상단 templat이 정의된 부분을 보자.

```cpp
template <typename T>
```
- 정의되는 클래스에 대해 템플릿을 정의한 것이다.
- 템플릿 인자 : T, 반드시 어떠한 타입의 이름임을 명시하고 있다.
- 템플릿 문장 아래에 오는 것이 class냐 function이냐에따라 클래스 템플릿, 함수 템플릿으로 나뉜다.

```cpp
template <class T>
```
- 위와 같이 쓰는 경우도 있다.
- 정확히 **typename T**와 동일한 것이다.
  - 되도록 typename 키워드를 사용하기를 권장한다.

```cpp
Vector<int> int_vec;
```
- 위 처럼 \<>안에 전달하려는 것을 명시해주면 된다.
- 위 경우 T에 int가 전달될 것이다.
- 이 떄까지 객체 or 값을 전달해봤지만 Type그 자체를 전달한 적은 없었다. **템플릿을 통해 타입을 전달할 수 있다 !**
- Vector의 T가 int로 치환된 클래스의 객체 int_vec을 생성하게 된다.
- 클래스 템플릿에 인자를 전달해서 실제 코드를 생성하는 것을 **클래스 템플릿 인스턴스화**(Class Template Instantiation)이라고한다.

```cpp
Vector<bool> boo_vec;
```
- C++에서 기본으로 처리하는 단위가 1 byte라는 점을 상기하자.
- bool은 1개 비트만으로 충분하지 않은가 ? 8비트는 낭비다. 
  - 따로 처리가 필요하다.

# Template Specialization (템플릿 특수화)
- 일부의 경우 따로 처리하는 것을 템플릿 특수화라고 한다.
- 특정 매개변수에 대해 별도 처리를 하고 싶은 경우 특수화 개념을 이용한다.
- 특정 매개변수 형에 대해서만 특수화된 템플릿 함수가 호출되는 것이다.

```cpp
template <typename A, typename B, typename C>
class test{};
```
- A가 int, C가 double일 때 따로 처리하고 싶다고 가정해보자.

```cpp
template <typename B>
class test<int, B ,double> {};
```
- 특수화 하고 싶은 부분에 원하는 타입을 전달하고 위에는 일반적인 템플릿을 쓰면된다.
- B도 특수화하고 싶으면 동일하게 하면 된다.
  - 단, template<>은 남겨놔야한다.

```cpp
template<>
class Vector<bool>
{
    ...
}
```
- 1개 int는 4바이트 이므로 32개의 bool 데이터를 한데 묶어 저장할 수 있다.
- 이를 이용하면 bool을 메모리를 아끼며 효율적으로 저장할 수 있다.
  - remove, push_back 등을 비트 연산으로 처리할 수 있다.
  - 구현이 까다로우므로 나중에 보자.
  - vector class도 bool을 이러한 template spcialization으로 처리하고 있다.

# Function template
```cpp
#include <iostream>
#include <string>

template <typename T>
T max(T& a, T& b)
{
    return a > b ? a : b;
}

int main()
{
    int a = 1, b = 2;

    std::cout << a << ", " << b << " = " << max(a, b) << std::endl;
}
```

```
1, 2 = 2
```
- 신기하게도 <>하는 부분이 없다.
- max<int>(a, b)라고 써야하는 것 아닌가 ?
  - 컴팡일러가 똑똑하게도 위와 같이 해석해준다.
  - string끼리의 연산도 알아서 저렇게 처리해준다.
- **Class를 인자로 할 경우 주의할 것이 있다.**
  - 해당 객체가 구현한 함수를 갖고 있지 않으면 컴파일 에러가 난다.
  - Template은 컴파일 시 실제 코드로 변환하여 실행한다. (에러 난다.)
  - Template 인스턴스화를 이용한 **템플릿 메타프로그래밍**이라는 것도 있다.

# Function Object
```cpp
struct Comp1
{
    bool opeator()(int a, int b) { return a > b; }
};
```
- Comp1은 아무것도 하지 않고 단순히 operator만 정의하고 있다.

```cpp
template<typename Cont, typename Comp>
void bubble_sort(Cont& cont, Comp& comp)
{
    ...
    if(!comp(cont[i], cont[j])))
    {
        ...
    }
}
```
- 위에서 comp객체는 함수인냥 사용이된다.
- **함수는 아니지만 함수인 척을 하는 객체를 함수 객체라고한다.**
  - **Functor**
- 실제 C++ 표준라이브러리 sort함수도 비교 클래스를 받지 않는 경우와 비교 클래스를 받는 경우로 오버로딩되어 있다.
  - Functor는 operator() 자체를 인라인화 시켜 매우 빠른 작업을 수행한다고 한다.

# Type이 아닌 템플릿 인자 (non-Type template arguments)
```cpp
#include <iostream>

template <typename T, int num>
T add_num(T t)
{
    return t + num;
}

int main()
{
    int x = 3;
    std::cout << "x : " << add_num<int, 5>(x) << std::endl;
}
```
- Type 말고도 마치 함수의 인자 처럼 값을 전달 받을 수 있다.
- int add_num(int) {return t +5;} 와 같이 작성 했다면 <>를 작성하지 않아 컴파일 타임오류가발생하게된다.
  - num에 뭐가 들어갈지 모르기 때문
- int num = 5; 처럼 디폴트 인자를 지정할 수 있다. **(디폴트 템플릿 인자)**
  - 그러면 add_num(x)가 가능해진다.

```cpp
#include <iostream>
#include <array>

int main()
{
    std::array<int , 5> arr ={1, 2, 3, 4, 5};

    for (int i = 0; i < arr.size(); i ++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
```
- 타입이 아닌 템플릿 인자를 가장 많이 활용하는 예시는 **컴파일 타임에 값들이 정해져야 한다는 것이다.**
  - 특히, 배열이라고 볼 수 있다.
  - 배열의 크기는 컴파일 타임에 정해지니 배열의 크기를 명시하면 크기에 대한 정보를 잃어버리지 않을 것이다.

```cpp
#include <iostream>
#include <array>

template <typename T>
void print_array(const T& arr) {
  for (int i = 0; i < arr.size(); i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::array<int, 5> arr = {1, 2, 3, 4, 5};
  std::array<int, 7> arr2 = {1, 2, 3, 4, 5, 6, 7};
  std::array<int, 3> arr3 = {1, 2, 3};

  print_array(arr);
  print_array(arr2);
  print_array(arr3);
}
```
- 위 처럼 array를 받는 경우 arr은 std::array<int, num> 그자체가 타입이다.