#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
void StartAreaChime(void);
void AreaChimeCallback(void *taskData);
#else
#include "s2/area_event.hpp"
#include "common/math.hpp"

#include <array>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr std::uint64_t quarterMinuteMask = (std::uint64_t{1} << 15U) | (std::uint64_t{1} << 30U) | (std::uint64_t{1} << 45U);
inline constexpr Vector3 chimePosition{.x = 301.29998779296875f, .y = 128.8000030517578125f, .z = 73.1999969482421875f};

inline constexpr shen_name CHIME_TASK{SHEN_NAME(chim)};
inline constexpr std::array<std::uint32_t, 30> AREA_CHIME_SOUND_TABLE{
#include "chime_sound_table.inl"
};

extern std::int32_t areaChimeLastSound;
void AreaChimeCallback(void *taskData);

} // namespace shenmue::s2::areas

#endif
