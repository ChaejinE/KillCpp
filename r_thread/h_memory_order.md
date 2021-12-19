# Overview
- atomic 객체들의 경우 원자적 연산 시 메모리에 접근 할 때 어떤 방식으로 접근할지를 지정할 수 있다.

# memory_order_relexed
- 가장 느슨한 조건이다.
- 이 방식으로 메모리에서 read/write 할 경우 주위 다른 메모리 접근들과 순서가 바뀌어도 무방한 것이다.

```cpp
#include <atomic>
#include <cstdio>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

void t1(std::atomic<int>* a, std::atomic<int>* b)
{
    b->store(1, memory_order_relaxed); // b = 1, write
    int x = a->load(memory_order_relaxed); // x = a, read

    printf("x : %d \n" , x);
}

void t2(std::atomic<int>* a, std::atomic<int>* b)
{
    a->store(1, memory_order_relaxed); // a = 1, write
    int y = b->load(memory_order_relaxed); // y = b, read

    printf("y : %d \n", y);
}

int main()
{
    std::vector<std::thread> threads;

    std::atomic<int> a(0);
    std::atomic<int> b(0);

    threads.push_back(std::thread(t1, &a, &b));
    threads.push_back(std::thread(t2, &a, &b));

    for (int i = 0; i < 2; i++)
        threads[i].join();
}
```

```
x : 0 
y : 1 
```
- store와 load는 atomic 객체들에 대해 원자적으로 쓰기와 읽기를 지원해주는 함수다.
  - 추가로 어떤 방식으로 memory_order를 지정할지 전달한다.

```
x : 0
y : 0
```
- 위와 같은 결과를 볼 수 있을까? 상식적으로 는 불가능하다.
- x = a, y = b 시점에서 모두 a, b가 0이어야만 하기 때문이다.
- 위 명령어들이 순서대로 실행된다면 이는 불가능하다는 것을 알 수 있다.
  - x에 0이 들어가려면 a가 0이어야한다. 즉, x = a 가 실행된 시점에서 a = 1이 실행되지 않았어야한다는 것이다.
    - t2 에서 y = b 할 때 이미 b는 1인 상태이므로 y는 반드시 1이 출력되어야한다.
- 하지만 실제로 위 설명같지 않다. memory_order_relaxed는 메모리 연산들 사이에서 어떤 제약조건도 없다.
  - 서로 다른 변수의 relaxed 메모리 연산은 CPU 마음대로 재배치 할 수 있다. (단일 스레드 관점에서 결과가 동일하다면)

```cpp
int x = a->load(memory_order_relaxed); // x = a, read
b->store(1, memory_order_relaxed); // b = 1, write
```
- 위와 같은 순으로 CPU가 순서를 재배치해서 실행해도 무방하다는 뜻이다.
  - 이러면 x, y에 모두 0이 들어갈 수 있다.
- memory_order_relaxed는 CPU에서 메모리 연산 순서에 관련해서 무한한 자유를 주는 것과 같다. 덕분에 CPU에서 매우 빠른 속도로 실행할 수 있게 된다.
  - 예상치 못한 결과가 나올 수 있지만 종종 사용할 수 있는 경우가 있다.

```cpp
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

void worker(std::atomic<int>* counter)
{
    for (int i = 0; i < 10000; i++)
    {
        counter->fetch_add(1, memory_order_relaxed);
    }
}

int main()
{
    std::vector<std::thread> threads;

    std::atomic<int> counter(0);

    for (int i = 0; i < 4; i++)
    {
        threads.push_back(std::thread(worker, &counter));
    }

    for (int i = 0; i < 4; i++)
        threads[i].join();

    std::cout << "Counter : " << counter << std::endl;
}
```

```
Counter : 40000
```
- 중요한 부분은 counter->fetch_add(...) 부분이다.
  - counter++와 정확히 하는 일이 동일하지만 counter++와 다르게 메모리 접근 방식을 설정할 수 있다.
  - 이 문장 역시 원자적으로 counter의 값을 읽고 +1 하고 그 결과를 다시 쓴다.
- memory_order_relaxed를 사용할 수 있던 이유는 다른 메모리 연산들 사이에서 굳이 counter를 증가시키는 작업을 재배치 못하게 막을 필요가 없기 때문이다.
  - 다른 메모리 연산들 보다 counter++이 늦게 된다고 하더라도 결과적으로 **증가만 하면 되기 때문에 문제될게 없기 때문이다.**

# memory_order_acquire & memory_oreder_release
- CPU에 너무 많은 자유를 부여하는 것은 사용 용도가 꽤나 제한적으로 된다는 것을 의미한다.
- 살짝 엄격한 친구들을 살펴보자.

```cpp
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

void producer(std::atomic<bool>* is_ready, int* data) {
  *data = 10;
  is_ready->store(true, memory_order_relaxed);
}

void consumer(std::atomic<bool>* is_ready, int* data) {
  // data 가 준비될 때 까지 기다린다.
  while (!is_ready->load(memory_order_relaxed)) {
  }

  std::cout << "Data : " << *data << std::endl;
}

int main() {
  std::vector<std::thread> threads;

  std::atomic<bool> is_ready(false);
  int data = 0;

  threads.push_back(std::thread(producer, &is_ready, &data));
  threads.push_back(std::thread(consumer, &is_ready, &data));

  for (int i = 0; i < 2; i++) {
    threads[i].join();
  }
}
```

```
Data : 10
```
- producer thread를 살펴보면, is_ready에 쓰는 연산이 relaxed이므로 *data=10과 순서가 바뀌어서 실행된다면 is_ready가 true 임에도 *data=10 이 실행이 끝나지 않을 수 있다.
  - 따라서 consumer thread에서 is_ready가 true임에도 제대로된 data를 읽을 수 없다.
  - Data : 0 같은 결과가 나올 수도 있다는 것이다.
- consumer thread에서도 마찬가지다.
  - data를 읽는 부분과 is_ready를 읽는 부분 순서가 바뀌면 is_ready가 true 되기 전에 data를 읽어버릴 수 있는 문제가 생긴다.
- 따라서 위와 같은 생산자 소비자 관계에서는 memory_order_relaxed를 사용할 수 없다.

```cpp
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

void producer(std::atomic<bool>* is_ready, int* data) {
  *data = 10;
  is_ready->store(true, std::memory_order_release);
}

void consumer(std::atomic<bool>* is_ready, int* data) {
  // data 가 준비될 때 까지 기다린다.
  while (!is_ready->load(std::memory_order_acquire)) {
  }

  std::cout << "Data : " << *data << std::endl;
}

int main() {
  std::vector<std::thread> threads;

  std::atomic<bool> is_ready(false);
  int data = 0;

  threads.push_back(std::thread(producer, &is_ready, &data));
  threads.push_back(std::thread(consumer, &is_ready, &data));

  for (int i = 0; i < 2; i++) {
    threads[i].join();
  }
}
```

```
Data : 10
```
- 위와 같은 경우 data에 0이들어가는 일은 불그낭하다.
- memory_order_release : 해당 명령 이전의 모든 메모리 명령들이 해당 명령 이후로 재배치되는 것을 금지한다.
- memory_order_acquire : 이 명령으로 읽는 thread가 있다면 memory_order_release 이전에 오는 모든 메모리 명령들이 해당 thread에 의해서 관찰될 수 있어야한다.
- 쉽게 말해 is_ready->store(...); 밑으로 *data=10이 올 수 없게된다.
- 또한, is_ready가 true가 되면 memory_order_acquire로 is_ready를 읽는 thread에서 data의 값을 확인 했을 때 10임을 관찰할 수 있어야한다.
  - 실제로 consumer thread에서 is_ready를 memory_order_acquire로 load하고 있어 is_ready가 true가 되면 data는 반드시 10이어야만 한다.
- memory_order_acquire의 경우, release와는 반대로 **해당 명령 뒤에 오는 모든 메모리 명령들이 해당 명령 위로 재배치 되는 것을 금지**한다.
  - 이와 같이 두 개의 다른 thread들이 같은 변수의 release와 acquire을 통해 동기화를 수행하는 것을 볼 수 있다.

```cpp
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

std::atomic<bool> is_ready;
std::atomic<int> data[3];

void producer()
{
    data[0].store(1, memory_order_relaxed);
    data[1].store(2, memory_order_relaxed);
    data[2].store(3, memory_order_relaxed);

    is_ready.store(true, std::memory_order_release);
}

void consumer()
{
    while (!is_ready.load(std::memory_order_acquire)) {}

    std::cout << "data[0] : " << data[0].load(memory_order_relaxed) << std::endl;
    std::cout << "data[1] : " << data[1].load(memory_order_relaxed) << std::endl;
    std::cout << "data[2] : " << data[2].load(memory_order_relaxed) << std::endl;
}

int main()
{
    std::vector<std::thread> threads;

    threads.push_back(std::thread(producer));
    threads.push_back(std::thread(consumer));

    for (int i = 0; i < 2; i++)
        threads[i].join();
}
```

```
data[0] : 1
data[1] : 2
data[2] : 3
```
- data의 원소들을 store 하는 명령들은 모두 relaxed해서 자기들끼리는 CPU 에서 마음대로 재배치 될 수 있으나 release 명령을 넘어가서 재배치 될 수는 없다.

![image](https://user-images.githubusercontent.com/69780812/146666603-03139c42-a83c-4cd1-b57d-9cb6c1efc096.png)
- consumer 에서 data들의 값을 확인했을 때, 언제나 정확히 1, 2, 3이 들어가 있게 된다 !

# memory_order_acq_rel
- acquire과 release를 모두 수행한다.
- 읽기와 쓰기 모두 수행하는 명령들, fetch_add와 같은 함수에서 사용될 수 있다.

# memory_order_seq_cst
- 메모리 명령의 순차적 일관성을 보장해준다.
  - sequential consistency : 메모리 명령 재배치도 없고, 모든 Thread에서 모든 시점에 동일한 값을 관찰할 수 있다. 내가 알고 있는 CPU가 작동하는 방식이다.
- 메모리 명령들 사이에서 이러한 순차적 일관성을 보장해주는 함수다.

```cpp
#include <atomic>
#include <iostream>
#include <thread>

std::atomic<bool> x(false);
std::atomic<bool> y(false);
std::atomic<int> z(0);

void write_x() { x.store(true, std::memory_order_release); }

void write_y() { y.store(true, std::memory_order_release); }

void read_x_then_y()
{
    while (!x.load(std::memory_order_acquire)) {}
    if (y.load(std::memory_order_acquire)) { ++z; }
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_acquire)) {}
    if (x.load(std::memory_order_acquire)) { ++z; }
}

int main()
{
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);

    a.join();
    b.join();
    c.join();
    d.join();

    std::cout << "z : " << z << std::endl;
}
```

```
z : 2
```
- z : 1이 나오기도 한다.
- write_x와 read_x_then_y 사이 release - acquire 동기화, write_y, read_y_then_x 사이 release - acquire 동기화가 이뤄지고 있다.
- 하지만 read_x_then_y와 read_y_then_x 두 thread가 같은 순서로 x.store, y.store를 관찰한다는 보장이 없다.
  - read_x_then_y 입장에서는 x.store가 y.store보다 먼저 발생해도 되고 그 반대도 그러하다.
- memory_order_seq_cst를 사용하게되면 메모리 연산들 끼리는 모든 thread에서 동일한 연산 순서를 관찰할 수 있도록 보장해준다.
  - 참고로 atomic객체 사용 시 memory_order를 지정해주지 않으면 default로 memory_order_seq_cst가 지정된다.
- ARM 계열의 CPU와 같은 경우 순차적 일관성을 보장하기 위해서 CPU의 동기화 비용이 매우크다. 그래서 이 명령은 꼭 필요할 때만 사용해야한다.

```cpp
#include <atomic>
#include <iostream>
#include <thread>

std::atomic<bool> x(false);
std::atomic<bool> y(false);
std::atomic<int> z(0);

void write_x() { x.store(true, std::memory_order_seq_cst); }

void write_y() { y.store(true, std::memory_order_seq_cst); }

void read_x_then_y()
{
    while (!x.load(std::memory_order_seq_cst)) {}
    if (y.load(std::memory_order_seq_cst)) { ++z; }
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst)) {}
    if (x.load(std::memory_order_seq_cst)) { ++z; }
}

int main()
{
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);

    a.join();
    b.join();
    c.join();
    d.join();

    std::cout << "z : " << z << std::endl;
}
```

```
z : 2
```
- z : 1이 나오기도 한다.
- x.store, y.store 모두 memory_order_seq_cst 이므로 read_x_then_y, read_y_then_x에서 관찰했을 때 x.store와 y.store가 같은 순서로 발생해야한다.
  - 따라서 z가 0이 되는 경우는 발생하지 않는다.

![image](https://user-images.githubusercontent.com/69780812/146672789-e8b842a8-1160-4c13-94c4-f5786091cd28.png)
- 정리해보자면 위와 같다.
- C++ atomic 객체들의 경우 따로 지정하지 않으면 기본으로 memory_order_seq_cst로 설정되어 일부 CPU에서 매우 값비싼 명령이다.
  - 제약 조건을 좀 더 느슨하게 할 수 있을 때 더 약한 수준의 memory_order를 사용한다면 프고르매의 성능을 더 크게 향상시킬 수 있다.