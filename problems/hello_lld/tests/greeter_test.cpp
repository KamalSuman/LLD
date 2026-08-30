#include "hello_lld/greeter.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    const lld::hello::Greeter greeter;
    const std::string expected = "Hello, Kamal! Your C++ LLD workspace is ready.";
    const std::string actual = greeter.greet("Kamal");

    if (actual != expected) {
        std::cerr << "Expected: " << expected << "\nActual:   " << actual << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
