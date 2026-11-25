#pragma once

#include "s2/map_event_character_dc.hpp"
#include "s2/startup_assets_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct MapEventTransition {
    std::int32_t eventId;
    shen_name areaId;
    std::int32_t entryId;
    Vector3 position;
    std::int32_t yaw;
    std::uint32_t scenarioFlags;
};
typedef char Layout_map_event_transition_dc_0[(sizeof(MapEventTransition) == 0x20) ? 1 : -1];
typedef char Layout_map_event_transition_dc_1[(offsetof(MapEventTransition, areaId) == 4) ? 1 : -1];
typedef char Layout_map_event_transition_dc_2[(offsetof(MapEventTransition, entryId) == 8) ? 1 : -1];
typedef char Layout_map_event_transition_dc_3[(offsetof(MapEventTransition, position) == 0x0c) ? 1 : -1];
typedef char Layout_map_event_transition_dc_4[(offsetof(MapEventTransition, yaw) == 0x18) ? 1 : -1];
typedef char Layout_map_event_transition_dc_5[(offsetof(MapEventTransition, scenarioFlags) == 0x1c) ? 1 : -1];
#define TASK_EVFO SHEN_NAME4('E', 'V', 'F', 'O')
extern shen_name g_newAreaId;
extern std::int32_t g_newEntryId;
extern std::uint32_t g_newScenarioFlags;
bool RetainQronMapEventDestination(shen_name areaId, std::int32_t eventId);
std::uint32_t SetPlayerControlEnabled(std::int32_t mode);
std::int32_t shenToggleAreaFlags(std::int32_t mode);
void spmFadeOut(std::int32_t sample);
void shenQueueChangeModuleAndScene(SystemModule module, std::int32_t actNumber, shen_name areaId, std::int32_t entryId);
void EV_scenQueueLoad(std::int32_t actNumber, shen_name areaId, std::int32_t entryId);
void Exec_MapEventSceneChange_Callback(void *data);

} // namespace s2
} // namespace shenmue

#endif
