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
