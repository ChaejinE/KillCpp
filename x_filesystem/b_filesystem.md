# file search
```cpp
#include <filesystem>
#include <iostream>

int main() {
  std::filesystem::path p("./some_file");

  std::cout << "Does " << p << " exist? [" << std::boolalpha
            << std::filesystem::exists(p) << "]" << std::endl;
  std::cout << "Is " << p << " file? [" << std::filesystem::is_regular_file(p)
            << "]" << std::endl;
  std::cout << "Is " << p << " directory? [" << std::filesystem::is_directory(p)
            << "]" << std::endl;
}
```

```
Does "./some_file" exist? [true]
Is "./some_file" file? [true]
Is "./some_file" directory? [false]
```
- g++로 컴파일 하는 사람들은 꼭 8버전 이상의 컴파일러가 설치되어있어야 filesystem을 사용할 수 있다.

```
g++-9 ex1.cpp -o ex1 --std=c++17
```
- 필요에 따라 -lstdc++fs를 추가해야할 수 있다.

# path
```cpp
std::filesystem::path p("./some_file");
```
- path : 컴퓨터 상 존재하는 해당 파일의 위치를 나타내는 고유 주소
  - 가장 맨 첫 번째 디렉토리 부터 순차적으로 찾아가므로 주소가 아니라 경로라고 부른다.
- 경로를 지정하는 방식에는 2가지가 있다.
  - 절대경로 : 가장 최상위 디렉토리(root directory)에서 내가 원하는 파일까지의 경로를 의미한다.
  - 상대경로 : 현재 프로그램이 실행되고 있는 위치에서 해당 파일을 찾아가는 경로
- filesystem 라이브러리는 파일이나 디렉토리를 다루는 모든 함수들은 파일을 나타내기 위해 path 객체로 인자를 받는다.
  - 1. 원하는 경로에 있는 파일/디렉토리의 path 정의
  - 2. 해당 path로 파일/디렉토리 정보 수집
  - 위 순서대로 작업을 하게 된다.
- path 클래스는 경로를 나타낼 뿐 실제 파일을 지칭하는 게 아니므로 실제로 존재하는지 알아보렴녀 exists 함수를 사용해야한다.
  - 파일이 존재하면 true라고 표시된다.
- 해당 위치에 있는 것이 파일인지 디렉토리인지를 알아내기 위해서는 is_regular_file or is_regular_directory 함수로 확인할 수있다.
- 리눅스 상 주변 장치(device)나 소켓(socket)들도 다 파일로 취급해서 regular 파일이라고한다.

# 경로 관련함수들
```cpp
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./some_file");

    std::cout << "current path : " << fs::current_path() << std::endl;
    std::cout << "relative path : " << p.relative_path() << std::endl;
    std::cout << "absolute path : " << fs::absolute(p) << std::endl;
    std::cout << "official abs path : " << fs::canonical(p) << std::endl;
}
```

```
current path : "/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem"
relative path : "./some_file"
absolute path : "/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/./some_file"
official abs path : "/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/some_file"
```
- current_path() : 프로그램 실해되는 경로를 return
  - 모든 상대경로가 이 프로그램의 현재 실행 경로를 기반해서 구해진다.
- absolute() 함수는 절대경로로 바꿔주기는 하지만 . 이나 ..와 같은 불필요한 요소들을 포함할 수 있어 canonical() 함수를 사용하면 깔끔하게나온다.
  - canonical() : 해당 파일의 경로를 가장 짧게 나타낼 수 있는 공식적 절대경로를 제공
- 위 모든 함수는 경로에 파일이 존재하지 않으면 예외를 throw 한다.
  - 호출 전 exists를 통해 파일이 존재하는지 확인해야한다.

# 디렉토리 관련 작업들
- 해당 디렉토리에서
  - 파일/폴더들 살펴보기
  - 폴더 생성하기
  - 파일/폴더 복사하기
  - 파일/폴더 삭제하기

## 디렉토리 모든 파일들 순회
```cpp
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::directory_iterator itr(fs::current_path() / "a");
    while (itr != fs::end(itr))
    {
        const fs::directory_entry& entry = *itr;
        std::cout << entry.path() << std::endl;
        itr++;
    }
}
```

```
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/1.txt"
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/2.txt"
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/3.txt"
```
- 디렉토리에 쉽게 접근하기 위해 filesystem에서는 directory_iterator라는 반복자를 제공한다.
- 기존 vector와 같은 컨테이너에서 반복자 정의를 위해 v.begin() 처럼 컨테이너 자체의 함수를 통해 정의했지만 직접 반복자를 정의하진 않았다.
- directory_iterator는 특이하게 반복자 자체를 스스로 정의해야하고, 생성자에 탐색할 경로를 전달해줘야한다.

```cpp
fs::current_path() / "a"
```
- path에는 operator/가 정의되어있어 편리하게 디렉터리를 추가할 수 있으며 매우 직관적이다.

```cpp
while(itr != fs.end(itr))
```
- end 함수에 현재 반복자를 전달하면 해당 반복자의 끝을 얻어낼 수 있다.
- 그리고 각각 반복자들은 디렉토리에 정의되어 있는 개개의 파일을 나타내는 directory_entry를 가리키고 있다.
  - directory_entry에는 파일 이름이나 크기 등등의 정보들이 저장되어 있다.

```cpp
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    for (const fs::directory_entry& entry : 
         fs::directory_iterator(fs::current_path() / "a"))
    {
        std::cout << entry.path() << std::endl;
    }
}
```
- range for 를 사용하면 더욱 간단히 작성할 수 있다.
- directory_iterator의 단점은 b 폴더 안에 여러 파일이 있어도 이들은 순회 대상에서 제외되는 것이다.
- 디렉토리 안에 서브 디렉토리까지 모두 순회할 수 있는 반복자를 사용하고 싶으면  recursive_directory_iterator를 사용하면 된다.

```cpp
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(fs::current_path() / "a"))
    {
        std::cout << entry.path() << std::endl;
    }
}
```

```
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/b"
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/b/5.txt"
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/b/4.txt"
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/b/6.txt"
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/1.txt"
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/2.txt"
"/home/cjlotto/git_clone/KillCpp/x_filesystem/Practice_filesystem/a/3.txt"
```