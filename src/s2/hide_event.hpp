#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"

namespace shenmue::s2 {

inline constexpr shen_name HIDE_AREA{SHEN_NAME(HIDE)}, HIDE_TASK{SHEN_NAME(hide)}, HIDE_RYO{SHEN_NAME(RYO_)}, HIDE_REN{SHEN_NAME(REN_)};
inline constexpr shen_name HIDE_DOOR{SHEN_NAME(R148)}, HIDE_AUTHOR{SHEN_NAME(AUTH)}, HIDE_LIGHT{SHEN_NAME(LGT1)};
inline constexpr Vector4 HIDE_REFBOARD{.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
inline constexpr char HIDE_MEDIA[] = "0214A000", HIDE_PACKAGE[] = "0214";
void HIDEInit();
void HIDEMain(std::uintptr_t argument);
std::uint64_t SetupRyoForHIDE();
void HIDEPlayAuthor(void *taskData);
void HIDEWaitAuthor(void *taskData);
void HIDEEndRoom(void *taskData);
extern const ScriptAreaDefinition hideScriptDefinition;

} // namespace shenmue::s2
