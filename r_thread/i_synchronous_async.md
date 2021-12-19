# 동기 비동기
```cpp
string txt = read("a.txt"); // 5 ms
string result = do_something_with_txt(txt); // 5ms

do_other_computation(); // 5ms (CPU)
```
- 위 작업들이 순차적으로 실행한다 했을 때 모두 종료되는데 15ms가 걸린다.
- 이러한 작업이 비효율적인 이유는 read 함수가 파일이 하드 디스크에서 읽어지는 동안 기다리기 때문이다.
  - read()는 파일 읽기가 끝나기 전까지 return 하지 않고, CPU는 아무것도 못한체 기다린다.
- 한 번에 하나씩 순차적으로 실행되는 작업을 **동기적(synchronous)**로 실행된다고 부른다.
  - 동기적인 작업들은 한 작업이 끝날 때까지 다음 작업으로 이동하지 않는다.
- read()로 하드 디스크에서 데이터를 불러오는 동안 CPU를 놀게하지 않고 do_other_computation을 하는 것이 바람직하다.

```cpp
void file_read(string* result)
{
    string txt = read("a.txt");
    *result = do_something_with_txt(txt);
}

int main()
{
    string result;
    thread t(file_read, &result);

    do_other_computation();

    t.join();
}
```
- 위 처럼 코드를 수행하면 file_read 함수 안에서 read()가 실행되는데 CPU는 하드 디스크에서 데이터를 기다리지 않고 원래 main 함수 thread로 넘어와서 do_other_computation() 을 수행하게 될 것이다.
- 5ms 후 do_other_computation()이 끝나면 t.join()을 수행하면서 다시 file_read thread를 실행할 것이다.
  - 이미 하드 디스크에서 a.txt 파일의 내용이 도착해 있어 바로 do_something_with_txt()를 실행할 수 있다.
  - 이 경우 5 + 5 = 10 ms로 수행이 끝나게 된다.
  - CPU는 한 순간도 놀지 않았다.
- 프로그램 실행이 한 갈래가 아니라 여러 갈래로 갈라져서 동시에 진행되는 것을 **비동기적(asynchronous) 실행**이라고 부른다.