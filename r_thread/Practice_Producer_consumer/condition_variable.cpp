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