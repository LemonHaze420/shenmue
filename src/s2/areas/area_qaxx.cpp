#include "s2/areas/area_qaxx.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaQAXXInit() {
}

// @verified - p
void AreaQAXXMainLoop(void *) {
}

// @verified - p
void AreaQAXXMain(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    std::int32_t objectFlag;
    if (EV_GetShenmueFlag(0xffU) != 0) {
        EV_SetWeatherInstruction(QAXX_FOG_EFFECT_MASK, 0);
        EV_SetWeatherInstruction(QAXX_LIGHTING_UPDATE_MASK, 0);
        EV_SetWeatherInstruction(QAXX_SCENE_LIGHTING_MASK, 0);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        AssignLightProgram(0);
        AssignLightProgram(2);
        AssignLightProgram(3);
        EV_AssignLightParams(1, 1);
        objectFlag = 1;
    } else {
        EV_SetWeatherInstruction(QAXX_FOG_EFFECT_MASK, 0);
        EV_SetWeatherInstruction(QAXX_LIGHTING_UPDATE_MASK, 0);
        EV_SetWeatherInstruction(QAXX_SCENE_LIGHTING_MASK, 0);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        AssignLightProgram(0);
        AssignLightProgram(2);
        AssignLightProgram(3);
        EV_AssignLightParams(1, 1);
        objectFlag = 0;
    }
    (void)EV_SetObjectFlagByIndex(5, objectFlag);
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    dateTime.hour = 8;
    dateTime.minute = 30;
    SetCurrentDateTimeTo(dateTime);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(QAXX_WEATHER_CLOUD, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(QAXX_WEATHER_RAIN, 0, 0, 0.0f);
    EV_UpdateWeather();
    EV_SetWeatherInstruction(QAXX_SCENE_LIGHTING_MASK, 0);
    HLib::EnqueueTaskWithoutParameter(AreaQAXXMainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
}

// @verified - p
void AreaQAXXCleanup(std::int32_t afternoon, std::uint32_t) {
    EV_SetWeatherInstruction(QAXX_FOG_EFFECT_MASK, 0);
    EV_SetWeatherInstruction(QAXX_LIGHTING_UPDATE_MASK, 0);
    EV_SetWeatherInstruction(QAXX_SCENE_LIGHTING_MASK, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    AssignLightProgram(2);
    AssignLightProgram(3);
    EV_AssignLightParams(1, 1);
    if (afternoon != 0) {
        (void)EV_SetObjectFlagByIndex(5, 1);
    } else {
        (void)EV_SetObjectFlagByIndex(5, 0);
    }
}

const GameplayAreaDefinition areaQAXXGameplayDefinition{
    .areaId = AREA_QAXX,
    .padding04 = 0,
    .init = &AreaQAXXInit,
    .main = &AreaQAXXMain,
    .cleanup = &AreaQAXXCleanup,
};

} // namespace shenmue::s2::areas
