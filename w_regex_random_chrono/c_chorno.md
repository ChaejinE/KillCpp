# Overview
- 시간 관련 데이터를 쉽게 계산할 수 있도록 도와주는 라이브러리다.
- 크게 3가지 요소들로 구성되어 있다.
  - 1. 현재 시간을 알려주는 시계 (system_clock)
  - 2. 특정 시간을 나타내는 time_stamp
  - 3. 시간의 간격을 나타내는 duration

## 지원하는 clock들
- 여러 종류으 시계들을 지원하고 있다.
- 일반적 상황에서 현재 컴퓨터 상 시간을 얻기위해서는 std::system_clock을, 좀 더 정밀한 시간 계산은 std::high_resolution_clock(프로그램 성능 측정 등)
- 지정된 시점으로 부터 **몇 번의 tick이 발생했는지 알려주는 time_stamp 객체**를 리턴 한다.
  - 예를들어 std::system_clock의 경우 1970/1/1 부터 현재까지 발생한 틱의 수를 리턴한다고 보면된다. (Unix Time)
- time_stamp 객체는 clock의 시작점과 현재 시간의 duration을 보관하는 객체다.
  - 컴퓨터도 내부 시계가 있어 특정 진동수로 tick tock 한다.
  - 시계마다 정밀도가 달라 clock에서 얻어지는 tick 값 자체는 조금씩 다르다. system_clock이 1초에 1tick이면 high_resolution_clock은 0.00000001초 마다 1tick인 느낌이다.

# chrono
```cpp
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(0, 1000);

    for (int total = 1; total <= 1000000; total *= 10)
    {
        std::vector<int> random_numbers;
        random_numbers.reserve(total);

        std::chrono::time_point<std::chrono::high_resolution_clock> start = 
            std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < total; i++)
            random_numbers.push_back(dist(gen));

        std::chrono::time_point<std::chrono::high_resolution_clock> end =
            std::chrono::high_resolution_clock::now();

        auto diff = end - start;

        std::cout << std::setw(7) << total
                  << "개 난수 생성 시 틱 횟수 : " << diff.count() << std::endl;
    }
}
```

```
      1개 난수 생성 시 틱 횟수 : 12657
     10개 난수 생성 시 틱 횟수 : 1547
    100개 난수 생성 시 틱 횟수 : 13317
   1000개 난수 생성 시 틱 횟수 : 137909
  10000개 난수 생성 시 틱 횟수 : 1535518
 100000개 난수 생성 시 틱 횟수 : 16296068
1000000개 난수 생성 시 틱 횟수 : 73282811
```
- chrono의 경우 다른 표준 라이브러리들과 달리 std::chrono namespace 공간안에 정의되어 있다.
- clock에는 현재 time_point를 리턴하는 static 함수인 now가 정의되어 잇다. now()를 호출하면 clock에 맞는 time_point 객체를 리턴한다.
- 우리는 high_resolution_clock::now()를 호출했으므로 std::chrono::time_point<\std::chrono::highresolution_clock>을 리턴한다.
- time_point가 clock을 템플릿 인자로 가지는 것은 clock 마다 1초에 발생하는 틱 횟수가 모두 달라서 실제 시간 변환 시 어떤 clock을 사용했는지에 대한 정보가 필요하기 때문이다.
- time_stamp의 차이는 duration 객체를 리턴한다.
  -  end - start
  - C++17 이상이라면 auto 로 받아서 사용하면된다.
  - 그 이전에는 duration 객체 템플릿인자를 전달해야하긴했다.
- duration 객체의 count() 메서드는 해당 시간 차이 동안 몇 번의 틱이 발생했는지를 알려준다.
- 실제 시간을 알기 위해서는 duration_cast를 사용하면된다.
  ```cpp
    std::chrono::duration_cast<std::chrono::microseconds>(diff).count()
  ```
  - duration_cast는 임의의 duration 객체를 받아 원하는 duration으로 캐스팅할 수 있다.
  - std::chrono::microseconds는 미리 정의 되어 있는 duration 객체중 하나로 1초에 10^6 번 tick을 하게 된다.
  - microseconds로 캐스팅한 뒤 리턴하는 count는 duration이 몇 마이크로초 인지를 나타낸다.
  - nanoseconds, milliseconds, seconds, minutes, hours가 정의되어있다.

# 현재 시간을 날짜로
- C++17 까지는 chrono 라이브러리 상 날짜를 간단하게 다룰 수 있도록 도와주는 클래스가 없다.
  - C의 함수들에 의존해야한다.

```cpp
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

int main()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::cout << " now time : " << std::put_time(std::localtime(&t), "%F %T %z")
              << '\n';
}
```

```
 now time : 2022-01-02 15:36:08 +0900
```
- system_clock에서 현재 time_point를 얻어온 후 날짜를 출력하기 위해 time_t객체로 변환해야한다.
  - 각 clock이 제공하는 static 함수인 to_time_t를 사용하면된다.
- 그 후 현재 시간을  std::tm 객체로 변환하기 위해 std::localtime에 t를 전달했고, std::put_time을 사용해 우리가 원하는 형태의 문자열로 구성할 수 있게 된다.