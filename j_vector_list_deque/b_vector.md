# STL Container - std::vector
- 컨테이너는 크게 두 가지 종류가 있다.
  - 1. Sequence container : 객체들을 순차적으로 보관 (array)
  - 2. Associative contaier(연관 컨테이너) : 키를 바탕으로 대응되는 값을 찾아준다.
- Vector에는 원소들이 메모리 상에 실제로 순차적으로 저장되어있고, 임의 위치에 있는 원소를 접근하는 것이 매우 빠르다.

# vector - Complexity
- 맨 뒤에 원소를 추가하는 작업은 엄밀히 amortized O(1)이라고한다.
- 보통 vector는 현재 가지고 있는 원소의 개수보다 더 많은 공간을 할당해 놓고 있다.
  - 따라서 만약 뒤에 새로운 원소를 추가하게 되면 새롭게 메모리를 할당할 필요 없이 이미 할당된 공간에 원소를 쓰기만 하면 된다.
  - 따라서 대부분 O(1)의 경우로 **맨 뒤에** 새로운 원소를 추가하거나 지울 수 있다.

![image](https://user-images.githubusercontent.com/69780812/142424639-ad700f64-618c-4a93-826b-76f75fafd108.png)
- 문제는 할당된 공간을 다 채웠을 떄 일어난다.
- 새로운 큰 공간을 어쩔 수 없이 다시 할당하고 기존의 원소들을 복사한다.
  - O(n) : 원소 복사
  - 하지만, 이러한 경우가 매우 드물기 때문에 O(1)이라고 하는 것이다.
- 그리고 임의 위치에 원소를 추가하거나 제거하는 것도 O(n)으로 느리다.
  - 어떤 원소를 추가할 경우 그 뒤에 오는 원소들을 한칸 씩 이동시켜줘야하기 때문이다. (n번의 복사)
- 임의 위치 원소 접근 : \[], at = O(1)
- 맨 뒤 원소 추가 및 제거 : push_back/pop_back = amortized O(1)
  - 최악의 경우 O(n)
- 임의 위치 원소 추가 및 제거 : insert, erase = O(n)
- 표준라이브러리를 잘 사용하기 위해서는 컨테이너가 어떤 작업을 위해 사용하는지 정확하 인지하여 적절한 컨테이너를 골라야한다.
  - 다른 자료 구조 사용 시 vector가 더 빠를 수도 느릴 수도 있기 때문이다.

# iterator
- vector의 경우 \[]로 정수형 변수로 배열처럼 임의 위치에 접근할 수 있지만, 반복자를 사용해서도 마찬가지 작업을 진행할 수 있다.
  - 알고리즘 라이브러리의 경우 대부분이 반복자를 인자로 받아 알고리즘을 수행한다.

![image](https://user-images.githubusercontent.com/69780812/142425729-44212112-662d-4695-bddf-527b1afc5dde.png)
- begin() : vector의 첫 번쨰 원소를 가리키는 반복자 return
- end() : vector의 **마지막 원소 한칸 뒤**를 가리키는 반복자 return
  - end()가 마지막 원소를 가리키면 비어있는 벡터를 표현할 수 없게 되기 때문이다.

```cpp
std::cout << *itr << std::endl;
```
- 실제 반복자가 가리키는 원소의 값을 보고 싶다면 * 연사자를 이용한다.
- itr은 실제 포인터가 아니고 *연산자를 오버로딩해서 포인터처럼 동작하게 만든 것이다.
  - \* 연산자는 iter가 가리키는 원소의 레퍼런스를 return한다.

```cpp
std::vector<int>::iterator itr = vec.begin() + 2;
```
- vector의 반복자 type은 std::vector<>::iterator 이다.
- \+ 연산자를 통해 그 만큼 떨어져있는 원소를 가리키게 할 수도 있다.

```cpp
vec.insert(vec.begin() + 2, 15); // v[2]앞에 15 추가
vec.erase(vec.begin() + 3); // v[3] 제거
```
- 반복자를 이용하면 insert, erase 함수를 사용할 수 있다.

```cpp
template <typename T>
void print_vector(std::vector<T>& vec)
{
    for (typename std::vector<T>::iterator itr = vec.begin(); iter != vec.end(); ++itr)
    {
        ...
    }
}
```
- 템플릿 버전의 경우 iterator는 typename을 추가해 줘야한다.
- iterator가 std::vector<T>의 의존 타입이기 때문이라고한다.

## const_iterator
- iteartor의 주의할 점은 컨테이너에 원소 추가, 제거하게되면 기존 사용하던 모든 반복자를 사용할 수 없게 된다.

```cpp
std::vector<int>::const_iterator citr = vec.cbegin() + 2;
```
- const 반복자가 가리키고 있는 값은 바꿀 수 없게 된다.
- cbegin(), cend() 함수로 이를 얻을 수 있다.
  - 많은 경우 반복자의 값을 바꾸지 않고, 참조만 하는 경우가 많으므로 const iterator를 적절히 이용하는 것이 좋다.
  - 값을 바꾸는 코드가 있으면 컴파일 에러 발생

## reverse iterator
- 반복자와 똑같지만 벡터 뒤에서 부터 앞으로 거꾸로 간다

```cpp
std::vector<int>::reverse_iterator r_iter = vec.rbegin();
for (; r_iter != vec.rend(); r_iter++)
{
    ...
}
```

![image](https://user-images.githubusercontent.com/69780812/142429357-3046184c-5cd3-4c78-9aaf-3a0fe323b8a2.png)
- 역반복자의 rend() 역시 **맨 앞 원소의 바로 앞**을 가리킨다.
- 값이 증가하면 앞쪽으로 원소가 가게된다.

```cpp
for(std::vecotr<int>::sizetype i = vec.size() - 1; i >=0; i--)
{
    ...
}
```
- 위의 경우 vector의 index를 담당하는 타입이 unsigned int인데 i-- 시, 해당 타입에서 가장 큰 정수가 되버린다. 영원히 종료할 수 없게 되는 것이다.
  - Segmentation fault 난다.
- 역으로 원소를 참조하고 싶다면, 역반복자를 사용하는 것이 현명하다.

# range based for loop
```cpp
for (int elem : vec)
{
    ...
}
```
- c++11에서 부터는 범위 기반 for문을 제공하고있다.
- elem = vec[i]가 되는 것처럼 매 루프마다 elem에 vec의 원소들이 복사되어 들어간다.
  - 복사하기 싫고 레퍼런스를 받고 싶다면 ?

```cpp
template <typename T>
void print_vector_ragne_based(std::vector<T>& vec)
{
    for (const auto& elem : vec)
    {
        ...
    }
}
```
- const auto&로 선언하여 vec의 원소들을 상수 레퍼런스로 접근하게 된다.
- range based for문을 활용하면 코드를 직관적으로 나타낼 수 있어 매우 편리하다.