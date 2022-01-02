#include <iostream>
#include <map>
#include <string>
#include <utility>

std::optional<std::string> GetValueFromMap(const std::map<int, std::string>& m,
                                           int key) {
  auto itr = m.find(key);
  if (itr != m.end()) {
    return itr->second;
  }

  // nullopt 는 <optional> 에 정의된 객체로 비어있는 optional 을 의미한다.
  return std::nullopt;
}

int main() {
  std::map<int, std::string> data = {{1, "hi"}, {2, "hello"}, {3, "hiroo"}};
  std::cout << "맵에서 2 에 대응되는 값은? " << GetValueFromMap(data, 2).value()
            << std::endl;
  std::cout << "맵에 4 는 존재하나요 " << std::boolalpha
            << GetValueFromMap(data, 4).has_value() << std::endl;
}