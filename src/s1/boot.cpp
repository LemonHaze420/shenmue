#include "s1/boot.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/colb.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/staf.hpp"
#include "s1/task/hplg.hpp"
#include "s1/task/pack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/task/vfma.hpp"
#include "s1/audio_stream.hpp"
#include "s1/scene.hpp"
#include "s1/subtitles.hpp"

#include <bit>

namespace shenmue::s1 {

std::int64_t Shenmue_Init(HLib::TaskData *);
std::int64_t SetToDisplayModule(HLib::TaskData *);
std::int64_t Audio_Module(HLib::TaskData *);
void HLib_SetCurrentTaskToNULL(HLib::TaskData *);
void HLib_InertTaskCallback(HLib::TaskData *);
void HarrierModule(HLib::TaskData *);
void HangOnCreate(HLib::TaskData *);

namespace {

constexpr std::int32_t kModuleToken = SHEN_NAME(MODU);
constexpr std::int64_t kModuleReservedData = 0x0cf00000;
constexpr std::int64_t kUnusedModuleReservedData = 0x0cfc0000;

ModuleTaskData g_rootTaskData{};
ModuleData g_rootModuleData{};

} // namespace

std::array<SystemModuleEntry, 22> SystemModules{{
    {InitColBar, 0, kModuleReservedData},
    {std::bit_cast<HLib::TaskCallback>(&Shenmue_Init), 0, kModuleReservedData},
    {std::bit_cast<HLib::TaskCallback>(&SetToDisplayModule), 0, kModuleReservedData},
    {std::bit_cast<HLib::TaskCallback>(&Audio_Module), 0, kModuleReservedData},
    {DISP_Module, 0, kModuleReservedData},
    {HLib_SetCurrentTaskToNULL, 0, kModuleReservedData},
    {HLib_InertTaskCallback, 0, kModuleReservedData},
    {MapControlTask_Init, 0, kModuleReservedData},
    {HLib_InertTaskCallback, 0, kModuleReservedData},
    {HarrierModule, 0, kModuleReservedData},
    {Module_STAF, 0, kModuleReservedData},
    {HangOnCreate, 0, kModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
    {nullptr, 0, kUnusedModuleReservedData},
}};

HLib::Task *g_pModuleTask = nullptr;
std::int32_t g_isRenderSuppressed = 0;

std::int32_t g_perspectiveProjection = 1;
std::int32_t Shenmue_nNumVBLANKs = 0;
std::int32_t Shenmue_nNumRenderedFrames = 0;
std::int32_t Shenmue_nPauseState = 0;
std::int64_t g_TotalSceneFileReadBytes = 0;
CVRSTable *g_CVRS = nullptr;
CVWK *g_pCVWK = nullptr;
std::uint32_t g_EnvPrngSeed = 0xec47094dU; // 1

void HLTaskFunc_InternalInit();

void ROOT_Callback(HLib::TaskData *) {
    HLib::Task *const moduleTask = HLib_EnqueueTaskWithoutParameter(SystemModules[static_cast<std::size_t>(SystemModule::Init)].callback, 0, 2, kModuleToken);
    g_pModuleTask = moduleTask;
    g_rootTaskData.data = &g_rootModuleData;
    if (moduleTask && !moduleTask->task_data_ptr) {
        moduleTask->task_data_ptr = reinterpret_cast<HLib::TaskData *>(&g_rootTaskData);
    }

    g_rootTaskData.currentModule = SystemModule::Init;
    g_rootTaskData.nextModule = SystemModule::Colbar;
    if (CurrentTask) {
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->mainTaskCallbackFnPtr = HLib_mainModule;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}

std::int64_t HLib_UpdateTaskQueue(SystemModule module) {
    const auto moduleIndex = static_cast<std::uint32_t>(module);
    if (module == SystemModule::Map) {
        g_perspectiveProjection = 0;
        g_isRenderSuppressed = 0;
    } else {
        g_isRenderSuppressed = moduleIndex == 2 || moduleIndex == 4;
    }
    if (moduleIndex >= SystemModules.size()) {
        return 0;
    }

    if (g_TaskInfo.bTaskPauseEnqueue) {
        HLib_UnpauseTaskEnqueue();
    }
    if (g_pModuleTask) {
        g_pModuleTask->autoInitTaskCallbackFnPtr = nullptr;
        g_pModuleTask->initCallbackFnPtr = InitMemory_callback;
        g_pModuleTask->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
        g_pModuleTask->mainExecutionIterations = 1;
    }
    Shenmue_nNumRenderedFrames = Shenmue_nNumVBLANKs;

    auto *const data = g_pModuleTask ? reinterpret_cast<ModuleTaskData *>(g_pModuleTask->task_data_ptr) : nullptr;
    data->previousModule = data->currentModule;
    data->previousNextModule = static_cast<std::uint32_t>(data->nextModule);
    data->currentModule = module;
    data->nextModule = SystemModule::Colbar;

    for (std::int16_t taskIndex = 1; taskIndex < 16; ++taskIndex) {
        HLib::Task *task = g_TaskInfo.moduleTaskQueue[taskIndex - 1]->nextInQueue;
        while (task->taskIndex == taskIndex) {
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task = task->nextInQueue;
        }
    }
    Shenmue_nPauseState = 0;
    return 1;
}

void InitMemory_callback(HLib::TaskData *taskData) {
    HLTaskFunc_InternalInit();
    InitSceneDataWrapper();
    for (std::uint32_t index = 0; index < 65536; ++index) {
        if (g_Scene.entries[index]) {
            D3T_RemoveSceneNodeWorldTransform(g_Scene.entries[index]);
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(g_Scene.entries[index]), SHEN_NAME(PAKM));
            FreeSceneSpace(index);
        }
    }

    HLib_FreeUseSpaces();
    MT5_ResetModelOwnershipRegistry();
    g_TotalSceneFileReadBytes = 0;
    g_CVRS = nullptr;
    g_pCVWK = 0;

    auto *const moduleData = reinterpret_cast<ModuleTaskData *>(taskData);
    if (CurrentTask) {
        CurrentTask->mainTaskCallbackFnPtr = SystemModules[static_cast<std::size_t>(moduleData->currentModule)].callback;
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}

int Shenmue_Main(int argc, char **argv) {
    const int initialized = Initialization(argc, argv);
    if (!initialized) {
        return initialized;
    }

    HLib::Task *task = CurrentTask;
    for (;;) {
        task->mainTaskCallbackFnPtr(task->task_data_ptr);
        task = CurrentTask->nextInQueue;
        CurrentTask = task;
    }
}

} // namespace shenmue::s1
