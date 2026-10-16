#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct KeyRepeatTaskData {
    std::uint32_t inputIndex;
    std::int32_t repeatCountdown[8];
    std::int32_t trailingCountdown;
};

static_assert(sizeof(KeyRepeatTaskData) == 0x28);
static_assert(offsetof(KeyRepeatTaskData, inputIndex) == 0x00);
static_assert(offsetof(KeyRepeatTaskData, repeatCountdown) == 0x04);
static_assert(offsetof(KeyRepeatTaskData, trailingCountdown) == 0x24);

void HLTaskFunc_KeyRepeat(HLib::TaskData *taskData);
std::int64_t krepEnqueue(std::uint32_t inputIndex);

} // namespace shenmue::s1
