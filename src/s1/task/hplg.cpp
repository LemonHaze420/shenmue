#include "s1/task/hplg.hpp"
#include "s1/task/save.hpp"
#include "s1/d3t/d3t_threads.hpp"
#include "s1/steam.hpp"
#include "s1/boot.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/input.hpp"
#include "s1/initialization.hpp"
#include "s1/render_passes.hpp"
#include "s1/mt5_render.hpp"
#include "s1/save_format.hpp"
#include "s1/shader_initialization.hpp"
#include "s1/task/effe.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/came.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/wexe.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <new>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>

namespace shenmue::s1 {

BookkeepingCache g_bookkeepingStatsCache{};
std::int32_t g_returnToTitleRequested = 0;

std::int32_t g_bookkeepingRefreshUpdatesDisabled = 0;
std::uint32_t g_bookkeepingProgressOverride = 0;
namespace {

constexpr std::int16_t kHotPlugTaskIndex = 2;
constexpr std::uint16_t kHotPlugTaskFlags = 2;
constexpr std::int32_t kHotPlugTaskToken = SHEN_NAME(HPLG);
constexpr std::int32_t kTaskDataToken = SHEN_NAME(TASK);
std::int32_t g_abufferClearPending{};




constexpr std::size_t kGlobalStatsSize = 0x1800;
constexpr std::size_t kAggregateProgressOffset = 0x1800;
constexpr std::size_t kSaveSummaryOffset = 0x1804;
constexpr std::size_t kSaveSummarySize = 0x40;
constexpr std::size_t kRefreshStatusOffset = 0x1a84;
constexpr std::size_t kInternalProgressOffset = 0x688;
constexpr std::size_t kInternalSettingsFlagsOffset = 0x68c;
constexpr std::size_t kMaximumSaveSlots = 10;
constexpr std::size_t kMaximumListeners = 16;

static_assert(offsetof(BookkeepingCache, globalStatsMirror) == 0);
static_assert(offsetof(BookkeepingCache, aggregateSaveProgress) == kAggregateProgressOffset);
static_assert(offsetof(BookkeepingCache, saveSummaries) == kSaveSummaryOffset);
static_assert(offsetof(BookkeepingCache, refreshStatus) == kRefreshStatusOffset);
static_assert(sizeof(BookkeepingCache) == kBookkeepingCacheSize);

struct BookkeepingListenerNode {
    BookkeepingListenerNode *previous;
    BookkeepingListenerNode *next;
    BookkeepingRefreshListener callback;
};

#pragma pack(push, 1)
struct HotPlugTaskData {
    std::byte reserved000[0x100];


    std::byte refreshScratch[0x140];
    std::int32_t connected;
    std::int32_t hasProgress;
    BookkeepingListenerNode listeners[kMaximumListeners];
    BookkeepingListenerNode *listenerHead;
};
#pragma pack(pop)

static_assert(sizeof(BookkeepingListenerNode) == 0x18);
static_assert(sizeof(HotPlugTaskData) == 0x3d0);
static_assert(offsetof(HotPlugTaskData, connected) == 0x240);
static_assert(offsetof(HotPlugTaskData, hasProgress) == 0x244);
static_assert(offsetof(HotPlugTaskData, listeners) == 0x248);
static_assert(offsetof(HotPlugTaskData, listenerHead) == 0x3c8);

std::array<std::byte, kGlobalStatsSize + sizeof(std::uint32_t)> g_bookkeepingGlobalStatsUploadBuffer{};
std::uint32_t g_bookkeepingRefreshCount = 0;
std::uint32_t g_bookkeepingStartupMirror = 0;
std::uint32_t g_bookkeepingGlobalStatsUploadCount = 0;

std::array<std::byte, kS1SaveImageSize> g_saveSlotUploadBuffer;

bool g_bookkeepingStorageConnected = true;

struct BookkeepingStorageRequest {
    char name[16];
    void *data;
    std::byte *buffer;
    std::uint32_t offset;
    std::uint32_t size;
    bool write;
    SteamStorageCallback callback;
};

HotPlugTaskData *HotPlugData() {
    return g_hotPlugTask && g_hotPlugTask->task_data_ptr ? reinterpret_cast<HotPlugTaskData *>(g_hotPlugTask->task_data_ptr) : nullptr;
}

std::int32_t CurrentPlatformStorageState() {
    return g_bookkeepingStorageConnected ? g_bookkeepingStorageState.load() : -1;
}

void WaitForBookkeepingStorage() {
    while (CurrentPlatformStorageState() == -2)
        SteamRemoteStorage_RunCallbacks();
}

// @verified - p
std::int32_t MapBookkeepingStorageResult(std ::int32_t result) {
    switch (result) {
    case -3:
        return -4;
    case -2:
        return -5;
    case -1:
        return -2;
    default:
        return result != 0 ? -6 : 0;
    }
}
std::string SaveSlotKey(std::size_t slot) {
    return "SHENMUE1_00" + std::to_string(slot);
}

std::filesystem::path BookkeepingStoragePath(const char *name) {
    return std::filesystem::path{"save"} / name;
}

std::filesystem::path BookkeepingReadPath(const char *name) {
    const std::filesystem::path path = BookkeepingStoragePath(name);
    std::error_code error;
    if (std::filesystem::exists(path, error) || std::strncmp(name, "SHENMUE1_00", 11) != 0)
        return path;
    const std::string slot{name + 11};
    const std::array<std::filesystem::path, 4> alternatives{BookkeepingStoragePath((std::string{name} + ".bin").c_str()),
        slot == "0" ? BookkeepingStoragePath("s1_new_game.bin") : std::filesystem::path{}, BookkeepingStoragePath(("s1_00" + slot + ".bin").c_str()),
        BookkeepingStoragePath(("s1_slot_" + slot + ".bin").c_str())};
    for (const std::filesystem::path &alternative : alternatives) {
        if (!alternative.empty() && std::filesystem::exists(alternative, error))
            return alternative;
    }
    return path;
}
std::int32_t CheckBookkeepingStorageOpen(const char *name) {
    std::error_code error;
    return std::filesystem::is_regular_file(BookkeepingReadPath(name), error) ? 0 : -3;
}

std::int32_t ReadBookkeepingStorageSize(const char *name) {
    std::error_code error;
    const std::uintmax_t size = std::filesystem::file_size(BookkeepingReadPath(name), error);
    return error ? -3 : static_cast<std::int32_t>(size);
}

std::int32_t BookkeepingStorageWorker(void *data) {
    BookkeepingStorageRequest &request = *static_cast<BookkeepingStorageRequest *>(data);
    S1SaveResult result;
    if (request.write)
        result = S1Save_WriteStorageBytes(BookkeepingStoragePath(request.name), request.buffer, request.size);
    else {
        result = S1Save_ReadStorageRange(BookkeepingReadPath(request.name), request.buffer, request.offset, request.size);
        if (result == S1SaveResult::Success)
            std::memcpy(request.data, request.buffer, request.size);
    }
    delete[] request.buffer;
    if (request.write)
        g_steamSaveFunction(3, result == S1SaveResult::Success ? 0 : -9, request.data);
    else
        request.callback(2, result == S1SaveResult::Success ? 0 : -4, request.data);
    return 0;
}

std::int32_t QueueBookkeepingStorage(const char *name, void *data, std::uint32_t offset, std::uint32_t size, bool write) {
    BookkeepingStorageRequest request{};
    std::snprintf(request.name, sizeof(request.name), "%s", name);
    request.data = data;
    request.callback = g_steamSaveFunction;
    request.buffer = new (std::nothrow) std::byte[size];
    if (!request.buffer) {
        if (!write)
            g_steamSaveFunction(2, -4, data);
        return 0;
    }
    request.offset = offset;
    request.size = size;
    request.write = write;
    if (write)
        std::memcpy(request.buffer, data, size);
    const d3t::D3TTaskDescriptor descriptor{.data = &request, .callback = BookkeepingStorageWorker, .completion = nullptr, .priority = 0};
    d3t::d3t_QueueTask(&descriptor, sizeof(request));
    return 0;
}

// @verified - p
std::int32_t CheckBookkeepingStorageCapacity(const char *name, std::uint32_t size) {
    std::int32_t oldSize = 0;
    constexpr std::int64_t availableBytes = 1000000000;
    if (CheckBookkeepingStorageOpen(name) == 0) {
        oldSize = ReadBookkeepingStorageSize(name);
        if (oldSize < 0)
            return MapBookkeepingStorageResult(oldSize);
    }
    return availableBytes >= static_cast<std::int32_t>(size) - oldSize ? 0 : -5;
}

// @verified - p
std::int32_t ReadBookkeepingStorage(const char *name, void *data, std::uint32_t offset, std::uint32_t size) {
    WaitForBookkeepingStorage();
    g_bookkeepingStorageState.store(-2);
    g_steamCallbackFunction = nullptr;
    const std::int32_t result = CheckBookkeepingStorageOpen(name);
    if (result != 0) {
        g_activeSaveData = data;
        g_bookkeepingStorageResult = -4;
        g_bookkeepingStorageState.store(0);
        if (g_steamCallbackFunction)
            g_steamCallbackFunction(-4, data);
        return MapBookkeepingStorageResult(result);
    }
    return MapBookkeepingStorageResult(QueueBookkeepingStorage(name, data, offset, size, false));
}

// @verified - p
std::int32_t WriteBookkeepingStorage(const char *name, void *data, std::uint32_t size, SaveStorageCompletion completion) {
    WaitForBookkeepingStorage();
    g_bookkeepingStorageState.store(-2);
    g_steamCallbackFunction = completion;
    const std::int32_t result = CheckBookkeepingStorageCapacity(name, size);
    if (result != 0) {
        g_bookkeepingStorageResult = -5;
        g_activeSaveData = data;
        g_bookkeepingStorageState.store(0);
        if (g_steamCallbackFunction)
            g_steamCallbackFunction(-5, data);
        return result;
    }
    return MapBookkeepingStorageResult(QueueBookkeepingStorage(name, data, 0, size, true));
}

void StageBookkeepingGlobalStatsUploadBuffer() {
    std::memcpy(g_bookkeepingGlobalStatsUploadBuffer.data(), &g_bookkeepingStatsCache, g_bookkeepingGlobalStatsUploadBuffer.size());
    ++g_bookkeepingGlobalStatsUploadCount;
}

// @verified - p
std::int32_t LoadBookkeepingGlobalStatsAndSaveHeaders() {
    std::memset(&g_bookkeepingStatsCache, 0, sizeof(g_bookkeepingStatsCache));
    g_playerStats.currentSaveProgress = 0;
    WaitForBookkeepingStorage();
    const std::int32_t storageState = CurrentPlatformStorageState();
    if (storageState != 0)
        return storageState;
    bool globalStatsLoaded = false;
    if (ReadBookkeepingStorage("BOOKKEEP", &g_bookkeepingStatsCache, 0, g_bookkeepingGlobalStatsUploadBuffer.size()) == 0) {
        WaitForBookkeepingStorage();
        globalStatsLoaded = g_bookkeepingStorageResult == 0;
    }
    bool shouldUploadGlobalStats = false;
    std::array<std::uint8_t, 0x200> header;
    for (std::size_t slot = 0; slot < kMaximumSaveSlots; ++slot) {
        std::snprintf(g_currentSaveSlot.data(), g_currentSaveSlot.size(), "%s", SaveSlotKey(slot).c_str());
        if (ReadBookkeepingStorage(g_currentSaveSlot.data(), header.data(), 0x600, header.size()) != 0)
            continue;
        WaitForBookkeepingStorage();
        if (g_bookkeepingStorageResult != 0)
            continue;
        WaitForBookkeepingStorage();
        if (CurrentPlatformStorageState() != 0)
            continue;
        std::uint32_t identifier, version;
        std::memcpy(&identifier, header.data() + 0x80, sizeof(identifier));
        std::memcpy(&version, header.data() + 0x84, sizeof(version));
        if (identifier != kS1SaveIdentifier.name || version != kS1SaveVersion)
            continue;
        std::memcpy(&g_bookkeepingStatsCache.saveSummaries[slot], header.data() + 0x140, kSaveSummarySize);

        if (!globalStatsLoaded) {
            if (ReadBookkeepingStorage(
                    g_currentSaveSlot.data(), g_bookkeepingStatsCache.globalStatsMirror.data(), kS1SaveGlobalStatsOffset, kGlobalStatsSize) == 0) {
                WaitForBookkeepingStorage();
                if (g_bookkeepingStorageResult == 0) {
                    WaitForBookkeepingStorage();
                    if (CurrentPlatformStorageState() == 0) {
                        globalStatsLoaded = true;
                        g_bookkeepingStatsCache.aggregateSaveProgress = header[kInternalProgressOffset - 0x600];
                    }
                }
            }
        }

        if (g_bookkeepingRefreshUpdatesDisabled == 0) {
            g_bookkeepingStatsCache.aggregateSaveProgress =
                static_cast<std::uint8_t>(g_bookkeepingStatsCache.aggregateSaveProgress + header[kInternalProgressOffset - 0x600]);
            std::uint32_t settingsFlags;
            std::memcpy(&settingsFlags, header.data() + kInternalSettingsFlagsOffset - 0x600, sizeof(settingsFlags));
            S1SettingsFlags_Set(settingsFlags);
            shouldUploadGlobalStats = true;
        }
    }
    const std::uint8_t progress = g_bookkeepingStatsCache.aggregateSaveProgress;
    g_bookkeepingStatsCache.refreshStatus.hasProgress = progress != 0;
    g_playerStats.currentSaveProgress = progress;
    g_bookkeepingStatsCache.refreshStatus.isLoaded = 1;
    return shouldUploadGlobalStats ? UploadBookkeepingGlobalStats() : progress;
}

// @verified - p
void RefreshBookkeepingGlobalStatsCache(HotPlugTaskData &data) {
    ++g_bookkeepingRefreshCount;
    std::memset(data.refreshScratch, 0, sizeof(data.refreshScratch));
    data.hasProgress = 0;
    LoadBookkeepingGlobalStatsAndSaveHeaders();
    data.hasProgress = static_cast<std::int32_t>(g_bookkeepingStatsCache.refreshStatus.hasProgress);

    for (BookkeepingListenerNode *node = data.listenerHead; node; node = node->previous)
        node->callback(reinterpret_cast<const std::byte *>(&g_bookkeepingStatsCache));
}

void bupHotPlugTask_HPLG_Callback(HLib::TaskData *taskData) {
    auto *const data = reinterpret_cast<HotPlugTaskData *>(taskData);
    if (!data)
        return;

    const std::int32_t connected = CurrentPlatformStorageState() == -1 ? 0 : 1;
    if (connected == data->connected)
        return;
    data->connected = connected;
    RefreshBookkeepingGlobalStatsCache(*data);
}

} // namespace

// @verified - p
void SteamSaveFn(std::int32_t, std::int32_t result, void *data) {
    g_activeSaveData = data;
    const std::int32_t mapped = MapBookkeepingStorageResult(result);
    const SaveStorageCompletion completion = g_steamCallbackFunction;
    g_bookkeepingStorageResult = mapped;
    g_bookkeepingStorageState.store(0);
    if (completion)
        completion(mapped, data);
}
// @verified - p
std::int32_t UploadBookkeepingGlobalStats() {
    StageBookkeepingGlobalStatsUploadBuffer();
    const std::int32_t result =
        WriteBookkeepingStorage("BOOKKEEP", g_bookkeepingGlobalStatsUploadBuffer.data(), g_bookkeepingGlobalStatsUploadBuffer.size(), nullptr);
    if (result != 0)
        return result;
    WaitForBookkeepingStorage();
    return g_bookkeepingStorageResult;
}

// @verified - p
std::int32_t UploadCurrentSaveSlot(S1SaveImage &image) {
    const std::int32_t result = UploadBookkeepingGlobalStats();
    if (result != 0)
        return result;
    S1Save_PrepareStorageWrite(image);
    std::memcpy(g_saveSlotUploadBuffer.data(), image.bytes.data(), g_saveSlotUploadBuffer.size());
    if (WriteBookkeepingStorage(g_currentSaveSlot.data(), g_saveSlotUploadBuffer.data(), g_saveSlotUploadBuffer.size(), nullptr) != 0)
        return -6;
    WaitForBookkeepingStorage();
    return g_bookkeepingStorageResult;
}

const S1SaveSummary *HPLG_GetSaveSummary(std::size_t slot) {
    return slot < kMaximumSaveSlots ? &g_bookkeepingStatsCache.saveSummaries[slot] : nullptr;
}

void HPLG_SetSaveSummary(std::size_t slot, const S1SaveSummary &summary) {
    if (slot < kMaximumSaveSlots)
        g_bookkeepingStatsCache.saveSummaries[slot] = summary;
}

std::filesystem::path HPLG_SaveSlotPath(std::size_t slot) {
    return std::filesystem::path{"save"} / SaveSlotKey(slot);
}

HLib::Task *g_hotPlugTask = nullptr;

void bupHotPlugTaskStart() {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(bupHotPlugTask_HPLG_Callback, kHotPlugTaskIndex, kHotPlugTaskFlags, kHotPlugTaskToken);
    g_hotPlugTask = task;
    if (!task)
        return;

    if (task->task_data_ptr) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        g_hotPlugTask = nullptr;
        return;
    }

    auto *const data = static_cast<HotPlugTaskData *>(HLib_AllocHeapMemBlock(sizeof(HotPlugTaskData), kTaskDataToken));
    if (!data) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
        g_hotPlugTask = nullptr;
        return;
    }

    HLib_EnableTokenCheck(data);
    std::memset(data, 0, sizeof(*data));
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    data->connected = CurrentPlatformStorageState() == -1 ? 0 : 1;
    RefreshBookkeepingGlobalStatsCache(*data);
    g_bookkeepingStartupMirror = static_cast<std::uint32_t>(data->hasProgress);
}

void HPLG_RegisterBookkeepingListener(BookkeepingRefreshListener listener) {
    HotPlugTaskData *const data = HotPlugData();
    if (!data || !listener)
        return;

    BookkeepingListenerNode *freeNode = nullptr;
    for (BookkeepingListenerNode &node : data->listeners) {
        if (!node.callback) {
            freeNode = &node;
            break;
        }
    }
    if (!freeNode)
        return;

    freeNode->previous = data->listenerHead;
    freeNode->next = nullptr;
    freeNode->callback = listener;
    if (data->listenerHead)
        data->listenerHead->next = freeNode;
    data->listenerHead = freeNode;
    listener(reinterpret_cast<const std::byte *>(&g_bookkeepingStatsCache));
}

std::uint32_t HPLG_GetBookkeepingProgress() {
    return g_bookkeepingStatsCache.aggregateSaveProgress;
}

bool HPLG_HasBookkeepingProgress() {

    return g_bookkeepingProgressOverride != 0 || g_bookkeepingStartupMirror != 0;
}

std::uint32_t HPLG_GetBookkeepingRefreshCount() {
    return g_bookkeepingRefreshCount;
}

bool HPLG_IsStorageConnected() {
    const HotPlugTaskData *const data = HotPlugData();
    return data && data->connected != 0;
}

void HPLG_SetStorageConnected(bool connected) {
    g_bookkeepingStorageConnected = connected;
}

extern std::int32_t g_pauseStateTemp;

void HLib_mainModule(HLib::TaskData *) {
    g_pauseStateTemp = Shenmue_nPauseState & 2;
    Fog_ApplySpecialSettings();
    BeginSceneFrameSubmission();
    if (g_abufferClearPending != 0) {
        ClearEffectABuffer();
        g_abufferClearPending = 0;
    }
    EFFE_ReplayQueuedEffects(0);
    DrawEffectCommandStreamDepthPass();
    BeginSceneCompositionSubmission();
    ComposeEffectABuffer();
    EFFE_ReplayQueuedEffects(1);
    DrawCharacterShadows();
    EFFE_DrawProjectedShadows();
    EFFE_ReplayQueue(0, 0, EffectQueueEntryKind::Run);
    SetupPPFX(DX11::g_ppD3DDeviceContext);
    EFFE_FinishQueuedEffects();
    FinishSceneFrameSubmission();
    if (g_nAuthorState == 0x14) {
        SetNewAuthorGameState(0);
    }
    CAME_HandleRootAuthorTransition(GetCurrentInputType() == InputMode::Vehicle);
    PresentLatestFrame();

    InitializeGlobalInput();
    g_viewportInterpolationProgress = std::fmaxf(0.0f, std::fminf(1.0f, g_bufferPassNotBusy ? g_renderPassProgress : g_depthPassProgress));
    g_bufferPassNotBusy = 0;
    if (g_returnToTitleRequested != 0) {
        g_returnToTitleRequested = 0;
        ReturnToTitleScreen();
    }

    d3t::ProcessMessages();
    UpdateInputsFromInputDevices();
    HandleUICallbacks();
    UpdateRootFrameCounters();
    HLib_UpdateFrameTiming();
    const InputSlot &input = g_inputState.slots[0];
    if (GetSkipCyclesFlag() != 0 && g_bPauseGameTasks != 0 && (input.pressedPriority & 0x0800U) != 0 && (input.held & 0x0606U) == 0x0606U) {
        g_ClearColour = 0xff000000U;
        HLib_UpdateTaskQueue(SystemModule::Audio);
    }
    IncrementRootFrameCopy();
}

} // namespace shenmue::s1
