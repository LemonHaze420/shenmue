#include "s1/task/sygu.hpp"

namespace shenmue::s1 {
void HLib_InertTaskCallback(HLib::TaskData *);
namespace {

constexpr std::int32_t kSyguToken = static_cast<std::int32_t>(SHEN_NAME(SYGU));
HLib::Task *g_syguTask = nullptr;

} // namespace

HLib::Task *EnqueueSYGU() {
    if (g_syguTask && g_syguTask->mainTaskCallbackFnPtr && g_syguTask->taskToken == static_cast<std::int32_t>(kSyguToken))
        return g_syguTask;

    g_syguTask = HLib_EnqueueTaskWithoutParameter(HLib_InertTaskCallback, 8, 0, kSyguToken);
    return g_syguTask;
}

void SYGU_Destroy() {
    HLib::Task *const task = g_syguTask;
    g_syguTask = nullptr;
    if (task)
        HLib_DestroyTask(task);
}

} // namespace shenmue::s1
