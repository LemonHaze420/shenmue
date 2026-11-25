#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC

#include <cstddef>
#include <cstdint>

#if SHENMUE_S2_GCC295
#define SHENMUE_MOTION_FILE_LAYOUT_NAME_(line) SHENMUE_MOTION_FILELayoutCheck##line
#define SHENMUE_MOTION_FILE_LAYOUT_NAME(line) SHENMUE_MOTION_FILE_LAYOUT_NAME_(line)
#define SHENMUE_MOTION_FILE_LAYOUT(condition) typedef char SHENMUE_MOTION_FILE_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#define SHENMUE_MOTION_FILE_CONSTANT static const
#define SHENMUE_MOTION_FILE_INITIALIZER =
#else
#define SHENMUE_MOTION_FILE_LAYOUT(condition) static_assert(condition)
#define SHENMUE_MOTION_FILE_CONSTANT inline constexpr
#define SHENMUE_MOTION_FILE_INITIALIZER
#endif

namespace shenmue {
namespace s1 {

SHENMUE_MOTION_FILE_CONSTANT std::uint8_t kMarkerRecordSizes[28] SHENMUE_MOTION_FILE_INITIALIZER{
    12, 4, 8, 16, 20, 12, 8, 4, 0, 16, 8, 12, 4, 4, 16, 8, 2, 8, 4, 4, 4, 4, 4, 4, 4, 4, 8, 8};
SHENMUE_MOTION_FILE_CONSTANT std::byte kEmptyMotionMarkerData[14] SHENMUE_MOTION_FILE_INITIALIZER {
#if SHENMUE_S2_GCC295
    0
#endif
};

struct BaseMotionFile {
    std::uint32_t handle;
    void *data;
    std::uint16_t firstAnimation;
    std::uint16_t endAnimation;
    BaseMotionFile *previous;
    BaseMotionFile *next;
    std::uint32_t dataSize;
};
SHENMUE_MOTION_FILE_LAYOUT(sizeof(BaseMotionFile) == 0x18);
SHENMUE_MOTION_FILE_LAYOUT(offsetof(BaseMotionFile, data) == 4);
SHENMUE_MOTION_FILE_LAYOUT(offsetof(BaseMotionFile, firstAnimation) == 8);
SHENMUE_MOTION_FILE_LAYOUT(offsetof(BaseMotionFile, endAnimation) == 0xa);
SHENMUE_MOTION_FILE_LAYOUT(offsetof(BaseMotionFile, previous) == 0xc);
SHENMUE_MOTION_FILE_LAYOUT(offsetof(BaseMotionFile, next) == 0x10);
SHENMUE_MOTION_FILE_LAYOUT(offsetof(BaseMotionFile, dataSize) == 0x14);

} // namespace s1
} // namespace shenmue
#undef SHENMUE_MOTION_FILE_LAYOUT_NAME_
#undef SHENMUE_MOTION_FILE_LAYOUT_NAME
#undef SHENMUE_MOTION_FILE_LAYOUT
#undef SHENMUE_MOTION_FILE_CONSTANT
#undef SHENMUE_MOTION_FILE_INITIALIZER

#endif
