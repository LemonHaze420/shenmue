#pragma once

#include "s2/map_control_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

struct MapEventCallbackEntry {
    void (*callback)(std::int32_t);
    std::int32_t eventId, argument;
};
struct MapEventTaskParameters {
    const MapEventCallbackEntry *callbacks;
};
typedef char Layout_map_event_task_dc_0[(sizeof(MapEventCallbackEntry) == 12) ? 1 : -1];
typedef char Layout_map_event_task_dc_1[(offsetof(MapEventCallbackEntry, eventId) == 4) ? 1 : -1];
typedef char Layout_map_event_task_dc_2[(offsetof(MapEventCallbackEntry, argument) == 8) ? 1 : -1];
typedef char Layout_map_event_task_dc_3[(sizeof(MapEventTaskParameters) == 4) ? 1 : -1];
#define TASK_MEVE SHEN_NAME4('M', 'E', 'V', 'E')
static const MapEventCallbackEntry kEmptyMapEventCallbacks[] = {{SHEN_FIELD(callback) 0, SHEN_FIELD(eventId) -1, SHEN_FIELD(argument) 0}};
void VisitMapEventCharacter(Character *character, HLib::Task *scenarioTask, MapEventTaskParameters *parameters);
void MapEventTaskEntry_Callback(void *data);
HLib::Task *Queue_Callback_MapEventTaskEntry(std::uint8_t nextFunctionIndex);

} } // namespace shenmue::s2

#endif
