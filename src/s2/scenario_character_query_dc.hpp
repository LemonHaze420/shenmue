#pragma once

#include "s2/scenario_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

Character *ctrlGetScenarioChara(shen_name name);
shen_name ctrlGetScenarioCharaName(const Character *character);
bool HasCharacterScenarioName(const Character *character, const char *name);
std::int32_t ScenarioCharacterIsRyo(shen_name name);

} // namespace s2
} // namespace shenmue

#endif
