#include "s2/areas/area_we00.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaWE00Init() {
}

// @verified - p
void AreaWE00Main(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(AreaWE00MainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    std::int64_t audioVariant;
    if (EV_GetShenmueFlag(WE00_NIGHT_FLAG) != 0) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        (void)EV_SetObjectFlagByIndex(WE00_NIGHT_OBJECT_INDEX, 1);
        audioVariant = 1;
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        (void)EV_SetObjectFlagByIndex(WE00_NIGHT_OBJECT_INDEX, 0);
        audioVariant = 0;
    }
    (void)EV_LoadAudio(WE00_AUDIO_BANK, audioVariant);
    (void)UpdateWE00BikeVisibility();
}

// @verified - p
void AreaWE00Cleanup(std::int32_t afternoon, std::uint32_t) {
    std::int64_t audioVariant;
    if (afternoon != 0) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        (void)EV_SetObjectFlagByIndex(WE00_NIGHT_OBJECT_INDEX, 1);
        audioVariant = 1;
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        (void)EV_SetObjectFlagByIndex(WE00_NIGHT_OBJECT_INDEX, 0);
        audioVariant = 0;
    }
    (void)EV_LoadAudio(WE00_AUDIO_BANK, audioVariant);
    (void)UpdateWE00BikeVisibility();
}

// @verified - p
std::int32_t UpdateWE00BikeVisibility() {
    if (EV_GetShenmueFlag(WE00_BIKE_FLAG) != 0) {
        if (!DoesScenarioCharacterExist(WE00_BIKE))
            return 0;
        EV_EnsureCharacterDraw(WE00_BIKE);
        EV_EnableCharacterDisplay(WE00_BIKE);
        return ToggleFlagOnFieldEntity(WE00_BIKE, 1);
    }
    if (!DoesScenarioCharacterExist(WE00_BIKE))
        return 0;
    EV_DisableCharacterDisplay(WE00_BIKE);
    EV_ToggleCharacterFlag(WE00_BIKE);
    return ToggleFlagOnFieldEntity(WE00_BIKE, 0);
}

// @verified - p
void AreaWE00MainLoop(void *) {
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    const std::uint8_t openHour = static_cast<std::uint8_t>(static_cast<std::uint8_t>(dateTime.hour) - WE00_OPENING_HOUR);
    if (openHour > WE00_OPEN_HOUR_SPAN) {
        EV_EnableCharacterDisplay(WE00_CLOSED_SIGN);
    } else {
        EV_DisableCharacterDisplay(WE00_CLOSED_SIGN);
    }
}

const GameplayAreaDefinition areaWE00GameplayDefinition{
    .areaId = AREA_WE00,
    .padding04 = 0,
    .init = &AreaWE00Init,
    .main = &AreaWE00Main,
    .cleanup = &AreaWE00Cleanup,
};

} // namespace shenmue::s2::areas
