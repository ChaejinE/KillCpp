# fstream
- file stream은 기본적은 istream이나 ostream 클래스보다 더 지원하는 기능이 많다.
  - ifstream, ofstream이  istream, ostream을 상속받아 작성된다.
  - fstream은 ifstream, ofstream을 모두 포함한다.
  - 그러므로 fstream을 사용하면 된다.

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream in("test.txt");
    std::string s;

    if (in.is_open())
    {
        in >> s;
        std::cout << "입력 받은 문자열 :: " << s << std::endl;
    } else {
        std::cout << "file을 찾을 수 없다." << std::endl;
    }

    return 0;
}
```

```
입력 받은 문자열 :: C++공부중
```
- test.txt를 읽어들이는 경로는 같은 파일 레벨이다.
- 파일을 찾을 수 없으면 is_open()을 통해 false를 반환 받을 수 있다.
- cout, cin 사용 시 이미 표준 입출력에 연동되어 있는 상황이다. 파일 입출력의 경우 어느 파일에 입출력해야할지 지정해줘야한다.
  - ifstream 객체에 생성자에 연동하고자하는 파일의 경로를 전달하면 된다.
- cin을 사용하는 것처럼 in 객체를 이용해서 파일로 부터 읽어들일 수 있다.
  - **공백 문자가 나올 때까지 읽어들인 것은 동일하다.**

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream in("test.txt");
    std::string s;

    if (in.is_open())
    {
        in >> s;
        std::cout << "입력 받은 문자열 :: " << s << std::endl;
    } else {
        std::cout << "file을 찾을 수 없다." << std::endl;
    }

    in.close();
    in.open("other.txt");

    if (in.is_open())
    {
        in >> s;
        std::cout << "입력 받은 문자열 :: " << s << std::endl;
    } else {
        std::cout << "file을 찾을 수 없다." << std::endl;
    }

    return 0;
}
```

```
입력 받은 문자열 :: C++공부중
입력 받은 문자열 :: 다른파일
```
- C언어와 달리 fclose를 해주지 않는다.
  - 이미 ifstream 객체의 소멸자에서 자동적으로 close를 해주기 때문이다.
- 다만, close를 해주는 경우도 있는데 같은 객체가 입력을 받기위해 기존 파일 Stream과 연결을 종료하고 새로운 파일과 연결하는 경우다.
  - 새로운 파일과의 연결은 open이 해주는 것을 알 수 있다.
  - ifstream 객체 생성자에 파일 경로를 바로 지정할 필요는 없고 나중에 open 으로 파일을 열어도 상관없다.

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream in("test.txt", std::ios::binary);
    std::string s;

    int x;
    if (in.is_open())
    {
        in.read((char*)(&x), 4);
        std::cout << "입력 받은 수 :: " << std::hex << x << std::endl;
    } else {
        std::cout << "file을 찾을 수 없다." << std::endl;
    }

    return 0;
}
```

```
입력 받은 수 :: ea2b2b43
```

![image](https://user-images.githubusercontent.com/69780812/141644265-a267f50f-1ef4-4ce4-b862-d15ddc164b99.png)
- hex editor로 봤을 때, 첫 부분만 일치한다.
  - **리틀 엔디안**이라는 것을 감안해야 한다. 우리가 쓰는 CPU의 경우 리틀 엔디안이어서 높은 주소 값에 높은 자리수가 온다.
  - EA / 2B / 2B / 43이 43 / 2B / 2B / EA로 변수에 기록된 것이다.
- ifstream 객체 생성 시 생성자에 옵션으로 binary 형태로 받는 다는 것을 명시할 수 있다.
  - 이 말은 문자열 데이터를 받는 것이 아니라 그냥 이진 그대로의 값을 받겠다는 의미다.
  - 아무것도 명시하지 않으면 문자열 그대로 받는다.
  - binary는 단순한 숫자로 ios에 정의되어 있다. OR 해서 여러 옵션을 조합할 수 있다.
- read() 함수는 말 그대로 4byte 내용을 읽으라는 의미다.
  - 첫 번째 인자 : 해당 버퍼, int 변수를 마치 4 Byte 짜리 char 배열이라 생각해서 위와 같이 전달한 것이다.
  - 두 번쨰 인자 : 몇 바이트를 읽을지 전달한다.

```cpp
char x[10];
in.read(x, 10);
```
- 예시 코드처럼 int 공간에 저장하는 경우는 없고 char 배열에 크기를 지정해서 읽어들이면 된다고한다.

## 파일 전체 읽기
```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream in("test2.txt");
    std::string s;

    if (in.is_open())
    {   
        // 위치 지정자를 파일 끝으로 옮긴다.
        in.seekg(0, std::ios::end);

        int size = in.tellg(); // 그 위치를 읽는다. (파일 크기)

        s.resize(size); // 그 크기의 문자열을 할당.

        in.seekg(0, std::ios::beg); // 위치 지정자를 다시 파일 맨 앞으로 옮긴다.

        in.read(&s[0], size); // 파일 전체 내용을 읽어 문자열에 저장.
        std::cout << s << std::endl;
    } else {
        std::cout << "file 찾을 수 없다." << std::endl;
    }

    return 0;
}
```

```
이렇게하면 되려나
맞는가 ?
아무튼 긴내용 적긴 힘들듯
```
- seekfg()과 같은 함수로 파일 위치 지정자를 사용자의 입맛에 맞게 이리저리 움직일 수 있다.
  - 두번째 인자 : 파일 내 위치
  - 첫번째 인자 : 그 위치로 부터 얼마나 떨어져있느냐를 의미
  - 위의 경우 위치 지정자를 파일의 끝에서 0만큼 떨어진 것. 즉 파일의 끝으로 이동시킨 것이다.
  - C의 fseek 함수와 같다.
- tellg() 함수는 위치 지정자의 시작점으로 부터의 위치를 반환한다.
  - 위의 경우 파일의 크기를 반환받는다.
  - 문자열에 그 만큼의 크기를 할당해주는 용도다.
- 파일을 읽을 때, 파일 위치 지정자를 다시 처음으로 옮겨줘야한다.
  - 그래야 내용들을 처음부터 읽을 수 있다.

## 파일 전체를 한 줄씩 읽기
```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream in("test2.txt");
    char buf[100];

    if (!in.is_open()) {
        std::cout << "file 찾을 수 없다." << std::endl;

        return 0;
    }

    while (in) {
        in.getline(buf, 100);
        std::cout << buf << std::endl;
    }

    return 0;
}
```

```
이렇게하면 되려나
맞는가 ?
아무튼 긴내용 적긴 힘들듯
```
- getline()함수는 ifstream 객체 멤버 함수다.
  - "\n" 개행문자가 나올 때까지 **최대 지정한 크기 -1**만큼 읽게된다.
  - 하나 적게 읽는 이유는 buf의 맨 마지막 문자로 NULL 종료 문자를 넣어줘야 하기 때문이다.
  - 위 경우 buf에 최대 99 글자 까지 입력 받는 것이다.
  ```cpp
  in.getline(buf, 100, '.');
  ```
  - 물론, 지정한 문자가 나올 때까지 읽는 것으로 바꿀 수 있으며 원하는 문자를 인자로 전달해주면 될 것이다.

- ifstream에 자기 자신을 bool로 casting 할수 있는 연산자가 오버로딩 되어있다. (operator bool())
  - 그래서 while 문 조건에 in을 전달하면 bool로 캐스팅하는 연산자 함수가 호출되어 사용이 가능해진다.
  - in이 true이기 위해서는 **다음 입력 작업이 성공적**이어야만한다. 현재 Stream에 오류 플래그가 켜져있지 않아야한다.
  - getline()함수에서는 개행 또는 지정한 문자가 나오기 전에 지정한 버퍼의 크기가 다차게되면 failbit을 키게되므로 **buffer의 크기를 너무 작게 만들면 정산적으로 데이터를 받을 수 없을 것**이다.
  - getline() 사용 전 이와 같은 조건을 꼭 확인하자.

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream in("test2.txt");

    if (!in.is_open())
    {
        std::cout << "file 못 찾음" << std::endl;

        return 0;
    } else {

        std::string s;
        while (in)
        {
            getline(in, s);
            std::cout << s << std::endl;
        }
    }

    return 0;
}
```
- 위와 같이 getline()의 한계점을 극복하기 위해 std::string에서 getline 함수를 제공하고 있다.
  - 첫 번째 인자 : istream 객체
  - 두 번째 인자 : 입력받은 문자열을 갖는 string 객체
- ifstream의 getline 활용 보다 훨씬 편하다.
  - 굳이 버퍼의 크기를 지정하지 않아도 개행 문자 혹은 파일 에 끝이 나올 때까지 입력받게 된다.
- while 과 같은 조건에 in 과 같은 stream 객체 자체를 전달하자. in.eof()와 같은 함수를 사용하면 파일 읽기가 안전하다는 것을 보장하지 않는다.
  - in.eof()는 EOF(파일끝)을 읽어야 true를 내뱉으므로 data를 넣는 타이밍이 잘 못될 수 있다.

## 파일에 쓰기
```cpp
#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ofstream out("write.txt");

    std::string s;
    if (out.is_open())
    {
        out << "쓰여질 내용";
    }

    return 0;
}
```
- write.txt 파일이 존재하지 않을 경우 생성한 뒤 생성에 성공하면 출력하게 된다.
- ofstream은 **열려있는 파일이 존재하지 않으면 해당 파일을 생성하고 열게된다.**
- 만일 이미 존재하는 파일이라면 내용이 덮어씌워진다.
  - 이전 내용은 다 날라간다.

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ofstream out("write.txt", std::ios::app);

    std::string s;
    if (out.is_open())
    {
        out << "덧붙여서 쓸 내용";
    }

    return 0;
}
```
- out 객체 생성시, app을 주게 되면 파일에 stream 연결 시 기존 파일 내용에 덧붙여서 내용을 쓴다.
- binary, app말고도 4개가 더있다.
  - ios::ate : 자동으로 파일 끝에서 부터 읽기와 쓰기 실시. 즉, 파일 열 때 위치 지정자가 파일 끝을 가리키고 있게 된다.
  - ios:trunc : 파일 Stream을 열면 기존 있던 내용들이 모두 지워진다. ofstream 객체의 deafult다.
  - ios::in, std::ios::out : 파일에 입력을 할 지, 출력을 할지 지정한다. ifstream, ofstream 객체 생성 시 각각 이미 설정되어있다.
- ios::ate의 경우 ios::app과 비슷해 보이지만 ios::app의 경우, 원본 파일의 내용을 무조건적으로 보장하는데 비해 ate는 위치 지정자를 그 이전으로 옮길 수 있다.
  - app은 끝이 시작점이라고 생각하고 동작한다.
  - ate는 기존 파일을 포함해서 움직인다. (거의 사용할일이 없다.)

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ofstream out("write_app.txt", std::ios::app);
    std::ofstream out2("write_ate.txt", std::ios::ate);

    out.seekp(3, std::ios::beg);
    out2.seekp(3, std::ios::beg);
    
    out << "추가";
    out2 << "추가";
}
```
- 파일 데이터 한 가운데에 무언가를 끼워넣는 예제다.
- app 사용 시 바로 "추가"문자열이 붙는다.
- ate 사용 시 3 칸 띄워진 곳에 "추가"라고 문자열이 출력되고, 기존 데이터는 지워졌다.
  - 즉, stream 위치 지정자는 움직여서 3칸 뒤에 출력되지만 기존에 써져있던 내용은 모두 지워진다.
  - **ate는 기존 파일의 내용 보존을 보장하지 않는다.**

## std::ofstream 연산자 오버로딩
```cpp
#include <iostream>
#include <fstream>
#include <string>

class Human
{
    std::string name;
    int age;

    public:
        Human(const std::string& name, int age) : name(name), age(age) {}
        std::string get_info()
        {
            return "Name : " + name + " / Age :: " + std::to_string(age);
        }

    friend std::ofstream& operator<<(std::ofstream& o, Human& h);
};

std::ofstream& operator<<(std::ofstream& o, Human& h)
{
    o << h.get_info();
    return o;
}

int main()
{
    std::ofstream out("test3.txt");

    Human h("Chaejin", 60);
    out << h << std::endl;

    return 0;
}
```
- ofstream 객체의 reference를 받고, 다시 이를 return 하는 operator<< 함수를 정의해주면 된다.

