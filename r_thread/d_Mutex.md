# Mutex
- 이전 race condition 내용에서 문제의 근본적 이유는 counter += 1; 이 부분을 여러 스레드에서 동시에 실행한 것이다.
- 접근을 통제하는 방법을 도입해야 완벽히 해결할 수 있다.
- C++에선 이러한 기능을 하는 객체를 제공하고있다.
  - mutex

```cpp
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void worker(int& result, std::mutex& m)
{
    for (int i = 0; i < 10000; i++)
    {
        m.lock();
        result += 1;
        m.unlock();
    }
}

int main()
{
    int counter = 0;
    std::mutex m;

    std::vector<std::thread> workers;
    for (int i = 0; i < 4; i++)
    {
        workers.push_back(std::thread(worker, std::ref(counter), std::ref(m)));
    }

    for (int i = 0; i < 4; i++)
        workers[i].join();

    std::cout << "Counter Result : " << counter << std::endl;
}
```

```
Counter Result : 40000
```
- 제대로 값이 나오는 것을 확인할 수 있다.
- mutex : 상호 배제 (mutual exclusion)이라는 단어에서 따온 단어다.
  - 어떤 스레드들이 동시에 어떤 코드에 접근하는 것을 배제한다는 의미다.
- m.lock() : 뮤텍스 m을 내가 쓰게해달라고 말하는 것이다.
  - 한 번에 한 쓰레드만 m의 사용 권한을 갖는다는 것이 중요하다.
  - m.unlock()을 통해 m을 반환할 때 까지 무한정 기다리게 된다.
- result += 1;은 아무리 많은 스레드들이 서로 다른 코어에서 돌아가더라도 결국 m은 한번에 한 스레드만 얻을 수 있게된다.
  - 결국 한 스레드만 유일하게 실행할 수 있게 된다.
- m.lock(), m.unlock() 사이 한 스레드만 유일하게 실행할 수 있는 코드 부분을 **임계영역 (critical section)**이라고 부른다.
  - unlock()안하면 프로그램이 끝나지 않아 강제로 종료해야만 한다. 잘 써주자. (이러한 상황을 **Deadlock** 이라한다.)

```cpp
void worker(int& result, std::mutex& m)
{
    for (int i = 0; i < 10000; i++)
    {
        std::lock_guard<std::mutex> lock(m);
        result += 1;
    }
}
```
- mutex도 unique_ptr 처럼 소멸자에서 해제해주는 처리를 통해 까먹어도 조심하도록 했다. 뮤텍스도 사용 후 해제 패턴을 따르므로 소멸자에서 처리할 수 있다.
- lock_guard 객체는 뮤텍스를 인자로 받아 생성하는데 이 때 생성자에서 뮤텍스를 lock하게되고 소멸될 때 알아서 Lock 했던 뮤텍스를 unlcok 하게 된다.
  - 사용자가 따로 unlcok을 신경쓰지 않아서 매우 편리하다.