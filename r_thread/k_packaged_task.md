# packaged_task
- promise-future 패턴을 비동기적 함수의 return값에 간단히 적용할 수 있는 packaged_task라는 것을 지원한다.

![image](https://user-images.githubusercontent.com/69780812/146677531-a8dc76e4-9bdd-46e3-9e39-0d2c41d329fb.png)
- packaged_task에 전달된 함수가 return할 때, 그 return 값을 promise에 set_value하고, 예외를 던졌다면 set_exception을 한다.
- 해당 future는 packaged_task가 return하는 future에서 접근할 수 있다.

```cpp
#include <future>
#include <iostream>
#include <thread>

int some_task(int x) { return 10 + x; }

int main()
{
    std::packaged_task<int(int)> task(some_task);

    std::future<int> start = task.get_future();
    std::thread t(std::move(task), 5);

    std::cout << "result : " << start.get() << std::endl;
    t.join();
}
```

```
result : 15
```
- packaged_task는 비동기적으로 수행할 함수 자체를 생성자의 인자로 받는다.
  - 템플릿 인자로는 함수의 타입을 명시해준다.
  - packaged_task는 전달된 함수를 실행해 그 return 값은 promise에 설정한다.
- 해당 promise에 대응되는 future는 get_future()로 얻는다.
- 생성된 packaged_task를 thread로 전달하면되며 복사 생성이 불가능하므로 명시적으로 move를 해줘야만 한다. (promise도 마찬가지)
- 이 처럼 pacakged_task를 사용하면 thread에 굳이 promise를 전달할 필요 없이 packaged_task가 함수의 return 값을 처리해줘서 매우 편리하다.