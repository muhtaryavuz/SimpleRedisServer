#pragma once

#include <string>

namespace app::net {
    class Utils{
        public:
        Utils() = default;
        ~Utils() = default;

        [[nodiscard]] static bool IsValidIPv4(const std::string& ip);
    };
} // namespace app::net