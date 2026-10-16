#include "s1/task/colb.hpp"

#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/screen_space.hpp"
#include "s1/texture_loader.hpp"

#include <bit>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

void HLib_InertTaskCallback(HLib::TaskData *taskData);

namespace {

struct ColourBarTaskData {
    std::uint32_t texture;
    OriginalStrip strip;
};

static_assert(sizeof(ColourBarTaskData) == 0x5c);
static_assert(offsetof(ColourBarTaskData, strip) == 4);

void SetClearColourForAspectRatioBars(HLib::TaskData *) {
    g_ClearColour = 0xff00af00;
}

} // namespace

void InitColBar(HLib::TaskData *) {
    if (CurrentTask) {
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->mainTaskCallbackFnPtr = SetClearColourForAspectRatioBars;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }

    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(HLib_InertTaskCallback, 15, 0, SHEN_NAME(COLB));
    ColourBarTaskData *data = nullptr;
    if (task && !task->task_data_ptr) {
        data = static_cast<ColourBarTaskData *>(HLib_AllocHeapMemBlock(sizeof(ColourBarTaskData), SHEN_NAME(TASK)));
        if (data) {
            HLib_EnableTokenCheck(data);
            task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
        }
    }
    if (!data && task) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
    }

    data->strip.flags = 0;
    data->strip.corner0 = 0xffffffff;
    data->strip.vertexCount = 0;
    data->strip.normalizedY1 = 0.0f;
    data->strip.normalizedX2 = 0.0f;
    data->strip.normalizedY3 = 0.0f;
    data->strip.normalizedX3 = 1.0f;
    data->strip.normalizedY2 = 1.0f;
    AdjustAspectRatioMatrix(&data->strip, 0, 0, 0, 1024, 512, 4);

    void *const file = ReadFile("sprite", "COLBAR.SPR", SHEN_NAME(COLB));
    data->texture = static_cast<std::uint32_t>(LoadTEXN(file));
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
}

} // namespace shenmue::s1
