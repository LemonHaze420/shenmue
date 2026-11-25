#pragma once

#include "s2/hlib.hpp"
#include "s2/system_modules.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if !SHENMUE_S2_GCC295
#include <array>
#endif

namespace shenmue { namespace s2 {

void EventRuntimeNullsub();
void EventSystemNullsub();
void EventSystemAlternateNullsub();

struct SystemModuleDefinition {
    HLib::TaskCallback callback;
    const char *filename;
    void *executionRegion;
};
typedef char SystemModuleDefinitionSizeCheck[sizeof(SystemModuleDefinition) == 0x0c ? 1 : -1];

#if SHENMUE_S2_GCC295
static const std::uintptr_t kModuleExecutionRegion = 0x0cf00000;
static const std::uintptr_t kExternalModuleExecutionRegion = 0x0cfc0000;
extern const SystemModuleDefinition systemModuleDefinitions[SystemModule_Count];
#else
inline constexpr std::uintptr_t kModuleExecutionRegion = 0x0cf00000;
inline constexpr std::uintptr_t kExternalModuleExecutionRegion = 0x0cfc0000;
extern const std::array<SystemModuleDefinition, static_cast<std::size_t>(SystemModule::Count)> systemModuleDefinitions;
#endif
extern HLib::Task *systemModuleTask;
extern SystemModuleTaskData systemModuleTaskData;

void InitializeShenmue();
void SetSceneFolders();
void COLB_Module(void *taskData);
void TitleModule(void *taskData);
void GameSys(void *taskData);
void GameOutRunModuleCallback(void *taskData);
void GameAfterBurnerModuleCallback(void *taskData);
void GameHangOnModuleCallback(void *taskData);
void GameSpaceHarrierModuleCallback(void *taskData);
void StaffRoll(void *taskData);

} } // namespace shenmue::s2

#endif
