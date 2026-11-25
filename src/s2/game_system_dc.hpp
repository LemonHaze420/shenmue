#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include <cstddef>
#endif

namespace shenmue {
namespace s2 {

#define TASK_SUPP SHEN_NAME4('S', 'U', 'P', 'P')
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct SupportTaskData {
    std::int32_t flagIndex, markedFlag;
};
typedef char Layout_game_system_dc_0[(sizeof(SupportTaskData) == 8) ? 1 : -1];
typedef char Layout_game_system_dc_1[(offsetof(SupportTaskData, markedFlag) == 4) ? 1 : -1];
std::uint32_t DebugMenuInputBlockedNullsub();
#endif

extern std::int32_t g_currentShenmue1SaveAreaId;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern std::uint32_t gameSysActivityFlags, supportIptMaximum, supportIptMinimum;
#else
extern std::uint32_t gameSysActivityFlags, gameSysUnusedColour;
#endif
extern std::int32_t gameSysActivityCounters[5];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern std::int32_t currentDebugMenuPage, inputSuppressionFrames;
#else
extern std::int32_t gameSysUnusedState, currentDebugMenuPage, inputSuppressionFrames;
#endif
extern std::uint32_t g_totalFrameCount;
extern std::int32_t g_authorFrameCount;
extern std::uint8_t frameCounterEnabled;
void ResetGameSysActivity();
void SetGameSysActivityFlag(std::int32_t index);
void ClearGameSysActivityFlag(std::int32_t index);
void UpdateGameSysActivity();
void InitializeGameInputSuppression();
void UpdateGameInputSuppression();
void SuppressGameInput(std::int32_t frames);
void InitializeFrameVars();
void IncrementFrameCount();
void AddFrameCount(std::int32_t count);
std::int32_t GetShenmue1SaveAreaId(shen_name areaId);
void SUPP_Callback(void *data);
void GameSys_Callback(void *data);
void GameSys(void *data);

} // namespace s2
} // namespace shenmue

#endif
