#include "hello_lld/greeter.hpp"

#include <string>

namespace lld::hello {

std::string Greeter::greet(const std::string_view name) const {
    return "Hello, " + std::string{name} + "! Your C++ LLD workspace is ready.";
}

}  // namespace lld::hello
