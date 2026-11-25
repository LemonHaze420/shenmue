#pragma once

#include "shenmue.h"
#include "common/math.hpp"
#include "common/types.hpp"
#if !SHENMUE_S2_GCC295
#include <cstddef>
#include <cstdint>
#endif

#if SHENMUE_S2_GCC295
#define SHENMUE_SEARCH_DATA_CHECK(name, test) typedef char name[(test) ? 1 : -1]
#else
#define SHENMUE_SEARCH_DATA_CHECK(name, test) static_assert(test)
#endif

namespace shenmue {
namespace s2 {

// PC records have a 0x20-byte stride, including the yaw-admission halfword.

struct SearchTarget {
    shen_name name;
    std::uint32_t flags;
    std::uint32_t field08;
    Vector3 position;
    float lockonAngle;
    std::int16_t yaw;
    std::uint16_t yawLimit;
};
SHENMUE_SEARCH_DATA_CHECK(SearchTargetSizeCheck, sizeof(SearchTarget) == 0x20);
SHENMUE_SEARCH_DATA_CHECK(SearchTargetPositionCheck, offsetof(SearchTarget, position) == 0x0c);
SHENMUE_SEARCH_DATA_CHECK(SearchTargetYawLimitCheck, offsetof(SearchTarget, yawLimit) == 0x1e);

struct SearchResourceContext {
    std::byte *resource;
    std::byte *cursor;
    std::byte *strings;
#if SHENMUE_IS_DC
    std::uint32_t retained;
#else
    std::uint64_t field18;
#endif
    std::uint32_t flags;
    float remainingTicks;
};
#if SHENMUE_IS_DC
SHENMUE_SEARCH_DATA_CHECK(SearchResourceContextSizeCheck, sizeof(SearchResourceContext) == 0x18);
SHENMUE_SEARCH_DATA_CHECK(SearchResourceRetainedCheck, offsetof(SearchResourceContext, retained) == 0x0c);
SHENMUE_SEARCH_DATA_CHECK(SearchResourceTicksCheck, offsetof(SearchResourceContext, remainingTicks) == 0x14);
#else
SHENMUE_SEARCH_DATA_CHECK(SearchResourcePcSizeCheck, sizeof(SearchResourceContext) == 0x28);
#endif

#undef SHENMUE_SEARCH_DATA_CHECK

} } // namespace shenmue::s2
