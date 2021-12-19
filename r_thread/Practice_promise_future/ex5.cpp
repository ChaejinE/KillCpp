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