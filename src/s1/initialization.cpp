#include "s1/d3t/namespace.hpp"
#include "s1/boot.hpp"
#include "s1/audio_stream.hpp"
#include "s1/d3t/d3t_archives.hpp"
#include "s1/d3t/d3t_asset_remap.hpp"
#include "s1/d3t/d3t_compression.hpp"
#include "s1/d3t/render_offsets.hpp"
#include "d3t/model.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/d3t_threads.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/file_state.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/initialization.hpp"
#include "s1/task/hplg.hpp"
#include "s1/steam.hpp"
#include "s1/input.hpp"
#include "s1/launch_options.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/scene_memory.hpp"
#include "s1/texture_cache.hpp"
#include "s1/window.hpp"

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <cstring>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace shenmue::s1 {

#pragma pack(push, 1)
struct PvrTaskDataNode {
    std::int16_t taskDataIndex;
    std::int16_t state;
    std::int32_t textureIdLow;
    std::int32_t textureIdHigh;
    std::byte reserved_0c[4];
    PvrTaskDataNode *next;
};
#pragma pack(pop)

static_assert(sizeof(PvrTaskDataNode) == 0x18);
static_assert(offsetof(PvrTaskDataNode, next) == 0x10);

namespace d3t {
void SetupAchievements();
void InitializeSteamAPI();
} // namespace d3t
void ReadConfigFromSave();
void SettingsFileParser();
void SetupEffeTransform();
void SteamPlatformContextRefresh();
extern std::uint32_t g_ClearColour;

PvrTaskDataNode *g_pendingPvrTaskData = nullptr;
PvrTaskDataNode *g_freePvrTaskData = nullptr;
PvrTaskDataNode *g_activePvrTaskData = nullptr;
PvrTaskDataNode g_pvrTaskDataNodes[640]{};
std::int32_t g_pvrTaskDataInitialized = 0;
std::int32_t g_texnTextureCacheBank[4]{};
std::uint64_t g_pvrSceneState = 0;
std::uint8_t g_sceneTextureMode = 0;
std::int32_t g_sceneInitFlag730 = 0;
std::int32_t g_sceneInitFlag71c = 0;
std::int32_t g_sceneInitFlag718 = 0;
std::int32_t g_sceneInitFlag72c = 0;
std::uint32_t g_memAlignment = 7;
void *g_globalMemory = nullptr;
std::uint32_t g_globalMemorySize = 0;
SteamStorageCallback g_steamSaveFunction = nullptr;
SaveStorageCompletion g_steamCallbackFunction = nullptr;
void *g_activeSaveData = nullptr;
std::int32_t g_bookkeepingStorageResult = 0;
std::atomic<std::int32_t> g_bookkeepingStorageState{0};
std::uint64_t g_initializationTemporaryClearColour = 0;
std::uint32_t g_hasDirectXInitialized = 0;
std::int32_t g_postInitState = 0;
Vector4 g_postInitVectorA{};
Vector4 g_postInitVectorB{};

void HLTaskFunc_InternalInit() {
    PvrTaskDataNode *node = g_pendingPvrTaskData;
    while (node) {
        InitPVRTaskData(node->taskDataIndex);
        PvrTaskDataNode *const next = node->next;
        node->next = g_freePvrTaskData;
        g_freePvrTaskData = node;
        node->state = 0;
        node->textureIdLow = 0;
        node->textureIdHigh = 0;
        node = next;
    }
    g_pendingPvrTaskData = nullptr;
    if (!g_pvrTaskDataInitialized) {
        g_pvrTaskDataInitialized = 1;
    }
}

void InitSceneDataMemory_0() {
    g_texnTextureCacheBank[0] = 0;
    InitializePvrTextureSlotStorage();
    for (std::int32_t index = 0; index < 640; ++index) {
        PvrTaskDataNode &node = g_pvrTaskDataNodes[index];
        node.taskDataIndex = static_cast<std::int16_t>(index);
        node.state = 0;
        node.textureIdLow = 0;
        node.textureIdHigh = 0;
        node.next = index + 1 < 640 ? &g_pvrTaskDataNodes[index + 1] : g_pvrTaskDataNodes + 640;
    }
    g_pvrSceneState = 0;
    g_freePvrTaskData = g_pvrTaskDataNodes;
    g_pendingPvrTaskData = nullptr;
    g_activePvrTaskData = nullptr;
    InitSceneDataMemory();
    g_sceneTextureMode = 2;
    if (!g_sceneInitFlag730)
        g_sceneInitFlag730 = 1;
    if (!g_sceneInitFlag71c)
        g_sceneInitFlag71c = 1;
    if (!g_sceneInitFlag718)
        g_sceneInitFlag718 = 1;
    if (!g_sceneInitFlag72c)
        g_sceneInitFlag72c = 1;
}

std::int32_t PVR_FindOrAllocateSurface(std::int32_t textureIdLow, std::int32_t textureIdHigh) {
    PvrTaskDataNode *&activeList = g_texnTextureCacheBank[0] ? g_activePvrTaskData : g_pendingPvrTaskData;
    for (PvrTaskDataNode *node = activeList; node; node = node->next) {
        if (node->textureIdLow == textureIdLow && node->textureIdHigh == textureIdHigh) {
            if (node->taskDataIndex < 0) {
                break;
            }
            ++node->state;
            return node->taskDataIndex;
        }
    }

    PvrTaskDataNode *const node = g_freePvrTaskData;
    if (!node) {
        return -1;
    }
    g_freePvrTaskData = node->next;
    node->next = activeList;
    activeList = node;
    node->textureIdLow = textureIdLow;
    node->state = 1;
    node->textureIdHigh = textureIdHigh;
    return node->taskDataIndex;
}

std::int32_t PVR_FindSurface(std::int32_t textureIdLow, std::int32_t textureIdHigh) {
    PvrTaskDataNode *node = g_texnTextureCacheBank[0] ? g_activePvrTaskData : g_pendingPvrTaskData;
    while (node) {
        if (node->textureIdLow == textureIdLow && node->textureIdHigh == textureIdHigh) {
            return node->taskDataIndex;
        }
        node = node->next;
    }
    return -1;
}

std::int64_t PVR_FreeAllocatedSurfaceByIndex(std::int32_t surfaceIndex) {
    PvrTaskDataNode *&activeList = g_texnTextureCacheBank[0] ? g_activePvrTaskData : g_pendingPvrTaskData;
    PvrTaskDataNode *previous = nullptr;
    PvrTaskDataNode *node = activeList;
    while (node && node->taskDataIndex != surfaceIndex) {
        previous = node;
        node = node->next;
    }
    if (!node || node->taskDataIndex < 0) {
        return -1;
    }

    if (--node->state <= 0) {
        InitPVRTaskData(node->taskDataIndex);
        if (previous) {
            previous->next = node->next;
        } else {
            activeList = node->next;
        }
        node->next = g_freePvrTaskData;
        g_freePvrTaskData = node;
        node->state = 0;
        node->textureIdLow = 0;
        node->textureIdHigh = 0;
    }
    return 0;
}

#pragma pack(push, 1)
struct RenderHandleEntry {
    std::byte unknown_000[0xc0];
    std::uint32_t pending;
    void *loadedAssetLog;
    std::uint32_t loadedAssetLogPosition;
    std::uint32_t loadedAssetLogCapacity;
    void *auxiliaryLoadedAssetLog;
    std::uint64_t auxiliaryLoadedAssetLogState;
    std::byte unknown_0e4[0x15c];
};
#pragma pack(pop)

static_assert(offsetof(SteamAppInfo, appName) == 0x8);
static_assert(sizeof(RenderHandleEntry) == 0x240);
static_assert(offsetof(RenderHandleEntry, loadedAssetLog) == 0xc4);
static_assert(offsetof(RenderHandleEntry, pending) == 0xc0);
static_assert(offsetof(RenderHandleEntry, loadedAssetLogPosition) == 0xcc);
static_assert(offsetof(RenderHandleEntry, auxiliaryLoadedAssetLog) == 0xd4);
static_assert(offsetof(RenderHandleEntry, auxiliaryLoadedAssetLogState) == 0xdc);

std::uint8_t smallTextures = 0;
std::uint8_t logLoadedAssets = 0;
alignas(16) std::array<std::byte, 0x2de88> MainMemory{};
std::array<RenderHandleEntry, 22> g_renderHandleTable{};
alignas(16) std::array<char, 261> g_textureCachePath{"./tex/cache"};
void *g_largeDataBuffer = nullptr;
std::uint32_t g_largeDataBufferSize = 0;
std::uint32_t g_fileLockHandle = 0xffffU;
std::int32_t g_doLowMipsOnly = 0;
std::int32_t g_dxPipelineLock = 0;
extern std::int32_t g_actNumber;
bool g_steamInitialized = false;

bool IsMainRenderHandleBusy() {
    return g_renderHandleTable[21].pending != 0;
}

int Initialization(int, char **) {

    const SteamAppInfo appInfo{758330, 0, "Coconut", "MD22_112855"};
    if (!Steam_InitApp(&appInfo)) {
        return 0;
    }
    g_steamInitialized = true;

    smallTextures = 0;

    std::memset(MainMemory.data(), 0, MainMemory.size());
    HLib_Malloc();
    Initialization_2();
    return 1;
}

std::int64_t InitShenmue(GameConfig *config) {
#if defined(_WIN32)
    if (!g_steamInitialized) {
        g_steamInitialized = Steam_InitApp(nullptr);
    }

    g_textureCachePath.fill('\0');
    constexpr char kDefaultTextureCachePath[] = "./tex/cache";
    std::memcpy(g_textureCachePath.data(), kDefaultTextureCachePath, sizeof(kDefaultTextureCachePath));

    char currentDirectory[260]{};
    const DWORD directoryLength = GetCurrentDirectoryA(static_cast<DWORD>(std::size(currentDirectory)), currentDirectory);
    if (directoryLength > 8 && std::strcmp(currentDirectory + directoryLength - 8, "src\\main") == 0) {
        SetCurrentDirectoryA("..\\..\\gamedata\\xbox");
        GetCurrentDirectoryA(static_cast<DWORD>(std::size(currentDirectory)), currentDirectory);
    }

    DisplayModeInfo *const screenResolution = GetScreenResolution();
    if (!screenResolution) {
        return -1;
    }
    const std::int32_t windowHeight = static_cast<std::int32_t>(screenResolution->height);
    const std::int32_t windowWidth = static_cast<std::int32_t>(screenResolution->width);
    ClearWindowInput();
    g_windowInput.width = windowWidth;
    g_windowInput.height = windowHeight;
    g_pendingClientRectUpdate = 1;
    InitInputMapping();

    RAWINPUTDEVICE keyboard{};
    keyboard.usUsagePage = 1;
    keyboard.usUsage = 6;
    RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard));
    CreateSMWindow();

    config->display.width = 640;
    config->display.height = 480;
    DX11_D3D11CreateDeviceAndSwapChain(screenResolution);
#else
    static_cast<void>(config);
#endif

    std::memset(&d3t::g_d3tModels, 0, sizeof(d3t::g_d3tModels));
    d3t::CreateTemp2x2Textures();
    SetupGlobalRenderBuffers();
    g_largeDataBufferSize = 0x800000;
#if defined(_WIN32)
    g_largeDataBuffer = _aligned_malloc(g_largeDataBufferSize, 16);
#else
    g_largeDataBuffer = std::aligned_alloc(16, g_largeDataBufferSize);
#endif
    g_fileLockHandle = AllocateFileLockHandle();
    if (smallTextures) {
        g_doLowMipsOnly = 0;
    }

#if defined(_WIN32)
    if (g_swapChain) {
        g_swapChain->SetFullscreenState(g_currentSettings.display.windowMode == 2, nullptr);
    }
    const UINT message = RegisterWindowMessageA("SHENMUE_LOADED");
    PostMessageA(HWND_BROADCAST, message, 0x1234, 0x5678);
    SetupImmutableDXTex2D();
#endif
    GenerateMipsAndUpdateResourcesForDirectX();
    g_dxPipelineLock = 0;
#if defined(_WIN32)
    DX11::g_ppD3DDeviceContext = d3t_DXGetPipelineContext(0);
#endif
    InitGeneralTextureResources();
    return 0;
}

std::int64_t ShenmueInit(GameConfig *config) {
    d3t_CreateThreads();
    if (!g_globalMemory) {
        constexpr std::size_t kGlobalMemorySize = 0x280000;
        g_globalMemory = std::malloc(kGlobalMemorySize * static_cast<std::size_t>(g_memAlignment));
        g_globalMemorySize = static_cast<std::uint32_t>(kGlobalMemorySize);
    }

    d3t::SetupAchievements();
    d3t::InitializeSteamAPI();
    for (AsyncArchiveRequest &request : g_asyncArchiveRequests) {
        request.state.exchange(0, std::memory_order_seq_cst);
    }
    d3t_InitializeOodleZLib();
    const char *const shenRoot = std::getenv("SHEN_ROOT");
    const std::filesystem::path gameRoot = shenRoot && *shenRoot ? std::filesystem::path(shenRoot) / "sm1" : std::filesystem::current_path();
    const std::string gameRootString = gameRoot.string();
    const D3TArchiveDirectory directory{gameRootString.c_str(), "dx11", 2, 0, 0, 0, {}};
    d3t_ReadArchives(&directory);
    d3t_ReadAssetRemapping();

    g_steamSaveFunction = SteamSaveFn;
    g_activeSaveData = nullptr;
    g_bookkeepingStorageResult = 0;
    g_bookkeepingStorageState.store(0);
    if (g_steamCallbackFunction) {
        g_steamCallbackFunction(0, nullptr);
    }

#if defined(_WIN32)
    EnumerateDisplays();
#endif
    InitializeD3TSettings();
    ReadConfigFromSave();
    SettingsFileParser();
    g_ClearColour = 0xff000000;
    g_initializationTemporaryClearColour = 0;

    InitShenmue(config);
    MatrixStack_InitTransformMatrixStack(config->matrices, config->matrixCount);
    InitRenderer();
    SetAlphaRenderFlags();
    Create_2x2_Texture();
    g_engineAlphaState.fill({});

    const float screenHeight = static_cast<float>(g_originalScreenHeight);
    const float screenWidth = static_cast<float>(g_originalScreenWidth);
    SetupDirectXWindow(0.0f, 0.0f, screenWidth, screenHeight);
    SetActiveStack(MatrixStackType::Transparent);
    Matrix4x4_SetupCameraProjectionMatrix(0x2aab, screenWidth / screenHeight, 0.1f, 60000.0f);
    SetActiveStack(MatrixStackType::DepthPrepass);
    SetupEffeTransform();

    g_postInitState = 4;
    g_postInitVectorA = {-0.5f, -0.5f, -0.5f, 0.5f};
    g_ClearColour = 0;
    g_postInitVectorB = {0.5f, 0.5f, 0.5f, -0.5f};
    SteamPlatformContextRefresh();
    g_hasDirectXInitialized |= 1;
    return 1;
}

void ReleaseAicaRam() {
    ReleaseEncodedAudioBuffers();
}

void ReleaseLargeDataBuffer() {
#if defined(_WIN32)
    _aligned_free(g_largeDataBuffer);
#else
    std::free(g_largeDataBuffer);
#endif
    g_largeDataBuffer = nullptr;
    g_largeDataBufferSize = 0;
}

void ReleaseRenderHandleLogs() {
    RenderHandleEntry &entry = g_renderHandleTable[21];
    std::free(entry.auxiliaryLoadedAssetLog);
    entry.auxiliaryLoadedAssetLog = nullptr;
    entry.auxiliaryLoadedAssetLogState = 0;
    std::free(entry.loadedAssetLog);
    entry.loadedAssetLog = nullptr;
    entry.loadedAssetLogPosition = 0;
    entry.loadedAssetLogCapacity = 0;
}

} // namespace shenmue::s1
