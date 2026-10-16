#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name TTRP_TASK{SHEN_NAME(ttrp)};
inline constexpr shen_name TTRP_RYO{SHEN_NAME(RYO_)}, TTRP_REN{SHEN_NAME(REN_)}, TTRP_CHR{SHEN_NAME(CHR_)};
inline constexpr shen_name TTRP_SCX{SHEN_NAME(SCX_)}, TTRP_TGY{SHEN_NAME(TGY_)};
inline constexpr shen_name TTRP_AUT1{SHEN_NAME(AUT1)}, TTRP_AUT2{SHEN_NAME(AUT2)}, TTRP_LIGHT{SHEN_NAME(LGT1)};
inline constexpr shen_name TTRP_A202{SHEN_NAME(a202)}, TTRP_A203{SHEN_NAME(a203)}, TTRP_INVALID{0xffffffffU};
inline constexpr shen_name TTRP_CHARACTERS[]{TTRP_REN, TTRP_CHR, TTRP_SCX, TTRP_TGY, TTRP_INVALID};
inline constexpr Vector4 TTRP_REFLECTOR{.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
void TtrpInit();
void TtrpDeveloperNullsub();
void TtrpMain(std::uintptr_t argument);
void PrepareTtrpCharacters();
void TtrpPlayAuthorCallback(void *taskData);
void TtrpWaitFirstAuthorCallback(void *taskData);
void TtrpWaitSecondAuthorCallback(void *taskData);
void TtrpEndCallback(void *taskData);
extern const ScriptAreaDefinition ttrpScriptDefinition;

} // namespace shenmue::s2
