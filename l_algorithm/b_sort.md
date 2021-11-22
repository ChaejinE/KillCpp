# Overview
- 정렬 알고리즘을 무려 3가지 함수로 제공하고있다.
- sort : 일반적 정렬 함수
- stable_sort : 정렬을 하되 원소들 간 순서를 보존한다.
  - \[a, b] 순일 때, a==b이면, [a, b] or [b, a]로 랜덤하게 될 수 있다.
  - stable_sort는 위의 경우 처럼 하지 않고 그 순서를 반드시 보존한다.
  - 이 때문에 sort 보다 좀 더 느리다.
- partial_sort : 배열의 일부분만 정렬한다.

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end)
{
    while (begin != end)
    {
        std::cout << *begin << " ";
        begin++;
    }

    std::cout << std::endl;
}

struct int_compare
{
    bool operator()(const int& a, const int& b) const { return a > b; }
};

int main()
{
    std::vector<int> vec;

    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(6);
    vec.push_back(4);
    vec.push_back(7);
    vec.push_back(2);

    std::cout << "오름 차순 정렬 전 ----" << std::endl;
    print(vec.begin(), vec.end());
    std::sort(vec.begin(), vec.end());

    std::cout << "오름 차순 정렬 후 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << "내림 차순 정렬 전 ----" << std::endl;
    print(vec.begin(), vec.end());
    std::sort(vec.begin(), vec.end(), int_compare());

    std::cout << "내림 차순 정렬 후 ----" << std::endl;
    print(vec.begin(), vec.end());

}
```

```
오름 차순 정렬 전 ----
5 3 1 6 4 7 2 
오름 차순 정렬 후 ----
1 2 3 4 5 6 7 
내림 차순 정렬 전 ----
1 2 3 4 5 6 7 
내림 차순 정렬 후 ----
7 6 5 4 3 2 1 
```
- sort함수는 정렬할 원소의 시작 위치와 마지막 위치 바로 뒤를 반복자로 받는다.
- sort에 들어가는 반복자의 경우 **반드시 RandomAccessIterator Type을 만족해야한다.**
  - vector, deque만 가능
- sort는 기본적으로 오름 차순 정렬을 해준다.
- 내림차순을 해주고 싶으면 3번째 인자로 특정한 조건을 전달하면된다.
  - 비교를 어떻게 수행할 것인지에 대해 알려주면 된다.
  - 함수 객체를 위한 구조체를 정의해주고, operator()함수만 만들어주면된다.
    - 함수 객체 !
- 사실, int나 string 같은 기본 타입들은 모두 <, > 연산자들이 기본으로 내장되어있어 함수 객체를 만들 필요가 없다.
  ```cpp
  std::sort(vec.begin(), vec.end(), grater<int>());
  ```
  - functional 헤더에 위와 같은 템플릿 클래스가 존재한다.
  - greater에 내가 사용하고자 하는 타입을 넣으면 위와 같은 함수 객체르 자동으로 자동으로 만들어 준다. 물론, 그 타입의 > 연산자가 존재해야한다.
  - int, string 같은 기본 타입은 > 연산자가 존재한다.

# partial_sort
```cpp
#include <algorithm>
#include <iostream>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end)
{
    while (begin != end)
    {
        std::cout << *begin << " ";
        begin++;
    }

    std::cout << std::endl;
}

struct int_compare
{
    bool operator()(const int& a, const int& b) const { return a > b; }
};

int main()
{
    std::vector<int> vec;

    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(6);
    vec.push_back(4);
    vec.push_back(7);
    vec.push_back(2);

    std::cout << "오름 차순 정렬 전 ----" << std::endl;
    print(vec.begin(), vec.end());
    std::partial_sort(vec.begin(), vec.begin() + 3, vec.end());

    std::cout << "오름 차순 정렬 후 ----" << std::endl;
    print(vec.begin(), vec.end());
}
```

```
오름 차순 정렬 전 ----
5 3 1 6 4 7 2 
오름 차순 정렬 후 ----
1 2 3 6 5 7 4 
```
- \[start, end) 전체 원소들 중 \[start, middle) 까지 원소들이 전체 원소들 중에서 제일 작은 것들 순으로 정렬 시킨다.
- 위의 경우 vec.begin() 부터 vec.end()까지 원소들 중 vec.begin()부터 vec.begin() + 3까지 전체에서 가장 작은 애들만 순서대로 저장하고 나머지 위치는 상관없다는 식이다.

```
5 3 1 6 4 7 2
```
- 위 중에서 가장 작은 3개 원소인 1, 2, 3만 정렬이 된다.

```
1 2 3 6 5 7 4
```
- 나머지 원소들은 랜덤하게 남아있을 뿐이다.
- 전체 원소 개수 N, 정렬 부분 크기가 M이면 O(NlogM)의 복잡도를 갖는다.
  - 만약 100 명중 상위 10명의 학생의 성적순을 보고 싶다면 굳이 sort로 전체를 정렬하지 않고 partial_sort로 10개만 정렬하는 것이 더 빠르게 된다.

# stable_sort
```cpp
#include <iostream>
#include <functional>
#include <algorithm>
#include <string>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end)
{
    while(begin != end)
    {
        std::cout << "[" << *begin <<"] ";
        begin++;
    }
    std::cout << std::endl;
}

struct User
{
    std::string name;
    int age;

    User(std::string name, int age) : name(name), age(age) {}

    bool operator<(const User& u) const { return age < u.age; }
};

std::ostream& operator<<(std::ostream& o, const User& u)
{
    o << u.name << ", " << u.age;
    return o;
}

int main()
{
    std::vector<User> vec;
    for (int i =0; i < 100; i ++)
    {
        std::string name = "";
        name.push_back('a' + i / 26);
        name.push_back('a' + i % 26);
        vec.push_back(User(name, static_cast<int>(rand() % 10)));
    }

    std::vector<User> vec2 = vec;

    std::cout << "정렬 전 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::sort(vec.begin(), vec.end());

    std::cout << "정렬 후 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << "stable_sort ----" << std::endl;
    std::stable_sort(vec2.begin(), vec2.end());
    print(vec2.begin(), vec2.end());
}
```

```
정렬 전 ----
[aa, 3] [ab, 6] [ac, 7] [ad, 5] [ae, 3] [af, 5] [ag, 6] [ah, 2] [ai, 9] [aj, 1] [ak, 2] [al, 7] [am, 0] [an, 9] [ao, 3] [ap, 6] [aq, 0] [ar, 6] [as, 2] [at, 6] [au, 1] [av, 8] [aw, 7] [ax, 9] [ay, 2] [az, 0] [ba, 2] [bb, 3] [bc, 7] [bd, 5] [be, 9] [bf, 2] [bg, 2] [bh, 8] [bi, 9] [bj, 7] [bk, 3] [bl, 6] [bm, 1] [bn, 2] [bo, 9] [bp, 3] [bq, 1] [br, 9] [bs, 4] [bt, 7] [bu, 8] [bv, 4] [bw, 5] [bx, 0] [by, 3] [bz, 6] [ca, 1] [cb, 0] [cc, 6] [cd, 3] [ce, 2] [cf, 0] [cg, 6] [ch, 1] [ci, 5] [cj, 5] [ck, 4] [cl, 7] [cm, 6] [cn, 5] [co, 6] [cp, 9] [cq, 3] [cr, 7] [cs, 4] [ct, 5] [cu, 2] [cv, 5] [cw, 4] [cx, 7] [cy, 4] [cz, 4] [da, 3] [db, 0] [dc, 7] [dd, 8] [de, 6] [df, 8] [dg, 8] [dh, 4] [di, 3] [dj, 1] [dk, 4] [dl, 9] [dm, 2] [dn, 0] [do, 6] [dp, 8] [dq, 9] [dr, 2] [ds, 6] [dt, 6] [du, 4] [dv, 9] 
정렬 후 ----
[db, 0] [cf, 0] [az, 0] [cb, 0] [aq, 0] [dn, 0] [bx, 0] [am, 0] [ch, 1] [bm, 1] [bq, 1] [dj, 1] [au, 1] [aj, 1] [ca, 1] [dr, 2] [bg, 2] [bf, 2] [ce, 2] [bn, 2] [ba, 2] [ay, 2] [ah, 2] [cu, 2] [as, 2] [dm, 2] [ak, 2] [bp, 3] [cq, 3] [cd, 3] [bk, 3] [aa, 3] [by, 3] [ae, 3] [ao, 3] [da, 3] [bb, 3] [di, 3] [dk, 4] [du, 4] [bv, 4] [cs, 4] [bs, 4] [dh, 4] [cw, 4] [ck, 4] [cy, 4] [cz, 4] [bw, 5] [cn, 5] [ci, 5] [cj, 5] [ad, 5] [af, 5] [ct, 5] [cv, 5] [bd, 5] [co, 6] [dt, 6] [ds, 6] [do, 6] [ab, 6] [de, 6] [cm, 6] [cg, 6] [cc, 6] [bz, 6] [ag, 6] [ap, 6] [ar, 6] [at, 6] [bl, 6] [bc, 7] [ac, 7] [al, 7] [aw, 7] [cl, 7] [dc, 7] [bj, 7] [cr, 7] [cx, 7] [bt, 7] [dd, 8] [df, 8] [dg, 8] [av, 8] [bh, 8] [bu, 8] [dp, 8] [dl, 9] [dv, 9] [cp, 9] [ai, 9] [dq, 9] [an, 9] [br, 9] [bo, 9] [ax, 9] [bi, 9] [be, 9] 
stable_sort ----
[am, 0] [aq, 0] [az, 0] [bx, 0] [cb, 0] [cf, 0] [db, 0] [dn, 0] [aj, 1] [au, 1] [bm, 1] [bq, 1] [ca, 1] [ch, 1] [dj, 1] [ah, 2] [ak, 2] [as, 2] [ay, 2] [ba, 2] [bf, 2] [bg, 2] [bn, 2] [ce, 2] [cu, 2] [dm, 2] [dr, 2] [aa, 3] [ae, 3] [ao, 3] [bb, 3] [bk, 3] [bp, 3] [by, 3] [cd, 3] [cq, 3] [da, 3] [di, 3] [bs, 4] [bv, 4] [ck, 4] [cs, 4] [cw, 4] [cy, 4] [cz, 4] [dh, 4] [dk, 4] [du, 4] [ad, 5] [af, 5] [bd, 5] [bw, 5] [ci, 5] [cj, 5] [cn, 5] [ct, 5] [cv, 5] [ab, 6] [ag, 6] [ap, 6] [ar, 6] [at, 6] [bl, 6] [bz, 6] [cc, 6] [cg, 6] [cm, 6] [co, 6] [de, 6] [do, 6] [ds, 6] [dt, 6] [ac, 7] [al, 7] [aw, 7] [bc, 7] [bj, 7] [bt, 7] [cl, 7] [cr, 7] [cx, 7] [dc, 7] [av, 8] [bh, 8] [bu, 8] [dd, 8] [df, 8] [dg, 8] [dp, 8] [ai, 9] [an, 9] [ax, 9] [be, 9] [bi, 9] [bo, 9] [br, 9] [cp, 9] [dl, 9] [dq, 9] [dv, 9]
```
- stable_sort는 원소가 삽입되어 있는 순서를 보존하는 정렬 방식이다.
- 크기 비교는 이름과 관계 없이 모두 age로 하게 된다.
  - age가 같다면 크기가 같다고 볼 수 있다.
- stable_sort는 같은 age라면 삽입된 순서(즉, name 순서)로 나오게 되는 것이다.
- sort함수의 경우 정렬과정에서 원소들 간 상대적 위치를 랜덤하게 바꿔버릴 수 있지만 그 순서를 처음 넣었던 상태 그대로 유지하려면 stable_sort를 써야한다.
  - 그래서 좀 더 오래걸린다.
- sort는 최악의 경우에도 O(N log N)을 보장하지만 stable_sort는 최악의 경우 O(N (logN)^2))으로 작동하게 된다.