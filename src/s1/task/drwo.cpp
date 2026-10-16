#include "s1/task/drwo.hpp"
#include "s1/mt5_render.hpp"

#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"

namespace shenmue::s1 {
namespace {

constexpr std::int32_t kDrwoTaskToken = SHEN_NAME(DRWO);
constexpr std::uint32_t kCharacterHandlerToken = SHEN_NAME(CEWP);

HLib::Task *g_drwoTask = nullptr;

MapControlData *CurrentMapControl() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    return reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
}
bool DispatchObject(DRWP_t &renderTask, Character *character) {
    if (!character || character->entity.name != kCharacterHandlerToken || MOTM_GetWork(character)) {
        return false;
    }
    DRWP_DispatchCharacter(character);
    ++renderTask.frameCount;
    return true;
}

} // namespace

HLib::Task *DRWO_GetTask() {
    return g_drwoTask;
}

HLib::Task *DRWO_Init() {
    g_drwoTask = HLib_EnqueueTaskWithoutParameter(CharDrawObject_DRWO_Task_Callback, 12, 2, kDrwoTaskToken);
    return g_drwoTask;
}

void DRWO_Shutdown() {
    if (g_drwoTask)
        HLib_DestroyTask(g_drwoTask);
    g_drwoTask = nullptr;
}

void CharDrawObject_DRWO_Task_Callback(HLib::TaskData *) {
    MapControlData *const control = CurrentMapControl();
    DRWP_t *const renderTask = DRWP_GetWork();
    if (!control || !renderTask || !g_bShouldDraw)
        return;

    // Preserve CTRL's 13-bucket/list order. Motion-owned characters were
    // already handled by DRWP/DRWH; this pass owns the remaining CEWP objects.
    for (void *const head : control->characterBuckets) {
        for (auto *node = static_cast<CharacterBucketNode *>(head); node; node = node->next) {
            DispatchObject(*renderTask, node->character);
        }
    }
}

} // namespace shenmue::s1
