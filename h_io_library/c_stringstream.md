# 문자열 스트림
```cpp
#include <iostream>
#include <sstream>

int main()
{
    std::istringstream ss("123");
    int x;

    ss >> x;

    std::cout << "input data : " << x << std::endl;

    return 0;
}
```

```
input data : 123
```
- std::istringstream : 문자열을 하나의 stream으로 생각하게 해주는 가상화 장치라고 보면된다.
  - sstream에 정의되어있다.
  - 마치 파일에 123이라 기록해놓고 거기서 입력받는 것과 동일하다.

```cpp
#include <iostream>
#include <sstream>
#include <string>

double to_number(std::string s)
{
    std::istringstream ss(s);
    double x;

    ss >> x;
    return x;
}

int main()
{
    std::cout << "변환 1 + 2 = " << to_number("1") + to_number("2") << std::endl;
}
```

```
변환 1 + 2 = 3
```
- 이를 활용하면 atoi와 같은 함수를 사용할 필요 없이 간편하게 문자열에서 숫자로 변환하는 함수를 만들 수 있다.
- 거꾸로 데이터를 출력할 수 있는 std::ostringstream이 있다. 비슷한 방법으로 거꾸로 숫자에서 문자열로 바꾸는 함수를 제작할 수 있을 것이다.

```cpp
#include <iostream>
#include <sstream>
#include <string>

std::string to_string(double n)
{
    std::ostringstream ss;

    ss << n;
    
    return ss.str();
}

int main()
{
    double n = 2.;

    std::cout << "to_string : " << to_string(n) << std::endl;
    std::cout << "type : " << typeid(to_string(n)).name() << std::endl;
}
```

```
to_string : 2
type : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
```
- str() 함수로 현재 문자열 스트림에 있는 값을 불러오기만 하면 된다.