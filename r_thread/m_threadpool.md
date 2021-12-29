# Overview
- thread들을 위한 직업소개소라고 보면 된다.
  - 여러 Thread 들이 대기하고 있다가 할일이 들어오면 대기하고 있던 Thread들 중 하나가 이를 받아서 실행하게된다.
  - 예를들어 Server의 경우 Client 에서 요청이 들어오면 해당 요청에 대한 처리를 threadpool에 추가만하면된다. 그러면 나중에 thread들 중 하나가 이를 처리하게되는 것이다.
- 처리해야할 작업들을 queue에 추가하여 구현한다.
  - C++에서 queue는 pop을 하면 맨 앞의 원소가 제거되지만 원소를 return하지 않는다. 그러므로 접근을 위해서는 front()를 호출해야한다.
- 가장 상식적인 방법으로 queue를 사용하지만 가장 최근 추가된 작업 요청을 먼저 처리해야할 때도 있으므로 다른 자료구조를 생각하는 것도 나쁘지 않다.

# Class Design
- 먼저, thread들을 보관할 컨테이너가 필요하다.
  - vector
- 다음으로 작업들을 어떻게 저장할지 생각해야한다.
  - queue : 가장 오래 전에 추가된 작업을 쉽게 알 수 있다.
  - queue는 모든 작업 thread들에서 접근 가능하다.
- queue는 Multi Thread 환경에서 안전하지 않아 race condition에서 보호할 장치들이 필요하다.
  - producer-consumer pattern
  - producer : Thread pool User
  - comsumer : worker(Threads)
- Thread들은 기본적으로 jobs를 처리하는 동안 무한루프를 돌고 있으므로 종료를 위해서는 무한루프를 나가도록 해줘야한다.

```cpp
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace ThreadPool
{
    class ThreadPool
    {
        public:
            ThreadPool(size_t num_threads);
            ~ThreadPool();
    
            // job 추가
            void EnqueueJob(std::function<void()> job);

        private:
            // 총 thread 개수
            size_t num_threads_;
            // thread 보관 vector
            std::vector<std::thread> worker_threads_;
            // 할일 보관 job queue
            std::queue<std::function<void()>> jobs_;
            // job queue를 위한 cv와 m
            std::condition_variable cv_job_q_;
            std::mutex m_job_q_;

            // 모든 thread 종료
            bool stop_all;

            // Worker Thread
            void WorkerThread();
    };

    ThreadPool::ThreadPool(size_t num_threads) 
        : num_threads_(num_threads), stop_all(false)
    {
        worker_threads_.reserve(num_threads_);
        for (size_t i = 0; i < num_threads_; ++i)
        {
            worker_threads_.emplace_back([this]() {this->WorkerThread(); });
        }
    }

    void ThreadPool::WorkerThread()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(m_job_q_);
            cv_job_q_.wait(lock, [this]() { return !this->jobs_.empty() || stop_all; });
            if (stop_all && this->jobs_.empty())
            {
                return;
            }

            // job 빼기
            std::function<void()> job = std::move(jobs_.front());
            jobs_.pop();
            lock.unlock();

            job();
        }
    }
    
    ThreadPool::~ThreadPool()
    {
        stop_all = true;
        cv_job_q_.notify_all();

        for (auto& t : worker_threads_)
            t.join();
    }

    void ThreadPool::EnqueueJob(std::function<void()> job)
    {
        if (stop_all)
        {
            throw std::runtime_error("ThreadPool 사용 중지됨");
        }
        
        {
            std::lock_guard<std::mutex> lock(m_job_q_);
            jobs_.push(std::move(job));
        }

        cv_job_q_.notify_one();
    }
};

void worker(int t, int id)
{
    printf("%d start \n", id);
    std::this_thread::sleep_for(std::chrono::seconds(t));
    printf("%d end after %d\n", id, t);
}

int main()
{
    ThreadPool::ThreadPool pool(3);

    for (int i = 0; i < 10; i++)
        pool.EnqueueJob([i]() { worker(i % 3 + 1, i); });
}
```

```
2 start 
0 start 
1 start 
0 end after 1
3 start 
1 end after 2
4 start 
3 end after 1
5 start 
2 end after 3
6 start 
6 end after 1
7 start 
4 end after 2
8 start 
5 end after 3
9 start 
7 end after 2
9 end after 1
8 end after 3
```
- EnqueueJob() : void() 형태의 람다로 감싸서 work() 함수를 전달했다.

# 임의의 함수 받기
- 위에서 구현한 것에서 문제는 전달한 함수가 어떠한 값을 Return할 때이다.
  - 더 나은 구조로는 EnqueueJob 함수가 임의의 형태의 함수를 받고 그 함수의 Return 값을 보관하는 future를 리턴하는 꼴이면 더 좋다.

```cpp
template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(F f, Args... args);
```
- 이를 반영한 함수이다.
- class... 는 가변 길이 Template으로 임의의 길이의 인자들을 받을 수 있다.

```cpp
EnqueueJob(func, 1, 2, 3);
```
위와 같이 함수를 호출하면 첫 번째 인자인 func는 f에들어가게되고 나머지 1,2,3이 args... 부분에 들어가게 된다.
- 전달 받은 함수 f의 return 값을 가지는 future를 return해야하는데 F의 Return 값은 std::result_of를 사용하면 알 수 있다.

```cpp
typename std::result_of<F(Args...)>::type
```
- f의 return 값이다.

```cpp
template<class F, class... Args>
std::future</* f return type */> EnqueueJob(F f, Args... args);
```
- 위의 형태라고 생각하면 된다.
- 그런데 임의의 함수와 원소들을 받을 수 있다고 해서 컨테이너에 추가할 수 있다는 것은 아니다.
  - 해당 함수의 실행을 void() 꼴 함수만 저장할 수 있는 컨테이너에 어떻게 넣을 것인가 ?

```cpp
jobs_.push([f, args...]() {f(args...); });
```
- thread 안에서 f(args...)를 실행할 수 있는 방법이다.
- 한가지 문제는 f(args)의 return 값을 얻을 길이 없어진다는 것이다.

```cpp
using return_type = typename std::result_f<F(Args...)>::type;
std::packaged_task<return_type()> job(std::bind(f, args...));
```
- f실행 결과를 저장하는 packaged_task인 job 객체를 정의한다.
- 한가지 중요한 점은 packaged_task의 생성자는 함수만을 받으므로 실제 job을 수행하기 위해서는 job(args...)와 같이 호출되거나 위처럼 인자들을 f에 bind 시켜주면된다.

```cpp
std::future<return_type> job_result_future = job.get_future();
{
    std::lock_guard<std::mutex> lock(m_job_q_);
    jobs_.push([&job]() { job(); });
}
```
- job의 실행 결과를 보관하는 job_result_future를 정의한다. 마지막으로 jobs_에 job을 실행하는 lambda 함수를 추가한다.
- job이 실행되면 f의 return 값이 job_result_future에 들어가게되고 이는 thread pool 사용자가 접근할 수 있게 된다.
- 하지만 구현해서 실행해보면 Broken promise 예외가 던져진다.
  - Broken promise exception : promise에 set_value 하기 전에 이미 promise의 future 객체가 파괴되면 발생하는 예외다.
- 왜 future 객체가 파괴된 걸까?

```cpp
std:;packaged_task<return_type()> job(std::bind(f, args...));
```
- EnqueueJob 함수에 정의된 job 객체는 지역변수다.
  - 즉, EnqueueJob 함수가 return 하면 파괴되는 객체다.
  - 따라서 [&job]() { job(); } 안에서 job을 접근할 때 이미 그 객체는 파괴되고 없어져 있다.
- 해결방법은 두가지를 생각해볼 수 있다.
  - 1. packaged_task를 따로 컨테이너에 저장해서 보관
  - 2. shared_ptr에 packaged_task를 보관
- 1번 방식은 더 이상 packaged_task를 사용하지 않을 때에도 컨테이너에 남아있다는 문제가 남아있다.
- 2번 방식의 경우 packaged_task를 사용하는 것이 없으면 알아서 shared_ptr이 객체를 소멸시켜줘서 훨씬 관리하기 편하다.

```cpp
auto job = 
    std::make_shared<std::packaged_task<return_type()>>(std::bind(f, args...));
std::future<return_type> job_result_future = job->get_future();
{
    std::lock_guard<std::mutex> lock(m_job_q_);
    jobs_.push([job]() { (*job)(); });
}
```
- 위 처럼 make_shared로 간단히 shared_ptr을 생성하고 람다 함수에 shared_ptr의 복사본을 전달하여 packaged_task의 shared_ptr 하나를 붙들고 있게 된다.
- 따라서 job을 실행하는 시점에서도 packaged_task 객체는 계속 살아있게 된다.

```cpp
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace ThreadPool
{
    class ThreadPool
    {
        public:
            ThreadPool(size_t num_threads);
            ~ThreadPool();

            template<class F, class... Args>
            std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(
                F f, Args... args);
            
        private:
            size_t num_threads_;
            std::vector<std::thread> worker_threads_;
            std::queue<std::function<void()>> jobs_;
            std::condition_variable cv_job_q_;
            std::mutex m_job_q_;

            bool stop_all;

            void WorkerThread();
    };

    ThreadPool::ThreadPool(size_t num_threads)
        : num_threads_(num_threads), stop_all(false) 
    {
        worker_threads_.reserve(num_threads_);
        for (size_t i = 0; i < num_threads_; ++i)
        {
            worker_threads_.emplace_back([this]() { this->WorkerThread(); });
        }
    }

    void ThreadPool::WorkerThread()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(m_job_q_);
            cv_job_q_.wait(lock, [this]() { return !this->jobs_.empty() || stop_all; });
            if (stop_all && this->jobs_.empty())
                return;

            // 맨 앞의 job을 뺀다.
            std::function<void()> job = std::move(jobs_.front());
            jobs_.pop();
            lock.unlock();

            // 해당 job을 수행한다.
            job();
        }
    }

    ThreadPool::~ThreadPool()
    {
        stop_all = true;
        cv_job_q_.notify_all();

        for (auto& t : worker_threads_)
            t.join();
    }

    template <class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> ThreadPool::EnqueueJob(
        F f, Args... args)
    {
        if (stop_all)
            throw std::runtime_error("ThreadPool 사용 중지");

        using return_type = typename std::result_of<F(Args...)>::type;
        auto job = 
            std::make_shared<std::packaged_task<return_type()>>(std::bind(f, args...));
        std::future<return_type> job_result_future = job->get_future();
        {
            std::lock_guard<std::mutex> lock(m_job_q_);
            jobs_.push([job]() { (*job)(); });
        }
        cv_job_q_.notify_one();

        return job_result_future;
    }
}

int work(int t, int id)
{
    printf("%d start \n", id);
    std::this_thread::sleep_for(std::chrono::seconds(t));
    printf("%d end after %ds\n", id, t);
    return t + id;
}

int main()
{
    ThreadPool::ThreadPool pool(3);

    std::vector<std::future<int>> futures;
    for (int i = 0; i < 10; i++)
        futures.emplace_back(pool.EnqueueJob(work, i % 3 + 1, i));
    
    for (auto& f : futures)
    {
        printf("result : %d \n", f.get());
    }
}
```

```
2 start 
0 start 
1 start 
0 end after 1s
result : 1 
3 start 
1 end after 2s
result : 3 
4 start 
3 end after 1s
5 start 
2 end after 3s
6 start 
result : 5 
result : 4 
6 end after 1s
7 start 
4 end after 2s
8 start 
result : 6 
5 end after 3s
9 start 
result : 8 
result : 7 
7 end after 2s
result : 9 
9 end after 1s
8 end after 3s
result : 11 
result : 10 
```
- 위와 같이 잘 실행이 된다.
- EnqueueJob 함수의 경우 한가지 문제점이 있다.

```cpp
ThreadPool::EnqueueJob(F f, Args... args);
```
- 위와 같이 인자들의 복사본을 받는 다는 것이다.
  - 불필요한 복사를 야기하여 **완벽한 전달** 패턴을 사용하는 것이 좋다.

```cpp
template <class F, class... Args>
std::future<typename std::result_of(F(Args...)>::type> EnqueueJob(
    F&& f, Args&&... args);
```
- EnqueueJob 함수의 인자들을 우측값 Reference로 바꿔 bind 함수에 forward로 인자를 전달해주면 된다.

```cpp
auto job = std::make_shared<std::packaged_task<return_type()>>(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...));
```
- 불필요한 복사 없이 Enqueue 함수에 인자들을 완벽히 전달할 수 있게 된다.

```cpp
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace ThreadPool
{
    class ThreadPool
    {
        public:
            ThreadPool(size_t num_threads);
            ~ThreadPool();

            template<class F, class... Args>
            std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(
                F&& f, Args&&... args);
            
        private:
            size_t num_threads_;
            std::vector<std::thread> worker_threads_;
            std::queue<std::function<void()>> jobs_;
            std::condition_variable cv_job_q_;
            std::mutex m_job_q_;

            bool stop_all;

            void WorkerThread();
    };

    ThreadPool::ThreadPool(size_t num_threads)
        : num_threads_(num_threads), stop_all(false) 
    {
        worker_threads_.reserve(num_threads_);
        for (size_t i = 0; i < num_threads_; ++i)
        {
            worker_threads_.emplace_back([this]() { this->WorkerThread(); });
        }
    }

    void ThreadPool::WorkerThread()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(m_job_q_);
            cv_job_q_.wait(lock, [this]() { return !this->jobs_.empty() || stop_all; });
            if (stop_all && this->jobs_.empty())
                return;

            // 맨 앞의 job을 뺀다.
            std::function<void()> job = std::move(jobs_.front());
            jobs_.pop();
            lock.unlock();

            // 해당 job을 수행한다.
            job();
        }
    }

    ThreadPool::~ThreadPool()
    {
        stop_all = true;
        cv_job_q_.notify_all();

        for (auto& t : worker_threads_)
            t.join();
    }

    template <class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> ThreadPool::EnqueueJob(
        F&& f, Args&&... args)
    {
        if (stop_all)
            throw std::runtime_error("ThreadPool 사용 중지");

        using return_type = typename std::result_of<F(Args...)>::type;
        auto job = 
            std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> job_result_future = job->get_future();
        {
            std::lock_guard<std::mutex> lock(m_job_q_);
            jobs_.push([job]() { (*job)(); });
        }
        cv_job_q_.notify_one();

        return job_result_future;
    }
}

int work(int t, int id)
{
    printf("%d start \n", id);
    std::this_thread::sleep_for(std::chrono::seconds(t));
    printf("%d end after %ds\n", id, t);
    return t + id;
}

int main()
{
    ThreadPool::ThreadPool pool(3);

    std::vector<std::future<int>> futures;
    for (int i = 0; i < 10; i++)
        futures.emplace_back(pool.EnqueueJob(work, i % 3 + 1, i));
    
    for (auto& f : futures)
    {
        printf("result : %d \n", f.get());
    }
}
```
- 최종 ThradPool 이다.