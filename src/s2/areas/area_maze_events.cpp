#include "s2/areas/area_maze_events.hpp"
#include "s2/system_modules.hpp"
#include "s2/map_control.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
namespace shenmue::s2::areas {
void RestoreMazeCompanionControl() {
    using enum CameraState;
    camCtrlUtil(static_cast<CameraUtilityMode>(0), 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(MAZE_EVENT_RYO), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(MAZE_EVENT_RYO), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MAZE_EVENT_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(MAZE_EVENT_SHENHUA), FacialNodeOriginal);
    SetHighModelXYZ(FindCharacterByScenarioName(MAZE_EVENT_SHENHUA), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MAZE_EVENT_SHENHUA), 0, 1, 0);
    InitiateCleanupForSceneChara(MAZE_EVENT_SHENHUA);
    shenToggleAreaFlags(2);
    SetPlayerControlEnabled(1);
}
void MazeTimeOverToForest4Callback(void *) {
    shenToggleAreaFlags(1);
    EV_stepSetFlags(0, 0xdc);
    scenSetTaskFlags(8);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(KMZ4), 99);
}
void MazeTimeOverTo552ECallback(void *) {
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    dateTime.hour += 2;
    SetCurrentDateTimeTo(dateTime);
    shenToggleAreaFlags(1);
    scenSetTaskFlags(8);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(552E), 0);
}
} // namespace shenmue::s2::areas
