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