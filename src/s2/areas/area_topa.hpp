#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name TOPA_TASK{SHEN_NAME(topa)};
inline constexpr shen_name TOPA_RYO{SHEN_NAME(RYO_)}, TOPA_REN{SHEN_NAME(REN_)}, TOPA_CHR{SHEN_NAME(CHR_)};
inline constexpr shen_name TOPA_SOY{SHEN_NAME(SOY_)}, TOPA_INVALID{0xffffffffU}, TOPA_DOOR{SHEN_NAME(R101)};
inline constexpr shen_name TOPA_AUT1{SHEN_NAME(AUT1)}, TOPA_AUT2{SHEN_NAME(AUT2)};
inline constexpr shen_name TOPA_CHARACTERS[]{TOPA_REN, TOPA_CHR, TOPA_SOY, TOPA_INVALID};
inline constexpr Vector4 TOPA_REFLECTOR{.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
extern std::int32_t topaMode;
void TopaInit();
void TopaMain(std::uintptr_t argument);
void PrepareTopaCharacters();
void TopaPlayAuthorCallback(void *taskData);
void TopaWaitAuthorCallback(void *taskData);
void TopaPlaySecondAuthorCallback(void *taskData);
void EndTopaEvent();
extern const ScriptAreaDefinition topaScriptDefinition;

} // namespace shenmue::s2
