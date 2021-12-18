# Deadlcok
```cpp
#include <iostream>
#include <mutex>
#include <thread>

void worker1(std::mutex& m1, std::mutex& m2)
{
    for (int i =0; i < 10000; i++)
    {
        std::lock_guard<std::mutex> lock1(m1);
        std::lock_guard<std::mutex> lock2(m2);
    }
}

void worker2(std::mutex& m1, std::mutex& m2)
{
    for (int i =0; i < 10000; i++)
    {
        std::lock_guard<std::mutex> lock2(m1);
        std::lock_guard<std::mutex> lock1(m2);
    }
}

int main()
{
    int counter = 0;
    std::mutex m1, m2;

    std::thread t1(worker1, std::ref(m1), std::ref(m2));
    std::thread t2(worker1, std::ref(m1), std::ref(m2));

    t1.join();
    t2.join();

    std::cout << "끝" << std::endl;
}
```

```
```
- 프로그램이 끝나지 않아서 강제종료해야된다.
  - 나는 끝나긴하더라.
- worker1에서는 m1을 먼저 lock하고 m2를 lock 한다. 반면, worker2는 m2를 먼저 lock 하고 m1을 lock한다.

![image](https://user-images.githubusercontent.com/69780812/146641813-1f5e816f-d4f5-47d2-8312-0fb464d142fb.png)
- 만약 worker1에서 m1을 lock, worker2에서 m2를 lock했다면, worker1에서 m2를 lock 할 수 없다.
- worker1에서 m2를 lock 하려면 worker2에서 m2를 unlock 해야된다. 하지만 그러기 위해서는 worekr2에서 m1을 lock 해야하는데 이 역시 불가능하다.
  - worker1에서 m1을 lock하고 있기 때문이다.
- 결국 이러지도 저리지도 목하는 DeadLock 상황에 빠지게된다.
- mutex를 lock 하면 반드시 unlock 한다는 원칙을 지켰음에도 DeadLock은 피할 수 없었다.
- 방법은 한 스레드에게 우선권을 주는 것이다.
- 한 스레드가 다른 스레드에 비해 우위를 가지면 한 스레드만 열심히 일하고 다른 스레드는 일할 수 없는 starvation이 발생할 수 있다.

```cpp
#include <iostream>
#include <mutex>
#include <thread>

void worker1(std::mutex& m1, std::mutex& m2)
{
    for (int i = 0; i < 10; i++)
    {
        m1.lock();
        m2.lock();
        std::cout << "Work1 : " << i << std::endl;

        m2.unlock();
        m1.unlock();
    }
}

void worker2(std::mutex& m1, std::mutex& m2)
{
    for (int i = 0; i < 10; i++)
    {
        while (true)
        {
            m2.lock();

            if (!m1.try_lock())
            {
                m2.unlock();
                continue;
            }

            std::cout << "Work2 : " << i << std::endl;
            m2.unlock();
            m1.unlock();
            break;
        }
    }
}

int main()
{
    std::mutex m1, m2;

    std::thread t1(worker1, std::ref(m1), std::ref(m2));
    std::thread t2(worker2, std::ref(m1), std::ref(m2));

    t1.join();
    t2.join();

    std::cout << "끝" << std::endl;
}
```

```
Work1 : 0
Work1 : 1
Work1 : 2
Work1 : 3
Work1 : 4
Work1 : 5
Work1 : 6
Work1 : 7
Work1 : 8
Work1 : 9
Work2 : 0
Work2 : 1
Work2 : 2
Work2 : 3
Work2 : 4
Work2 : 5
Work2 : 6
Work2 : 7
Work2 : 8
Work2 : 9
끝
```
- 데드락 상황없이 잘 실행된다.
- worker2 보다 worker1이 우선권을 갖는 상황이다.
- m2는 아무 문제 없이 lock할 수 있다. 문제는 m1을 lock 하는 과정이다.
- worker1 m1을 lock 하고 있다면 m1.lcok()을 호출한 Deadlock이 될 것이다.
- m1.try_lock()을 통해 true를 return해야 worker2가 m1과 m2를 성공적으로 lock()한 상황이므로 그대로 처리한다.
  - false 라면 worker1이 이미 m1을 lock했다는 의미이며 worker1에게 우선권을 줘야한다. 그래서 이미 얻은 m2 역시 worker1에게 제공해야 한다. 교차로에서 후진한다고 보면된다.
- 이 처럼 데드락을 해결하는 것은 매우 복잡하고 완벽하지도 않다.
- C++ Concurrency In Action이란 책에서는 가이드라인을 제시하고 있다.
  - 1. 중첩된 Lock을 사용하는 것을 피하라.
    - 모든 스레드들이 최대 1개의 lock만 소유한다면 데드락 상황이 발생하는 것을 피할 수 있다. 대부분 디자인에서는 1 개의 lock으로도 충분하다. 여러개의 lock이 필요하다면 진짜 필요한지를 되돌아보는게 좋다.
  - 2. Lock을 소유하고 있을 때 유저 코드를 호출하는 것을 피하라.
    - 유저 코드에서 Lock을 소유할 수도 있기 때문에 중첩된 Lock을 얻는 것을 피하려면 Lock 소유시 유저 코드를 호출하는 것을 지양해야 한다.
  - 3. Lock들을 언제나 정해진 순서로 획득해라.
    - 여러 Lock들을 획득할 상황이 온다면 반드시 이 Lock들을 정해진 순서로 획득해야한다.
    - 위 예제에서 데드락이 발생했던 이유 역시 worker1에서 m1, m2순으로 lock 을했지만 worker2에서는 m2, m1 순으로 lock을 했기 때문이었다.