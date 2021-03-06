# C++ STL(Standard Template Library)
-  STL을 일컬을 때 세 개의 라이브러리를 의미한다.
  - 1. Container : 임의 타입의 객체를 보관할 수 있는 컨테이너
  - 2. iterator : 컨테이너에 보관된 원소에 접근할 수 있는 반복자
  - 3. algorithm : 반복자들을 가지고 일련의 작업을 수행하는 알고리즘

# Complexity
- 컴퓨터 공학에서는 어떠한 작업의 처리 속도를 복잡도라고 부른다.
  - Big O표기법으로 나타낸다.
- N개의 데이터가 주어졌을 때 그 작업을 수행하기 위해 몇 번의 작업을 필요로하는지를 N에대한 식으로 표현하는 방식이다.
- 예를들어, 버블정렬의 경우 N(N-1) / 2 번의 반복이 필요하다.
  - O(N(N-1)/2)라고 볼 수 있는데 Big O 표현법 시 보통 최고차항만 나타낸다.(최고차항 계수는 생략)
  - 따라서 O(N^2)이 라고 볼 수 있다.

![image](https://user-images.githubusercontent.com/69780812/142424118-325e5895-0058-4dbe-b369-e9132a9c723c.png)
- 각각의 O에 대해 복잡도가 어떻게 증가하는 지 보여주는 그림이다.
- 가장 이상적 : O(1)
- 보통 O(logN)이 알고리즘이 낼 수 있는 가장 빠른 속도다.
