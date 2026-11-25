#pragma once
#include "shenmue.h"
#include "common/types.hpp"

#if !SHENMUE_S2_GCC295
#include <cstddef>
#include <cstdint>
#endif

#if SHENMUE_S2_GCC295
#define SHENMUE_MODULE_LAYOUT_NAME_(line) ShenmueModuleLayoutLayoutCheck##line
#define SHENMUE_MODULE_LAYOUT_NAME(line) SHENMUE_MODULE_LAYOUT_NAME_(line)
#define SHENMUE_MODULE_LAYOUT(condition) typedef char SHENMUE_MODULE_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#else
#define SHENMUE_MODULE_LAYOUT(condition) static_assert(condition)
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_S2_GCC295
using ::shenmue::shen_name;
#endif
#if SHENMUE_S2_GCC295
#define TASK_MODU shen_name(SHEN_NAME4('M', 'O', 'D', 'U'))
#else
inline constexpr shen_name TASK_MODU{SHEN_NAME(MODU)};
#endif

#if SHENMUE_IS_DC
typedef std::int32_t SystemModule;
enum {
    SystemModule_Test = 0,
    SystemModule_Cold = 1,
    SystemModule_Logo = 2,
    SystemModule_Hot = 3,
    SystemModule_Title = 4,
    SystemModule_Option = 5,
    SystemModule_Null = 6,
    SystemModule_Game = 7,
    SystemModule_Sample = 8,
    SystemModule_OutRun = 9,
    SystemModule_AfterBurner = 10,
    SystemModule_HangOn = 11,
    SystemModule_SpaceHarrier = 12,
    SystemModule_Credits = 13,
    SystemModule_External14 = 14,
    SystemModule_External15 = 15,
    SystemModule_External16 = 16,
    SystemModule_External17 = 17,
    SystemModule_External18 = 18,
    SystemModule_External19 = 19,
    SystemModule_External20 = 20,
    SystemModule_External21 = 21,
    SystemModule_External22 = 22,
    SystemModule_External23 = 23,
    SystemModule_Null24 = 24,
    SystemModule_Count = 25,

};
#else
enum class SystemModule : std::int32_t {
    Test = 0,
    Cold = 1,
    Logo = 2,
    Hot = 3,
    Title = 4,
    Option = 5,
#if SHENMUE_IS_DC
    Null = 6,
    Game = 7,
    Sample = 8,
    OutRun = 9,
    AfterBurner = 10,
    HangOn = 11,
    SpaceHarrier = 12,
    Credits = 13,
    External14 = 14,
    External15 = 15,
    External16 = 16,
    External17 = 17,
    External18 = 18,
    External19 = 19,
    External20 = 20,
    External21 = 21,
    External22 = 22,
    External23 = 23,
    Null24 = 24,
    Count = 25,
#else
    Game = 6,
    Sample = 7,
    OutRun = 8,
    AfterBurner = 9,
    HangOn = 10,
    SpaceHarrier = 11,
    Credits = 12,
    Movie = 13,
    Omake = 14,
    DiskChange = 15,
    MovieToOmake = 16,
    Count = 17,
#endif
};
#endif
extern SystemModule hotLoadModule;
void ChangeDisc();

struct SystemModuleTaskData {
    SystemModule currentModule;
    std::int32_t currentEntry;
    SystemModule previousModule;
    std::int32_t previousEntry;
    void *work;
#if !SHENMUE_IS_DC
    std::byte workStorage[0x100];
#endif
};

SHENMUE_MODULE_LAYOUT(sizeof(SystemModuleTaskData) == (SHENMUE_IS_DC ? 0x14 : 0x118));
SHENMUE_MODULE_LAYOUT(offsetof(SystemModuleTaskData, work) == 0x10);
#if !SHENMUE_IS_DC
SHENMUE_MODULE_LAYOUT(offsetof(SystemModuleTaskData, workStorage) == 0x18);
#endif
#if SHENMUE_S2_GCC295
typedef void (*FrontendTaskCallback)(void *);
#else
using FrontendTaskCallback = void (*)(void *);
#endif

struct FrontendPlatformAdapters {
    std::uint16_t (*controllerUserHandle)();
    bool (*continueSaveAvailable)();
    void (*requestQuit)();
    // These callbacks become the owning HLib task; middleware must perform
    // its normal task/module transition rather than run a nested loop here.
    FrontendTaskCallback newGameTask;
    FrontendTaskCallback continueSaveTask;
    FrontendTaskCallback titleLoadTask;
    FrontendTaskCallback seventyManTask;
};

void SetFrontendPlatformAdapters(const FrontendPlatformAdapters &adapters);
std::uint16_t FrontendControllerUserHandle();
void RequestFrontendQuit();
bool FrontendQuitRequested();
void ClearFrontendQuitRequested();
bool ContinueModuleIsAvailable();
bool SystemModuleIsAvailable(SystemModule module);
std::int64_t ResetSceneFolderReadiness();
void SetSceneFolderReadiness(std::int32_t ready);
void SetNextSceneNumber(std::uint32_t sceneNumber);
extern std::int32_t g_loadedActNumber;
std::uint64_t SetSceneNumber();
bool ExecuteSystemModule(SystemModule module, std::int32_t entry);
bool ChangeSystemModule(SystemModule module, std::int32_t entry);
void ChangeModuleTask_Init(void *taskData);
// InitScene includes the cold allocator/archive handoff.
void InitializeScene();
void ResetToTitleScreen();
void ResetSceneArchiveFileHandles();
// Native bPaused: set by boot and the title owner; cleared when Press Start is
// accepted and on direct game boot. Gates the CTRL ST fades and PAUS.
extern bool g_bPaused;
bool BeginContinueModuleHandoff();
bool BeginTitleLoadModuleHandoff();
bool BeginSeventyManModuleHandoff();
bool BeginOptionsModuleHandoff();
bool ReturnToTitleMenuHandoff();
bool BeginNewGameModuleHandoff();

} // namespace s2
} // namespace shenmue
#if SHENMUE_S2_GCC295
#undef SHENMUE_MODULE_LAYOUT_NAME
#undef SHENMUE_MODULE_LAYOUT_NAME_
#endif
#undef SHENMUE_MODULE_LAYOUT
