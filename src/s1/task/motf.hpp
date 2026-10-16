#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct InvalidatedMotionRange {
    std::uint16_t endAnimId;
    std::uint16_t firstAnimId;
    std::uint32_t reserved_04;
    InvalidatedMotionRange *next;
};
static_assert(sizeof(InvalidatedMotionRange) == 0x10);
static_assert(offsetof(InvalidatedMotionRange, endAnimId) == 0x00);
static_assert(offsetof(InvalidatedMotionRange, firstAnimId) == 0x02);
static_assert(offsetof(InvalidatedMotionRange, reserved_04) == 0x04);
static_assert(offsetof(InvalidatedMotionRange, next) == 0x08);


extern InvalidatedMotionRange *g_invalidatedMotionRange;

HLib::Task *MOTF_Init();
void MOTF_Shutdown();
HLib::Task *MOTF_GetTask();
void MotionFree_Callback(HLib::TaskData *taskData);




bool MOTF_RegisterInvalidatedRange(std::uint16_t firstAnimId, std::uint16_t endAnimId);
bool MOTF_IsAnimationInvalidated(std::uint16_t animationId);
void MOTF_DrainInvalidatedRanges();

} // namespace shenmue::s1
