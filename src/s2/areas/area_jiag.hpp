#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name JIAG_TASK{SHEN_NAME(main)};
inline constexpr shen_name JIAG_LOAD_TASK{SHEN_NAME(load)};
inline constexpr shen_name JIAG_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name JIAG_INVALID{0xffffffffU};
inline constexpr shen_name JIAG_START_AUTHOR{SHEN_NAME(STRT)};
inline constexpr shen_name JIAG_WIN_AUTHOR{SHEN_NAME(WIN_)};
inline constexpr shen_name JIAG_RETRY_TITLE{SHEN_NAME(RTR2)};
inline constexpr shen_name JIAG_CHARACTERS[8]{
    SHEN_NAME(KEK_), SHEN_NAME(RIK_), SHEN_NAME(KHO_), SHEN_NAME(III_), SHEN_NAME(VUN_), SHEN_NAME(RNT_), SHEN_NAME(IKO_), JIAG_INVALID};
void AreaJIAGInit();
void AreaJIAGMain(std::uintptr_t parameter);
void JIAGPrepareEvent();
void JIAGPositionBattleCharacters();
void JIAGReleaseEvent();
void JIAGPlayPreAuthorCallback(void *data);
void JIAGWaitPreAuthorCallback(void *data);
void JIAGStartBattleCallback(void *data);
void JIAGWaitBattleCallback(void *data);
void JIAGPlayWinAuthorCallback(void *data);
void JIAGRetryCallback(void *data);
void JIAGRetryReturnCallback(void *data);
void JIAGWaitWinAuthorCallback(void *data);
void JIAGFinishCallback(void *data);
extern const ScriptAreaDefinition areaJIAGScriptDefinition;
} // namespace shenmue::s2::areas
