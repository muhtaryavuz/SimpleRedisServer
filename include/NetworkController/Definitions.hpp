#pragma once  

#include <cstdint>

namespace app::net 
{
    constexpr int kInvalidSocketId = -1;

    struct Port{
        uint16_t value{0};
    };
    using port_t = Port;

    constexpr char kCR = 13;
    constexpr char kLF = 10;
} // namespace app::net