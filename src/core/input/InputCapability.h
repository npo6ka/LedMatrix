#pragma once

#include <stdint.h>

enum class InputCapability : uint32_t {
    None = 0,
    Audio = 1U << 0,
};

inline InputCapability operator|(InputCapability a, InputCapability b) {
    return static_cast<InputCapability>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline InputCapability operator&(InputCapability a, InputCapability b) {
    return static_cast<InputCapability>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline bool hasCapability(InputCapability mask, InputCapability cap) {
    return (static_cast<uint32_t>(mask) & static_cast<uint32_t>(cap)) != 0U;
}
