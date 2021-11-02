#include <iostream>

namespace header1 {
    int foo() {
        std::cout << "namespace header 1 foo()" << std::endl;
        return 0;};
    void bar();
}