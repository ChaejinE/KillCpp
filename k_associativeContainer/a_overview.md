# Overview
- Sequence Container : vector, list, deque, 이들은 원소 자체를 보관하는 컨테이너들이다.
- Association Container
  - key - value 구조를 가진다.
  - 특정 키를 넣으면, 이에 대응되는 값을 들려준다는 것이다.

![image](https://user-images.githubusercontent.com/69780812/142537565-d0c77485-a969-4c2f-b03b-3d68280cc640.png)
- 아이디 - 비밀번호 형식으로 할 수 있다.
- 위 처럼 연관 컨테이너는 키를 바탕으로 이에 대응되는 값을 얻을 수 있는 구조이다.
- 보통 위와 같은 자료에서는 두 가지 종류의 질문을 할 수 있다.
  - 1. 특정 키가 연관 컨테이너에 존재하는지 유무 -> True False
  - 2. 존재하면, 이에 대응되는 값은 무엇인가 ?
- 위 두가지 작업을 처리할 수 있는 것이 연관 컨테이너다.
- 전자의 경우 **set, multiset**, 후자의 경우 **map, multimap**이다.
- 맵, 멀티맵을 셋처럼 사용할 수 있으나 사용하는 메모리 크기가 맵의 경우, 셋보다 크기 때문에 키의 존재 유무만 궁금하다면 셋을 사용하는 것이 좋다.

# 뭘 써야 되나 ?
- 데이터의 졵 유무만 궁금한 경우 : set
- 중복 데이터를 허락할 경우 -> multiset
  - insert, erase, find 모두 O(logN)
  - 최악의 경우에도 O(logN)
- 데이터에 대응되는 데이터를 저장하고 싶은 경우 : map
- 중복 키를 허락할 경우 : multimap
  - insert, erase, find 모두 O(logN)
  - 최악의 경우에도 O(logN)
- 속도가 매우매우 중요해서 최적화 해야하는 경우 : unordered_set, unordered_map
  - insert, erase ,find 모두 O(1)
  - 최악의 경우 O(N) -> 해시 함수와 상자 개수를 잘 설정해야한다.