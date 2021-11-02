#include <iostream>

namespace {
    // static int x와 동일
    int only_in_this_file = 0;

    // static int OnlyInThisFile()과 동일
    int OnlyInThisFile() {
        std::cout << only_in_this_file << std::endl;
    }

    // 이 파일안에서만 접근된다.
}

int main() {
    OnlyInThisFile();
    only_in_this_file = 3;
    OnlyInThisFile();
}
