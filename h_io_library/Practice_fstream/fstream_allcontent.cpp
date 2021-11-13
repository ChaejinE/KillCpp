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