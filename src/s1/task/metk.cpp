#include "s1/task/metk.hpp"

#include "s1/task/mmtk.hpp"
#include "common/types.hpp"

#include <cstdint>

namespace shenmue::s1 {
namespace {

constexpr std::int32_t kMetkToken = static_cast<std::int32_t>(SHEN_NAME(METK));

HLib::Task *g_metkTask = nullptr;

} // namespace

void METK_Callback(HLib::TaskData *) {
    if (MMTK_HasScriptSession() && ProcessNotebookStates()) {
        shenEndNotebook();
    }
}

HLib::Task *METK_Init() {
    if (g_metkTask && g_metkTask->mainTaskCallbackFnPtr == METK_Callback) {
        return g_metkTask;
    }
    g_metkTask = nullptr;

    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(METK_Callback, 15, 0, kMetkToken);
    if (!task || task->task_data_ptr) {
        if (task)
            HLib_DestroyTask(task);
        return nullptr;
    }
    g_metkTask = task;
    return task;
}

bool EnqueueMETK() {
    MMTK_ResetForEnqueue();
    if (!MMTK_Init())
        return false;
    if (!METK_Init()) {
        MMTK_Shutdown();
        return false;
    }
    return true;
}

void DestroyMETK() {
    HLib::Task *const task = g_metkTask;
    g_metkTask = nullptr;
    if (task && task->mainTaskCallbackFnPtr == METK_Callback) {
        HLib_DestroyTask(task);
    }
    MMTK_Shutdown();
}

HLib::Task *METK_GetTask() {
    return g_metkTask && g_metkTask->mainTaskCallbackFnPtr == METK_Callback ? g_metkTask : nullptr;
}

} // namespace shenmue::s1
