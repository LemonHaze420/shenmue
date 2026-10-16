#include "s1/task/dwmz.hpp"

#include "common/types.hpp"

#include <cstdint>

namespace shenmue::s1 {

void HLib_InertTaskCallback(HLib::TaskData *taskData);

namespace {

constexpr std::int16_t kDwmzQueue = 13;
constexpr std::uint16_t kDwmzOrder = 6;
constexpr std::int32_t kDwmzToken = static_cast<std::int32_t>(SHEN_NAME(DWMZ));
static_assert(kDwmzToken == SHEN_NAME(DWMZ));

HLib::Task *g_dwmzTask = nullptr;

} // namespace


HLib::Task *DWMZ_Init() {
    g_dwmzTask = HLib_EnqueueTaskWithoutParameter(HLib_InertTaskCallback, kDwmzQueue, kDwmzOrder, kDwmzToken);
    return g_dwmzTask;
}

void DWMZ_Destroy() {
    if (!g_dwmzTask)
        return;
    HLib_DestroyTask(g_dwmzTask);
    g_dwmzTask = nullptr;
}

} // namespace shenmue::s1
