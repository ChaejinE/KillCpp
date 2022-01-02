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

# 디렉터리 생성
```cpp
std::ofstream out("a.txt");
out << "hi;
```
- ofstream 라이브러리를 사용하면 파일을 간단하게 생성할 수 있다.
  - a.txt라는 파일이 존재하지 않을 시 파일이 생성되며 내용이 작성된다.
  - 그런데 디렉토리를 생성할 수 있는 라이브러리는 아니다.

```cpp
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./a/c");
    std::cout << p << "exist ? [" << std::boolalpha << fs::exists(p)
              << "]" << std::endl;

    fs::create_directory(p);

    std::cout << p << "exist ? [" << fs::exists(p) << "]" << std::endl;
    std::cout << p << "directory ? [" << fs::is_directory(p) << "]" << std::endl;
}
```

```
"./a/c"exist ? [false]
"./a/c"exist ? [true]
"./a/c"directory ? [true]
```
- create_directory 함수는 주어진 경로를 인자로 받아 디렉토리를 생성한다.
  - **생성하는 디렉토리의 부모디렉토리는 반드시 존재**하고 있어야한다.

```cpp
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./c/d/e/f");
    std::cout << p << "exist ? [" << std::boolalpha << fs::exists(p)
              << "]" << std::endl;

    fs::create_directories(p);

    std::cout << p << "exist ? [" << fs::exists(p) << "]" << std::endl;
    std::cout << p << "directory ? [" << fs::is_directory(p) << "]" << std::endl;
}
```

``` 
"./c/d/e/f"exist ? [false]
"./c/d/e/f"exist ? [true]
"./c/d/e/f"directory ? [true]
```

# 파일과 폴더 복사/삭제하기
```cpp
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    fs::path from("./a");
    fs::path to("./c");

    fs::copy(from, to, fs::copy_options::recursive);
}
```
- copy함수에 복사할 대상과 복사할 위치를 차례대로 인자로 전달하면 된다.
- 마지막 인자로 어떠한 방식으로 복사할지 지정해준다.
- rucrusive 옵션은 복사할 대상에 존재하는 모든 디렉토리와 파일들을 복사하게했다.
  - default는 파일들만 복사하게된다.
- 만약 복사할 대상이 이미 존재하고 있으면 에외를 던진다.
  - skip_existing : 이미 존재하는 파일은 무시 (예외 pass)
  - overwrite_existing : 이미 존재하는 파일 덮어쓰기
  - update_existing : 이미 존재하는 파일이 더 오래되었을 경우 덮어 쓴다.

# 파일 / 디렉토리 삭제
```cpp
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./a/b.txt");
    std::cout << p << " exist ? " << std::boolalpha << fs::exists(p) << std::endl;

    fs::remove(p);
    std::cout << p << " exist ? " << std::boolalpha << fs::exists(p) << std::endl;
}
```

```
"./a/b.txt" exist ? true
"./a/b.txt" exist ? false
```
- 파일뿐만 아니라 디렉토리 역시 지울 수 있다.
  - 단, **디렉토리가 반드시 빈 디렉토리어야한다.**
- 비어있지 않는 디렉토리를 삭제하려면 remove_all()함수를 사용해야된다.

```cpp
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./c/b");

    std::error_code err;
    fs::remove(p, err);
    std::cout << err.message() << std::endl;

    fs::remove_all(p);
}
```

```
Directory not empty
```

# directory_iterator 사용시 주의
- 특정 확장자를 모두 삭제하는 프로그램을 만들 때,  directory_iterator를 통해  코드그를 짜면 파일 삭제 시 마다 해당 기렉토리의 구조가 바뀌게된다.
  - 그러므로 iterator는 무효화 되는 것이다.
- fs:;remove후 entry는 사용할 수 없는 반복자가 되는 것이다.
  - ++entry가 다음 디렉토리를 가리키는 것을 보장할 수 없게되는 것이다.

```cpp
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./a");

    while(true)
    {
        bool is_modified = false;
        for (const auto& entry : fs::directory_iterator("./a"))
        {
            const std::string ext = entry.path().extension();
            if (ext == ".txt")
            {
                fs::remove(entry.path());
                is_modified = true;
                break;
            }
        }

        if (!is_modified)
            break;
    }
}
```
- 위처럼 어쩔 수 없이 삭제 마다 반복자를 초기화 해줘야한다.