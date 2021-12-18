# Overview
- 현대의 CPU는 여러 개의 코어를 지원하고 있다.
  - 여러 스레드를 동시에 실행할 수 있다.
- 어떨 때 프로그램을 멀티 스레드로 만드는 것이 유리한가 ?
  - 병렬 가능한 작업
  - 대기시간이 긴 작업들

## 병렬 작업
- 어떤 작업을 여러개의 스레드로 좀 더 빠르게 수행하는 것을 parallelize라고 부른다.
  - 피보나치와 같은 프로그램은 병렬화하기가 매우 까다롭다.
  - 어떠한 연산(A)을 수행하기 위해 다른 연산(B)의 결과가 필요하기 때문이다. 이러한 상황을 A가 B에 dependent한다라고한다.
- 의존관계가 많을 수록 병렬화가 어려워지며 반대로 다른 연산의 결과와 관계없이 독립적으로 수행할 수 있는 구조가 많다면 병렬화가 매우 쉬워진다.

## 긴 대기시간 작업
- 인터넷은 CPU 처리 속도에 비해 매우 느리다.
- ping : 보낸 요청이 상대 서버에서 도착해서 다시 나에게 돌아오는 데 걸리는 시간 (첫번째 응답에 대해서)
  - 국내 : 30ms, 해외 : 150ms..
- 엄청난 횟수의 연산을 처리할 수 있는 현대 CPU가 응답을 단순히 기다리기만 한다면 비효율적이다.

![image](https://user-images.githubusercontent.com/69780812/146202048-42e60f85-9718-4b54-becc-6a7b015a70ae.png)

![image](https://user-images.githubusercontent.com/69780812/146202000-0191375c-d584-4a52-bcf8-3c4cddbb4b00.png)
- download() 함수가 있다면 여러 스레드에서 부르면된다.
- 같은 Core안에서 thread 들이 context switching을 통해 기다리는 시간 없이 CPU를 최대한으로 사용하는 것을 볼 수 있다.
  - CPU 시간을 낭비하지 않고 효율적으로 작업을 처리한다는 점이 요지이다.

# thread 생성
```cpp
#include <iostream>
#include <thread>
using std::thread;

void func1()
{
    for (int i =0; i < 10; i++)
    {
        std::cout << "thread 1 작동중: \n";
    }
}

void func2()
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << "thread 2 작동중: \n";
    }
}

void func3()
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << "thread 3 작동중: \n";
    }
}

int main()
{
    thread t1(func1);
    thread t2(func2);
    thread t3(func3);

    t1.join();
    t2.join();
    t3.join();
}
```

```
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 2 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 2 작동중: 
thread 2 작동중: 
thread 2 작동중: 
thread 2 작동중: 
thread 2 작동중: 
thread 2 작동중: 
thread 2 작동중: 
thread 2 작동중: 
thread 2 작동중:
```
- 컴파일 시 -pthread를 넣어줘야된다 (linux)
- join() : 해당하는 스레드들이 실행을 종료하면 Return 하는 함수다.
  - t2가 t1보다 먼저 종료되면 ? 상관없다. 이미 종료된 상태면 바로 함수가 return된다.
  - join, detach 되지 않은 스레드들의 소멸자가 호출되면 예외가 발생하도록 되어 있어 잘 써줘야한다.
  - 여기서 detach는 해당 스레드를 실행시킨 후 잊어버리는 것이라고 생각하면된다. (background에서 돌아간다.)

```cpp
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
thread 1 작동중: 
메인 종료thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중: 
thread 3 작동중:
```
- detach하면 위와 같이 된다.
- 프로세스 종료 시 해당 프로세스 안의 모든 스레드들은 종료여부와 상관없이 자동으로 종료된다.
  - 프로세스 종료 시 모두 더이상 스레드 작동중을 출력할 수 없게되는 것이다.
- detach()시, main 함수는 더이상 스레드들이 종료될때 까지 기다리지 않는다.
- 위를 보면 스레드 1, 3은 운이 좋게도 적당히 메세지를 출력했지만 스레드 2의 경우 메세지를 출력하지도 못하고 종료됐다.

![image](https://user-images.githubusercontent.com/69780812/146203651-6d9ffe7a-6f17-46d0-8168-d7ebcacd1c83.png)
- 한가지 중요한 사실을 thread 들이 CPU 코어에 어떻게 할당되고 언제 context switching을 할지는 전적으로 OS 마음에 달려있다는 점이다.
  - 스레드 3개라고 해서 반드시 3개의 각기 다른 코어에 할당되는 것은 아니며 운이 좋으면 그렇게 되겠지만 한 코어에 스레드가 3개 컨텍스트 스위칭을 하면서 돌아갈 수도 있다.
