# Overview
- 서로 다른 스레드들에서 같은 메모리를 공유할 때 발생하는 문제를 경쟁상태 (race condition)이라고 부른다.

# Example
```cpp
#include <iostream>
#include <thread>
#include <vector>

void worker(int& counter)
{
    for (int i = 0; i < 10000; i++)
    {
        counter += 1;
    }
}

int main()
{
    int counter = 0;
    std::vector<std::thread> workers;

    for (int i = 0; i < 4; i++)
    {
        workers.push_back(std::thread(worker, std::ref(counter)));
    }

    for (int i = 0; i < 4; i++)
    {
        workers[i].join();
    }

    std::cout << "Counter Result : " << counter << std::endl;
}
```

```
Counter Result : 22266
```
- 문제는 counter += 1; 을 하기위한 명령에 있다.

## CPU
- CPU 연산을 위해서는 CPU의 Register라는 곳에 데이터를 기록한 다음 연산을 수행해야한다.

![image](https://user-images.githubusercontent.com/69780812/146635089-655a78bd-d4af-4510-b17e-99ea6436bbb4.png)
- 레지스터의 크기는 매우 작으며 64비트의 경우 레지스터 크기들이 8바이트에 불과하다.
- 뿐만 아니라 레지스터의 개수는 그리 많지 않으며 일반적 연산에서 사용되는 범용 레지스터의 경우 16개 밖에 없다.
- 모든 데이터들은 메모리에 저장되어있고, 연산할 때 마다 메모리에서 레지스터로 값을 가져온 뒤 빠르게 연산을 하고 다시 메모리에 가져다 놓는 식으로 작동한다.

```
mov rax, qword ptr [rbp - 8]
mov ecx, dword ptr [rax]
add ecx, 1
mov dword ptr [rax], ecx
```
- counter += 1이 실제로 위와 같은 어셈블리코드로 나타난다.
- 어셈블리 코드는 CPU가 실제로 실행하는 기계어와 1대1대응 관계이다.
  - 위 명령을 한줄 한줄 CPU가 처리한다고 생각해도 무방하다.
- rax, rbp는 모두 CPU의 register를 의미한다.
- 먼저, mov는 대입 명령이다.(move) \[rbp - 8]이 rax에 대입되는 것이다.
  - \[]의 의미는 역참조이다. rbp - 8을 주소값이라 하면 해당 주소에 있는 값을 읽으라는 뜻이다. C++의 pointer 연산과 동일
- qword는 8바이트를 의미한다. 주소값의 크기가 8바이트다.
  - 이는 주소로부터 얼마나 읽어야하는지에대한 것이다.
- 실제로 rgb - 8에는 counter 의 주소값이 담겨서 rax에 counter 주소값을 복사하는 과정인 것이다.
- 그 다음으로는 rax에 result의 주소값이 담겨있다. 따라서 ecx에는 result의 현재 값이 들어가게 된다.
- add는 더하는 명령으로 ecx에 1을 더하는 명령이다.
  - 즉, Result에 1이 더해진다.
- 마지막으로 result에 이전의 result에 1이 더해진 값이 저장된다.
- CPU 구조상 add 명령은 역참조한 메모리에서 직접 사용할 수 없고 반드시 레지스터에만 내릴 수 있어서 ecx에 저장하는 것이다.

![image](https://user-images.githubusercontent.com/69780812/146635297-4f922205-790f-45e5-9445-9ee7b4aed329.png)
- 왜 counter의 값이 이상하게 나왔는지 짐작해보자.
- 위와 같은 상황을 생각해보자. 처음에 counter가 0이었다고 가정한다.

```
mov rax, qword ptr [rbp - 8]
mov ecx, dword ptr [rax]
```
- thread 1에서 여가까지 실행했다고 생각해보자.
- 이 시점에서 스레드 1의 ecx 레지스터에는 counter의 초기 값인 0이 들어가게 된다.
- 다음 thread2에서 전체 명령을 모두 실행했다고 해보자. 현재 thread 1이 counter의 값을 바꾸지 않은 상태이므로 thread 2에서 읽은 counter 값도 역시 0이다.
  - 따라서 thread 2가 counter +=1을 마치면 counter에는 1이 들어가 있다.
- thread는 **메모리를 공유하지만 Register는 공유하지 않는다.**
  - 각 스레드 별로 고유의 레지스터들을 가지고 있다고 생각하면 된다.
  - 즉, thraed 1의 ecx, thread 2의 ecx는 서로 다르다고 보면 된다.

```
add ecx, 1
mov dword ptr [rax], ecx
```
- 다시 thread 1의 차례로 보면 thread 1의 ecx는 0 이었기 때문에 add 명령 후 ecx 역시 1이 된다.
- counter에는 2가 아닌 1이 기록되는 것이다.
- 운이 좋게 정상적으로 될 수 있지만, 운영체제 마음대로 이므로 우리는 그런 행운을 항상 바랄 순 없다.
- 여태까지 실행한 모든 프로그램은 몇번을 실행하더라도 같은 결과가 나왔지만 멀티 스레드 프로그램의 경우 프로그램 실행마다 그 결과가 달라질 수 있다.
  - **디버깅이 존나 어렵다는 뜻**
