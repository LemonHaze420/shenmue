#include "s2/areas/area_arc0.hpp"
#include "s2/areas/area_wecf.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_characters.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {

// @verified - p
void ARC0PostUpdateCallback(void *) {
}

// @verified - p
void AreaARC0Init() {
}

// @verified - p
void AreaARC0Main(std::uint32_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ARC0PostUpdateCallback, 4, 0x0b, 8, EVENT_CHARACTER_TASK);
    HLib::Task **const parameter = static_cast<HLib::Task **>(HLib::GetTaskParameter(task));
    *parameter = nullptr;
    *parameter = task;
    EV_SetWeatherInstruction(ARC0_FOG_WEATHER_MASK, 0);
    EV_SetWeatherInstruction(ARC0_GLOBAL_LIGHTING_MASK, 0);
    SetMapControlStatusFlag(0, 0);
    if (EV_GetShenmueFlag(ARC0_NIGHT_FLAG)) {
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(3, 1);
    } else {
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(3, 0);
    }
    EV_AssignLightParams(1, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    SetupLightConfig();
    (void)BeginSecondarySampleExecution(0);
    HLib::EnqueueTaskWithoutParameter(JukeboxInitCallback, 5, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    EV_EventSetUpHelpIcon_0(0, 0);
}

// @verified - p
void AreaARC0Cleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon) {
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(3, 1);
    } else {
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(3, 0);
    }
}

const GameplayAreaDefinition areaARC0GameplayDefinition{
    .areaId = AREA_ARC0,
    .padding04 = 0,
    .init = &AreaARC0Init,
    .main = &AreaARC0Main,
    .cleanup = &AreaARC0Cleanup,
};

} // namespace shenmue::s2::areas
