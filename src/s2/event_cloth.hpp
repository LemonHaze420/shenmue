#pragma once
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

namespace shenmue::s2 {

struct ClothControlParamEntry {
    std::int32_t frame;
    shen_name character;
    std::uint32_t flag;
    std::int32_t enabled;
};
struct ClothControlTaskData {
    HLib::Task *task;
    const ClothControlParamEntry *entries;
    std::int32_t frame;
    std::uint32_t reserved14;
};
static_assert(sizeof(ClothControlParamEntry) == 0x10);
static_assert(sizeof(ClothControlTaskData) == 0x18);
inline constexpr shen_name EVENT_CLOTH_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name EVENT_CLOTH_END{0xffffffffU};
inline constexpr ClothControlParamEntry EVENT_CLOTH_PARAMETERS[]{{.frame = 0, .character = EVENT_CLOTH_RYO, .flag = 4, .enabled = 1},
    {.frame = 24, .character = EVENT_CLOTH_RYO, .flag = 4, .enabled = 0}, {.frame = 24, .character = EVENT_CLOTH_RYO, .flag = 16, .enabled = 1},
    {.frame = -1, .character = EVENT_CLOTH_END, .flag = 0, .enabled = 0}};
void CreateClothControlTask();
void ET_ClothControl_Callback(void *);

} // namespace shenmue::s2
