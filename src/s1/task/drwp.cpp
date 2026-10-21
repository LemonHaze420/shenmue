#include "s1/task/drwp.hpp"

#include "s1/task/char.hpp"
#include "s1/lighting.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwh.hpp"
#include "s1/task/drwo.hpp"

#include <bit>
#include <cstring>

namespace shenmue::s1 {

HLib::Task *g_DRWP_MainTask = nullptr;
std::int32_t g_MaxEntitiesPerType = 5;
std::int32_t g_bShouldDraw = 1;
Character *g_CEWP_Ptr_0 = nullptr;
std::int32_t g_bDisableGlobalLightingAdjustment = 0;
std::int32_t g_restoreCharacterLightMask = 0;
float g_fLightingIntensity = 0.0f;

namespace {
Vector4 g_tmpLightColour{};
Vector4 g_globalLightColour{};
Vector3 g_globalLightDirection{};
}

void UpdateGlobalLighting() {
    g_bDisableGlobalLightingAdjustment = g_cachedGlobalLightingAdjustFlag;
    g_tmpLightColour = g_sceneLightingBaseColour;
    const ShenmueLightSlot *const light = LGHT_GetDataSlot(0);
    if (!light) {
        g_globalLightColour = {};
        g_fLightingIntensity = 0.0f;
        g_globalLightDirection = {};
        return;
    }

    g_globalLightColour = {light->colourScale, light->colourR, light->colourG, light->colourB};
    g_fLightingIntensity = light->intensity * 1.5f;
    const auto yaw = static_cast<std::uint16_t>(g_fCamAccumYaw);
    std::int16_t folded = static_cast<std::int16_t>(-yaw);
    if (yaw <= 0x8000U)
        folded = static_cast<std::int16_t>(yaw);
    g_globalLightDirection.x = sin_fixed16(static_cast<std::int16_t>(yaw)) * -0.70710701f;
    g_globalLightDirection.y = -0.70710701f;
    g_globalLightDirection.z = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)) * 0.70710701f;
}

void charaEvaluateLightOverride() {
    if (g_bDisableGlobalLightingAdjustment == 1)
        return;

    const float y = g_tmpLightColour.y * 1.25f;
    const float z = g_tmpLightColour.z * 1.25f;
    const float w = g_tmpLightColour.w * 1.25f;
    g_sceneLightingBaseColour = {g_tmpLightColour.x, y, z, w};
    g_sceneLightingActiveColour = g_sceneLightingBaseColour;
}

void charaRestoreLightOverride() {
    if (g_bDisableGlobalLightingAdjustment == 1)
        return;

    g_cachedGlobalLightingAdjustFlag = g_bDisableGlobalLightingAdjustment;
    g_effectsNeedInitialization = 1;
    g_sceneLightingBaseColour = g_tmpLightColour;
    g_sceneLightingActiveColour = g_tmpLightColour;
}

namespace {

constexpr std::int32_t kTaskDataToken = SHEN_NAME(TASK);
constexpr std::int32_t kDrwpTaskToken = SHEN_NAME(DRWP);
constexpr std::uint32_t kCharacterHandlerToken = SHEN_NAME(CEWP);
constexpr std::int32_t kNativeMaximumHumanDraws = 5;

MapControlData *CurrentMapControl() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;

    return reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
}

void ConfigureTaskForDestruction(HLib::Task *task) {
    if (!task)
        return;

    task->initCallbackFnPtr = nullptr;
    task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
    task->autoInitTaskCallbackFnPtr = nullptr;
}

} // namespace

HLib::Task *DRWP_GetTask() {
    return g_DRWP_MainTask;
}

DRWP_t *DRWP_GetWork() {
    return g_DRWP_MainTask && g_DRWP_MainTask->task_data_ptr ? reinterpret_cast<DRWP_t *>(g_DRWP_MainTask->task_data_ptr) : nullptr;
}

bool DRWP_IsRetainedCharacter(const Character *character) {
    if (!character || character->entity.name != kCharacterHandlerToken)
        return false;
    const std::uint32_t flags = character->flags.value;
    return (flags & CharacterFlagUnknown) != 0U || ((flags & 2U) != 0U && (character->motionFlags & 1U) != 0U);
}

void DRWP_DispatchCharacter(Character *character) {
    if (character) {
        for (CharacterComponentNode *component = character->componentList; component; component = component->next) {

            if (component->drawCallback)
                component->drawCallback(character, static_cast<void *>(component + 1));
        }
    }
    Character_SubmitForDrawing(character);
}

void CharDraw_DRWP(HLib::TaskData *taskData) {
    auto *const renderTask = reinterpret_cast<DRWP_t *>(taskData);
    MapControlData *const control = CurrentMapControl();
    if (!renderTask || !control)
        return;

    Character *const ryo = static_cast<Character *>(control->ryo);
    g_CEWP_Ptr_0 = ryo;
    if (!g_bShouldDraw)
        return;

    renderTask->entityCount = 0;
    renderTask->acceptedCandidateCount = 0;
    renderTask->frameCount = 0;
    UpdateGlobalLighting();
    charaEvaluateLightOverride();

    if (g_bDisableGlobalLightingAdjustment != 1) {
        const std::uint32_t lightMask = ryo ? ryo->lightMask : 0U;
        g_restoreCharacterLightMask = static_cast<std::int32_t>((lightMask >> 2U) & 1U);
        if (ryo) {
            ryo->lightMask = lightMask & ~4U;
        } else {
            g_defaultLightMask = lightMask & ~4U;
        }
        LGHT_SetRenderIntensity(0, g_fLightingIntensity);
    }

    DRWP_DispatchCharacter(ryo);
    if (g_restoreCharacterLightMask != 0) {
        if (g_CEWP_Ptr_0)
            g_CEWP_Ptr_0->lightMask |= 4U;
        else
            g_defaultLightMask = 4U;
    }
    charaRestoreLightOverride();
    ++renderTask->acceptedCandidateCount;
}

HLib::Task *DRWP_Init() {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(CharDraw_DRWP, 12, 2, kDrwpTaskToken);
    if (!task || task->task_data_ptr) {
        ConfigureTaskForDestruction(task);
        g_DRWP_MainTask = nullptr;
        return nullptr;
    }

    auto *const data = static_cast<DRWP_t *>(HLib_AllocHeapMemBlock(sizeof(DRWP_t), kTaskDataToken));
    if (!data) {
        ConfigureTaskForDestruction(task);
        g_DRWP_MainTask = nullptr;
        return nullptr;
    }

    HLib_EnableTokenCheck(data);
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    g_DRWP_MainTask = task;
    return task;
}

HLib::Task *Rendering_n_Init() {
    // Equal queue/order tasks execute in enqueue order: Ryo owner, retained
    // motion humans, then non-motion CEWP objects.
    HLib::Task *const drwp = DRWP_Init();
    DRWH_Init();
    DRWO_Init();
    if (drwp && drwp->task_data_ptr) {
        std::memset(drwp->task_data_ptr, 0, sizeof(DRWP_t));
    }

    g_MaxEntitiesPerType = kNativeMaximumHumanDraws;
    SetCharacterDrawMode(0, 0);
    SetCharacterDrawMode(1, 0);
    return drwp;
}

void Rendering_n_Shutdown() {
    // Consumers must leave the queue before the shared DRWP payload owner.
    DRWO_Shutdown();
    DRWH_Shutdown();
    if (g_DRWP_MainTask)
        HLib_DestroyTask(g_DRWP_MainTask);
    g_DRWP_MainTask = nullptr;
}

} // namespace shenmue::s1
