#include "s1/task/staf.hpp"

#include "s1/boot.hpp"
#include "s1/launch_options.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/input.hpp"
#include "s1/task/film.hpp"
#include "s1/task/save.hpp"
#include "s1/task/sond.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

void HLTaskFunc_InternalInit();
std::int64_t krepEnqueue(std::uint32_t index);
void HLib_InertTaskCallback(HLib::TaskData *);
std::int64_t SetToDisplayModule(HLib::TaskData *);
void *LoadGzippedSpriteIntoHeapMem(const char *filename, std::int32_t allocationToken, std::uint32_t *decompressedSize);
extern std::int32_t g_actNumber;
extern std::uint8_t g_bDebugLog;
extern std::int32_t g_busy_reset;

namespace {

constexpr std::uint32_t kStaffToken = SHEN_NAME(STAF);
constexpr std::uint32_t kTaskDataToken = SHEN_NAME(TASK);
constexpr std::int32_t kCreditsUiIndex = 0x4d;
constexpr std::uint16_t kStaffFilmIndex = 8;

#pragma pack(push, 1)
struct StaffTaskData {
    const std::byte *cursor;
    const std::byte *sectionBegin;
    const std::byte *sectionCursor;
    const std::byte *sectionEnd;
    std::uint64_t field020;
    HLib::Task *filmTask;
    std::uint64_t field030;
    void *spriteData;
    std::int32_t textureSurface;
    std::uint32_t reserved044;
};
#pragma pack(pop)

static_assert(sizeof(StaffTaskData) == 0x48);
static_assert(offsetof(StaffTaskData, filmTask) == 0x28);
static_assert(offsetof(StaffTaskData, spriteData) == 0x38);
static_assert(offsetof(StaffTaskData, textureSurface) == 0x40);

StaffTaskData *g_staffTaskData = nullptr;
HLib::Task *g_staffTask = nullptr;

void ReleaseStaffResources() {
    if (!g_staffTaskData)
        return;
    if (g_staffTaskData->textureSurface >= 0) {
        PVR_FreeAllocatedSurfaceByIndex(g_staffTaskData->textureSurface);
        g_staffTaskData->textureSurface = -1;
    }
    if (g_staffTaskData->spriteData) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(g_staffTaskData->spriteData), 0);
        g_staffTaskData->spriteData = nullptr;
    }
}

void SetTaskToDisplayModule(HLib::TaskData *taskData) {
    if (g_saveDisplayModuleState != SAVE_DISPLAY_MODULE_NONE) {
        return;
    }

    auto *const data = reinterpret_cast<StaffTaskData *>(taskData);
    const ColourBlendPair fade{{0, 0, 0, 0}, {255, 0, 0, 0}, 30};
    ConstructColours(data ? data->filmTask : nullptr, &fade, ConstructColourMode::ApplyImmediately);
    if (CurrentTask) {
        CurrentTask->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&SetToDisplayModule);
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
        CurrentTask->initCallbackFnPtr = CurrentTask->mainTaskCallbackFnPtr;
        CurrentTask->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
        CurrentTask->mainExecutionIterations = 32;
    }
}



void STAF_Callback(HLib::TaskData *) {
    if (g_creditsScrollProgress <= g_creditsScrollSpeed * 10.0f + ((g_creditsTables[0].height + 0.0f) + g_creditsTables[1].height + g_creditsTables[2].height))
        return;
    UIBaseRuntime &credits = g_uiElements[kCreditsUiIndex];
    if ((g_currUIState == -1 || g_currUIState == kCreditsUiIndex) && (!credits.isVisible || !credits.destroy || credits.destroy() == 1 || g_busy_reset == 1)) {
        credits.isVisible = 0;
    }
    SAVE_OpenDisplayModule(SAVE_DISPLAY_MODULE_SAVE_PROGRESS);
    if (CurrentTask) {
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->mainTaskCallbackFnPtr = SetTaskToDisplayModule;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}

} // namespace

HLib::Task *STAF_GetTask() noexcept {
    if (!g_staffTask || !g_staffTaskData || !g_staffTask->mainTaskCallbackFnPtr ||
        g_staffTask->task_data_ptr != reinterpret_cast<HLib::TaskData *>(g_staffTaskData)) {
        return nullptr;
    }
    return g_staffTask;
}

bool STAF_HasOwnedResources() noexcept {
    return STAF_GetTask() && (g_staffTaskData->spriteData || g_staffTaskData->textureSurface >= 0);
}

void Module_STAF(HLib::TaskData *) {
    if (STAF_GetTask())
        ReleaseStaffResources();
    g_staffTask = nullptr;
    g_staffTaskData = nullptr;
    HLTaskFunc_InternalInit();
    HLib::Task *const film = FILM_Init(kStaffFilmIndex);
    const ColourBlendPair fade{{255, 0, 0, 0}, {0, 0, 0, 0}, 30};
    ConstructColours(film, &fade, ConstructColourMode::ApplyImmediately);
    g_ClearColour = 0xff000000;
    g_bDebugLog = 0;

    if (CurrentTask) {
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->mainTaskCallbackFnPtr = HLib_InertTaskCallback;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
    UIBaseRuntime &credits = g_uiElements[kCreditsUiIndex];
    if (g_currUIState == -1 || g_currUIState == kCreditsUiIndex) {
        if (!credits.isVisible) {
            if (credits.render) {
                credits.render();
                if (credits.tick) {
                    g_waitingForNextTick = 0;
                    credits.tick();
                    g_waitingForNextTick = 1;
                }
            } else if (credits.initialize) {
                credits.initialize();
                if (credits.postInit) {
                    g_waitingForNextTick = 0;
                    credits.postInit();
                    g_waitingForNextTick = 1;
                }
            }
            credits.isVisible = 1;
        }
        credits.isHandledExternally = 0;
    }
    g_forceXInputAnalogRefresh = 1;

    HLib::Task *const staffTask = HLib_EnqueueTaskWithoutParameter(STAF_Callback, 8, 0, kStaffToken);
    StaffTaskData *data = nullptr;
    if (staffTask && !staffTask->task_data_ptr) {
        data = static_cast<StaffTaskData *>(HLib_AllocHeapMemBlock(sizeof(StaffTaskData), kTaskDataToken));
        if (data) {
            HLib_EnableTokenCheck(data);
            staffTask->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
        }
    }
    if (!data && staffTask) {
        staffTask->initCallbackFnPtr = nullptr;
        staffTask->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        staffTask->autoInitTaskCallbackFnPtr = nullptr;
    }
    if (!data)
        return;

    std::memset(data, 0, sizeof(*data));
    data->textureSurface = -1;

    std::uint32_t spriteSize = 0;
    data->spriteData = LoadGzippedSpriteIntoHeapMem("STAFF_S.GZ", kStaffToken, &spriteSize);
    if (data->spriteData && spriteSize >= 32) {
        auto *const bytes = static_cast<std::byte *>(data->spriteData);
        data->textureSurface = LoadTEXN(bytes);
        std::uint32_t firstChunkOffset = 0;
        std::memcpy(&firstChunkOffset, bytes + 4, sizeof(firstChunkOffset));
        if (firstChunkOffset < spriteSize) {
            data->cursor = bytes + firstChunkOffset;
            data->sectionBegin = data->cursor;
            data->sectionCursor = data->cursor;
        }
        const auto *const allocation = static_cast<HLib::TaskData *>(data->spriteData) - 1;
        data->sectionEnd = bytes + allocation->content_size - sizeof(HLib::TaskData) - 32;
    }
    data->filmTask = film;
    g_staffTaskData = data;
    g_staffTask = staffTask;

    sondLoadSndFromDirIntoBank(g_actNumber, 2, "rim004.snd");
    shenExecuteAICACommand(0x9a8, 0, 0);
    shenExecuteAICACommand(0x4a0, 2, 98);
    krepEnqueue(0);
    SAVE_Init(1);
}

} // namespace shenmue::s1
