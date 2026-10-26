#include "s1/boot.hpp"

#include "s1/archive_data.hpp"
#include "s1/audio_stream.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_content.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "d3t/shader_load.hpp"
#include "s1/d3t/d3t_render_handles.hpp"

#include "s1/mt5_render.hpp"
#include "s1/task/effe.hpp"
#include "s1/file_state.hpp"
#include "s1/file_loader.hpp"

#include "s1/frame_pipeline.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/input.hpp"
#include "s1/initialization.hpp"
#include "s1/face.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/scen.hpp"
#include "s1/date_time.hpp"
#include "s1/lighting.hpp"
#include "s1/modern_lighting.hpp"
#include "s1/task/lght.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mip_streaming.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/render_array.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/cdtt.hpp"
#include "s1/task/film.hpp"
#include "s1/task/scrl.hpp"
#include "s1/player_stats.hpp"
#include "s1/save_format.hpp"
#include "s1/task/save.hpp"
#include "s1/task/sond.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/render_passes.hpp"
#include "s1/shader_initialization.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/timing.hpp"

#include <array>
#include <atomic>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <json/json.h>

namespace shenmue::s1 {

std::uint8_t g_hmdlEntryDirty = 0;
EffectProbeSlot g_effectProbeSlots[32]{};

std::int32_t g_updateRenderFrameMode = -1;
std::uint8_t g_bDebugLog = 1;

struct LegacyScalarFactor {
    std::uint32_t angle;
    float value;
};

static_assert(sizeof(LegacyScalarFactor) == 8);

using GenericFunction = void (*)();
using UpdateTimerFunction = void (*)();
using RenderPipelineFunction = void (*)(void *, void *);
using IsValidCEWPFunction = std::int64_t (*)(void *);

void SGAAS_Init(std::uint64_t steamId);
char *GetBuffer();
void UpdateTimer();
void DispatchRenderPipeline(void *renderContext, void *frameInfo);
std::int64_t isValidCEWP(void *model);
void AllocMemory();
void Audio_n_Init();
std::int64_t InitEffectBuffers();
void InitSceneDataMemory_0();
void SetupEffeTransform();
void InitializeD3TSettings();
void ReadConfigFromSave();
void SettingsFileParser();
void FontLoader();
void SetVolumeForAicaBank(float volume, std::uint32_t bank);

void *g_renderIndexStorageA = nullptr;

std::uint32_t g_bPauseGameTasks = 0;
std::int32_t g_framesPerSecond = 0;
std::uint8_t g_debugUnknown2 = 0;
std::uint8_t g_debugUnknown3 = 0;
std::uint8_t g_debugFaceUnknown = 0;

std::uint8_t g_forceDisplayCursor = 0;
std::uint8_t g_bSkipCycles = 0;
void (*g_audioInitCallback)() = nullptr;

namespace {

char *(*fnptr_GetBuffer)() = nullptr;
IsValidCEWPFunction g_isValidCEWP = nullptr;

std::int32_t g_unusedFlag0 = 0;
std::int32_t Shenmue_FramesPast_Copy = 0;
std::int32_t FramesPast = 0;
std::int32_t PreviousFramesPast = 0;

alignas(32) std::array<LegacyScalarFactor, 193> g_legacyScalarStorage{};
LegacyScalarFactor *g_roLegacyScalarFactors = nullptr;
alignas(32) LightStorage g_defaultLightStorage{};
alignas(32) std::array<std::byte, 5 * 0x820> g_spriteBankStorage{};
std::byte *g_spriteBankBase = nullptr;
std::byte *g_spriteBanks[5]{};
std::int32_t g_bSpriteBankInit = 0;

alignas(32) std::array<Matrix4x4, 65> g_defaultMatrices{{
    Matrix4x4{{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }},
}};

std::array<std::byte, 0x120> g_previousInputData{};

char g_emptyString = '\0';
} // namespace

void *g_renderObjectStorage = nullptr;
std::uint8_t g_frameWaitEnabled[2]{};
float g_deltaTime = 0.0f;
void *g_renderLookupStorage = nullptr;
void *g_renderPacketStorage = nullptr;
void *g_renderIndexStorageB = nullptr;
const std::array<RenderPipelinePreset, 8> g_inputLayoutTypes{{
    {.vertexShaderAndLayoutIndex = 23, .pixelShaderIndex = 7, .vertexStride = 40},
    {.vertexShaderAndLayoutIndex = 24, .pixelShaderIndex = 8, .vertexStride = 28},
    {.vertexShaderAndLayoutIndex = 25, .pixelShaderIndex = 9, .vertexStride = 32},
    {.vertexShaderAndLayoutIndex = 26, .pixelShaderIndex = 10, .vertexStride = 16},
    {.vertexShaderAndLayoutIndex = 27, .pixelShaderIndex = 11, .vertexStride = 24},
    {.vertexShaderAndLayoutIndex = 28, .pixelShaderIndex = 12, .vertexStride = 28},
    {.vertexShaderAndLayoutIndex = 1, .pixelShaderIndex = 3, .vertexStride = 0},
    {.vertexShaderAndLayoutIndex = 0, .pixelShaderIndex = 2, .vertexStride = 3},
}};
std::int32_t g_renderVertexShaderAndLayoutIndex = 0;
std::int32_t g_renderPixelShaderIndex = 0;
std::int32_t g_renderVertexStride = 0;
std::int32_t g_renderPassIndex = 0;

std::int32_t g_requestedUiLanguage = 0;
namespace {

alignas(16) std::array<std::byte, 0x58> g_effectInitializationState{};
std::int32_t g_effectFlagA = 0;
std::int32_t g_effectFlagB = 0;
Vector3 g_effectScaleA{};
Vector3 g_effectScaleB{};

constexpr std::uint32_t kOriginalScreenHeight = 480;
constexpr std::uint32_t kOriginalScreenWidth = 640;

std::int32_t g_pendingThreadLockValid = 0;
std::int32_t g_pendingThreadLock = 0;

float g_timeMultiplier1 = 0.0f;
float g_defaultBlendFactor = 0.0f;
std::uint8_t g_loadingWidgetEnabled[2]{};
float g_defaultInputFactor = 0.0f;
std::uint8_t g_unusedDebugTimeFlag = 0;
std::uint64_t g_debugPointerState = 0;
std::int32_t g_debugEnableFlags[11]{};
std::uint64_t g_debugClearedState[4]{};

} // namespace

std::uint8_t g_debugUiDisabled = 0;
std::uint8_t g_debugUnknown0 = 0;
EyeRigParameters g_eyeRigParameters{};
float g_blinkDeltaStepTime = 0.0f;

namespace {

std::int32_t g_debugState4 = 0;
Vector3 g_debugScale{};

void storeZero(std::size_t offset, std::size_t size) {
    auto *const bytes = reinterpret_cast<std::byte *>(&g_inputState);
    std::memset(bytes + offset, 0, size);
}

} // namespace
std::uint32_t GetDebugLogFlag() {
    return g_bDebugLog;
}

std::uint8_t GetSkipCyclesFlag() {
    return g_bSkipCycles;
}

std::uint32_t GetFramesPast() {
    return static_cast<std::uint32_t>(FramesPast);
}

std::uint32_t GetFramesPastCopy() {
    return static_cast<std::uint32_t>(Shenmue_FramesPast_Copy);
}
void UpdateRootFrameCounters() {
    const std::uint32_t next = static_cast<std::uint32_t>(Shenmue_nNumRenderedFrames) - 1U + static_cast<std::uint32_t>(PreviousFramesPast);
    if (next > static_cast<std::uint32_t>(FramesPast))
        FramesPast = std::bit_cast<std::int32_t>(next);
    PreviousFramesPast = FramesPast;
}

void IncrementRootFrameCopy() {
    ++Shenmue_FramesPast_Copy;
}

UpdateTimerFunction fnptr_UpdateTimer = nullptr;
RenderPipelineFunction fnptr_DispatchRenderPipeline = nullptr;
void (*ExecuteWeatherTaskQueue)() = nullptr;

char *GetBuffer() {
    return &g_emptyString;
}

void AllocMemory() {
    g_renderObjectStorage = ::operator new(374792);
    auto *const objects = static_cast<std::byte *>(g_renderObjectStorage);
    for (std::size_t index = 0; index < 1024; ++index) {
        std::memset(objects + index * 360, 0, 360);
        std::memset(objects + 368640 + index * sizeof(std::uint32_t), 0, sizeof(std::uint32_t));
        std::memset(objects + 372736 + index * sizeof(std::uint16_t), 0, sizeof(std::uint16_t));
    }
    std::memset(objects + 374784, 0, sizeof(std::uint32_t));

    g_renderLookupStorage = ::operator new(10244);
    std::memset(g_renderLookupStorage, 0, 10244);

    g_renderPacketStorage = ::operator new(35123208);
    auto *const packets = static_cast<std::byte *>(g_renderPacketStorage);
    for (std::size_t index = 0; index < 1024; ++index) {
        std::byte *const record = packets + index * 34296;
        std::memset(record + 0x18, 0, sizeof(std::uint32_t));
        constexpr std::array<std::size_t, 10> fields{
            0x78,
            0x80,
            0x88,
            0xac,
            0xb4,
            0xbc,
            0x85cc,
            0x85d4,
            0x85dc,
            0x85e4,
        };
        for (const std::size_t offset : fields) {
            std::memset(record + offset, 0, sizeof(std::uint64_t));
        }
    }
    std::memset(packets + 35119104, 0, 4096);
    std::memset(packets + 35123200, 0, sizeof(std::uint64_t));

    g_renderIndexStorageA = ::operator new(229384);
    std::memset(g_renderIndexStorageA, 0, 229384);
    g_renderIndexStorageB = ::operator new(0x38008);
    std::memset(g_renderIndexStorageB, 0, 0x38008);
}

void Shenmue_Free() {
    ReleaseCachedSkinningBuffer(g_mainSkinningRoot);
    ReleaseCachedSkinningBuffer(g_sdOverrideSkinningRoot);
    ReleaseHmdlInstanceCpuMirrors();
    ::operator delete(g_renderObjectStorage);
    ::operator delete(g_renderLookupStorage);
    ::operator delete(g_renderPacketStorage);
    ::operator delete(g_renderIndexStorageA);
    ::operator delete(g_renderIndexStorageB);
    g_renderObjectStorage = nullptr;
    g_renderLookupStorage = nullptr;
    g_renderPacketStorage = nullptr;
    g_renderIndexStorageA = nullptr;
    g_renderIndexStorageB = nullptr;
}

void shenSetupDefaultAndDebugStates() {
    g_timeMultiplier1 = 1.0f;
    g_defaultBlendFactor = 0.89999998f;
    g_loadingWidgetEnabled[0] = 1;
    g_loadingWidgetEnabled[1] = 1;
    g_frameWaitEnabled[0] = 1;
    g_frameWaitEnabled[1] = 1;
    g_defaultInputFactor = 0.75f;
    g_unusedDebugTimeFlag = 0;
    g_skipArchiveReadWait = true;
    g_debugPointerState = 0;
    for (std::int32_t &flag : g_debugEnableFlags) {
        flag = 1;
    }
    g_deltaTime = 0.0f;
    g_framesPerSecond = 30;
    std::memset(g_debugClearedState, 0, sizeof(g_debugClearedState));
    g_forceDisplayCursor = 0;
    g_uiDebugSuspendElements = 0;
    g_debugUiDisabled = 1;
    g_debugUnknown0 = 0;
    g_notebookDebugMode = 0;
    g_uiDebugElementDispatch = 1;
    g_debugState4 = 2;
    for (float &scale : g_debugScale) {
        scale = 1.0f;
    }
    g_eyeRigParameters = {.lidYawScale = 0.09f, .lidPitchScale = 0.15f, .yawLimit = 0.395f, .pitchLimit = 0.34f};
    g_blinkDeltaStepTime = 10.0f;
    g_debugUnknown3 = 1;
    g_debugUnknown2 = 1;
    g_debugFaceUnknown = 0;
}


void UpdateTimer() {
    if (!g_NumFramesProcessed)
        return;
    UIAnim_UpdateDeltaTime(g_bEnableFrameTimeSmoothing ? fFrameTimeMs : 0.033333335f);
    if (DISP_IsOnMainMenu()) {
        g_uiElements[0x3c].isActive = 1;
    } else {
        TryInvokeRegisteredCallback(0x4e);
        LoadScreen.isActive = 1;
    }
}

namespace {


std::int32_t CEWP_CollectModelOwners(const void *model, std::array<Character *, 32> &owners) {
    const MapControlData *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (!control)
        return 0;
    std::int32_t count = 0;
    for (void *const head : control->characterBuckets) {
        for (const CharacterBucketNode *node = static_cast<const CharacterBucketNode *>(head); node; node = node->next) {
            Character *const character = node->character;
            if (!character || character->entity.name != SHEN_NAME(CEWP))
                continue;
            const std::uint32_t flags = character->flags.value;
            if ((flags & 1U) == 0U && ((flags & 2U) == 0U || (character->motionFlags & 1U) == 0U))
                continue;
            bool draws = character->model == model;
            if (!draws) {
                const ImageModuleWork *const image = IMGM_GetWork(character);
                const MdlmImageSlot *const slot = image && image->images ? image->images[image->requestedImage] : nullptr;
                draws = slot && slot->selectedModel && slot->selectedModel->model == model;
            }
            if (draws && count < 32)
                owners[static_cast<std::size_t>(count++)] = character;
        }
    }
    return count;
}

} // namespace



std::int64_t isValidCEWP(void *model) {
    std::array<Character *, 32> owners{};
    if (CEWP_CollectModelOwners(model, owners) <= 0)
        return 0;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr);
    HLib::Task *const scenarioTask = control->scenarioTask;
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(HLib_GetTaskDataForTask(scenarioTask));
    if (!scenario)
        return 0;
    Character *const owner = owners[0];
    CharacterIdLookupNode **link = &scenario->characterBuckets[reinterpret_cast<std::uintptr_t>(owner) % scenario->characterBuckets.size()];
    if (!*link)
        return 0;
    while ((*link)->character != owner) {
        link = &(*link)->bucketNext;
        if (!*link)
            return 0;
    }
    CharacterIdLookupNode *const node = *link;
    CharacterIdLookupNode *&head = scenario->characterBuckets[reinterpret_cast<std::uintptr_t>(owner) % scenario->characterBuckets.size()];
    *link = node->bucketNext;
    node->bucketNext = head;
    head = node;
    return node->id;
}

void HLTaskFunc_InternalInit();
extern void *g_scrollCurrentInfo;
extern std::int32_t g_bShouldFreeActiveScroll;



void Audio_n_Init() {
    if (g_bIsAICAInitialized) {
        const float volume = g_currentSettings.sfxVolume;
        for (std::uint32_t bank = 0; bank < 8; ++bank) {
            g_sfxVolume = volume;
            SetVolumeForAicaBank(volume, bank);
        }
        g_musicVolume = g_currentSettings.musicVolume;
    }
    SetAudioLanguage(static_cast<std::uint32_t>(g_currentSettings.audioLanguage));
    const std::int32_t subtitleLanguage = g_currentSettings.subtitleLanguage;
    if (static_cast<std::uint32_t>(subtitleLanguage) + 1U <= 7U)
        g_requestedUiLanguage = subtitleLanguage;
    const std::uint8_t mode = static_cast<std::uint8_t>(g_currentSettings.dialogueMode);
    std::uint32_t flags = (S1SettingsFlags_Get() & 0xffffffcfU) | (static_cast<std::uint32_t>(mode) << 4U);
    flags = g_dialogueModeVoiceTextEnable[mode][0] ? flags | 2U : flags & ~2U;
    flags = g_dialogueModeVoiceTextEnable[mode][1] ? flags | 1U : flags & ~1U;
    S1SettingsFlags_Set(flags);
    InitialiseShenmueSettings();
}


void PauseGameAudioSequence() {
    g_bPauseGameTasks = 1;
    g_isOnMainMenu = 1;
    HLTaskFunc_InternalInit();
    ReleaseEncodedAudioBuffers();
    for (FileState &file : g_files) {
        ReadFile_0(0, &file.handle);
    }
    g_shouldUseTemporaryClearColour = false;
    g_temporaryClearColour = {.red = 0xff, .green = 0, .blue = 0, .alpha = 0};
    shenInitDefaultStats();
    SCRL_SetLuminanceEnabled(false);
    SCRL_SetLuminanceChannels(0, 0, 0);
    InitializeAicaForNotebook();
    InitializeEncodedAudioBuffers();
    CreateNotebookTexture2DForPages();
}




void InitSettings() {
    shenInitDefaultSettings();
    shenInitDefaultStats();
    shenInitDefaultCassetteInventory();
    SCRL_SetLuminanceEnabled(false);
    SCRL_SetLuminanceChannels(0, 0, 0);
    CDTT_SetMode(CDTTMode::TitleOrIntroDateSetup);
    g_scrollCurrentInfo = nullptr;
    g_bShouldFreeActiveScroll = 1;
    g_displayActNumber = 0;
    InternalInit();
    Arcade_InitDefaultHangOnRanking();
    Arcade_InitDefaultSpaceHarrierRanking();
}

std::int32_t g_loadingSprite = -1;

void InitSpriteBank(std::int32_t initSpriteTextures) {
    if (!g_bSpriteBankInit)
        g_bSpriteBankInit = 1;
    if (!g_spriteBankBase)
        g_spriteBankBase = g_spriteBankStorage.data();
    for (std::size_t index = 0; index < std::size(g_spriteBanks); ++index) {
        g_spriteBanks[index] = g_spriteBankBase + index * 0x820;
    }
    if (!initSpriteTextures)
        return;

    PACK_Init();
    PackTaskData *const pack = g_packTaskData ? g_packTaskData : reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    pack->flags = PackDataLoadPksDirect;
    PACK_QueueFile("/MISC", "TEXTURES");
    PACK_Drain();
    if (PackSlot *const ready = pack->readyPack) {
        if (const void *const snow = LoadFileIntoHeapMem(ready, "w_snow.pvr", 0x204e4942U))
            CreateDDSorPVR(snow);
        if (const void *const hamon = LoadFileIntoHeapMem(ready, "w_hamon.pvr", 0x204e4942U))
            EFFE_CreateHamonTexture(hamon);
        g_texnTextureCacheBank[0] = 2;
        LoadFileIntoHeapMem(ready, "ASCII.PVR", 0x204e4942U);
        COMICONSPRLoad(ready);
        void *const loading = LoadFileIntoHeapMem(ready, "LD_nl.SPR", 0x204e4942U);
        g_texnTextureCacheBank[0] = 2;
        g_loadingSprite = loading ? LoadTEXN(loading) : -1;
        g_texnTextureCacheBank[0] = 0;
    }
    PACK_Shutdown();
}

extern std::uint32_t g_ClearColour;

void Initialization_2() {
    const float firstTime = QueryFrameTime();
    frameTime = firstTime;
    g_LastFrameTime = QueryFrameTime();
    const float thirdTime = QueryFrameTime();

    g_audioInitCallback = Audio_n_Init;
    g_unusedFlag0 = 2;
    Shenmue_nNumVBLANKs = 2;
    Shenmue_nNumRenderedFrames = 2;
    Shenmue_FramesPast_Copy = 0;
    FramesPast = 0;
    PreviousFramesPast = 0;
    Shenmue_nPauseState = 0;
    g_bDebugLog = 0;
    g_bPauseGameTasks = 0;
    g_updateRenderFrameMode = -1;
    g_bSkipCycles = 0;
    g_forcePrimaryCachedModel = 0;
    g_LastFrameTime += thirdTime - firstTime;

    g_roLegacyScalarFactors = g_legacyScalarStorage.data();
    g_lightDataStatics = &g_defaultLightStorage;
    g_spriteBankBase = g_spriteBankStorage.data();

    GameConfig config{};
    config.display.flags = 0x2201;
    config.display.width = 640;
    config.display.height = 480;
    config.refreshRateNumerator = 0;
    config.refreshRateDenominator = 4;
    config.two = 2;
    config.sixteen = 16;
    config.one = 1;
    config.matrices = g_defaultMatrices.data();
    config.matrixCount = 64;
    config.memoryLimit = 100000;
    ShenmueInit(&config);

#if !defined(SHENMUE_STEAM)
    std::uint64_t steamId = 1024;
    if (Steam_TryGetCurrentUserId(&steamId)) {
        SGAAS_Init(steamId);
    }
#endif

    fnptr_GetBuffer = GetBuffer;
    fnptr_UpdateTimer = UpdateTimer;
    fnptr_DispatchRenderPipeline = DispatchRenderPipeline;
    g_isValidCEWP = isValidCEWP;
    shenSetupDefaultAndDebugStates();
    AllocMemory();
    LoadShaders();

    if (!g_bSpriteBankInit) {
        g_bSpriteBankInit = 1;
    }
    for (std::size_t index = 0; index < std::size(g_spriteBanks); ++index) {
        g_spriteBanks[index] = g_spriteBankBase + index * 0x820;
    }
    g_controllers.fill({});
    std::memset(g_previousInputData.data(), 0, g_previousInputData.size());
    ExecuteWeatherTaskQueue = ExecuteWeatherTaskQueue_Internal;
    InitEffectBuffers();

    auto storeEffect32 = [](std::size_t offset, std::uint32_t value) { std::memcpy(g_effectInitializationState.data() + offset, &value, sizeof(value)); };
    auto storeEffect64 = [](std::size_t offset, std::uint64_t value) { std::memcpy(g_effectInitializationState.data() + offset, &value, sizeof(value)); };
    std::uint32_t firstEffectWord = 0;
    std::memcpy(&firstEffectWord, g_effectInitializationState.data(), sizeof(firstEffectWord));
    storeEffect32(0, firstEffectWord | 2);
    storeEffect64(8, 0);
    std::memset(g_effectInitializationState.data() + 0x10, 0, 0x20);
    storeEffect64(0x30, 0);
    storeEffect64(0x38, 0);
    storeEffect32(0x40, 0);
    for (std::size_t offset = 0x44; offset <= 0x54; offset += 4) {
        storeEffect32(offset, 1);
    }
    g_effectFlagA = 0;
    g_effectFlagB = 0;
    for (float &value : g_effectScaleA) {
        value = 1.0f;
    }
    for (float &value : g_effectScaleB) {
        value = 1.0f;
    }

    SetScreenDimensions(kOriginalScreenWidth, kOriginalScreenHeight);
    MatrixStack_InitTransformMatrixStack(g_defaultMatrices.data(), 64);
    SetActiveStack(MatrixStackType::Transparent);
    SetActiveStack(MatrixStackType::DepthPrepass);
    *g_matrixStack.top.stack = workTransformMatrix;
    workTransformMatrix = transformMatrix_prePass;
    g_matrixStack.top.stack = g_matrixStack.top.base;
    g_matrixStack.top.info.index = 0;
    g_ClearColour = 0xff000000;

    InitSceneDataMemory_0();
    g_LightData = g_lightDataStatics;
    g_lightRenderSlots = g_lightDataStatics->renderSlots;
    std::memset(g_lightDataStatics, 0, sizeof(*g_lightDataStatics));
    SetupEffeTransform();
    for (FileState &file : g_files) {
        ReadFile_0(0, &file.handle);
    }
    SteamInit();

    std::memset(&g_inputState, 0, 0xc);
    storeZero(0x12, 2);
    storeZero(0x16, 16);
    storeZero(0x28, 2);
    storeZero(0x2c, 8);
    storeZero(0x34, 4);
    storeZero(0x3e, 2);
    storeZero(0x42, 16);
    storeZero(0x4a, 4);
    storeZero(0x54, 2);
    storeZero(0x58, 8);
    storeZero(0x60, 4);
    storeZero(0x6a, 2);

    HLib_Init();
    HLib_TaskQueueSetNONE();
    g_renderVertexShaderAndLayoutIndex = g_inputLayoutTypes[2].vertexShaderAndLayoutIndex;
    g_renderPixelShaderIndex = g_inputLayoutTypes[2].pixelShaderIndex;
    g_renderVertexStride = g_inputLayoutTypes[2].vertexStride;
    g_renderPassIndex = 0;
    InitializeEncodedAudioBuffers();

    InitializeD3TSettings();
    ReadConfigFromSave();
    SettingsFileParser();
    if (g_pendingThreadLockValid == 1) {
        g_requestedUiLanguage = g_pendingThreadLock;
        g_pendingThreadLockValid = 0;
    }
    FontLoader();
}

} // namespace shenmue::s1
