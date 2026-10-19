#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0278_RYO{SHEN_NAME(RYO_)}, AREA_0278_SYUEI{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0278_FANGMEI{SHEN_NAME(KNK_)}, AREA_0278_HUNHUI{SHEN_NAME(KSY_)};
inline constexpr shen_name AREA_0278_ZHOUSHAN{SHEN_NAME(KSM_)}, AREA_0278_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name AREA_0278_RETURN{SHEN_NAME(KRF1)};
inline constexpr shen_name AREA_0278_LIGHTS[]{SHEN_NAME(l001), SHEN_NAME(l101), SHEN_NAME(l301)};
inline constexpr shen_name AREA_0278_CHARACTERS[]{AREA_0278_HUNHUI, AREA_0278_ZHOUSHAN, AREA_0278_SYUEI, AREA_0278_FANGMEI, shen_name{0xffffffffU}};
inline constexpr shen_name AREA_0278_MODELS[]{
    SHEN_NAME(a015), SHEN_NAME(a016), SHEN_NAME(a236), SHEN_NAME(a411), SHEN_NAME(a410), SHEN_NAME(a716), SHEN_NAME(aa34)};
extern const ScriptAreaDefinition area0278ScriptDefinition;
} // namespace shenmue::s2::areas
