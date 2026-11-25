#pragma once

#include "s2/hlib.hpp"
#include "s2/flag_context_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

struct ShenPlayerData;
extern ShenPlayerData shenPlayerData;
static const std::uint32_t FLAG_CALLBACK_CYCLE = SHEN_NAME4('c', 'y', 'l', 'f');
static const std::size_t kFlagCallbackContextOffset = 0x40;
void shenRestartPlayerDataFlagEvaluation();
std::int32_t plymSetCallback(void (*callback)());
void InitializeFlagWeatherMode();
std::int32_t flagInitContext(ShenPlayerData &data, shen_name context);
std::int32_t shenUpdatePlayerDataFlags(std::int32_t active);
std::int32_t Queue_FLAG_CheckTask(shen_name area);

} } // namespace shenmue::s2

#endif
