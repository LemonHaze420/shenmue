#pragma once

#include "s2/map_event_character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

bool plymIsSearchBlocked();
std::int32_t srchCheckFlags(std::int32_t mode);
std::int32_t PlayerBusyCheckNullsub();
bool shenCheckIsBusy();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool shenGetIsPlayerInMenu();
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool plymCanEvaluateSearchTargets();
#endif
std::int32_t IsCtrlSpeakIconInactive();
std::uint32_t IsSaveAllowed();
std::int32_t HasScenarioCharacterControl();

} // namespace s2
} // namespace shenmue

#endif
