#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
enum {
    WE00_NIGHT_OBJECT_INDEX = 9,
    WE00_BIKE_FLAG = 0x3b1,
    WE00_BIKE = SHEN_NAME4('b','i','k','e')
};
void AreaCleanup(int afternoon);
void AreaWE00LoadAudio(int afternoon);
void UpdateWE00BikeVisibility(void);
void AreaWE00MainLoop(void *taskData);
void SetWE00ClosedSignVisibility(int visible);
#else
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
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaWE00Main(std::uint32_t) {
#endif
#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaWE00MainLoop, 4, 0x0b,
        TASK_EVNT);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    UpdateWE00BikeVisibility();
#endif
#else
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
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaWE00Cleanup(std::int32_t afternoon, std::uint32_t) {
    std::int64_t audioVariant;
#endif
    if (afternoon != 0) {
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.CheckIsArgEquZero(1, 1);
#else
        (void)CheckIsArgEquZero(1);
#endif
        (void)FTBL.SetObjectFlagByIndex(WE00_NIGHT_OBJECT_INDEX, 1);
#if !defined(SHENMUE_EVENT_OVERLAY)
        audioVariant = 1;
#endif
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        (void)FTBL.SetObjectFlagByIndex(WE00_NIGHT_OBJECT_INDEX, 0);
#if !defined(SHENMUE_EVENT_OVERLAY)
        audioVariant = 0;
#endif
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaWE00LoadAudio(afternoon);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    UpdateWE00BikeVisibility();
#endif
#else
    (void)EV_LoadAudio(WE00_AUDIO_BANK, audioVariant);
    (void)UpdateWE00BikeVisibility();
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaWE00LoadAudio(int afternoon) {
    if (afternoon)
        FTBL.EV_LoadAudio("WE00_000", 1);
    else
        FTBL.EV_LoadAudio("WE00_000", 0);
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void UpdateWE00BikeVisibility() {
#else
std::int32_t UpdateWE00BikeVisibility() {
#endif
    if (FTBL.EV_GetShenmueFlag(WE00_BIKE_FLAG) != 0) {
        if (!FTBL.DoesScenarioCharacterExist(WE00_BIKE))
#if defined(SHENMUE_EVENT_OVERLAY)
            return;
#else
            return 0;
#endif
        FTBL.EV_EnsureCharacterDraw(WE00_BIKE);
        FTBL.EV_EnableCharacterDisplay(WE00_BIKE);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.ToggleFlagOnFieldEntity(SHEN_NAME4('b','i','k','e'), 1);
    } else {
#else
        return ToggleFlagOnFieldEntity(WE00_BIKE, 1);
    }
#endif
        if (!FTBL.DoesScenarioCharacterExist(WE00_BIKE))
#if defined(SHENMUE_EVENT_OVERLAY)
            return;
#else
            return 0;
#endif
        FTBL.EV_DisableCharacterDisplay(WE00_BIKE);
        FTBL.EV_ToggleCharacterFlag(WE00_BIKE);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.ToggleFlagOnFieldEntity(SHEN_NAME4('b','i','k','e'), 0);
    }
#else
    return ToggleFlagOnFieldEntity(WE00_BIKE, 0);
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY) || !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - p
void AreaWE00MainLoop(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime dateTime;
    unsigned char openHour;
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    openHour = (unsigned char)(dateTime.hour - 8);
#else
    DateTime dateTime;
    shenGetCurrentDateTime(dateTime);
    const std::uint8_t openHour = static_cast<std::uint8_t>(static_cast<std::uint8_t>(dateTime.hour) - WE00_OPENING_HOUR);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (openHour <= 11) {
#else
    if (openHour > WE00_OPEN_HOUR_SPAN) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        SetWE00ClosedSignVisibility(0);
#else
        EV_EnableCharacterDisplay(WE00_CLOSED_SIGN);
#endif
    } else {
#if defined(SHENMUE_EVENT_OVERLAY)
        SetWE00ClosedSignVisibility(1);
#else
        EV_DisableCharacterDisplay(WE00_CLOSED_SIGN);
#endif
    }
}
#if defined(SHENMUE_EVENT_OVERLAY)
void SetWE00ClosedSignVisibility(int visible) {
    if (visible)
        FTBL.EV_EnableCharacterDisplay(SHEN_NAME4('c','l','s','e'));
    else
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('c','l','s','e'));
}
#endif
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaWE00GameplayDefinition{
    .areaId = AREA_WE00,
    .padding04 = 0,
    .init = &AreaWE00Init,
    .main = &AreaWE00Main,
    .cleanup = &AreaWE00Cleanup,
};

} // namespace shenmue::s2::areas
#endif
