# List
- 양방향 연결 구조를 가진 자료형이라고 볼 수 있다.

![image](https://user-images.githubusercontent.com/69780812/142430535-bab1e6d7-a4ba-40ac-9005-03794848d347.png)
- Vector와 달리 임의의 위치에 있는 원소에 접근을 바로 할 수 없다.
- list container는 시작 원소와 마지막 원소 위치만 기억한다.
  - 임의 원소에 접근하기 위해서는 하나씩 링크를 따라가야한다.
  - \[], at()이 정의되어 있지 않다.

```cpp
std::lst<int> lst;

lst.push_back(10);
lst.push_back(20);

for (std::list<int>iterator itr = lst.begin(); itr != lst.end(); ++itr)
{
    if (*itr == 20)
    {
        lst.erase(itr);
        break;
    }
}
```
- vector의 경우 맨 뒤를 제외하고는 임의 위치에 원소를 추가하거나 제거하는 작업이 O(n)이다.
- list의 경우는 O(1)로 매우 빠르게 수행된다.
  - 원하는 위치의 앞과 뒤 링크값만 바꿔주면 되기 때문이다.
- 벡터와 달리 **원소를 지워도 반복자가 무효화 되지 않는다.**
  - 원소들의 주소값들은 바뀌지 않기 때문이다.

```cpp
itr++ 
itr-- //--itr도 가능
```
- list의 반복자의 경우 위와같은 연산만 가능하다.

```cpp
itr + 5
```
- 위 처럼 임의의 위치에 있는 원소를 가리킬 수 없다.
- 오직 **한 칸씩** 움직인다.
- 리스트는 왼쪽 혹은 오른쪽을 가리키고있는 원소들의 모임으로 이루어져있어 한 번에 한 칸씩 밖에 이동할 수 없다.
  - 메모리 상 원소들이 연속적으로 존재하지 않는다.
- 리스트에서 정의되는 반복자 타입을 보면 BidirectionalIterator 타입이다.
  - 양방향으로 이동할 수 있되 한 칸씩 밖에 이동안된다.
  - 벡터는 RandomAccessIterator 타입의 반복자다. 리스트의 반복자를 상속받고있다.
