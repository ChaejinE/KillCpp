#include <iostream>
#include <set>

template <typename T>
void print_set(std::set<T>& s) {
  // 셋의 모든 원소들을 출력하기
  std::cout << "[ ";
  for (const auto& elem : s) {
    std::cout << elem << " ";
  }
  std::cout << " ] " << std::endl;
}

int main() {
  std::set<int> s;
  s.insert(10);
  s.insert(20);
  s.insert(30);
  s.insert(20);
  s.insert(10);

  print_set(s);
}