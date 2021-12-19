#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

void producer(std::atomic<bool>* is_ready, int* data) {
  *data = 10;
  is_ready->store(true, std::memory_order_release);
}

void consumer(std::atomic<bool>* is_ready, int* data) {
  // data 가 준비될 때 까지 기다린다.
  while (!is_ready->load(std::memory_order_acquire)) {
  }

  std::cout << "Data : " << *data << std::endl;
}

int main() {
  std::vector<std::thread> threads;

  std::atomic<bool> is_ready(false);
  int data = 0;

  threads.push_back(std::thread(producer, &is_ready, &data));
  threads.push_back(std::thread(consumer, &is_ready, &data));

  for (int i = 0; i < 2; i++) {
    threads[i].join();
  }
}