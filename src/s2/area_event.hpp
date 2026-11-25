#pragma once

#include "shenmue.h"
#include "common/types.hpp"

#if SHENMUE_S2_GCC295
#define SHENMUE_AREA_CONSTANT const
#define SHENMUE_AREA_ENUM_CLASS enum
#define SHENMUE_AREA_ENUM_BASE
#else
#include <array>
#include <cstddef>
#include <cstdint>
#define SHENMUE_AREA_CONSTANT inline constexpr
#define SHENMUE_AREA_ENUM_CLASS enum class
#define SHENMUE_AREA_ENUM_BASE : std::uint32_t
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_S2_GCC295
inline shen_name AreaEventTaskName() {
    return shen_name(SHEN_NAME4('E', 'V', 'N', 'T'));
}
inline shen_name EventHighModelTaskName() {
    return shen_name(SHEN_NAME4('S', 'H', 'I', 'G'));
}
#define AREA_EVENT_TASK AreaEventTaskName()
#define EVENT_HIGH_MODEL_TASK EventHighModelTaskName()
#else
inline constexpr shen_name AREA_EVENT_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name EVENT_HIGH_MODEL_TASK{SHEN_NAME(SHIG)};
inline constexpr shen_name AREA_NIGHT_AUTHOR{SHEN_NAME(YORU)};
#endif

namespace HLib {
struct Task;
}

SHENMUE_AREA_ENUM_CLASS EventFunctionType SHENMUE_AREA_ENUM_BASE{
    Event1 = 1,
    Event2 = 2,
    Event3 = 3,
};

enum AreaFlags SHENMUE_AREA_ENUM_BASE {
    AreaFlag_KeepPlayerControlLocked = 0x1,
    AreaFlag_DeferPreAreaMainLoop = 0x2,
    AreaFlag_SuppressAutoControlLock = 0x4,
};

struct AreaTerminalMediaState {
#if SHENMUE_S2_GCC295
    shen_name cutsceneArea;
    char audioBank[16];
    std::int32_t audioVariant;
    bool sampleExecutionActive;
    AreaTerminalMediaState() : cutsceneArea(), audioVariant(0), sampleExecutionActive(false) {
        for (unsigned i = 0; i < 16; ++i)
            audioBank[i] = 0;
    }
#else
    shen_name cutsceneArea{};
    std::array<char, 16> audioBank{};
    std::int32_t audioVariant{};
    bool sampleExecutionActive{};
#endif
};

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct AreaStartupFadeState {
    std::uint32_t retained[2];
    HLib::Task *nextTask;
};
#if SHENMUE_S2_GCC295
typedef char AreaStartupFadeSizeCheck[sizeof(AreaStartupFadeState) == 0x0c ? 1 : -1];
typedef char AreaStartupFadeTaskCheck[offsetof(AreaStartupFadeState, nextTask) == 8 ? 1 : -1];
inline shen_name AreaStartupTaskName() {
    return shen_name(SHEN_NAME4('E', 'V', 'B', 'S'));
}
inline shen_name AreaEntryPlacementName() {
    return shen_name(SHEN_NAME4('0', '0', '0', '0'));
}
#define AREA_STARTUP_TASK AreaStartupTaskName()
#define AREA_ENTRY_PLACEMENT AreaEntryPlacementName()
#else
static_assert(sizeof(AreaStartupFadeState) == 0x0c);
static_assert(offsetof(AreaStartupFadeState, nextTask) == 8);
inline constexpr shen_name AREA_STARTUP_TASK{SHEN_NAME(EVBS)};
inline constexpr shen_name AREA_ENTRY_PLACEMENT{SHEN_NAME(0000)};
#endif
#endif

#if SHENMUE_S2_GCC295
typedef void (*AreaInitFunction)();
typedef void (*ScriptAreaMainFunction)(std::uintptr_t argument);
typedef void (*GameplayAreaMainFunction)(std::uint32_t startParameters);
typedef void (*GameplayAreaCleanupFunction)(std::int32_t afternoon, std::uint32_t areaIndex);
typedef void (*AreaBaseCallback)(std::uint32_t *parameters);
extern AreaBaseCallback areaBaseCallbacks[10];
#else
using AreaInitFunction = void (*)();
using ScriptAreaMainFunction = void (*)(std::uintptr_t argument);
using GameplayAreaMainFunction = void (*)(std::uint32_t startParameters);
using GameplayAreaCleanupFunction = void (*)(std::int32_t afternoon, std::uint32_t areaIndex);

using AreaBaseCallback = void (*)(std::uint32_t *parameters);
extern std::array<AreaBaseCallback, 10> areaBaseCallbacks;
#endif
extern std::uint32_t areaFlags;
void AreaBaseDummyCallback(std::uint32_t *parameters);
// PC v1.07 area_def_t: 351 specialized rows plus a null terminator,
// in g_area_definitions, used by LoadAreaFuncByIDAndFuncType.
struct ScriptAreaDefinition {
    shen_name areaId;
    std::uint32_t field04;
    AreaInitFunction init;
    ScriptAreaMainFunction main;
};

// PC v1.07 EVT_AreaDefinition: 98 ordinary-area rows plus a null terminator,
// in g_areaDefs.area_tbl, after the ten g_areaDefs.base_tbl entries.
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
#if SHENMUE_S2_GCC295
extern std::uintptr_t eventExecutionRegions[4];
#else
extern const std::array<std::uintptr_t, 4> eventExecutionRegions;
#endif
void *GetEventExecutionRegion(EventFunctionType functionType);
void InitializeEventExecutionRegions();
const void *InstallEventOverlay(const void *payload, const char *stem, std::size_t length, EventFunctionType functionType);
void ExecuteEventOverlay(std::uint32_t slot, std::uintptr_t argument);
#else
// Callers own the area slot name, as in native.
void LoadAreaFuncByIDAndFuncType(EventFunctionType functionType, const char *areaId);
// Native PC FTBL 263/264 name-based dispatch and slot-index wrapper.
std::int64_t ExecuteCurrAreaMainFunc(const char *slotName, std::uintptr_t argument);
std::int64_t ExecuteCurrAreaMainFunc_Wrapper(std::uint32_t slot, std::uintptr_t argument);
#endif
// The trailing digit selects the slot; an unchanged slot is not reloaded unless forced.
bool LoadAreaFunc_Wrapper_0(const char *areaId, std::int32_t force);
// Native shenGetAreaSlotByIndex; null above slot three.
const char *GetAreaSlotName(std::uint32_t slot);
void scenFreeEvent(const char *requestedScenario);
void scenFreeEventByIndex(std::uint32_t slot);
std::int32_t shenGetAreaSlotByName(const char *areaId);
// AllocatePAKStuff's EV1/EV2/EV3 package-entry owner.
bool shenLoadEventFrom(const char *areaId, EventFunctionType functionType);
bool ExecuteAreaMainFunction(const char *areaId, std::uintptr_t argument = 0);
bool ExecuteCurrentAreaMainFunction(EventFunctionType functionType, std::uintptr_t argument = 0);
bool ExecuteGameplayArea(std::uint32_t startParameters = 0);
bool ExecuteGameplayAreaCleanup(bool afternoon);
#if SHENMUE_IS_DC
HLib::Task *QueueAreaEventEntry();
#else
bool QueueAreaEventEntry();
#endif
void Startup_EventBase_Callback(void *taskData);
#if SHENMUE_IS_DC
void Queue_EI_Startup_EventFade1();
#else
bool Queue_EI_Startup_EventFade1();
#endif
void QueueImmediateNextFadeTask(HLib::Task *task);
// Native loading-phase bits read by the EVBS startup fade.
SHENMUE_AREA_CONSTANT std::uint32_t kLoadingPhaseSkipStartupFadeWait = 0x1U;
SHENMUE_AREA_CONSTANT std::uint32_t kLoadingPhaseWaitNpcBeforeFade = 0x100U;
SHENMUE_AREA_CONSTANT std::uint32_t kLoadingPhaseSkipTitleFade = 0x200U;
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

#if SHENMUE_S2_GCC295
typedef char EventFunctionTypeSizeCheck[sizeof(EventFunctionType) == 4 ? 1 : -1];
typedef char AreaFlagsSizeCheck[sizeof(AreaFlags) == 4 ? 1 : -1];
typedef char AreaTerminalMediaSizeCheck[sizeof(AreaTerminalMediaState) == 0x1c ? 1 : -1];
#endif

} // namespace s2
} // namespace shenmue

#undef SHENMUE_AREA_CONSTANT
#undef SHENMUE_AREA_ENUM_CLASS
#undef SHENMUE_AREA_ENUM_BASE
