# Overview
![image](https://user-images.githubusercontent.com/69780812/146642329-83f03b4c-1193-4f33-9b4d-6b8cea5b8ece.png)
- 멀티 스레드 프로그램에서 가장 많이 등장하는 패턴이다.

# Example
- 인터넷에서 페이지를 긁어서 분석하는 프로그램을 만든다고 생각해보자.
- 생산자 : 무언가 처리할 일을 받아오는 thread
  - 페이지를 긁어오는 thread
- 소비자 : 받은 일을 처리하는 thread
  - 긁어온 페이지를 분석하는 thread

```cpp
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

void producer(std::queue<std::string>* downloaded_pages, std::mutex* m,
              int index)
{
    for (int i = 0; i < 5; i++)
    {
        // thread 별로 다운로드 하는데 걸리는 시간이 다르다.
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
        std::string content = "Web site : " + std::to_string(i) + " from thread(" +
                               std::to_string(index) + ")\n";

        // data는 thread 사이에서 공유되므로 critical section에 넣어야한다.
        m->lock();
        downloaded_pages->push(content);
        m->unlock();
    }
}

void consumer(std::queue<std::string>* downloaded_pages, std::mutex* m,
              int* num_processed)
{
    while (*num_processed < 25)
    {
        m->lock();

        // 현재 다운로드한 페이지가 없다면 다시 대기
        if (downloaded_pages->empty())
        {
            m->unlock();

            // 10 밀리초 뒤에 다시 확인
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // 맨 앞의 페이지를 읽고 대기 목록에서 제거
        std::string content = downloaded_pages->front();
        downloaded_pages->pop();

        (*num_processed)++;
        m->unlock();

        // content 처리
        std::cout << content;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

int main()
{
    std::queue<std::string> downloaded_pages;
    std::mutex m;

    std::vector<std::thread> producers;
    for (int i = 0; i < 5; i++)
    {
        producers.push_back(std::thread(producer, &downloaded_pages, &m, i + 1));
    }

    int num_processed = 0;
    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; i++)
    {
        consumers.push_back(std::thread(consumer, &downloaded_pages, &m, &num_processed));
    }

    for (int i = 0; i < 5; i++)
        producers[i].join();
    
    for (int i = 0; i < 3; i++)
        consumers[i].join();
}
```

```
Web site : 0 from thread(1)
Web site : 1 from thread(1)
Web site : 0 from thread(2)
Web site : 0 from thread(3)
Web site : 2 from thread(1)
Web site : 0 from thread(4)
Web site : 1 from thread(2)
Web site : 3 from thread(1)
Web site : 0 from thread(5)
Web site : 4 from thread(1)
Web site : 1 from thread(3)
Web site : 2 from thread(2)
Web site : 1 from thread(4)
Web site : 3 from thread(2)
Web site : 2 from thread(3)
Web site : 1 from thread(5)
Web site : 4 from thread(2)
Web site : 2 from thread(4)
Web site : 3 from thread(3)
Web site : 2 from thread(5)
Web site : 4 from thread(3)
Web site : 3 from thread(4)
Web site : 3 from thread(5)
Web site : 4 from thread(4)
Web site : 4 from thread(5)
```

![image](https://user-images.githubusercontent.com/69780812/146642892-a49ec6a7-64af-41b2-b426-cbec2fe31aa2.png)
- producer thread에서는 웹사이트에서 페이지를 계속 다운로드 하는 역할을 하게 된다.
- 이 때 다운로드 페이지들은 downloaded_pages라는 queue에 저장된다.
  - FIFO 특성을 이용하기 위해 queue를 사용한다. 먼저 다운로드한 페이지를 먼저 처리하기 위함이다.
- std::this_thrad::sleep_for() 함수는 전달된 시간 만큼 thread를 sleep 시킨다.
  - chrono는 C++ 11에 새로 추가된 시간관련 라이브러리로 기존 C의 time.h보다 훨씬 편리한 기능을 제공한다.
- producer thread는 5개나 있다. 따라서 downloaded_pages에 접근하는 스레드들 사이에 race condition이 발생할 수 있다.
  - 이를 방지하기 위해 mutex m으로 해당 코드를 감싸서 문제가 발생하지 않도록 해준다.
---
- consumer thread 입장에서는 언제 일이 올지 알 수 없다.
- 따라서 downloaded_pages가 비어있지 않을 때까지 계속 while 루프를 돌아야한다.
  - CPU 속도에 비해 웹사이트 정보가 큐에 추가되는 속도는 매우느리다.(문제)
  - 따라서 downloaded_pages->empty() 이면 thread를 sleep 시켜서 다시 확인하도록 코드가 짜져있다.
- if 문 안에서 m->unlock() 부분을 하지 않으면 Deadlock 문제가 있었을 것이다.
- Content 처리 과정은 front를 통해 맨 앞의 원소를 얻고 pop을 호출하면 맨 앞의 원소를 queue에서 제거한다.
  - m->unlock()을 통해 다른 thread에서도 다음 원소를 바로 처리할 수 있도록한다.

![image](https://user-images.githubusercontent.com/69780812/146643099-606e1ce7-49a6-4c0c-abde-1aeddfa0a26b.png)
- 구현한 모습이 위와 같다.
- consumer thread가 10밀리초마다 downloaded_pages에 할일이 있는지 확인하고 없으면 기다리는 형태를 취하고있다.
  - 매우 비효율적
  - 매 번 언제 올지도 모르는 데이터를 확인하기 위해 지속적으로 mutex를 lock하고 queue 확인하고 ..

![image](https://user-images.githubusercontent.com/69780812/146643161-6704ff0c-8752-448a-ab16-1d529cde4563.png)
- 차라리 producer에서 데이터가 오면 consumer를 깨우는 방식이 나을 것이다.
- thread를 재워 놓게 되면 그 사이 다른 thread들이 일을 할 수 있어 CPU를 더 효율적으로 쓸 수 있을 것이다.
  - C++은 이를 위해 생산자 소비자 패턴을 구현할 수 있는 여러 도구를 제공하고 있다.

# condition_variable
- **어떠한 조건을 만족할 때까지 자라!**, 조건변수를 통해 해결할 수 있다.

```cpp
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

void producer(std::queue<std::string>* downloaded_pages, std::mutex* m,
              int index, std::condition_variable* cv)
{
    for (int i = 0; i < 5; i++)
    {
        // thread 별로 다운로드 하는데 걸리는 시간이 다르다.
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
        std::string content = "Web site : " + std::to_string(i) + " from thread(" +
                               std::to_string(index) + ")\n";

        // data는 thread 사이에서 공유되므로 critical section에 넣어야한다.
        m->lock();
        downloaded_pages->push(content);
        m->unlock();

        // consumer에게 content가 준비되었음을 알린다.
        cv->notify_one();
    }
}

void consumer(std::queue<std::string>* downloaded_pages, std::mutex* m,
              int* num_processed, std::condition_variable* cv)
{
    while (*num_processed < 25)
    {
        std::unique_lock<std::mutex> lk(*m);

        cv->wait(lk, [&] { return !downloaded_pages->empty() || *num_processed == 25; });
        
        if (*num_processed == 25)
        {
            lk.unlock();
            return;
        }

        // 맨 앞의 페이지를 읽고 대기 목록에서 제거
        std::string content = downloaded_pages->front();
        downloaded_pages->pop();

        (*num_processed)++;
        lk.unlock();

        // content 처리
        std::cout << content;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

int main()
{
    std::queue<std::string> downloaded_pages;
    std::mutex m;
    std::condition_variable cv;

    std::vector<std::thread> producers;
    for (int i = 0; i < 5; i++)
    {
        producers.push_back(std::thread(producer, &downloaded_pages, &m, i + 1, &cv));
    }

    int num_processed = 0;
    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; i++)
    {
        consumers.push_back(std::thread(consumer, &downloaded_pages, &m, &num_processed, &cv));
    }

    for (int i = 0; i < 5; i++)
        producers[i].join();
    
    // 나머지 자고있는 thread 들을 모두 깨운다.
    cv.notify_all();

    for (int i = 0; i < 3; i++)
        consumers[i].join();
}
```

```
Web site : 0 from thread(1)
Web site : 0 from thread(2)
Web site : 1 from thread(1)
Web site : 0 from thread(3)
Web site : 2 from thread(1)
Web site : 1 from thread(2)
Web site : 0 from thread(4)
Web site : 3 from thread(1)
Web site : 0 from thread(5)
Web site : 4 from thread(1)
Web site : 2 from thread(2)
Web site : 1 from thread(3)
Web site : 1 from thread(4)
Web site : 3 from thread(2)
Web site : 2 from thread(3)
Web site : 1 from thread(5)
Web site : 4 from thread(2)
Web site : 2 from thread(4)
Web site : 3 from thread(3)
Web site : 2 from thread(5)
Web site : 4 from thread(3)
Web site : 3 from thread(4)
Web site : 3 from thread(5)
Web site : 4 from thread(4)
Web site : 4 from thread(5)
```
- mutex 정의와 같이 condition_variable 정의했다.
- condition_variable의 wait() 함수에 어떤 조건이 참이 될 떄 까지 기다릴지 해당 조건을 인자로 전달해야한다.
  - 위의 경우 !downloaded_pages->empty() || *num_processed == 25;를 넣어줬다.
  - downloaded_pages에 원소가 있거나 처리 페이지 개수가 25개 일 때 wait을 중지하도록 한 것이다.
- 조건 변수는 해당 조건이 거짓이라면 lk를 unlcok하고 영원히 sleep 한다.
  - 이 thread는 누가 깨워주기 전까지 계속 sleep으로 기다리게 된다.
- 반면에 조건 참이라면 cv.wait은 그대로 return 하여 consumer의 content를 처리하는 부분이 그대로 실행되게 된다.
- 기존의 lock_guard와 다르게 unique_lock을 정의하였는데 unique_lock은 거의 동일하다.
  - lock_guard의 경우 생성자 말고는 따로 lock 할수 없는데 unique_lock은 unlock 후에 다시 lock 할수 있다.
  - unique_lcok을 사용한 이유는 cv->wait가 unique_lock을 인자로 받기 때문이다.
- cv.wait() 이후 num_processed 가 25인지 확인하는 구문은 wait()에서 탈출한 것이 모든 페이지 처리 완료해서 인지, 페이지가 추가되서인지를 알 수 없기 때문이다.
  - 모든 페이지 처리가 끝난 것이라면 thread를 종료해야한다.
- producer는 cv->notify_one() 함수를 호출한다.
  - 잠자고 있는 thread중 하나를 깨우는 것이다.
  - 모든 thread가 일하고 있다면 아무 일도 일어나지 않는다.
- notify_one 함수는 조건이 거짓인 바람에 자고 있는 thread 중 하나를 깨워 조건을 다시 검사하게 해준다.
  - 만일 조건이 참이면 thread는 일을 할 것이다.
- producer들이 모두 일을 끝낸 시점을 생각해보면 자고있는 일부 consumer thread들이 있을 것이다.
  - cv.notify_all()을 하지 않으면 자고있는 consumer thread의 경우 join 되지 않는 문제가 발생한다.
- 따라서 cv.notify_all()을 통해 모든 thread를 깨워 조건을 검사하도록 하여 종료할 수 있게 된다.