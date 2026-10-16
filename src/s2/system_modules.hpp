#pragma once
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name TASK_MODU{SHEN_NAME(MODU)};

enum class SystemModule : std::int32_t {
    Test = 0,
    Cold = 1,
    Logo = 2,
    Hot = 3,
    Title = 4,
    Option = 5,
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
};
extern SystemModule hotLoadModule;
void ChangeDisc();

struct SystemModuleTaskData {
    SystemModule currentModule;
    std::int32_t currentEntry;
    SystemModule previousModule;
    std::int32_t previousEntry;
    void *work;
    std::byte workStorage[0x100];
};

static_assert(sizeof(SystemModuleTaskData) == 0x118);
static_assert(offsetof(SystemModuleTaskData, work) == 0x10);
static_assert(offsetof(SystemModuleTaskData, workStorage) == 0x18);
using FrontendTaskCallback = void (*)(void *);

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

void InitializeScene();
void ResetToTitleScreen();
void ResetSceneArchiveFileHandles();


extern bool g_bPaused;
bool BeginContinueModuleHandoff();
bool BeginTitleLoadModuleHandoff();
bool BeginSeventyManModuleHandoff();
bool BeginOptionsModuleHandoff();
bool ReturnToTitleMenuHandoff();
bool BeginNewGameModuleHandoff();

} // namespace shenmue::s2
