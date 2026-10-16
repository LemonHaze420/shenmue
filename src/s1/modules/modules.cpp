#include "logging.hpp"
#include "s1/boot.hpp"
#include "s1/launch_options.hpp"
#include "s1/save_format.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/input.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/save.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct AudioModuleContext {
    std::byte reserved[8];
    std::uint32_t mode;
};

static_assert(sizeof(AudioModuleContext) == 0xc);
static_assert(offsetof(AudioModuleContext, mode) == 0x8);

void InitMain();
void PauseGameAudioSequence();

void *VFMA_TaskPtr = nullptr;
extern std::int32_t g_busy_reset;

std::uint32_t g_ClearColour = 0;

void HLib_InertTaskCallback(HLib::TaskData *) {
}

std::int64_t Shenmue_Init(HLib::TaskData *) {
    InitMain();
    return HLib_UpdateTaskQueue(SystemModule::Audio);
}

std::int64_t SetToDisplayModule(HLib::TaskData *) {
    return HLib_UpdateTaskQueue(SystemModule::Display);
}

std::int64_t Audio_Module(HLib::TaskData *taskData) {
    PauseGameAudioSequence();
    VFMA_TaskPtr = nullptr;
    if (g_mapLaunchOptions.active) {
        const std::int32_t requestedScene = g_mapLaunchOptions.sceneId;
        const std::uint32_t requestedArea = static_cast<std::uint32_t>(g_mapLaunchOptions.areaId.name);
        const std::uint32_t requestedEntry = g_mapLaunchOptions.entryPoint;





        g_sceneId = 1;
        g_areaId = AREA_OPENING.name;
        g_entryPoint = 0;
        shenSetupPlayerDefaultsNewGame();
        g_isOnMainMenu = 0;
        HLTaskFunc_InternalWeatherInit(1);

        g_sceneId = requestedScene;
        g_areaId = requestedArea;
        g_entryPoint = requestedEntry;
        return HLib_UpdateTaskQueue(SystemModule::Map);
    }
    const auto *const context = reinterpret_cast<const AudioModuleContext *>(taskData);
    return HLib_UpdateTaskQueue(context->mode == 1 ? SystemModule::SetToDisplay : SystemModule::Display);
}

void HLib_SetCurrentTaskToNULL(HLib::TaskData *) {
    g_ClearColour = 0xff00af00;
    if (CurrentTask) {
        CurrentTask->mainTaskCallbackFnPtr = HLib_InertTaskCallback;
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}

} // namespace shenmue::s1
