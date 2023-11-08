#pragma once

#include <string>

namespace textspotter {
    class TextSpotter {
    public:
        TextSpotter() = default;
        ~TextSpotter() = default;

        auto Hello() -> std::string;
    };
}