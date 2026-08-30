#include "hello_lld/greeter.hpp"

#include <iostream>

int main() {
    const lld::hello::Greeter greeter;
    std::cout << greeter.greet("Kamal") << '\n';
}
