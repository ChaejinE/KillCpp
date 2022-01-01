#include <iostream>

constexpr int Factorial(int n) {
  int total = 1;
  for (int i = 1; i <= n; i++) {
    total *= i;
  }
  return total;
}

template <int N>
struct A {
  int operator()() { return N; }
};

int main() {
  A<Factorial(10)> a;

  std::cout << a() << std::endl;
}