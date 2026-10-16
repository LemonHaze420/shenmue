#pragma once

#include "common/types.hpp"
#include "s2/hlib.hpp"

#include <cstddef>

namespace shenmue::s2::areas {

inline constexpr std::uint32_t AREA_AUTHOR_ROOT_POSITION_FLAGS = 0x78000000U;
inline constexpr shen_name AREA_AUTHOR_TASK{SHEN_NAME(EVNT)}, AREA_AUTHOR_INVALID{0xffffffffU};
inline constexpr shen_name AREA_AUTHOR_RYO{SHEN_NAME(RYO_)}, AREA_AUTHOR_REN{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_AUTHOR_SIN{SHEN_NAME(SIN_)};

struct AreaAuthorTaskParameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::int16_t selection;
    std::int16_t state;
};
struct AreaAuthorLoopTaskParameter {
    AreaAuthorTaskParameter *parent;
};
struct AreaAuthorFrameTaskParameter {
    AreaAuthorTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(AreaAuthorTaskParameter) == 0x10);
static_assert(offsetof(AreaAuthorTaskParameter, motion) == 8);
static_assert(offsetof(AreaAuthorTaskParameter, selection) == 12);
static_assert(offsetof(AreaAuthorTaskParameter, state) == 14);
static_assert(sizeof(AreaAuthorLoopTaskParameter) == 8);
static_assert(sizeof(AreaAuthorFrameTaskParameter) == 0x10);
static_assert(offsetof(AreaAuthorFrameTaskParameter, currentFrame) == 8);

struct AreaImmediateAuthorTaskParameter {
    HLib::Task *self;
    std::int16_t selection;
    std::int16_t state;
    std::uint32_t reserved0c;
};
struct AreaImmediateAuthorFrameParameter {
    AreaImmediateAuthorTaskParameter *parent;
    std::int16_t state;
};
static_assert(sizeof(AreaImmediateAuthorTaskParameter) == 0x10);
static_assert(offsetof(AreaImmediateAuthorTaskParameter, state) == 0x0a);
static_assert(sizeof(AreaImmediateAuthorFrameParameter) == 0x10);
static_assert(offsetof(AreaImmediateAuthorFrameParameter, state) == 8);

void ResetAreaAuthorCharacter(shen_name character, float unusedNativeArgument);
void EnableCharDisplayAndSetMotionworkData(shen_name character);
void SetupAreaRyoSinAuthorCharacters();

} // namespace shenmue::s2::areas
