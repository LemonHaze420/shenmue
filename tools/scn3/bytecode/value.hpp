#pragma once

#include <cstdint>

namespace shenmue::scenario {

enum class VmValueKind { Undefined, Unknown, Integer, CodeAddress, DataAddress, FrameAddress, TaskAddress };

struct VmValue {
    VmValueKind kind = VmValueKind::Undefined;
    std::uint32_t bits = 0;
    bool operator==(const VmValue &) const = default;
};

} // namespace shenmue::scenario
