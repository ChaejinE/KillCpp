#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

int main()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::cout << " now time : " << std::put_time(std::localtime(&t), "%F %T %z")
              << '\n';
}