# Heap
- 메모리를 관리하는 문제는 언제나 중요하다.
- 컴파일 시 모든 변수의 주소값이 확정되어야 했다.
- 하지만, 이를 위해서는 프로그램에 많은 제약이 따르므로 프로그램 실행 시에 자유롭게 할당하고 해제할 수 있는 Heap이라는 공간이 따로 생겼다.

# Stack
- 컴파일러에 의해 어느정도 안정성이 보장된다.
- Heap은 사용자가 스스로 제어해야하는 만큼 책임이 따른다.

# new, delete
- C언어에서는 malloc, free 함수를 지원하여 힙 상에 메모리 할당을 지원했다.
- C++에서는 new, delete를 지원한다.
  - new : malloc과 대응되며 메모리를 할당한다.
  - delete : free에 대응되며 메모리를 해제한다.

```cpp
#include <iostream>

int main() {
    int* p = new int;
    *p = 10;

    std::cout << *p << std::endl;

    delete p;
    return 0;
}
```
```
10
```
- int 영역이 잘 할당 되어 10이 출력된다.

```cpp
int *p = new int;
```
- int 크기의 공간을 할당해 그 주소값을 p에 집어 넣은 것이다.

```cpp
T* poinrter = new T;
```
- new를 사용하는 방법은 위와 같다.
  - T : 임의의 type

```cpp
*p = 10;
```
- p 위치에 할당된 공간에 값을 집어넣는다.

```cpp
delete p;
```
- p에 할당된 공간이 해제된다.
- new로 할당한 공간만 해제된다.

```cpp
#include <iostream>

int main() {
    int a = 5;
    delete &a;
    return 0;
}
```
- 위와 같은 경우 지역변수를 무리하게 delete 했다.
  - Heap이 아닌 공간을 해제하려고 한다는 경고 메세지가 나타나게된다.

# new로 배열 할당
```cpp
#include <iostream>

int main() {
    int arr_size;
    std::cout << "array size : ";
    std::cin >> arr_size;
    int *list = new int[arr_size];
    for (int i = 0; i < arr_size; i++) {
        std::cin >> list[i];
    }
    for (int i = 0; i < arr_size; i++) {
        std::cout << i << "th elem of list : "<< list[i] << std::endl;
    }

    delete[] list;
    return 0;
}
```
- list는 arr_size 만큼의 크기를 갖는 int 배열이된다.

```cpp
T* pointer = new T[size];
```
- 배열에 new 사용하는 방법이다.

```cpp
#include <iostream>

typedef struct Animal {
    char name[30];
    int age;

    int health;
    int food; // 배부른 정도
    int clean; // 깨끗한 정도
} Animal;

void create_animal(Animal *animal) {
    std::cout << "동물의 이름? ";
    std::cin >> animal->name;

    std::cout << "동물의 나이? ";
    std::cin >> animal->age;

    animal->health = 100;
    animal->food = 100;
    animal->clean = 100;
}

void play(Animal *animal) {
    animal->health += 10;
    animal->food -= 20;
    animal->clean -=30;
}

void one_day_pass(Animal *animal) {
  // 하루가 지나면
  animal->health -= 10;
  animal->food -= 30;
  animal->clean -= 20;
}

void show_stat(Animal *animal) {
  std::cout << animal->name << "의 상태" << std::endl;
  std::cout << "체력    : " << animal->health << std::endl;
  std::cout << "배부름 : " << animal->food << std::endl;
  std::cout << "청결    : " << animal->clean << std::endl;
}

int main() {
  Animal *list[10];
  int animal_num = 0;

  for (;;) {
    std::cout << "1. 동물 추가하기" << std::endl;
    std::cout << "2. 놀기 " << std::endl;
    std::cout << "3. 상태 보기 " << std::endl;

    int input;
    std::cin >> input;

    switch (input) {
      int play_with;
      case 1:
        list[animal_num] = new Animal;
        create_animal(list[animal_num]);

        animal_num++;
        break;
      case 2:
        std::cout << "누구랑 놀게? : ";
        std::cin >> play_with;

        if (play_with < animal_num) play(list[play_with]);

        break;

      case 3:
        std::cout << "누구껄 보게? : ";
        std::cin >> play_with;
        if (play_with < animal_num) show_stat(list[play_with]);
        break;
    }

    for (int i = 0; i != animal_num; i++) {
      one_day_pass(list[i]);
    }
  }
  for (int i = 0; i != animal_num; i++) {
    delete list[i];
  }
}
```
- my_pat.cpp 내용이다.


