#pragma once

#include "s2/scenario_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

bool IsScenarioCharacterDisplayEnabled(shen_name name);
void EV_EnableCharacterDisplay(shen_name name);
void EV_DisableCharacterDisplay(shen_name name);
void SetScenarioCharacterKillMode(shen_name name, std::int32_t mode);
void EV_EnsureCharacterDraw(shen_name name);
bool IsScenarioCharacterActive(shen_name name);
std::int32_t DoesScenarioCharacterExist(shen_name name);
std::int32_t EV_SetCharacterAlphaEnabled(shen_name name, std::int32_t enabled);
void SetScenarioCharacterTransparency(shen_name name, float alpha);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct ReflectorBoardWork;
Character *ctrlGetCharaByName(shen_name name);
std::int32_t CheckIsArgEquZero(std::int32_t value);
void SetScenarioCharacterDrawControl(shen_name name, std::int32_t mode, std::int32_t argument);
ReflectorBoardWork *SetScenarioReflectorBoardValues(shen_name name, const Vector4 *values);
void SetScenarioReflectorBoardMode(shen_name name, std::int32_t sector);
#endif

} // namespace s2
} // namespace shenmue

#endif
