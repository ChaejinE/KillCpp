# std::promise & std::future
- 비동기적 실행으로 하고 싶은 일은 어떤 데이터를 다른 thread를 통해 처리해서 받아내는 것이다.
- 어떤 thread T를 통해 비동기적으로 값을 받아내겠다는 의미는 **미래에(future) thread T가 원하는 data 를 돌려주겠다는 약속 (promise)**라고 볼 수 있다.

```cpp
#include <future>
#include <iostream>
#include <string>
#include <thread>
using std::string;

void worker(std::promise<string>* p)
{
    p->set_value("some data");
}

int main()
{
    std::promise<string> p;

    // 미래 string 데이터를 돌려주겠다는 약속
    std::future<string> data = p.get_future();

    std::thread t(worker, &p);

    // 미래에 약속된 데이터를 받을 때까지 기다린다.
    data.wait();

    // wait이 return 했다는 것은 future에 데이터가 준비되었다는 것을 의미
    // wait 없이 그냥 get 해도 wait 한 것과 같긴하다.
    std::cout << "받은 데이터 : " << data.get() << std::endl;

    t.join();
}
```

```
받은 데이터 : some data
```
- pomise 객체 정의 시 연산을 수행후에 돌려줄 객체의 type을 템플릿 인자로 받는다.
- 연산이 끝나고 promise 객체는 자신이 가지고 있는 future 객체에 값을 넣어주게 된다.
  - promise 객체에 대응되는 future 객체는 get_future 함수를 통해 얻을 수 있다.
  - 실제 결과를 포함하기 위해서는 promise 객체가 자신의 future 객체에 데이터를 제공한 후에 future 객체의 get 함수를 통해 얻어 낼 수 있다.
- 중요한 점은 promise가 future에 값을 전달하기 전까지 wait 함수가 기다린다는 점이다.
  - wait()을 따로 호출할 필요는 없는데, get()을 바로 호출하더라도 알아서 promise가 future에 객체를 전달할 때까지 기다린 다음에 Return 한다.
  - get() 호출 시 future 객체 내에 있던 data가 이동되는 것이다.
  - 설정된 객체가 이동되는 것이므로 절대로 get을 두번 호출하면 안된다.
- promise : producer 역할, future : consumer 역할을 수행한다고 보면된다.

```cpp
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

std::condition_variable cv;
std::mutex m;
bool done = false;
std::string info;

void worker()
{
    {
        std::lock_guard<std::mutex> lk(m);
        info = "some data"; // p->set_value에 대응
        done = true;
    }

    cv.notify_all();
}

int main()
{
    std:: thread t(worker);

    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [] { return done; }); // data.wait()에 대응
    lk.unlock();

    std::cout << "받은 데이터 : " << info << std::endl;

    t.join();
}
```
- 이처럼 promise - future 패턴을 표현을 구현할 수 있지만 promise와 future를 이용하는 것이 훨씬 깔끔하고 이해하기도 쉽다.
  - future에 예외도 전달할 수 있어 더 우수하다.

```cpp
#include <exception>
#include <future>
#include <iostream>
#include <string>
#include <thread>
using std::string;

void worker(std::promise<string>* p)
{
    try
    {
        throw std::runtime_error("Some Error");
    } catch (...) {
        p->set_exception(std::current_exception());
    }
}

int main()
{
    std::promise<string> p;

    std::future<string> data = p.get_future();
    std::thread t(worker, &p);

    data.wait();

    try
    {
        data.get();
    } catch (const std::exception& e) {
        std::cout << "예외 : " << e.what() << std::endl;
    }

    t.join();
}
```

```
예외 : Some Error
```
- 위와 같이 예외가 제대로 전달되었음을 알 수 있다.
- set_exception() : 예외 객체가 아니라 exception_ptr을 전달해야된다.
  - exception_ptr : catch로 받은 예외 객체 포인터가 아니라 현재 catch 된 예외에 관한 정보를 반환하는 current_exception 함수가 return하는 객체다.
- 위 처럼 get 함수를 호출하면서 future에 전달된 예외 객체가 던져지고 마치 try와 catch문을 사용한 것처럼 예외 처리를 할  수 있게 된다.

## wait_for
```cpp
#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <string>
#include <thread>

void worker(std::promise<void>* p)
{
    std::this_thread::sleep_for(std::chrono::seconds(10));
    p->set_value();
}

int main()
{
    // void의 경우 어떤 객체도 전달하지 않지만, future가 set 되었는지 안되었는지에 대한
    // flag 역할을 수행할 수 있다.
    std::promise<void> p;

    std::future<void> data = p.get_future();

    std::thread t(worker, &p);

    while(true)
    {
        std::future_status status = data.wait_for(std::chrono::seconds(1));

        if (status == std::future_status::timeout)
        {
            std::cerr << ">";
        }
        else if (status == std::future_status::ready)
        {
            break;
        }
    }

    t.join();
}
```

```
>>>>>>>>>
```
- wait_for 함수는 promise가 설정될 때까지 기다리는 대신 전달된 시간 만큼 기다렸다가 바로 return 해버린다.
  - return 값은 future의 상태를 나타내는 future_status 객체다.
- future_stats
  - ready : 값이 설정 됬을 때
  - timeout : wait_for에 지정한 시간이 지났지만 값이 설정되지 않아서 return한 경우
  - deferred : 결과 값을 계산하는 함수가 실행되지 않았다는 의미

# shared_future
```cpp
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
using std::thread;

void runner(std::shared_future<void>* start)
{
    start->get();
    std::cout << "start" << std::endl;
}

int main()
{
    std::promise<void> p;
    std::shared_future<void> start = p.get_future();

    thread t1(runner, &start);
    thread t2(runner, &start);
    thread t3(runner, &start);
    thread t4(runner, &start);

    // cerr : 버퍼를 사용하지 않고 터미널에 바로 출력
    std::cerr << " 준비 ";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cerr << " 땅 " << std::endl;

    p.set_value();

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
```

```
 준비  땅 
start
start
start
start
```
- future의 경우 한번만 get()을 할수 있었다.
  - get() 호출 시 future 내부 객체가 이동되기 떄문이다.
- 종종 여러 개의 다른 thread에서 future를 get할 필요가 있다.
- condition_variable로 동일하게 작성할 수 있으나 보다시피 future를 사용하는 것이 훨씬 편리하다.