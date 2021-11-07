#include <iostream>
#include <string.h>

class Marine
{
    int hp;
    int coord_x, coord_y;
    int damage;
    bool is_dead;
    char *name; // 마린 이름

public:
    Marine();                                      // 기본생성자
    Marine(int x, int y);                          // x, y 좌표에 마린 생성하는 생성자
    Marine(int x, int y, const char *marine_name); // 이름까지 지정하는 생성자
    ~Marine();

    int attack();                      // 데미지 return
    void be_attacked(int damage_earn); // 입은 데미지
    void move(int x, int y);
    void show_status();
};

Marine::Marine(int x, int y)
{
    coord_x = x;
    coord_y = y;
    hp = 50;
    damage = 5;
    is_dead = false;

    name = NULL;
}

Marine::Marine(int x, int y, const char *marine_name)
{
    name = new char[strlen(marine_name) + 1];
    strcpy(name, marine_name);

    coord_x = x;
    coord_y = y;
    hp = 50;
    damage = 5;
    is_dead = false;
}

void Marine::move(int x, int y)
{
    coord_x = x;
    coord_y = y;
}

int Marine::attack() { return damage; }
void Marine::be_attacked(int damage_earn)
{
    hp -= damage_earn;
    if (hp < 0)
        is_dead = true;
}

void Marine::show_status()
{
    std::cout << " Marine " << name << std::endl;
    std::cout << "Location : ( " << coord_x << " , " << coord_y << " )"
              << std::endl;

    std::cout << " HP : " << hp << std::endl;
}

Marine::~Marine()
{
    std::cout << name << " 의 소멸자 호출 " << std::endl;
    if (name != NULL)
    {
        delete[] name;
    }
}

int main()
{
    Marine *marines[100];

    marines[0] = new Marine(2, 3, "Marine 2");
    marines[1] = new Marine(1, 5, "Marine 1");

    marines[0]->show_status();
    marines[1]->show_status();

    std::cout << std::endl
              << "마린 1이 마린 2 공격" << std::endl;

    marines[0]->be_attacked(marines[1]->attack());

    marines[0]->show_status();
    marines[1]->show_status();

    delete marines[0];
    delete marines[1];

    return 0;
}
