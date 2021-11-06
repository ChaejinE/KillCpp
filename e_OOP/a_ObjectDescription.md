# 객체란 ?
```cpp
typedef struct Animal {
    char name[30];
    int age;

    int health; // 체력
    int food; // 배부른 정도
    int clean; // 깨끗한 정도
} Animal
```
- 위와 같은 구조체를 정의했다고 해보자.

```cpp
play(list[play_width])
```
- Play라는 함수를 통해 Animal Dynamic Array를 전달해서 각 Animal 들이 play라는 기능을 하도록 한다고 해보자.
- 곰곰히 생각해 봤을 때,  Play 함수에 Animal 배열을 전달하는 것은 불필요해보인다.
  - "Play가 Animal 합니다"가 맞을까 "Animal이 Play 합니다."가 맞을까

```cpp
Animal animal;

animal.play();
animal.sleep();
```
- 이렇게한다면 Play라는 함수에 Animal을 인자로 주지 않아도 된다.
  - 내 정보를 play함수가 다 안다면 굳이 그럴 필요가 없지 않을까?
  - 내 정보 : health(체력), food(배부른 정도), clean(깨끗한 정도) 등
- Animal 이라하는 것은 variable(내 정보)과 method(play, sleep, 수행하는 행동들)로 이루어 졌다는 것을 알 수 있다.
- 객체란 **변수들과 참고 자료들로 이루어진 소프트웨어 덩이리**다.

![image](https://user-images.githubusercontent.com/69780812/140602415-22fe0bb1-c5b1-4d0d-88b9-2391632453bf.png)
- 흔히 객체를 나타내기 위한 그림이다.
- 자기만의 정보를 나타내는 변수 + 이를 가지고 어떤 작업을 하는 함수로 구성되어있다.
- 객체의 변수 : instance variable
- 객체의 함수 : instance method

```cpp
Animal animal;

animal.food += 100;
animal.increase_food(100);
```
- method가 변수들을 감싸고 있는 것처럼 그리는 이유는 변수들이 **외부로 부터 보호**되고 있기 떄문이다.
  - 즉, 외부에서 객체의 instance varialbe을 바꾸는 것이 아니라 오직 객체의 instance method를 통해서만 가능하도록 한다는 것이다. (극단적으로 불가능한 것은 아니다.)
- 항상 instance method를 통해 간접적으로 조절하는 것을 **Encapsulation**이라한다.
- Encapsulation의 장점은 객체가 내부적으로 어떻게 작동하는지 몰라도 사용할줄 알게된다는 점에 있다.
  - 예컨데, food가 올라가면 weight도 올라가야하지 않겠는가 ?
  - food++ 이면 weight++도해야하는 복잡한 과정을 User가 알필요가 있는가 ? No

# Class
![image](https://user-images.githubusercontent.com/69780812/140602599-4493995b-54e4-4582-9b73-b8d87a2696bd.png)
- Object는 C++에서 어떻게 만들어 내는가 ?
- Object의 **"설계도"**라는 객체를 만들 수 있는 장치를 준비한다.
  - 이것이 CLass다.
- **Class를 이용해서 만들어진 Object : Instance 라고 부른다.**

```cpp
#include <iostream>

class Animal
{
private:
    int food;
    int weight;

public:
    void set_animal(int _food, int _weight)
    {
        food = _food;
        weight = _weight;
    }

    void increase_food(int inc)
    {
        food += inc;
        weight += (inc / 3);
    }

    void view_stat()
    {
        std::cout << "이 동물의 food : " << food << std::endl;
        std::cout << "이 동물의 weight : " << weight << std::endl;
    }
};
```
- Animal Class : 생성될 임의의 Animal Object에 대한 설계도라 보면된다.
- member variable : food, weight
- member function : set_animal(), increase_food(), view_stat()
- instance로 생성된 Object는 Instance variable, Instacne method라 부른다.
- member variable, function은 **실재하는 것이 아니다.** 단지, 설계도일 뿐
  - **Instance가 만들어져야 비로소 실제로 존재하는 것이다.**
- private keyword
  - 객체 내에서 보호되고 있다는 의미
  - 자기 객체 안에서만 접근할 수 있다. (외부 에선 절대 안됨)
  - set_animal 등과 같은 member function에서는 접근이 가능하지만, main 같은 외부함수에서는 접근 안된다.
- public keyword
  - 말 그대로 공개된 것. 외부에서 맘껏 이용가능하다.

```cpp
class Animal {
    int food;
    int weight;
    ...
    ...
}
```
- **기본적으로 키워드를 명시하지 않았다면, Private이 된다.**
