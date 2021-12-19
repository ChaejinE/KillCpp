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