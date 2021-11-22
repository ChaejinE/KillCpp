# Overview
- 알고리즘 라이브러리는 컨테이너 반복자들을 가지고 이런 저런 작업을 쉽게 수행할 수 있도록 도와주는 라이브러리다.
  - 이런 저런 작업 : 정렬이나 검색과 같이 단순한 작업들 말고도 이런 조건이 만족하면 컨테이너에서 지워준 다던지, 이런 조건이 만족하면 1을 더해주라는 복잡한 명령의 작업
- 알고리즘에 정의되어 있는 여러가지 함수들로 작업을 수행하게 된다.
  - 크게 두 개의 형태를 가지고 있다.

```cpp
template <typename Iter>
void do_something(Iter begin, Iter end);
```
- 한 가지 꼴이다.
- 알고리즘을 수행할 반복자의 시작점과 끝점 바로 뒤를 받는다.

```cpp
template <typename Iter, typename Pred>
void do_something(Iter begin, Iter end, Pred pred)
```
- 또 다른 꼴이다.
- 반복자는 동일하게 받되, 특정한 조건을 추가 인자로 받게 된다.
- 특정한 조건 : 서술자 (Predicate)이라고 부른다.
  - 보통 bool을 Return 하는 함수 객체 **(Functor)**를 전달하게 된다.
