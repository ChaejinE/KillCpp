# Overview
- regular expression, 정규표현식 라이브러리에 대해 알아보자.
- 정규 표현식은 문자열에서 패턴을 찾는데 사용한다.
  - 1. 주어진 문자열이 주어진 규칙에 맞는지 확인할 때
  - 2. 주어진 문자열에서 원하는 패턴의 문자열을 검색할 때
  - 3. 주어진 문자열에서 원하는 패턴의 문자열로 치환할 때
  - 등등 많은 경우 유용하게 사용된다.

# 전체 문자열 매칭
- 서버 관리 시 해당 서버에서 매 시간마다 로그 파일을 생성한다고 해보자.
  - 로그 파일은  db-(time)-log.txt와 같은 형태로 생성된다고하자.
- 여러 파일이 섞여있는 폴더에서 원하는 로그 파일들만 어떻게 읽어들일 수 있을 까?

```
db-\d*-log\.txt
```
- \d* : 임의의 개수 숫자를 의미
- .앞에 \를 붙인 이유는 .을 그냥 쓰면 임의의 문자로 해석되기 떄문이다.

```cpp
#include <iostream>
#include <regex>
#include <vector>

int main()
{
    std::vector<std::string> file_names = {"db-123-log.txt", "db-124-log.txt",
                                           "not_db-log.txt", "db-12-log.txt",
                                           "db-12-log.jpg"};

    std::regex re("db-\\d*-log\\.txt");
    for (const auto &file_name : file_names)
    {
        std::cout << file_name << " : " << std::boolalpha
                  << std::regex_match(file_name, re) << "\n";
    }
}
```

```
db-123-log.txt : true
db-124-log.txt : true
not_db-log.txt : false
db-12-log.txt : true
db-12-log.jpg : false
```
- 정규표현식에 맞는 파일 이름들만 true로 나오는 것을 알 수 있다.
- 정규 표현식 사용을 위해 먼저 std:;regex re("...")와 같은 정규표현식 객체를 정의해야된다.
  - 정규 표현식 문법의 종류와 처리하는 엔진 역시 여러 종류가있다.
  - 생성자에 추가적으로 인자를 전달할 수 있다.
    - grep 방식이면 std::regex::grep
    - default로 std::regex::ECMScript가 들어간다.
    - 어떤 문법을 사용할지도 전달하고 싶을 때가 있는데 예를들어 std::regex::icase를 전달하면 대소 문자를 구분하지 않게 된다.
    ```cpp
    std::regex re("db-....", std::regex::grep | std::rgex::icase);
    ```
    - 정규 표현식의 성능이 중요하면 std::regex::optimize를 추가로 전달할 수 있다. 이 경우 정규 표현식 객체를 생성하는 데 시간이 좀 더 걸리지만 정규 표현식 객체를 사용하는 작업을 좀 더 빠르게 수행한다.
- std::regex_match(filenamae, re_obj)
  - file_name이 re_obj와 완전히 매칭된다면 true를 return 한다.
- 완전히 매칭된다는 것은 정규 표현식의 패턴에 부합해야한다는 것을 말한다.

# 부분 매칭
- 전화번호를 받는 정규표현식ㅇ르 생각해보자.
  - 1. (숫자)-(숫자)-(숫자) 꼴
  - 2. 맨 앞자리는 반드시 3자리, 0 or 1로 이루어져야한다.
  - 3. 가운데 자리는 3 자리 혹은 4자리 여야한다.
  - 4. 마지막 자리는 4자리 여야한다.

```cpp
010-1234-5678
010-123-4567
011-1234-5678
```
- 위와 같은 번호들이 조건을 만족한다.

```cpp
[01]{3}-\d{3,4}-\d{4}
```
- \[01]의 뜻은 0 혹은 1이라는 의미다.
- {3}은 해당 종류의 문자가 3번 나타날 수 있다는 의미다.

```cpp
#include <iostream>
#include <regex>
#include <vector>

int main()
{
    std::vector<std::string> phone_numbers = {"010-1234-5678", "010-123-4567",
                                            "011-1234-5567", "010-12345-6789",
                                            "123-4567-8901", "010-1234-567"};
    
    std::regex re("[01]{3}-\\d{3,4}-\\d{4}", std::regex::optimize);
    for (const auto &number : phone_numbers)
    {
        std::cout << number << ": " << std::boolalpha
                  << std::regex_match(number, re) << '\n';
    }
}
```

```
010-1234-5678: true
010-123-4567: true
011-1234-5567: true
010-12345-6789: false
123-4567-8901: false
010-1234-567: false
```
- 만약 조건에 만족하는 전화번호중 가운데 번호를 추출하고 싶다면 ?
  - capture group을 사용하면 된다.

```cpp
#include <iostream>
#include <regex>
#include <vector>

int main()
{
    std::vector<std::string> phone_numbers = {"010-1234-5678", "010-123-4567",
                                            "011-1234-5567", "010-12345-6789",
                                            "123-4567-8901", "010-1234-567"};
    
    std::regex re("[01]{3}-(\\d{3,4})-\\d{4}", std::regex::optimize);
    std::smatch match; // matching 결과 string으로 보관
    for (const auto &number : phone_numbers)
    {
        if (std::regex_match(number, match, re))
        {
            for (size_t i = 0; i < match.size(); i++)
                std::cout << "Match : " << match[i].str() << std::endl;

            std::cout << "----------------------------\n";
        }
    }
}
```

```
Match : 010-1234-5678
Match : 1234
----------------------------
Match : 010-123-4567
Match : 123
----------------------------
Match : 011-1234-5567
Match : 1234
----------------------------
```
- ```cpp
  std::regex re("[01]{3}-(\\d{3,4})-\\d{4}", std::regex::optimize);
  ```
  - 이처럼 원하는 부분을 ( )로 감싸게 되면 부분에 매칭된 문자열을 얻을 수 있게된다.
- smatch 경우 매칭된 문자열을 std::string의 형태로 돌려준다.
  - char* 로 받고 싶다면 cmatch를 사용해야된다.
  - cmatch 시 match\[i].str() 이면 const char*이 된다.
- regex_match의 경우 전체 문자열이 매칭된 것이어서 첫 번째 결과에 전체 문자열이 나타나게된다.
  - 다음으로 ()에 들어있던 문자열이 나타나게된다. 만약 ()가 여러개 잇었다면 for문을 통해 순차적으로 접근할 수 있다.

# 원하는 패턴 검색
- regex_match를 통해 문자열 전체가 패턴에 부합하는 지 확인하는 작업을 했다.
- 패턴을 만족하는 문자열 일부를 검색하는 작업을 수행해보자.

```html
<div class="sk...">...</div>
```
- HTML 문서에서 위와 같은 형태의 태그만 읽어들이는 것이다.

```cpp
<div class="sk[\w -]*">\w*</div>
```
- 해당 조건을 만족하는 정규 표현식은 위와같이 작성할 수 있다.

```cpp
#include <iostream>
#include <regex>

int main()
{
    std::string html = R"(
        <div class="social-login">
          <div class="small-comment">다음으로 로그인 </div>
          <div>
            <i class="xi-facebook-official fb-login"></i>
            <i class="xi-google-plus goog-login"></i>
          </div>
        </div>
        <div class="manual">
          <div class="small-comment">
            또는 직접 입력하세요 (댓글 수정시 비밀번호가 필요합니다)
          </div>
          <input name="name" id="name" placeholder="이름">
          <input name="password" id="password" type="password" placeholder="비밀번호">
        </div>
        <div id="adding-comment" class="sk-fading-circle">
          <div class="sk-circle1 sk-circle">a</div>
          <div class="sk-circle2 sk-circle">b</div>
          <div class="sk-circle3 sk-circle">asd</div>
          <div class="sk-circle4 sk-circle">asdfasf</div>
          <div class="sk-circle5 sk-circle">123</div>
          <div class="sk-circle6 sk-circle">aax</div>
          <div class="sk-circle7 sk-circle">sxz</div>
        </div>
  )";

  std::regex re(R"(<div class="sk[\w -]*">\w*</div>)");
  std::smatch match;

  while (std::regex_search(html, match, re))
  {
      std::cout << match.str() << '\n';
      html = match.suffix();
  }
}
```

```
<div class="sk-circle1 sk-circle">a</div>
<div class="sk-circle2 sk-circle">b</div>
<div class="sk-circle3 sk-circle">asd</div>
<div class="sk-circle4 sk-circle">asdfasf</div>
<div class="sk-circle5 sk-circle">123</div>
<div class="sk-circle6 sk-circle">aax</div>
<div class="sk-circle7 sk-circle">sxz</div>
```
- 문자열에서 원하는 패턴을 검색하는 일은 regex_search를 사용하면 된다.
  - regex_match 처럼 첫 번째에 검색하고 싶은 문자열, 두 번째에 패턴을 보관할 match객체, 마지막으로 정규표현식 객체를 전달하면 된다.
- html 에서 정규표현식과 매칭되는 패턴이 존재한다면 regex_search가 true를 return하게 된다.
  - 매칭된 패턴은 match.str()을 통해 접근할 수 있다.
- 이전에 찾았던 패턴들말고 다른 패턴을 찾기 위해 html 변수를 업데이트해서 검색된 패턴 바로 뒤부터 검색하도록 해야된다.
  - 이 것이 match.suffix()의 역할이다.
  - match.suffix()는 std::sub_match 객체를 리턴한다.
  - sub_match는 단순히 어떤 문자열의 시작과 끝을 가리키는 반복자 두 개를 가지고 있다.
  - suffix의 경우 원 문자열에서 검색된 패턴 바로 뒤 부터, 이전 문자열의 끝까지에해당하는 sub_match 객체를 리턴한다.

![image](https://user-images.githubusercontent.com/69780812/147867127-dbb5acc5-7f64-4404-b9be-ef4063877490.png)
- sub_match에는 string으로 변환할 수 있는 캐스팅 연산자가 들어있다.
- html에 그냥 대입하게 되면 알아서 문자열로 변환되서 들어가게된다.
  - 덕분에 이미 찾은 패턴 뒤 부터 다시 새로운 검색을 시작할 수 있다.

# std::regex_iterator
```cpp
#include <iostream>
#include <regex>

int main()
{
    std::string html = R"(
        <div class="social-login">
          <div class="small-comment">다음으로 로그인 </div>
          <div>
            <i class="xi-facebook-official fb-login"></i>
            <i class="xi-google-plus goog-login"></i>
          </div>
        </div>
        <div class="manual">
          <div class="small-comment">
            또는 직접 입력하세요 (댓글 수정시 비밀번호가 필요합니다)
          </div>
          <input name="name" id="name" placeholder="이름">
          <input name="password" id="password" type="password" placeholder="비밀번호">
        </div>
        <div id="adding-comment" class="sk-fading-circle">
          <div class="sk-circle1 sk-circle">a</div>
          <div class="sk-circle2 sk-circle">b</div>
          <div class="sk-circle3 sk-circle">asd</div>
          <div class="sk-circle4 sk-circle">asdfasf</div>
          <div class="sk-circle5 sk-circle">123</div>
          <div class="sk-circle6 sk-circle">aax</div>
          <div class="sk-circle7 sk-circle">sxz</div>
        </div>
  )";

  std::regex re(R"(<div class="sk[\w -]*">\w*</div>)");
  std::smatch match;

  auto start = std::sregex_iterator(html.begin(), html.end(), re);
  auto end = std::sregex_iterator();

  while (start != end)
  {
      std::cout << start->str() << std::endl;
      ++start;
  }
}
```

```
<div class="sk-circle1 sk-circle">a</div>
<div class="sk-circle2 sk-circle">b</div>
<div class="sk-circle3 sk-circle">asd</div>
<div class="sk-circle4 sk-circle">asdfasf</div>
<div class="sk-circle5 sk-circle">123</div>
<div class="sk-circle6 sk-circle">aax</div>
<div class="sk-circle7 sk-circle">sxz</div>
```
- std::regex_iterator는 주어진 문자열에서 정규표현식으로 매칭된 문자열들을 뽑아내는 반복자다.
  - 첫 번째 인자, 두 번째 인자 = 문자열의 시작, 끝
  - 마지막 인자 : 정규 표현식 객체를 전달하면된다.
- sregex_iterator는 regex_iterator 중 string을 사용하는 반복자다.
- regex_iterator의 경우 처음 생성될 때와 ++ 연산자로 증감될 때마다 regex_search를 통해 초기 주어진 문자열 범위 내 패턴에 맞는 문자열을 검색한다.
- *을 통해 역참조하게 되면 현재 검색된 패턴에 대한 match_results 객체를 얻어낼 수 있다.

# 원하는 패턴으로 치환
- std::regex_replace를 통해 패턴의 문자열을 다른 문자열로 치환하는 작업이다.
- html에서 sk-circle과 같은 문자열을 1-sk-circle로 바꿔보는 정규식을 생각해보자.

```
sk-circle\d
```
- sk-circle1과 같은 형태를 regex로 매칭시키는 방법이다.

```
숫자-sk-circle
```
- 어떤 형태로 치환시킬지도 생각해본다.
- 위처럼해야되는데 위 숫자에 해당하는 부분이 매칭된 패턴에 \d에 해당하는 부분이라는 점이다. 캡쳐그룹을 이용하여 간단히 해결할 수 있다.

```
sk-circle(\d)
```
- 숫자에 해당하는 부분을 첫 번째 캡쳐그룹으로 만든다.
- 그 다음 치환할 때 첫 번째 캡쳐 그룹을 표현하기 위해 $1로 명시할 수 있다.
  - 이런 요소를 back reference라고 부른다.

```
$1-sk-circle
```
- 따라서 위와같이 표현할 수 있다.

```cpp
#include <iostream>
#include <regex>

int main()
{
    std::string html = R"(
        <div class="social-login">
          <div class="small-comment">다음으로 로그인 </div>
          <div>
            <i class="xi-facebook-official fb-login"></i>
            <i class="xi-google-plus goog-login"></i>
          </div>
        </div>
        <div class="manual">
          <div class="small-comment">
            또는 직접 입력하세요 (댓글 수정시 비밀번호가 필요합니다)
          </div>
          <input name="name" id="name" placeholder="이름">
          <input name="password" id="password" type="password" placeholder="비밀번호">
        </div>
        <div id="adding-comment" class="sk-fading-circle">
          <div class="sk-circle1 sk-circle">a</div>
          <div class="sk-circle2 sk-circle">b</div>
          <div class="sk-circle3 sk-circle">asd</div>
          <div class="sk-circle4 sk-circle">asdfasf</div>
          <div class="sk-circle5 sk-circle">123</div>
          <div class="sk-circle6 sk-circle">aax</div>
          <div class="sk-circle7 sk-circle">sxz</div>
        </div>
  )";

  std::regex re(R"r(sk-circle(\d))r");
  std::smatch match;

  std::string modified_html = std::regex_replace(html, re, "$1-sk-circle");
  std::cout << modified_html;
}
```

```
        <div class="social-login">
          <div class="small-comment">다음으로 로그인 </div>
          <div>
            <i class="xi-facebook-official fb-login"></i>
            <i class="xi-google-plus goog-login"></i>
          </div>
        </div>
        <div class="manual">
          <div class="small-comment">
            또는 직접 입력하세요 (댓글 수정시 비밀번호가 필요합니다)
          </div>
          <input name="name" id="name" placeholder="이름">
          <input name="password" id="password" type="password" placeholder="비밀번호">
        </div>
        <div id="adding-comment" class="sk-fading-circle">
          <div class="1-sk-circle sk-circle">a</div>
          <div class="2-sk-circle sk-circle">b</div>
          <div class="3-sk-circle sk-circle">asd</div>
          <div class="4-sk-circle sk-circle">asdfasf</div>
          <div class="5-sk-circle sk-circle">123</div>
          <div class="6-sk-circle sk-circle">aax</div>
          <div class="7-sk-circle sk-circle">sxz</div>
        </div>
```
- regex_replace로 문자열을 치환할 때 첫 번째 인자로 치환하고자하는 문자열을, 두 번째 인자로 정규 표현식 객체를, 마지막으로 치환 시 어떤 패턴으로 바꿀지 적어주면된다.
- regex_replace는 여러 형태의 오버로딩이있는데 위 형태의 경우 치환된 문자열을 생성해서 돌려준다.
- 치환된 문자열을 생성하지 않고 그냥 stdout에 출력하고 싶으면 어떤 조작을 해주면 된다.
  ```cpp
  std::regex_replace(std::ostreambuf_teartor<char>(std::cout), html.begin(), html.end(), re, "$1-sk-circle");
  ```

```html
<div class="sk-circle skcircle">a</div>
```
- 더 나아가 이를 아래와 같이 치환하고 싶다고 해보자.

```html
<div class="1-sk-circle">a</div>
```
- 뒷 부분의 sk-circle을 완전히 날려버리는 것이다.
- 이를 위해 두 개의 캡쳐 그룹이 필요하다.

```
(sk-circle\d sk-circle)
```
- 일단 전체 문자열을 건드려야하므로 위 처럼 전체 패턴에 해당하는 캡처 그룹이 필요하다.

```
(sk-circle(\d) sk-circle)
```
- 그 다음 숫자 부분만 다시 뽑아내야 하므로 위 처럼 해야된다.
- 문제는 위 처럼 캡처 그룹이 중첩되었을 때는 어느 것이 $1이고 $2인지 알아야하는데 **괄호가 열리는 순서대로** $1, $2... 로 진행한다고 생각하면된다.
  - 즉, 해당 캡처 전체 그룹이 $1, \d에 해당하는 캡처 그룹이 $2가 된다.

```
$2-sk-circle
```
- 따라서 치환된 패턴은 위와 같다.

```cpp
#include <iostream>
#include <regex>

int main()
{
    std::string html = R"(
        <div class="social-login">
          <div class="small-comment">다음으로 로그인 </div>
          <div>
            <i class="xi-facebook-official fb-login"></i>
            <i class="xi-google-plus goog-login"></i>
          </div>
        </div>
        <div class="manual">
          <div class="small-comment">
            또는 직접 입력하세요 (댓글 수정시 비밀번호가 필요합니다)
          </div>
          <input name="name" id="name" placeholder="이름">
          <input name="password" id="password" type="password" placeholder="비밀번호">
        </div>
        <div id="adding-comment" class="sk-fading-circle">
          <div class="sk-circle1 sk-circle">a</div>
          <div class="sk-circle2 sk-circle">b</div>
          <div class="sk-circle3 sk-circle">asd</div>
          <div class="sk-circle4 sk-circle">asdfasf</div>
          <div class="sk-circle5 sk-circle">123</div>
          <div class="sk-circle6 sk-circle">aax</div>
          <div class="sk-circle7 sk-circle">sxz</div>
        </div>
  )";

  std::regex re(R"r((sk-circle(\d) sk-circle))r");
  std::smatch match;

  std::string modified_html = std::regex_replace(html, re, "$2-sk-circle");
  std::cout << modified_html;
}
```

```
        <div class="social-login">
          <div class="small-comment">다음으로 로그인 </div>
          <div>
            <i class="xi-facebook-official fb-login"></i>
            <i class="xi-google-plus goog-login"></i>
          </div>
        </div>
        <div class="manual">
          <div class="small-comment">
            또는 직접 입력하세요 (댓글 수정시 비밀번호가 필요합니다)
          </div>
          <input name="name" id="name" placeholder="이름">
          <input name="password" id="password" type="password" placeholder="비밀번호">
        </div>
        <div id="adding-comment" class="sk-fading-circle">
          <div class="1-sk-circle">a</div>
          <div class="2-sk-circle">b</div>
          <div class="3-sk-circle">asd</div>
          <div class="4-sk-circle">asdfasf</div>
          <div class="5-sk-circle">123</div>
          <div class="6-sk-circle">aax</div>
          <div class="7-sk-circle">sxz</div>
        </div>
```
- 아하주 잘바뀌었다.