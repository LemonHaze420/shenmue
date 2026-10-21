#include "s1/task/disp.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/boot.hpp"
#include "s1/input.hpp"
#include "s1/date_time.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/player_stats.hpp"
#include "s1/scene.hpp"
#include "s1/task/cdtt.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/edtk.hpp"
#include "s1/task/hplg.hpp"
#include "s1/task/mitk.hpp"
#include "s1/task/save.hpp"
#include "s1/save_format.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/time.hpp"
#include "s1/task/tmlt.hpp"
#include "s1/task/updb.hpp"
#include "s1/task/warn.hpp"
#include "s1/task/wexe.hpp"
#include "s1/timing.hpp"
#include "s1/texture_loader.hpp"
#include "s1/d3t/d3t_texture.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {

void HLTaskFunc_InternalInit();
std::int64_t krepEnqueue(std::uint32_t index);
HLib::Task *FILM_Init(std::uint16_t nextFunctionIndex);
std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t argument, std::int32_t parameter);
extern std::int32_t g_actNumber;
void *LoadGzippedSpriteIntoHeapMem(const char *filename, std::int32_t allocationToken, std::uint32_t *decompressedSize);

std::int32_t g_bIsNewGame = 0;
std::uint8_t g_displayActNumber = 0;
std::int32_t g_mainMenuOpened = 0;
std::int32_t g_returnToPressStart = 0;
std::uint8_t g_hasPickedSaveData = 0;
std::int32_t g_numRemainLoadAttempts = 0;
std::int32_t g_is70ManBattleLoad = 0;
std::int32_t g_pickedSaveResumeKind = 0;
std::int32_t g_settingFlagsCaptured = 0;
std::int32_t g_70ManBattleLoadBeforeSettings = 0;

namespace {
#pragma pack(push, 1)
struct DisplayTaskData {
    std::byte reserved_000[0x1dc];
    std::uint32_t flags;
    std::byte reserved_1e0[0x20];
    std::uint32_t colour;
    std::byte reserved_204[0x0c];
    std::uint32_t field_210;
    std::byte reserved_214[0x0c];
    std::uint64_t field_220;
    float scaleX;
    float scaleY;
    std::uint32_t field_230;
};
#pragma pack(pop)

static_assert(sizeof(DisplayTaskData) == 0x234);
static_assert(offsetof(DisplayTaskData, flags) == 0x1dc);
static_assert(offsetof(DisplayTaskData, colour) == 0x200);

DisplayTaskData *g_displayTaskData = nullptr;
std::int32_t g_displayFadeOverrideActive = 0;
std::int32_t g_displayState = 1;
std::array<std::int32_t, 3> g_displayTextureResources{};
float g_screenDisplayAlpha = 0.0f;
std::int32_t g_bookkeepingRefreshLoaded = 0;

const std::byte *g_loadSummaryCache = nullptr;
std::int16_t g_loadSummaryBlock = 0;
std::int16_t g_load70ManBattleScene = 0;
std::uint8_t g_bLoad70MB = 0;

const std::byte *g_bookkeepingCache = nullptr;

void MainMenu_OnBookkeepingCacheRefreshed(const std::byte *cache) {
    g_bookkeepingCache = cache;
    if (!cache) {
        g_bookkeepingRefreshLoaded = 0;
        return;
    }
    std::uint32_t loaded = 0;
    std::memcpy(&loaded, cache + 0x1a84, sizeof(loaded));
    g_bookkeepingRefreshLoaded = loaded != 0;
}

const S1SaveSummary *BookkeepingSummary(const std::byte *cache, std::int32_t slot) {
    return cache ? reinterpret_cast<const S1SaveSummary *>(cache + 0x1804 + 0x40 * slot) : nullptr;
}

void SetIntroSceneInfoCallback(HLib::TaskData *) {
    g_sceneId = 1;
    g_areaId = SHEN_NAME(OP02);
    g_entryPoint = 1;
    UIBaseRuntime &pressStart = g_uiElements[0x3c];
    if ((g_currUIState == -1 || g_currUIState == 0x3c) && (!pressStart.isVisible || !pressStart.destroy || pressStart.destroy() == 1 || g_busy_reset == 1)) {
        pressStart.isVisible = 0;
    }
    shenExecuteAICACommand(0x1200a0, 2, 0);
    HLib_UpdateTaskQueue(SystemModule::Map);
}

void SetPickSaveFileCallback();
void SetPausedAndInit();



void SetSettingFlags() {
    g_settingFlagsCaptured = 1;
    g_bookkeepingRefreshUpdatesDisabled = 1;
}

void SetIntroSceneInfo(HLib::TaskData *) {
    bool mainMenuOpened = false;
    if (g_mainMenuOpened == 1) {
        mainMenuOpened = true;
        g_mainMenuOpened = 0;
    } else if (g_currentSplashScreenIndex >= 6) {
        TryInvokeRegisteredCallback(0x3c);
    }
    if (g_bIsNewGame) {
        g_bIsNewGame = 0;
        if (CurrentTask) {
            CurrentTask->initCallbackFnPtr = SetIntroSceneInfoCallback;
            CurrentTask->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
            CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
            CurrentTask->mainExecutionIterations = 30;
        }
    } else if (mainMenuOpened) {
        SetSettingFlags();
        SetPickSaveFileCallback();
    }
}

void Shenmue_LoadIntro(HLib::TaskData *) {
    if (--g_numRemainLoadAttempts <= 0) {
        g_numRemainLoadAttempts = 0;
        LoadIntro();
    }
}


void PickSaveFile_callbackFn(HLib::TaskData *) {
    if (g_returnToPressStart) {
        SetPausedAndInit();
        return;
    }
    if (g_loadRequested == 1) {
        g_loadRequested = 0;
        const S1SaveSummary *const summary = BookkeepingSummary(g_bookkeepingCache, g_currentSlotIndex);
        if (g_is70ManBattleLoad != 2) {
            g_is70ManBattleLoad = 1;
            g_pickedSaveResumeKind = (summary && summary->saveState == 3) + 1;
        }
        std::snprintf(g_currentSaveSlot.data(), g_currentSaveSlot.size(), "SHENMUE1_00%d", g_currentSlotIndex);
        LoadSceneForSaveFile(summary ? summary->actNumber : 0);
    }
    if (g_hasPickedSaveData) {
        g_hasPickedSaveData = 0;
        g_numRemainLoadAttempts = 2;
        if (CurrentTask) {
            CurrentTask->initCallbackFnPtr = nullptr;
            CurrentTask->mainTaskCallbackFnPtr = Shenmue_LoadIntro;
            CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
        }
        d3t::ResetElementByIdx(4);
    } else if (TryInvokeRegisteredCallback(4)) {
        g_pickedSaveResumeKind = 0;
    }
}


void SetPickSaveFileCallback() {
    g_screenDisplayAlpha = 0.0f;
    g_displayState = -1;
    g_displayTextureResources[0] = 0;
    g_displayTextureResources[1] = 0;
    g_displayTextureResources[2] = 0;
    g_displayFadeOverrideActive = 1;
    g_isOnMainMenu = 0;
    if (CurrentTask) {
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->mainTaskCallbackFnPtr = PickSaveFile_callbackFn;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}


void SetPausedAndInit() {
    g_screenDisplayAlpha = 0.0f;
    g_returnToPressStart = 0;
    g_displayFadeOverrideActive = 0;
    g_isOnMainMenu = 1;
    g_displayState = 2;
    g_displayTextureResources[0] = 0;
    g_displayTextureResources[1] = 0;
    g_displayTextureResources[2] = 0;
    if (CurrentTask) {
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->mainTaskCallbackFnPtr = SetIntroSceneInfo;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}

void DISP_Callback(HLib::TaskData *taskData) {
    auto *const display = reinterpret_cast<DisplayTaskData *>(taskData);
    if (!display || (!g_displayFadeOverrideActive && g_currentSplashScreenIndex < 6))
        return;

    display->flags = 0x400;
    display->field_210 = 0;
    display->field_220 = 0;
    display->colour = 0xffffffff;
    display->scaleX = 1.0f;
    display->scaleY = 1.0f;
    display->field_230 = 0;

    float alpha = g_screenDisplayAlpha;
    if (g_displayFadeOverrideActive && g_displayState != 2) {
        alpha += g_bEnableFrameTimeSmoothing ? fFrameTimeMs * 120.0f : 4.0f;
        if (alpha > 255.0f) {
            alpha = 255.0f;
        }
        g_screenDisplayAlpha = alpha;
    }
    if (g_displayState != 2) {
        display->colour = (static_cast<std::uint32_t>(alpha) << 24) | 0x00ffffff;
    }
    if (g_displayState == 6 || g_displayState <= 2 || g_displayState >= 9) {
        g_ClearColour = 0xff000000;
    } else {
        g_ClearColour = 0xffffffff;
    }
}
} // namespace

std::int32_t g_isOnMainMenu = 0;



void InternalInit() {
    g_displayActNumber = 0;
    CDTT_SetMode(CDTTMode::GameplayDateSetup);

    const std::uint8_t hours = g_sceneId == 80 ? 12 : 8;
    const std::uint8_t minutes = g_sceneId == 80 ? 0 : 30;
    g_currentDateTime = {86, 12, 3, 0, hours, minutes, 0, 0};
    RecalculateDayInTheWeek(&g_currentDateTime);
    g_maximumDateTime = g_currentDateTime;
    TIME_ResetCycleTime();
    SCEN_SetCurrentScenarioStep(0);
    RequestStep(0);
    g_MITK_Task = nullptr;
    g_pEDTK_Task = nullptr;
    g_TMLT_Task = nullptr;
    CDTT_ResetPublishedState();
}

void HLTaskFunc_InternalWeatherInit(std::int32_t mode) {
    const std::uint32_t dialogueMode = static_cast<std::uint32_t>(std::clamp(g_applicationSettings.savedDialogueMode, 0, 3));

    void *const bootRomScratch = HLib_AllocMemBlock(0x4000, SHEN_NAME(BROM));
    const bool soundEnabled = bootRomScratch != nullptr;
    if (bootRomScratch) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(bootRomScratch), 0);
    }
    shenExecuteAICACommand(soundEnabled ? 0x900a0U : 0x800a0U, 0, 0);
    SOND_ExecuteRawAicaCommand(static_cast<std::uint32_t>(((soundEnabled ? 14 : 15) << 16) + 0x1ba0));

    std::uint32_t flags = S1SettingsFlags_Get();
    if (mode != 0) {
        flags = (flags & 0xffffffcbU) | (dialogueMode << 4U);
        if (g_dialogueModeVoiceTextEnable[dialogueMode][0] != 0)
            flags |= 2U;
        else
            flags &= ~2U;

        if (g_dialogueModeVoiceTextEnable[dialogueMode][1] != 0)
            flags |= 1U;
        else
            flags &= ~1U;

        if (g_applicationSettings.savedInput.swapTriggers == 0 || g_keyboardActive == 1) {
            flags |= 0x40U;
        } else {
            flags &= ~0x40U;
        }
        if (g_applicationSettings.savedInput.weatherMode != 0)
            flags |= 0x80U;
        else
            flags &= ~0x80U;
    }
    if (HPLG_GetBookkeepingProgress() == 0)
        flags &= ~0x80U;
    S1SettingsFlags_Set(flags);
    Weather_SetWeatherDataByMode(static_cast<std::int32_t>((flags >> 7U) & 1U));
}


std::int64_t LoadIntro() {
    shenSetupPlayerDefaultsNewGame();
    g_sceneId = 1;
    g_warningMessageIndex = 0;
    g_warningChoiceOrDisc = 0;
    g_entryPoint = 0;
    g_areaId = SHEN_NAME(OP00);
    HLTaskFunc_InternalInit();
    HLTaskFunc_InternalWeatherInit(1);
    g_isOnMainMenu = 0;
    shenExecuteAICACommand(0x1200a0U, 2, 0);
    return HLib_UpdateTaskQueue(SystemModule::Map);
}


void GetSaveSlotString() {
    g_ClearColour = 0xff000000;
    std::int32_t slot = 0;
    while (slot < 10) {
        const S1SaveSummary *const summary = BookkeepingSummary(g_bookkeepingCache, slot);
        if (summary && summary->numberOfSaves)
            break;
        ++slot;
    }
    if (slot >= 10) {
        std::snprintf(g_currentSaveSlot.data(), g_currentSaveSlot.size(), "%s", "SHENMUE1_000");
        InitSaveData();
        g_hasPickedSaveData = 1;
        return;
    }
    std::snprintf(g_currentSaveSlot.data(), g_currentSaveSlot.size(), "SHENMUE1_00%d", slot);
    g_hasPickedSaveData = 1;
}


void HLTaskFunc_Reinitialize(HLib::TaskData *) {
    if (g_saveDisplayModuleState != SAVE_DISPLAY_MODULE_NONE)
        return;
    HLTaskFunc_InternalInit();
    shenExecuteAICACommand(0x1200a0U, 2, 0);
    HLTaskFunc_InternalWeatherInit(0);
    HLib_UpdateTaskQueue(SystemModule::Map);
}


std::int64_t LoadSceneFromID(std::int32_t sceneId, std::uint32_t areaId, std::uint32_t entryId, std::int32_t block) {
    const std::int64_t result = LoadScene(block);
    if (!result)
        return result;
    CAME_ClearSequenceActive();
    g_isRenderSuppressed = 1;
    FlushRenderQueue();
    if (g_load70ManBattleScene == 1 && g_actNumber == 3) {
        g_sceneId = sceneId;
        g_areaId = areaId;
        g_entryPoint = entryId;
    }
    g_isOnMainMenu = 0;
    if (block) {
        HLTaskFunc_InternalInit();
        shenExecuteAICACommand(0x1200a0U, 2, 0);
        HLTaskFunc_InternalWeatherInit(0);
        HLib_UpdateTaskQueue(SystemModule::Map);
    } else {
        SAVE_OpenDisplayModule(SAVE_DISPLAY_MODULE_VARIANT8);
        if (CurrentTask) {
            CurrentTask->initCallbackFnPtr = nullptr;
            CurrentTask->mainTaskCallbackFnPtr = HLTaskFunc_Reinitialize;
            CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
        }
    }
    return 1;
}



std::int64_t shenLoadSaveDataForNormalOr70MB() {
    g_numRemainLoadAttempts = 0;
    if (g_bLoad70MB)
        return LoadSceneFromID(3, SHEN_NAME(MFBT), 99, g_loadSummaryBlock + 1);
    const S1SaveSummary *const summary = BookkeepingSummary(g_loadSummaryCache, g_loadSummaryBlock + 1);
    return LoadSceneFromID(
        summary ? summary->sceneId : 0, summary ? summary->areaId : 0, summary ? static_cast<std::uint32_t>(summary->entryId) : 0, g_loadSummaryBlock + 1);
}


void LoadSceneForSaveFile(std::int32_t actNumber) {
    g_loadSummaryCache = g_bookkeepingCache;
    g_loadSummaryBlock = 0;
    g_load70ManBattleScene = 0;
    g_numRemainLoadAttempts = 2;
    if ((g_is70ManBattleLoad & 0xffff) == 2) {
        g_is70ManBattleLoad &= ~0xffff;
        g_load70ManBattleScene = 1;
        g_bLoad70MB = 1;
    } else {
        g_actNumber = actNumber;
        g_bLoad70MB = 0;
    }
    shenLoadSaveDataForNormalOr70MB();
}

void Script_cdttUtil(ScenarioContext *, void *rawParams) {
    if (!rawParams)
        return;
    CDTT_SetMode(static_cast<CDTTMode>(*static_cast<const std::int32_t *>(rawParams)));
}

void Script_CtrlTimeLimit(ScenarioContext *context, void *rawParams) {
    const TMLTControlRequest *const params = static_cast<const TMLTControlRequest *>(rawParams);
    switch (params->mode) {
    case TMLTControlMode::Init:
        InitTMLT(0);
        break;
    case TMLTControlMode::Update: {
        std::int32_t time = params->seconds;
        g_BeepIntervalFrames = 30;
        g_BeepFrameCounter = 30;
        g_tmlt_Active = 1;
        if (time) {
            if (time >= 86400)
                time = 86400;
            g_curr_time_left = time;
            g_FrameCounter30 = 0;
        }
        break;
    }
    case TMLTControlMode::Disable:
        g_tmlt_Active = 0;
        break;
    case TMLTControlMode::Destroy:
        FreeTMLT();
        break;
    case TMLTControlMode::GetTimeLeft:
        context->stack->accumulator = static_cast<std::uint32_t>(g_curr_time_left);
        break;
    case TMLTControlMode::InitAndReset:
        g_tmp_tmlt_textures_initialised = 1;
        g_TMLT_Task = HLib_EnqueueTaskWithoutParameter(TMLT_Callback, 6, 0, static_cast<std::int32_t>(TASK_TMLT.name));
        g_tmlt_Active = 0;
        g_FrameCounter30 = 0;
        g_BeepIntervalFrames = 30;
        g_BeepFrameCounter = 30;
        break;
    }
}

void DISP_Module(HLib::TaskData *taskData) {
    g_actNumber = 1;
    g_isOnMainMenu = 1;
    for (std::int32_t &texture : g_displayTextureResources) {
        if (texture != 0 && texture != -1)
            InitTexture2DResource(static_cast<std::uint32_t>(texture));
        texture = 0;
    }
    g_displayTaskData = nullptr;
    HLTaskFunc_InternalInit();
    InitSceneDataWrapper();
    for (std::uint32_t index = 0; index < 65536; ++index) {
        if (!g_Scene.entries[index])
            continue;
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(g_Scene.entries[index]), SHEN_NAME(PAKM));
        FreeSceneSpace(index);
    }
    shenExecuteAICACommand(0x100a0, 0, 0);
    HLib::Task *const displayTask = HLib_EnqueueTaskWithoutParameter(DISP_Callback, 8, 0, SHEN_NAME(DISP));
    if (displayTask && !displayTask->task_data_ptr) {
        auto *const display = static_cast<DisplayTaskData *>(HLib_AllocHeapMemBlock(sizeof(DisplayTaskData), SHEN_NAME(TASK)));
        if (display) {
            std::memset(display, 0, sizeof(*display));
            HLib_EnableTokenCheck(display);
            displayTask->task_data_ptr = reinterpret_cast<HLib::TaskData *>(display);
            g_displayTaskData = display;
        }
    }
    if (displayTask && !g_displayTaskData) {
        displayTask->initCallbackFnPtr = nullptr;
        displayTask->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        displayTask->autoInitTaskCallbackFnPtr = nullptr;
    }

    auto *const module = reinterpret_cast<ModuleTaskData *>(taskData);
    if (module && module->data) {
        module->data->data[0] = 300;
        module->data->data[1] = reinterpret_cast<std::int64_t>(FILM_Init(15));
    }
    g_displayFadeOverrideActive = 0;
    g_numRemainLoadAttempts = 0;
    g_screenDisplayAlpha = 0.0f;
    g_displayActNumber = 0;
    CDTT_SetMode(CDTTMode::TitleOrIntroDateSetup);
    InternalInit();

    std::uint32_t titleSpriteSize = 0;
    void *const titleSprite = LoadGzippedSpriteIntoHeapMem("TITLE4_A.GZ", SHEN_NAME(TEMP), &titleSpriteSize);
    if (titleSprite && g_displayTaskData) {
        auto *const bytes = static_cast<std::byte *>(titleSprite);
        std::size_t offset = 0;
        std::size_t textureIndex = 0;
        auto *const textureHandles = reinterpret_cast<std::int32_t *>(g_displayTaskData);
        while (offset + 8 <= titleSpriteSize && textureIndex < 0x77) {
            std::uint32_t tag = 0;
            std::uint32_t nextOffset = 0;
            std::memcpy(&tag, bytes + offset, sizeof(tag));
            std::memcpy(&nextOffset, bytes + offset + 4, sizeof(nextOffset));
            if (tag != SHEN_NAME(TEXN) || nextOffset < 8 || nextOffset > titleSpriteSize - offset) {
                break;
            }
            const std::int32_t texture = LoadTEXN(bytes + offset);
            textureHandles[textureIndex] = texture;
            ++textureIndex;
            offset += nextOffset;
        }
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(titleSprite), 0);
    } else if (titleSprite) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(titleSprite), 0);
    }

    bupHotPlugTaskStart();
    HPLG_RegisterBookkeepingListener(MainMenu_OnBookkeepingCacheRefreshed);

    const SystemModule previous = module ? module->previousModule : SystemModule::Audio;
    if (previous == SystemModule::Audio) {
        g_displayState = 1;
    } else if (previous == SystemModule::SetToDisplay) {
        g_displayState = g_bookkeepingRefreshLoaded ? 1 : 9;
        if (g_currentSplashScreenIndex < 6)
            d3t::ActivateElementByIdx(0x3b);
    } else {
        if (g_entryPoint == 49) {
            g_displayState = 1;
        } else if (g_displayState == 6) {
            g_displayState = 7;
        } else {
            g_displayState = 1;
        }
        d3t::ActivateElementByIdx(0x3c);
    }

    krepEnqueue(0);
    krepEnqueue(4);
    if (CurrentTask) {
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->mainTaskCallbackFnPtr = SetIntroSceneInfo;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
    g_ClearColour = 0xff000000;
    SAVE_Init(1);
}

std::int32_t DISP_GetTitleState() noexcept {
    return g_displayState;
}

bool DISP_IsOnMainMenu() noexcept {
    return g_isOnMainMenu != 0;
}

} // namespace shenmue::s1
