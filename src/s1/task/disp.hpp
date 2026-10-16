#pragma once

#include "s1/hlib.hpp"
#include <cstdint>

namespace shenmue::s1 {

extern std::int32_t g_bIsNewGame;
extern std::uint8_t g_displayActNumber;
extern std::int32_t g_isOnMainMenu;



extern std::int32_t g_mainMenuOpened;
extern std::int32_t g_returnToPressStart;
extern std::uint8_t g_hasPickedSaveData;
extern std::int32_t g_numRemainLoadAttempts;
extern std::int32_t g_is70ManBattleLoad;
extern std::int32_t g_pickedSaveResumeKind;
extern std::int32_t g_settingFlagsCaptured;
extern std::int32_t g_70ManBattleLoadBeforeSettings;

std::int32_t DISP_GetTitleState() noexcept;
bool DISP_IsOnMainMenu() noexcept;
struct ScenarioContext;

void InternalInit();
void DISP_Module(HLib::TaskData *taskData);
std::int64_t LoadIntro();
void GetSaveSlotString();
void LoadSceneForSaveFile(std::int32_t actNumber);
void HLTaskFunc_InternalWeatherInit(std::int32_t mode);
void Script_cdttUtil(ScenarioContext *context, void *rawParams);
void Script_CtrlTimeLimit(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
