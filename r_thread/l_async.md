# Overview
- promise, packaged_task는 비동기적으로 실행하기 위해서 thread를 명시적으로 생성해서 실행해야만 했다.
- std::async에 어떤 함수를 전달하면, 아예 thread를 알아서 만들어서 해당 함수를 비동기적으로 실행하고 그 결과값을 future에 전달한다.

```cpp
#include <future>
#include <iostream>
#include <thread>
#include <vector>

int sum(const std::vector<int>& v, int start, int end)
{
    int total = 0;
    for (int i = start; i < end; ++i)
    {
        total += v[i];
    }

    return total;
}

int parallel_sum(const std::vector<int>& v)
{
    // 1 ~ 500 까지 비동기적으로 더한다.
    // lambda 사용하면 더 깔끔하게 표현할 수도 있다.
    // std::async([&v]() { return sum(v, 0, v.size() / 2); })
    std::future<int> lower_half_future =
        std::async(std::launch::async, sum, cref(v), 0, v.size() / 2);
    
    // 501 ~ 1000까지 더한다.
    int upper_half = sum(v, v.size() / 2, v.size());

    return lower_half_future.get() + upper_half;
}

int main()
{
    std::vector<int> v;
    v.reserve(1000);

    for (int i = 0; i < 1000; ++i)
        v.emplace_back(i + 1);
    
    std::cout << "1 부터 1000까지의 합 : " << parallel_sum(v) << std::endl;
}
```

```
1 부터 1000까지의 합 : 500500
```
- async()는 인자로 받은 함수를 비동기적으로 실행한 후 해당 결과값을 보관할 future를 return한다.
  - 첫번째 인자 : 어떠한 형태로 실행할 것인가 ?
    - std::launch::async : 바로 thread 생성해서 인자로 전달된 함수 실행
    - std::launch::dferred : future의 get 함수가 호출 되었을 때 실행 (새로운 thread 생성안한다.)
    - launch::asynch는 그 자리에서 thread 생성해서 실행하게 되고, launch::deferred는 future의 get을 했을 때 비로소 동기적으로 실행하게 되는 것이다.
    - 해당 함수를 바로 당장 비동기적으로 실행할 필요가 없으면 deferred 옵션을 주면된다.
- async()는 실행 함수의 결과를 포함한 future를 return 한다.
  - 결과값은 return future의 get()을 통해 얻어낼 수 있다.
- 2개 thread를 통해 실행한다고 보면 1 ~ 500은 async를 통해, 생성된 새로운 thread를 통해 더하고 나머지 501 ~ 1000은 원래 thread에서 처리하므로 2배 빠르게 작업을 수행할 수 있다.

```cpp
#include <future>
#include <iostream>
#include <thread>

int do_work(int x)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return x;
}

void do_work_parallel()
{
    auto f1 = std::async([]() { do_work(3); });
    auto f2 = std::async([]() { do_work(3); });

    do_work(3);

    f1.get();
    f2.get();
}

void do_work_sequential()
{
    do_work(3);
    do_work(3);
    do_work(3);
}
```

```cpp
int main() { do_work_parallel(); }
```

```
time ./ex2
```
- time은 프로그램 실행 시간과 CPU 사용률을 간단히 측정할 수 있는 프로그램이다.

```
real    0m3.005s
user    0m0.000s
sys     0m0.005s
```
- 3초 정도가 걸리는 것을 알 수 있다.
- do_work()를 비동기적으로 호출했기 때문이다.
  - 즉, 3개의 do_work()를 동시에 각기 다른 thread에서 실행한 덕분에 3초만에 끝났다.

```cpp
int main { do_work_sequential(); }
```

```
time ./ex2_seq

real    0m9.005s
user    0m0.005s
sys     0m0.000s
```
- 동기적으로 실행하면 3 + 3 + 3 = 9 초가 걸림을 알 수 있다.
---
- C++에서 제공하는 promise, future, packaged_task, async를 잘 활용하면 귀찮게 mutex, condition_variable을 사용하지 않고도 매우 편리하게 비동기적 작업을 수행할 수 있다.