#include "s2/event_cloth.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

// @verified - p
void CreateClothControlTask() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ET_ClothControl_Callback, 5, 0x0b, sizeof(ClothControlTaskData), eventTaskToken);
    ClothControlTaskData *const work = static_cast<ClothControlTaskData *>(HLib::GetTaskParameter(task));
    work->entries = EVENT_CLOTH_PARAMETERS;
    work->task = task;
    work->frame = 0;
    EV_debug_log("common\\et_cloth.c", 40, "*** CreateClothControlTask\n");
}

// @verified - p
void ET_ClothControl_Callback(void *data) {
    ClothControlTaskData &work = *static_cast<ClothControlTaskData *>(data);
    const ClothControlParamEntry *entry = work.entries;
    for (; entry->frame == work.frame; ++entry) {
        if (!DoesScenarioCharacterExist(entry->character))
            continue;
        SetClothFlag(FindCharacterByScenarioName(entry->character), static_cast<std::uint8_t>(entry->flag), entry->enabled);
        EV_debug_log("common\\et_cloth.c", 59, "*** SetClothFlag:%x %x %x\n", entry->character.name, entry->flag, entry->enabled);
    }
    ++work.frame;
    work.entries = entry;
    if (entry->frame < 0 || entry->character.name == EVENT_CLOTH_END.name || work.frame > 60000)
        HLib::DestroyTaskInternal(work.task);
}

} // namespace shenmue::s2
