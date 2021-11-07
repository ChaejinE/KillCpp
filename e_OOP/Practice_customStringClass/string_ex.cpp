#include <iostream>
#include <string.h>
/*
    1. 문자, C형식 문자열 str에서 생성할 수 있는 생성자, 복사 생성자
    2. 문자열의 길이를 리턴하는 함수 (length)
    3. 문자열 대입 함수 (assign)
    4. 문자열 메모리 할당 함수(reserve)
    5. 현재 할당된 크기 (capacity)
    6. 특정 위치 문자 리턴하는 함수(at)
    7. 특정 문자열 삽입 함수 (insert)
    8. 특정 위치의 특정 개수의 문자를 지우는 함수 (erase)
    9. 특정 위치를 시작으로 특정 문자열을 검색하는 함수 (find)
    10. 두 문자열을 사전식 비교하는 함수 (compare)
*/
class MyString
{
    char *string_content; // 문자열 데이터를 가리키는 포인터
    int string_length;    // 문자열 길이
    int memory_capacity;

public:
    // 문자 하나로 생성
    MyString(char c);

    // 문자열로 부터 생성
    MyString(const char *str);

    // 복사 생성자
    MyString(const MyString &str);

    ~MyString();

    /* 
        내부 member variable을 변경하지 않는 다면 const 를 명시해주는 것이 좋다.
    */
    int length() const;

    void print() const;
    void println() const;

    /*
            = 연산과 동일한 역할하는 assign()
    */
    MyString &assign(const MyString &str);
    MyString &assign(const char *str);

    /*
        매우 긴 string assign -> 짧은 string assign -> 다시 조금 긴 string assign 시 문제 발생
        다시 조금 긴 string 할당 시 이미 매우 긴 string으로 인해 확보되어 있던 공간을 놔두고, 해제하고 새롭게 assign 하게 된다.
        - 매우 비효율적이다.
        capacity 도입으로 할당할 문자열을 미리 예약해놓는 reserve 함수와
        현재 문자열의 할당된 메모리 크기를 나타내는 함수를 만들 수 있다.
    */
    int capacity();
    void reserve(int size);

    char at(int i) const; // 특정 위치 문자를 얻는 함수

    /*
        insert(1, "d")시 abc이면 adbc가 삽입되도록한다.
    */
    MyString &insert(int loc, const MyString &str);
    MyString &insert(int loc, const char *str);
    MyString &insert(int loc, char c);

    MyString &erase(int loc, int num);

    /*
    문자열의 길이에 따라 연산량이 오래 걸릴 수 있다.
    문자열 검색 알고리즘은 수없이 많지만 어떤 상황에서도
    좋은 성능을 발휘하는 알고리즘은 없다.
    클래스의 사용 목적이 명확해서 그 알고리즘이 좋으 성능을 발휘하는 경우에만
    사용하는 것이 보통이다.
    */
    int find(int find_from, MyString &stri) const;
    int find(int find_from, char *stri) const;
    int find(int find_from, char c) const;

    /*
        문자열 간 크기 비교 함수
        크기를 비교한다 : 어떤 문자열이 더 뒤에 오는지 판단하는 의미다.
    */
    int compare(const MyString &str) const;
};

MyString::MyString(char c)
{
    string_content = new char[1];
    string_content[0] = c;
}
MyString::MyString(const char *str)
{
    string_length = strlen(str);
    string_content = new char[string_length];

    for (int i = 0; i != string_length; i++)
    {
        string_content[i] = str[i];
    }
}
MyString::MyString(const MyString &str)
{
    string_length = str.string_length;
    string_content = new char[string_length];

    for (int i = 0; i != string_length; i++)
        string_content[i] = str.string_content[i];
}
MyString::~MyString() { delete[] string_content; }
int MyString::length() const { return string_length; }

void MyString::print() const
{
    for (int i = 0; i != string_length; i++)
    {
        std::cout << string_content[i];
    }
}
void MyString::println() const
{
    for (int i = 0; i != string_length; i++)
    {
        std::cout << string_content[i];
    }

    std::cout << std::endl;
}
MyString &MyString::assign(const MyString &str)
{
    if (str.string_length > memory_capacity)
    {
        // 공간 재할당
        delete[] string_content;
        string_content = new char[str.string_length];

        memory_capacity = str.string_length;
    }

    for (int i = 0; i != str.string_length; i++)
        string_content[i] = str.string_content[i];
    // str.string_length + 1 ~ string_length 부분은 초기화 시킬 필요 없다.
    // 거기까지는 읽어들이지 않기 때문이다.

    string_length = str.string_length;

    return *this;
}
MyString &MyString::assign(const char *str)
{
    int str_length = strlen(str);
    if (str_length > memory_capacity)
    {
        delete[] string_content;
        string_content = new char[str_length];
        memory_capacity = str_length;
    }
    for (int i = 0; i != str_length; i++)
        string_content[i] = str[i];

    string_length = str_length;

    return *this;
}
int MyString::capacity() { return memory_capacity; }
void MyString::reserve(int size)
{
    if (size > memory_capacity)
    {
        char *prev_string_content = string_content;

        string_content = new char[size];
        memory_capacity = size;

        for (int i = 0; i != string_length; i++)
            string_content[i] = prev_string_content[i];

        delete[] prev_string_content;
    }
}
char MyString::at(int i) const
{
    // i가 허용 범위 초과하면 NULL return
    if (i >= string_length || i < 0)
        return NULL;
    else
        return string_content[i];
}
MyString &MyString::insert(int loc, const MyString &str)
{
    // 범위 벗어나는 입력에 대해 삽입 수행하지 않음
    if (loc < 0 || loc > string_length)
        return *this;

    if (string_length + str.string_length > memory_capacity)
    {
        // 공간이 더 필요하면 공간을 재할당해야한다.
        // 계속 메모리 수용공간을 만들어 내는 것은 꽤나 부담이다.
        // 그러므로 적당한 길이일 때, 2배로 크기를 미리 늘려놓는 것이 효율적이다.
        // 2배 이상인 경우 너무 긴 문자열이므로 필요한 만큼만 늘려준다.
        // 이는 메모리할당 방법에서 자주 쓰이는 방법이므로 기억해 두자.
        if (memory_capacity * 2 > string_length + str.string_length)
            memory_capacity *= 2;
        else
            memory_capacity = string_length + str.string_length;

        char *prev_string_content = string_content;
        string_content = new char[memory_capacity];

        // insert 되는 부분 직전까지의 내용 복사
        int i;
        for (i = 0; i < loc; i++)
            string_content[i] = prev_string_content[i];

        // 새롭게 insert되는 문자열을 넣는다.
        for (int j = 0; j != str.string_length; j++)
            string_content[i + j] = str.string_content[j];

        // 다시 원 문자열의 나머지 뒷부분 복사
        for (; i < string_length; i++)
            string_content[str.string_length + i] = prev_string_content[i];

        delete[] prev_string_content;

        string_length = string_length + str.string_length;
        return *this;
    }

    // 초과하지 않는 경우 동적할당할 필요 x
    // 효율적 insert를 위해 밀리는 부분을 먼저 뒤로 밀어버린다.
    // 추가되는 길이 만큼 삽입 위치의 문자들을 밀어준다.
    for (int i = string_length - 1; i >= loc; i--)
        string_content[i + str.string_length] = string_content[i];

    // insert 되는 문자 집어넣기
    for (int i = 0; i < str.string_length; i++)
        string_content[i + loc] = str.string_content[i];

    string_length = string_length + str.string_length;
    return *this;
}
MyString &MyString::insert(int loc, const char *str)
{
    MyString temp(str);
    return insert(loc, temp);
}
MyString &MyString::insert(int loc, char c)
{
    MyString temp(c);
    return insert(loc, temp);
}
MyString &MyString::erase(int loc, int num)
{
    if (num < 0 || loc < 0 || loc > string_length)
        return *this;

    // loc의 앞 부터 시작해서 num 문자 지우기
    // 지운 다는 것은 문자들을 앞으로 끌고온다고 생각하면 된다.
    for (int i = loc + num; i < string_length; i++)
        string_content[i - num] = string_content[i];

    // string_length 덕분에 범위 밖 문자들은 초기화 처리해주지 않아도 되고,
    // 신경안써도 된다.
    string_length -= num;
    return *this;
}
int MyString::find(int find_from, MyString &str) const
{
    // find_from 부터 시작해서 가장 첫번째 str의 위치를 return
    int i, j;
    if (str.string_length == 0)
        return -1;
    for (i = find_from; i < string_length - str.string_length; i++)
    {
        for (j = 0; j < str.string_length; j++)
            if (string_content[i + j] != str.string_content[j])
                break;

        if (j == str.string_length)
            return i;
    }
    return -1; // 찾지 못함.
}
int MyString::find(int find_from, char *str) const
{
    MyString temp(str);
    return find(find_from, temp);
}
int MyString::find(int find_from, char c) const
{
    MyString temp(c);
    return find(find_from, temp);
}
int MyString::compare(const MyString &str) const
{
    // (*this) - (str) 수행해서 1, 0, -1로 그결과 리턴
    // 1은 (*this)가 사전식으로 더 뒤에 온다는 의미.
    // 0은 두 문자열이 같다는 의미
    // -1은 (*this)가 사전식으로 더 앞에 온다는 의미

    for (int i = 0; i < std::min(string_length, str.string_length); i++)
    {
        if (string_content[i] > str.string_content[i])
            return 1;
        else if (string_content[i] < str.string_content[i])
            return -1;
    }

    // 끝나지 않았다면 앞 부분 까지 모두 똑같은 것이 된다.
    // 만일 문자열 길이가 같다면 두 문자열은 아예 같은 문자열이 된다.
    if (string_length == str.string_length)
        return 0;

    // abc, abcd 같은 경우 abcd가 더 뒤에 오게된다.
    else if (string_length > str.string_length)
        return 1;

    return -1;
}

int main()
{
    MyString str1("hello world!");
    MyString str2(str1);
    MyString str3("very very very long string");
    MyString str4("<some string inserted between>");
    MyString str5("this is a very very long string");
    MyString str6("abc");
    MyString str7("abcd");
    str3.reserve(30);

    str1.println();
    str2.println();

    std::cout << "Capacity : " << str3.capacity() << std::endl;
    std::cout << "String length : " << str3.length() << std::endl;
    str3.println();

    str3.insert(5, str4);
    str3.println();

    std::cout << "Capacity : " << str3.capacity() << std::endl;
    std::cout << "String length : " << str3.length() << std::endl;
    str3.println();

    std::cout << "Location of first <very> in the string : " << str5.find(0, "very")
              << std::endl;
    std::cout << "Location of second <very> in the string : "
              << str5.find(str5.find(0, "very") + 1, "very") << std::endl;

    std::cout << "str6 and str7 compare : " << str6.compare(str7) << std::endl;
}