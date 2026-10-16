#pragma once

#include "shenmue.h"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name AREA_EVENT_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name EVENT_HIGH_MODEL_TASK{SHEN_NAME(SHIG)};

namespace HLib {
struct Task;
}

enum class EventFunctionType : std::uint32_t {
    Event1 = 1,
    Event2 = 2,
    Event3 = 3,
};

enum AreaFlags : std::uint32_t {
    AreaFlag_KeepPlayerControlLocked = 0x1,
    AreaFlag_DeferPreAreaMainLoop = 0x2,
    AreaFlag_SuppressAutoControlLock = 0x4,
};

struct AreaTerminalMediaState {
    shen_name cutsceneArea{};
    std::array<char, 16> audioBank{};
    std::int32_t audioVariant{};
    bool sampleExecutionActive{};
};

using AreaInitFunction = void (*)();
using ScriptAreaMainFunction = void (*)(std::uintptr_t argument);
using GameplayAreaMainFunction = void (*)(std::uint32_t startParameters);
using GameplayAreaCleanupFunction = void (*)(std::int32_t afternoon, std::uint32_t areaIndex);

using AreaBaseCallback = void (*)(std::uint32_t *parameters);
extern std::array<AreaBaseCallback, 10> areaBaseCallbacks;
extern std::uint32_t areaFlags;
void AreaBaseDummyCallback(std::uint32_t *parameters);


struct ScriptAreaDefinition {
    shen_name areaId;
    std::uint32_t field04;
    AreaInitFunction init;
    ScriptAreaMainFunction main;
};



struct GameplayAreaDefinition {
    shen_name areaId;
    std::uint32_t padding04;
    AreaInitFunction init;
    GameplayAreaMainFunction main;
    GameplayAreaCleanupFunction cleanup;
};

#if !SHENMUE_IS_DC
static_assert(sizeof(ScriptAreaDefinition) == 0x18);
static_assert(offsetof(ScriptAreaDefinition, init) == 0x08);
static_assert(offsetof(ScriptAreaDefinition, main) == 0x10);
static_assert(sizeof(GameplayAreaDefinition) == 0x20);
static_assert(offsetof(GameplayAreaDefinition, cleanup) == 0x18);
#endif

void scenInitCallbacks();
extern std::int32_t g_areaHeldItemRestorePending;
#if SHENMUE_IS_DC
extern const std::array<std::uintptr_t, 4> eventExecutionRegions;
void *GetEventExecutionRegion(EventFunctionType functionType);
void InitializeEventExecutionRegions();
const void *InstallEventOverlay(const void *payload, const char *stem, std::size_t length, EventFunctionType functionType);
void ExecuteEventOverlay(std::uint32_t slot, std::uintptr_t argument);
#else

void LoadAreaFuncByIDAndFuncType(EventFunctionType functionType, const char *areaId);

std::int64_t ExecuteCurrAreaMainFunc(const char *slotName, std::uintptr_t argument);
std::int64_t ExecuteCurrAreaMainFunc_Wrapper(std::uint32_t slot, std::uintptr_t argument);
#endif
// The trailing digit selects the slot; an unchanged slot is not reloaded unless forced.
bool LoadAreaFunc_Wrapper_0(const char *areaId, std::int32_t force);

const char *GetAreaSlotName(std::uint32_t slot);
void scenFreeEvent(const char *requestedScenario);
// AllocatePAKStuff's EV1/EV2/EV3 package-entry owner.
bool shenLoadEventFrom(const char *areaId, EventFunctionType functionType);
bool ExecuteAreaMainFunction(const char *areaId, std::uintptr_t argument = 0);
bool ExecuteCurrentAreaMainFunction(EventFunctionType functionType, std::uintptr_t argument = 0);
bool ExecuteGameplayArea(std::uint32_t startParameters = 0);
bool ExecuteGameplayAreaCleanup(bool afternoon);
bool QueueAreaEventEntry();
void Startup_EventBase_Callback(void *taskData);
bool Queue_EI_Startup_EventFade1();
void QueueImmediateNextFadeTask(HLib::Task *task);

inline constexpr std::uint32_t kLoadingPhaseSkipStartupFadeWait = 0x1U;
inline constexpr std::uint32_t kLoadingPhaseWaitNpcBeforeFade = 0x100U;
inline constexpr std::uint32_t kLoadingPhaseSkipTitleFade = 0x200U;
void EI_Startup_BeginEventFade();
void EI_Startup_EventFade1_Callback(void *taskData);
void EI_Startup_EventFade2_Callback(void *taskData);
void EVT_NightAuthor_Callback(void *taskData);
void EVT_NightChange_Callback(void *taskData);
void ReleaseAreaEventTaskOwners();
HLib::Task *GetCurrentAreaScreenFilmTask();
void SetCharaKill(shen_name characterId, std::uint32_t mode);
void SetCurrentAreaControlLock(bool locked);
bool CurrentAreaOwnsControlLock();
bool IsAreaItemSelectionActive();
void PublishAreaTerminalMediaState(shen_name cutsceneArea, const char *audioBank, std::int32_t audioVariant, bool sampleExecutionActive);
const AreaTerminalMediaState &GetAreaTerminalMediaState();

} // namespace shenmue::s2
