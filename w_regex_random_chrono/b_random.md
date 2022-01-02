# Overview
- 난수 (random number)를 쉽게 생성해주는 라이브러리다.
- C++에서는 C의 srand, rand는 갖다 버린다.
  - 난수의 품질이 좋지 않다. (자세한건 모름)

# random library
```cpp
#include <iostream>
#include <random>

int main()
{
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dis(0, 99);

    for (int i = 0; i < 5; i++)
        std::cout << "난수 : " << dis(gen) << std::endl;
}
```

```
난수 : 12
난수 : 1
난수 : 54
난수 : 73
난수 : 23
```
- C++에서는 양질의 시드값을 얻기 위해 random_divice라는 것을 제공한다.
- 운영체제 대부분은 진짜 난수를 얻을 여러 방식을 제공하는데, 리눅스의 경우 /dev/random, /dev/urandom 등을 통해 난수값을 얻을 수 있다. 무슨 수학적 알고리즘이 아니라 진짜 컴퓨터 실행하면서 마주치는 무작위적인 요소들을 기반으로 진정한 난수를 제공한다. (장치 드라이버들의 noise 등 기반)
- random_device를 이용하면 운영체제 단에서 제공하는 진짜 난수를 사용할 수 있다.
  - 난수 생성속도가 매우 느리긴하다.(주변 환경과 상호 작용하면서 만들기 때문)
  - 따라서 난수 엔진을 초기화 하는데 사용하고 그 이후 난수열은 난수 엔진으로 생성하는 것이 적합하다.
- random_device 객체를 이용해서 난수 생성 엔진 객체를 정의할 수 있다.

```cpp
std::mt199937 gen(1234);
```
- random_device 대신 원하는 시드값을 넣어서 난수 생성 엔진을 정의할 수도 있다.
- mt199937은 메르센 트위스터라는 알고리즘을 사용하고, c의 rand의 선형 합동 방식보다 양질의 난수열을 생성한다고 알려져있다.
  - 하지만 생각보다 객체 크기가 커서 메모리가 부족한 시스템에서는 오히려 minstd_rand가 적합할 수 있다.
- 생성 엔진을 만든 후 C++의 경우 어디에서 수를 뽑아낼 지 분포를 정의해야한다.
  - 위에서는 균등한 확률로 정수를 뽑아내고 싶으므로 uniform distribution 객체를 정의했다.
- 분포에 사용할 난수 엔진을 전달함으로써 균등 분포에서 무작위로 샘플을 뽑아낼 수 있다.

```cpp
#include <iomanip>
#include <iostream>
#include <map>
#include <random>

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0, 1); // 평균, 표준편차

    std::map<int, int> hist{};
    for (int n = 0; n < 10000; ++n)
        ++hist[std::round(dist(gen))];

    for (auto p : hist)
    {
        std::cout << std::setw(2) << p.first << ' '
                  << std::string(p.second / 100, '*') << " " << p.second << '\n';
    }
}
```

```
-4  1
-3  62
-2 ****** 607
-1 ************************ 2459
 0 ************************************* 3769
 1 ************************ 2416
 2 ****** 624
 3  62
```
- 평균이 0이고 표준편차가 1인 정규 분포를 정의했다.
- 이를 바탕으로 정규분포에서 10000개의 샘플을 무작위로 뽑아내게되고, 저육 분포 곡선이 나옴을 확읺라 수 있었다.