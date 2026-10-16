#include "s1/task/modu.hpp"
#include "s1/boot.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/input.hpp"
#include "s1/task/film.hpp"
#include "s1/subtitles.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/scrl.hpp"
#include "s1/ui_runtime.hpp"

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
namespace shenmue::s1 {
namespace CycleManager {


void ResetPublishedSceneState();
} // namespace CycleManager

extern std::int32_t g_actNumber;
extern std::int32_t g_busy_reset;
extern std::array<std::uint32_t, 5> g_saveDataRelated;

std::uint32_t g_moduleInputActivityFlags = 0;
std::uint32_t g_moduleInputActivityFrames = 0;
std::uint32_t g_moduleFrameCount = 0;
std::int32_t g_ctrlInitializedFrameCount = 0;
bool g_ctrlInitialized = false;
std::int32_t g_moduleInputSuppressFrames = 0;

namespace {

void UpdateModuleInputBookkeeping() {
    const InputSlot &input = g_inputState.slots[0];
    std::uint32_t flags = g_moduleInputActivityFlags & ~0x1fU;
    if ((input.held & 0xf000U) != 0)
        flags |= 1U;
    if ((input.held & 0x0040U) != 0)
        flags |= 2U;
    if ((input.held & 0x0080U) != 0)
        flags |= 4U;

    const std::uint8_t rightX = static_cast<std::uint8_t>(input.rightStick & 0xffU);
    const std::uint8_t rightY = static_cast<std::uint8_t>(input.rightStick >> 8U);
    if (rightX < 126U || rightX > 130U || rightY < 126U || rightY > 130U) {
        flags |= 8U;
    }
    if (SCEN_IsConversationCameraActive())
        flags |= 0x10U;
    g_moduleInputActivityFlags = flags;

    if (g_isPlayerGameStateActive != 0 && !g_notebookHelpOverrideActive && flags != 0) {
        ++g_moduleInputActivityFrames;
        for (std::size_t index = 0; index < 4; ++index)
            --g_saveDataRelated[index];
    }
}

void CloseSubtitleOwner() {
    if (!g_pCVWK)
        return;

    constexpr std::size_t kSubtitleUiIndex = 0x3a;
    if (g_currUIState == -1 || g_currUIState == static_cast<std::int32_t>(kSubtitleUiIndex)) {
        UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiIndex];
        if (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1) {
            subtitles.isVisible = 0;
        }
    }

    for (CVWKRuntimeSlot &slot : g_pCVWK->slots) {
        if (slot.callback)
            SEQ_FreeFileHandles(slot);
    }
}

bool UsesTemporaryClearColour(SystemModule module) {
    return module == SystemModule::Map || module == SystemModule::Harrier || module == SystemModule::HangOn;
}

} // namespace
void MODU_Callback(HLib::TaskData *) {
    if (--g_moduleInputSuppressFrames >= 0) {
        InputSlot &input = g_inputState.slots[0];
        input.triggers = 0;
        input.rightStick = 0x8080U;
        input.held = 0;
        input.previousHeld = 0;
        input.pressedPriority = 0;
        input.repeatedPresses = 0;
        input.leftStick &= 0x0000ffffU;
    } else {
        g_moduleInputSuppressFrames = 0;
    }

    UpdateModuleInputBookkeeping();

    const auto *const control = reinterpret_cast<const MapControlData *>(g_mapControlTask ? g_mapControlTask->task_data_ptr : nullptr);
    if ((control->flags & 1U) != 0) {
        g_sceneId = control->sceneId;
        g_areaId = control->areaId;
        g_entryPoint = control->entryPoint;
        g_actNumber = g_sceneId;
        const SystemModule module = control->module;

        CloseSubtitleOwner();

        if (CurrentTask) {
            CurrentTask->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            HLib::Task *const child = CurrentTask->child;
            if (child) {
                child->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
                if (child->child)
                    MODU_DestroyAllTasks(child->child);
                if (child->nextSibling)
                    MODU_DestroyAllTasks(child->nextSibling);
            }
        }

        g_ClearColour = 0xff000000U;
        if (g_shouldUseTemporaryClearColour && UsesTemporaryClearColour(module)) {
            g_ClearColour = (static_cast<std::uint32_t>(g_temporaryClearColour.red) << 24U) |
                (static_cast<std::uint32_t>(g_temporaryClearColour.green) << 16U) | (static_cast<std::uint32_t>(g_temporaryClearColour.blue) << 8U) |
                static_cast<std::uint32_t>(g_temporaryClearColour.alpha);
        }
        HLib_UpdateTaskQueue(module);
    }

    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    if (g_ctrlInitialized) {
        ++g_moduleFrameCount;
        if (g_ctrlInitializedFrameCount != -1)
            ++g_ctrlInitializedFrameCount;
    }
}

void MODU_Script_shenRequestGameModule(ScenarioContext *, void *rawParams) {
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    LoadGameModule(static_cast<SystemModule>(params[0]), static_cast<std::int32_t>(params[1]), params[2], params[3]);
}

} // namespace shenmue::s1
