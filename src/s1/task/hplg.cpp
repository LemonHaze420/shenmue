#include "s1/task/hplg.hpp"
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
[[maybe_unused]] std::uint32_t g_bookkeepingStartupMirror = 0;
[[maybe_unused]] std::uint32_t g_bookkeepingGlobalStatsUploadCount = 0;




std::int32_t g_globalStatsUploadGate = 0;



bool g_bookkeepingStorageConnected = true;

enum class PlatformStorageResult : std::int32_t {
    Success = 0,
    Unavailable = -1,
    Pending = -2,
    Failed = -3,
};

HotPlugTaskData *HotPlugData() {
    return g_hotPlugTask && g_hotPlugTask->task_data_ptr ? reinterpret_cast<HotPlugTaskData *>(g_hotPlugTask->task_data_ptr) : nullptr;
}

PlatformStorageResult CurrentPlatformStorageState() {
    return g_bookkeepingStorageConnected ? PlatformStorageResult::Success : PlatformStorageResult::Unavailable;
}

PlatformStorageResult ServicePendingPlatformLoad(PlatformStorageResult result) {
    if (result != PlatformStorageResult::Pending)
        return result;






    return PlatformStorageResult::Unavailable;
}

PlatformStorageResult LoadBookkeepingFromPlatform() {





    const PlatformStorageResult platformState = CurrentPlatformStorageState();
    if (platformState == PlatformStorageResult::Pending)
        return ServicePendingPlatformLoad(platformState);
    return PlatformStorageResult::Unavailable;
}

std::string SaveSlotKey(std::size_t slot) {
    return "SHENMUE1_00" + std::to_string(slot);
}

bool ReadLocalSaveSlot(std::size_t slot, S1SaveImage &image) {
    const std::string key = SaveSlotKey(slot);
    const std::string shortSlot = std::to_string(slot);
    const std::array<std::filesystem::path, 5> paths{
        std::filesystem::path{"save"} / key,
        std::filesystem::path{"save"} / (key + ".bin"),
        slot == 0 ? std::filesystem::path{"save/s1_new_game.bin"} : std::filesystem::path{},
        std::filesystem::path{"save"} / ("s1_00" + shortSlot + ".bin"),
        std::filesystem::path{"save"} / ("s1_slot_" + shortSlot + ".bin"),
    };

    for (const std::filesystem::path &path : paths) {
        if (path.empty())
            continue;
        if (S1Save_Read(path, image) == S1SaveResult::Success)
            return true;
    }
    return false;
}

void StageBookkeepingGlobalStatsUploadBuffer() {
    // UploadBookkeepingGlobalStats copies exactly 0x1804 bytes and then
    // dispatches Steam_Callbacks(..., "BOOKKEEP", 0x1804).  Keep the staging
    // and gate semantics game-owned; the final Steam dispatch is intentionally
    // isolated because that third-party adapter is unavailable.
    std::memcpy(g_bookkeepingGlobalStatsUploadBuffer.data(), &g_bookkeepingStatsCache, g_bookkeepingGlobalStatsUploadBuffer.size());
    ++g_bookkeepingGlobalStatsUploadCount;
}

void RefreshBookkeepingGlobalStatsCache(HotPlugTaskData &data) {
    ++g_bookkeepingRefreshCount;
    std::memset(data.refreshScratch, 0, sizeof(data.refreshScratch));
    std::memset(&g_bookkeepingStatsCache, 0, sizeof(g_bookkeepingStatsCache));
    g_playerStats.currentSaveProgress = 0;





    const bool bookkeepingLoaded = LoadBookkeepingFromPlatform() == PlatformStorageResult::Success;
    bool slotStatsLoaded = bookkeepingLoaded;
    bool shouldUploadGlobalStats = false;

    for (std::size_t slot = 0; slot < kMaximumSaveSlots; ++slot) {
        S1SaveImage image{};
        if (!ReadLocalSaveSlot(slot, image))
            continue;

        const auto *const bytes = image.bytes.data();
        std::memcpy(&g_bookkeepingStatsCache.saveSummaries[slot], bytes + kS1SaveSummaryOffset, kSaveSummarySize);



        if (!slotStatsLoaded) {
            std::memcpy(g_bookkeepingStatsCache.globalStatsMirror.data(), bytes + kS1SaveGlobalStatsOffset, kGlobalStatsSize);
            slotStatsLoaded = true;
        }



        if (g_globalStatsUploadGate == 0) {
            g_bookkeepingStatsCache.aggregateSaveProgress =
                static_cast<std::uint8_t>(g_bookkeepingStatsCache.aggregateSaveProgress + bytes[kInternalProgressOffset]);
            std::uint32_t settingsFlags = 0;
            std::memcpy(&settingsFlags, bytes + kInternalSettingsFlagsOffset, sizeof(settingsFlags));
            S1SettingsFlags_Set(settingsFlags);
            shouldUploadGlobalStats = true;
        }
    }

    g_playerStats.currentSaveProgress = g_bookkeepingStatsCache.aggregateSaveProgress;
    g_bookkeepingStatsCache.refreshStatus.isLoaded = 1;
    g_bookkeepingStatsCache.refreshStatus.hasProgress = g_bookkeepingStatsCache.aggregateSaveProgress != 0 ? 1U : 0U;
    data.hasProgress = static_cast<std::int32_t>(g_bookkeepingStatsCache.refreshStatus.hasProgress);

    if (shouldUploadGlobalStats)
        StageBookkeepingGlobalStatsUploadBuffer();

    for (BookkeepingListenerNode *node = data.listenerHead; node; node = node->previous) {
        if (node->callback) {
            node->callback(reinterpret_cast<const std::byte *>(&g_bookkeepingStatsCache));
        }
    }
}

void bupHotPlugTask_HPLG_Callback(HLib::TaskData *taskData) {
    auto *const data = reinterpret_cast<HotPlugTaskData *>(taskData);
    if (!data)
        return;

    const std::int32_t connected = CurrentPlatformStorageState() == PlatformStorageResult::Unavailable ? 0 : 1;
    if (connected == data->connected)
        return;
    data->connected = connected;
    RefreshBookkeepingGlobalStatsCache(*data);
}

} // namespace



std::int32_t UploadBookkeepingGlobalStats() {
    StageBookkeepingGlobalStatsUploadBuffer();
    return 0;
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
    data->connected = CurrentPlatformStorageState() == PlatformStorageResult::Unavailable ? 0 : 1;
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
    const HotPlugTaskData *const data = HotPlugData();
    return data && data->hasProgress != 0;
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
