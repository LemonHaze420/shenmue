#include "s2/system_modules.hpp"

#include "d3t/buffer.hpp"
#include "d3t/archive_handles.hpp"
#include "d3t/file_state.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/area_event.hpp"
#include "s2/area_jump.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/audio_data.hpp"
#include "s2/battle_qte.hpp"
#include "s2/cold_archive.hpp"
#include "s2/conversation.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/font.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/initialization.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/load_title.hpp"
#include "s2/effect_texture.hpp"
#include "s2/movie.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/maze.hpp"
#include "s2/memo.hpp"
#include "s2/motion.hpp"
#include "s2/mt7.hpp"
#include "s2/navi.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/pawn.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/title.hpp"
#include "s2/input.hpp"
#include "s2/ui.hpp"
#include "s2/sound_program.hpp"
#include "s2/world_render.hpp"
#include "s2/help.hpp"
#include "s2/areas/game/arcade.hpp"
#include "s2/directory_cache.hpp"

#include <array>
#include <cstdint>
#include <cstdio>

namespace shenmue::s2 {

bool g_bPaused = false;
std::int32_t g_loadedActNumber = 1;
SystemModule hotLoadModule = SystemModule::Test;

namespace {
std::int32_t sceneFolderReadiness{};
std::int32_t unusedSceneFolderWord{};
}

// @verified - p
std::int64_t ResetSceneFolderReadiness() {
    unusedSceneFolderWord = 0;
    sceneFolderReadiness = 0;
    return 0;
}

// @verified - p
void SetSceneFolderReadiness(std::int32_t ready) {
    sceneFolderReadiness = ready;
}

// @verified - p
void mstsInit() {
    ResetNativePlayerStats();
    texSetMotionFlag(0);
    MSTS_Reader(0, 0, 0);
}

namespace {

using ModuleCallback = HLib::TaskCallback;

void ChangeToTitleControlTask(void *taskData);
void ChangeModuleTask_USERFILT(void *taskData);
void ChangeModuleTask_Callback(void *taskData);
void InitDiscChange(void *taskData);
void QueueOmake(void *taskData);

FrontendPlatformAdapters frontendPlatformAdapters{
    .controllerUserHandle = nullptr,
    .continueSaveAvailable = S2Save_HasContinueSlot,
    .requestQuit = nullptr,
    .newGameTask = S2Save_NewGameTitleTask,
    .continueSaveTask = S2Save_ContinueTitleTask,
    .titleLoadTask = S2Save_TitleLoadTask,
    .seventyManTask = nullptr,
};
bool frontendQuitRequested = false;

void ModuleNoOp(void *) {
}

void SystemModuleExecNullsub() {
}

std::array<ModuleCallback, static_cast<std::size_t>(SystemModule::Count)> moduleCallbacks{
    nullptr,
    ChangeModuleTask_Init,
    ChangeToTitleControlTask,
    ChangeModuleTask_USERFILT,
    TitleModule,
    nullptr,
    GameSys,
    ModuleNoOp,
    GameOutRunModuleCallback,
    GameAfterBurnerModuleCallback,
    GameHangOnModuleCallback,
    GameSpaceHarrierModuleCallback,
    StaffRoll,
    MovieTask,
    ModuleNoOp,
    InitDiscChange,
    QueueOmake,
};

HLib::Task *systemModuleTask = nullptr;
SystemModuleTaskData systemModuleTaskData{};

std::uint32_t nextSceneNumber = 0;
std::int32_t updateTimingMode = 2;
std::int32_t previousUpdateTimingMode = 2;
bool notBusyLoading = false;
bool hasInitialSceneInfo = false;
std::int32_t unusedBootstrapWordA{}, unusedBootstrapWordB{}, unusedBootstrapWordC{};

void FinishDiscChange(void *) {
    ChangeDisc();
    notBusyLoading = true;
    g_bPaused = false;
    ChangeSystemModule(SystemModule::Cold, 0);
}

void InitDiscChangeCallback(void *) {
    g_bPaused = false;
    HLib::SetCurrentTaskTo(FinishDiscChange);
}

void InitDiscChange(void *) {
    updateTimingMode = 1;
    ExecuteAicaCommand(0x100a0U, 0, 0);
    d3t::SetSceneClearColour(0xff000000U);
    InitSceneEffects();
    const shen_name previousMap = g_currentMap;
    g_currentMap = SHEN_NAME(DSCC);
    (void)ReloadSpriteResources();
    g_currentMap = previousMap;
    HLib::SetCurrentTaskTo(InitDiscChangeCallback);
}

void QueueOmake(void *) {
    g_currentMap = SHEN_NAME(OMAK);
    g_actNumber = 4;
    g_entryPoint = 0;
    SetSceneInfos(4, g_currentMap, 0);
    S2Save_CheckData();
    ResetGlobalStateFlags();
    ExecuteAicaCommand(0x1200a0U, 5, 0);
    SetNextSceneNumber(4);
    ChangeSystemModule(SystemModule::DiskChange, 0);
}

void SetSceneFolders(SystemModuleTaskData *) {
    SetSceneFolderReadiness(0);
    InitializeSceneData();
    if (GetIsRunningMovieRenderer()) {
        FreeMovieResources();
        nullsub();
        (void)ProfileFramePerformance();
    }
    InitializeEffectTextureReferences();
    SetSceneNumber();
    InitializeSprites(0, 0);
    ResetLegacySpriteSlotDrawState();
    char directory[256];
    std::snprintf(directory, sizeof(directory), "/SCENE/%02d/STREAM", g_loadedActNumber);
    AllocateDIRE(directory, 1, 216);
    AllocateDIRE("MISC", 1, 100);
    AllocateDIRE("SPRITE", 1, 414);
    AllocateDIRE("MODEL/ITEM", 1, 380);
    std::snprintf(directory, sizeof(directory), "/SCENE/%02d/SOUND", g_loadedActNumber);
    AllocateDIRE(directory, 1, 325);
    std::snprintf(directory, sizeof(directory), "/SCENE/%02d/MODEL/CHARA", g_loadedActNumber);
    AllocateDIRE(directory, 1, 51);
    std::snprintf(directory, sizeof(directory), "/SCENE/%02d/PACK", g_loadedActNumber);
    AllocateDIRE(directory, 1, 137);
    std::snprintf(directory, sizeof(directory), "/SCENE/%02d/NPC", g_loadedActNumber);
    AllocateDIRE(directory, 1, 65);
    std::snprintf(directory, sizeof(directory), "/SCENE/%02d/MODEL/HAND", g_loadedActNumber);
    AllocateDIRE(directory, 1, 186);
    AllocateDIRE("MODEL/PATI", 1, 250);
    std::snprintf(directory, sizeof(directory), "/SCENE/%02d/MODEL/SIGNBD", g_loadedActNumber);
    AllocateDIRE(directory, 1, 45);
    LoadColdArchive();
    SetTempMapIDs();
    nullsub();
    AllocateSTCH();
    LoadSYD_S();
    LoadSYD();
    AllocateAndLoadMOTFiles();
    InitializeWeatherParameters();
    LoadSystemTalk();
    AllocateLCDM();
    nullsub();
    AllocateARMM();
    LoadWeatherConditionData();
    AllocateAudioIndexData();
    AllocateFlagWork(g_actNumber);
    LoadMemoData();
    npcInitData();
    AllocateFONT();
    SetSceneFolderReadiness(1);
    FreeColdArchive();
    HLib::PartitionSceneHeap();
    HLib::ValidateOrResetHeap();
    (void)ResetUnusedReloadState();
    LoadPawnData();
    SetRootRenderDeferredMode(1);
}

void ChangeToTitleControlTask(void *) {
    ChangeSystemModule(SystemModule::Title, 0);
}

void InitShenmue() {
    SetLoadingDrawFlags(0);
    DrawLoadingModes();
    FlushQueuedStripType3Batches();
    (void)ProfileFramePerformance();
    DispatchPostFxFilterRender(0);
    InitializeSoftResetInput();
    g_bPaused = true;
    InitSceneEffects();
    ResetTaskWaitFlags();
    mstsInit();
    InitAica();
    InitializePlayerRuntime();
    scenInitCallbacks();
    unusedBootstrapWordA = 0;
    unusedBootstrapWordB = 0;
    unusedBootstrapWordC = 0;
    SetupNotebook();
#if defined(_WIN32)
    SetupCharacterDebug();
#endif
    InitializePostFxModuleDefaults();
    LoadPostFxFilterFile();
    SetLegacyFogColour(0);
    InitModelVector();
    authInit();
    ResetSoundProgramPhaseFlags(3);
    mtResetOverrideFlag();
    ResetNaviInputPhase();
    gameSysInit();
    RetireLoadingDraw();
}

void ChangeModuleTask_USERFILT(void *taskData) {
    auto *const module = static_cast<SystemModuleTaskData *>(taskData);
    if (!module) {
        return;
    }

    InitShenmue();
    if (module->previousModule != SystemModule::Cold) {
        ChangeSystemModule(SystemModule::Title, 0);
    } else if (!notBusyLoading) {
        ChangeSystemModule(hotLoadModule == SystemModule::Cold ? SystemModule::Title : SystemModule::Logo, 0);
    } else if (!hasInitialSceneInfo) {
        RestoreCurrentSceneInfoFromSave();
        g_bPaused = false;
        ChangeSystemModule(SystemModule::Game, 0);
    } else {
        hasInitialSceneInfo = false;
        ChangeSystemModule(SystemModule::Movie, 0);
    }
    hotLoadModule = SystemModule::Test;
    notBusyLoading = false;
    hasInitialSceneInfo = false;
}

void ChangeModuleTask_Callback(void *taskData) {
    auto *const module = static_cast<SystemModuleTaskData *>(taskData);
    ResetMapsData();
    ResetShadowData();
    d3t::ResetSceneData();
    RetireSharedShadowModel();
    FreeUnusedSceneData();

    if (CheckTextureData())
        ReinitTextureData();
    RetireMazeRuntime();
    RetirePropAssets();
    HLib::FreeUnusedHeapMem();
    ResetBattleQteTaskPointer();
    ClearSTCH();
    AllocateSHDS();
    Read_SYDNSRSSS();
    const std::uint32_t moduleIndex = static_cast<std::uint32_t>(module->currentModule);
    if (moduleIndex > static_cast<std::uint32_t>(SystemModule::SpaceHarrier) || (3392U & (1U << moduleIndex)) == 0)
        shenSetForcedFilmbars(1);
    ModuleCallback callback = moduleCallbacks[moduleIndex];
    if (callback)
        HLib::SetCurrentTaskTo(callback);
}
} // namespace
void ChangeDisc() {
    ResetMapsData();
    ResetShadowData();


    ReleaseMapControlOwners();
    RetireMazeRuntime();
    RetirePropAssets();
    HLib::InitializeAllocator();
    ResetSceneArchiveFileHandles();
}

void ResetSceneArchiveFileHandles() {


    for (auto &file : d3t::g_files)
        d3t::ReadFile_0(0, &file.handle);
}

void InitializeScene() {
    ResetSelectedInputDevice();
    d3t::SetSceneClearColour(0xff000000U);
    nullsub();
    notBusyLoading = false;
    hasInitialSceneInfo = false;
    nullsub(0);
    g_currentMap = shen_name{};
    ResetLoadingDrawState();
    g_actNumber = 1;
    g_entryPoint = 0;
    g_loadedActNumber = 1;
    SetNextSceneNumber(1);
    hotLoadModule = SystemModule::Cold;
    SetPlayerControlEnabled(0);
    if (GetIsRunningMovieRenderer())
        g_titleFlagShouldRun = 0;
    ChangeDisc();
    ChangeSystemModule(SystemModule::Cold, 0);
}

// @verified - p: ResetToTitleScreen.

void ResetToTitleScreen() {
    CloseVisibleUIElements();
    DisableHelpIcon();
    ResetPressStartLogoImages();
    ResetGameMenuActive();
    ResetSelectedInputDevice();
    d3t::SetSceneClearColour(0xff000000U);
    notBusyLoading = false;
    hasInitialSceneInfo = false;
    nullsub(0);
    g_currentMap = shen_name{};
    ResetLoadingDrawState();
    g_actNumber = 1;
    g_entryPoint = 0;
    g_loadedActNumber = 1;
    SetNextSceneNumber(1);
    SetPlayerControlEnabled(0);
    if (GetIsRunningMovieRenderer())
        g_titleFlagShouldRun = 0;
    ChangeDisc();
    ChangeSystemModule(SystemModule::Cold, 0);
    ForceInputDeviceDisconnected(0);
    RequestGameArcadeReturnToTitle();
    RetireGameHangOn();
}
void ChangeModuleTask_Init(void *taskData) {
    SetSceneFolders(static_cast<SystemModuleTaskData *>(taskData));
    ChangeSystemModule(SystemModule::Hot, 0);
}
void SetNextSceneNumber(std::uint32_t sceneNumber) {
    nextSceneNumber = sceneNumber;
}

// @verified - p
std::uint64_t SetSceneNumber() {
    g_loadedActNumber = static_cast<std::int32_t>(nextSceneNumber);
    return nextSceneNumber;
}

void SetFrontendPlatformAdapters(const FrontendPlatformAdapters &adapters) {
    frontendPlatformAdapters = adapters;
    frontendPlatformAdapters.continueSaveAvailable = S2Save_HasContinueSlot;
    frontendPlatformAdapters.newGameTask = S2Save_NewGameTitleTask;
    frontendPlatformAdapters.continueSaveTask = S2Save_ContinueTitleTask;
    frontendPlatformAdapters.titleLoadTask = S2Save_TitleLoadTask;
    SetContinueAvailability(ContinueModuleIsAvailable());
}

std::uint16_t FrontendControllerUserHandle() {
    if (frontendPlatformAdapters.controllerUserHandle)
        return frontendPlatformAdapters.controllerUserHandle();
    return g_inputBuffer[0].connectionState;
}

void RequestFrontendQuit() {
    frontendQuitRequested = true;
    if (frontendPlatformAdapters.requestQuit)
        frontendPlatformAdapters.requestQuit();
}

bool FrontendQuitRequested() {
    return frontendQuitRequested;
}

void ClearFrontendQuitRequested() {
    frontendQuitRequested = false;
}

bool SystemModuleIsAvailable(SystemModule module) {
    const std::size_t index = static_cast<std::size_t>(module);
    if (index >= moduleCallbacks.size() || !moduleCallbacks[index]) {
        return false;
    }
    switch (module) {
    case SystemModule::OutRun:
        return GameArcadeModuleAvailable(EmulatorGame::OutRun);
    case SystemModule::AfterBurner:
        return GameArcadeModuleAvailable(EmulatorGame::AfterBurner);
    case SystemModule::HangOn:
        return GameArcadeModuleAvailable(EmulatorGame::HangOn);
    case SystemModule::SpaceHarrier:
        return GameArcadeModuleAvailable(EmulatorGame::SpaceHarrier);
    default:
        return true;
    }
}

bool ExecuteSystemModule(SystemModule module, std::int32_t entry) {
    const std::size_t index = static_cast<std::size_t>(module);
    if (index >= moduleCallbacks.size() || !moduleCallbacks[index]) {
        return false;
    }
    systemModuleTask = HLib::EnqueueTaskWithoutParameter(moduleCallbacks[index], 0, 2, static_cast<std::int32_t>(TASK_MODU.name));
    if (!systemModuleTask) {
        return false;
    }
    systemModuleTaskData.work = systemModuleTaskData.workStorage;
    HLib::AssignTaskDataToTask(systemModuleTask, &systemModuleTaskData);
    systemModuleTaskData.currentModule = module;
    systemModuleTaskData.currentEntry = entry;
    SetNextSceneNumber(1);
    SystemModuleExecNullsub();
    return true;
}

bool ChangeSystemModule(SystemModule module, std::int32_t entry) {
    if (static_cast<std::size_t>(module) >= moduleCallbacks.size() || !systemModuleTask) {
        return false;
    }
    if (module == SystemModule::Game)
        SetProjectionOff();
    DispatchPostFxFilterRender(0);
    SelectPostFxBank(1, 0);
    SelectPostFxBank(3, 0);
    ReleaseMapControlOwners();
    HLib::UnpauseTaskEnqueue();
    HLib::AssignCallbackForTask(systemModuleTask, ChangeModuleTask_Callback);
    HLib::SetupTimedTask(systemModuleTask, 1);
    updateTimingMode = previousUpdateTimingMode;
    systemModuleTaskData.previousModule = systemModuleTaskData.currentModule;
    systemModuleTaskData.previousEntry = systemModuleTaskData.currentEntry;
    systemModuleTaskData.currentModule = module;
    systemModuleTaskData.currentEntry = entry;
    HLib::DestroyTasksFromIndex(1);
    g_loadingFlag = 0;
    return true;
}

bool BeginNewGameModuleHandoff() {
    if (frontendPlatformAdapters.newGameTask) {
        return BeginTitlePlatformTask(frontendPlatformAdapters.newGameTask);
    }
    g_actNumber = 1;
    g_currentMap = SHEN_NAME(0004);
    g_entryPoint = 0;
    return ChangeSystemModule(SystemModule::Game, 0);
}

bool BeginContinueModuleHandoff() {
    if (!ContinueModuleIsAvailable()) {
        return false;
    }
    return BeginTitlePlatformTask(frontendPlatformAdapters.continueSaveTask);
}

bool BeginTitleLoadModuleHandoff() {
    if (!frontendPlatformAdapters.titleLoadTask) {
        return false;
    }
    return BeginTitlePlatformTask(frontendPlatformAdapters.titleLoadTask);
}

bool BeginSeventyManModuleHandoff() {
    if (!frontendPlatformAdapters.seventyManTask) {
        return false;
    }
    return BeginTitlePlatformTask(frontendPlatformAdapters.seventyManTask);
}

bool BeginOptionsModuleHandoff() {
    return BeginTitleOptionsTask();
}

bool ReturnToTitleMenuHandoff() {
    return ReturnToTitleMenuTask();
}

bool ContinueModuleIsAvailable() {
    return frontendPlatformAdapters.continueSaveTask && frontendPlatformAdapters.continueSaveAvailable && frontendPlatformAdapters.continueSaveAvailable();
}

} // namespace shenmue::s2
