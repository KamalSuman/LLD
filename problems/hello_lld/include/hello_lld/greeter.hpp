#pragma once

#include <string>
#include <string_view>

namespace lld::hello {

class Greeter {
public:
    [[nodiscard]] std::string greet(std::string_view name) const;
};

}  // namespace lld::hello
