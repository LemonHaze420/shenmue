#include "s1/task/drwh.hpp"

#include "s1/task/char.hpp"
#include "s1/lighting.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"

#include <algorithm>
#include <cstddef>

namespace shenmue::s1 {

extern std::int32_t g_drawMode;

namespace {

constexpr std::int32_t kDrwhTaskToken = SHEN_NAME(DRWH);
constexpr std::int32_t kMaximumVisibleHumans = 32;
constexpr std::uint32_t kRestoredLightMask = 4U;

HLib::Task *g_drwhTask = nullptr;

MapControlData *CurrentMapControl() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;

    return reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
}

void InsertHuman(DRWP_t &renderTask, Character *character) {
    if (!character || character == g_CEWP_Ptr_0 || !MOTM_GetWork(character) || (character->motionFlags & 0x3fU) != 0x3fU || character->renderOpacity == 0.0f) {
        return;
    }

    const float renderDistance = character->viewPosition.z < 0.0f ? character->viewDistance : 1000.0f;
    const std::int32_t maximum = std::clamp(g_MaxEntitiesPerType, 0, kMaximumVisibleHumans);
    const std::int32_t count = std::clamp(renderTask.entityCount, 0, maximum);




    std::int32_t insertion = 0;
    while (insertion < count && renderTask.entitySlots[insertion].renderDistance <= renderDistance) {
        ++insertion;
    }
    for (std::int32_t index = count; index > insertion; --index) {
        renderTask.entitySlots[index] = renderTask.entitySlots[index - 1];
    }
    renderTask.entitySlots[insertion].renderDistance = renderDistance;
    renderTask.entitySlots[insertion].character = character;
    renderTask.entityCount = std::min(count + 1, maximum);
    ++renderTask.acceptedCandidateCount;
}

void DispatchHumans(DRWP_t &renderTask, bool drawMode) {
    const std::int32_t maximum = std::clamp(g_MaxEntitiesPerType, 0, kMaximumVisibleHumans);
    const std::int32_t count = std::clamp(renderTask.entityCount, 0, maximum);

    for (std::int32_t index = 0; index < count; ++index) {
        Character *const character = renderTask.entitySlots[index].character;
        DRWP_DispatchCharacter(character);
        if (drawMode && g_restoreCharacterLightMask != 0) {
            if (character)
                character->lightMask |= kRestoredLightMask;
            else
                g_defaultLightMask = kRestoredLightMask;
        }
    }
}

} // namespace

HLib::Task *DRWH_GetTask() {
    return g_drwhTask;
}

HLib::Task *DRWH_Init() {
    g_drwhTask = HLib_EnqueueTaskWithoutParameter(CharDrawHuman_DRWH_Task_Callback, 12, 2, kDrwhTaskToken);
    return g_drwhTask;
}

void DRWH_Shutdown() {
    if (g_drwhTask)
        HLib_DestroyTask(g_drwhTask);
    g_drwhTask = nullptr;
}

void CharDrawHuman_DRWH_Task_Callback(HLib::TaskData *) {
    MapControlData *const control = CurrentMapControl();
    DRWP_t *const renderTask = DRWP_GetWork();
    if (!control || !renderTask || !g_bShouldDraw)
        return;

    for (void *const head : control->characterBuckets) {
        for (auto *node = static_cast<CharacterBucketNode *>(head); node; node = node->next) {
            Character *const character = node->character;
            if (!character) {
                continue;
            }
            InsertHuman(*renderTask, character);
        }
    }

    const bool drawMode = g_drawMode != 0;
    if (drawMode)
        charaEvaluateLightOverride();
    DispatchHumans(*renderTask, drawMode);
    if (drawMode)
        charaRestoreLightOverride();
}

} // namespace shenmue::s1
