#include "s1/task/cycle_man_internal.hpp"

namespace shenmue::s1 {
using namespace CycleManager;

// @verified - p
bool SetMOBJTaskPtr() {
#if SHENMUE_IS_PC
    g_pMOBJ = static_cast<std::byte *>(HLib_AllocHeapMemBlock(kMobjCoreAllocationSize - kHLibAllocationHeaderSize, TASK_MOBJ.name));
    if (!g_pMOBJ)
        return false;
    (reinterpret_cast<HLib::TaskData *>(g_pMOBJ) - 1)->alignment |= 1;
    return true;
#else
    g_pMOBJ = g_mobjCoreAllocation.data() + kHLibAllocationHeaderSize;
    return true;
#endif
}

// @verified - p
bool MOBJ_AllocateTaskMemory() {
#if SHENMUE_IS_PC
    g_pMOBJ_HeapHead = static_cast<std::byte *>(HLib_AllocHeapMemBlock(kMobjHeapAllocationSize - kHLibAllocationHeaderSize, TASK_MOBJ.name));
    if (!g_pMOBJ_HeapHead)
        return false;
    (reinterpret_cast<HLib::TaskData *>(g_pMOBJ_HeapHead) - 1)->alignment |= 1;
    return true;
#else
    g_pMOBJ_HeapHead = g_mobjHeapAllocation.data() + kHLibAllocationHeaderSize;
    return true;
#endif
}

// @verified - p
bool MOBJ_SetupTaskPointer() {
#if SHENMUE_IS_PC
    g_tmp_MOBJ = static_cast<std::byte *>(HLib_AllocHeapMemBlock(kMobjDynamicWorkSize, TASK_MOBJ.name));
    if (!g_tmp_MOBJ)
        return false;
    (reinterpret_cast<HLib::TaskData *>(g_tmp_MOBJ) - 1)->alignment |= 1;
    return true;
#else
    g_tmp_MOBJ = g_mobjDynamicAllocation.data() + kHLibAllocationHeaderSize;
    return true;
#endif
}

void MOBJ_Cleanup() {
    if (g_mobjMotionTask) {
        HLib_DestroyTask(g_mobjMotionTask);
        g_mobjMotionTask = nullptr;
    }
    if (g_mobjCoreTask) {
        HLib_DestroyTask(g_mobjCoreTask);
        g_mobjCoreTask = nullptr;
    }

    CycleManager::ReleaseWorldOwnersForTeardown();

    if (g_tmp_MOBJ)
        std::memset(g_tmp_MOBJ, 0, kMobjDynamicWorkSize);
    MOBJ_ResetDynamicRuntimes();

    CycleManager::ClearProgramLists();
    g_cycleManagerRuntime = CycleManager::RuntimeState{};
    g_tmp_cm_path[0] = 0;
    g_pMOBJ = nullptr;
    g_pMOBJ_HeapHead = nullptr;
    g_tmp_MOBJ = nullptr;
}


bool MOBJ_Setup(const char *filename, shen_name areaId) {
    g_cycleManagerRuntime.areaId = areaId;
    CycleManager::Setup(filename);
    return true;
}



void MOBJ_SetEnabled(bool enabled) {
    gCycleManager = enabled ? 1 : 0;
}

void MOBJ_CoreCallback(HLib::TaskData *taskData) {
    CycleManager::CoreCallback_Mo_Exec(taskData);
}

} // namespace shenmue::s1
