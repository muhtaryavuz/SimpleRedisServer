#pragma once  

#include <cstdint>

namespace app::net 
{
    constexpr int InvalidSocketId = -1;

    struct Port{
        uint16_t value{0};
    };
    using port_t = Port;
} // namespace app::net