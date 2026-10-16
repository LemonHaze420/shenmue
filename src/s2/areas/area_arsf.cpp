#include "s2/areas/area_arsf.hpp"

#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/player.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaARSFInit() {
}

// @verified - p
void AreaARSFMain(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(eQTB1_EVT_mainloop_Callback, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    if (EV_GetShenmueFlag(ARSF_NIGHT_FLAG) != 0) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        (void)EV_SetObjectFlagByIndex(ARSF_NIGHT_OBJECT_INDEX, 1);
        (void)EV_LoadAudio(ARSF_AUDIO_BANK, 1);
    } else {
        (void)CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        (void)EV_SetObjectFlagByIndex(ARSF_NIGHT_OBJECT_INDEX, 0);
        (void)EV_LoadAudio(ARSF_AUDIO_BANK, 0);
    }
}

// @verified - p
void AreaARSFCleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon != 0) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        (void)EV_SetObjectFlagByIndex(ARSF_NIGHT_OBJECT_INDEX, 1);
        (void)EV_LoadAudio(ARSF_AUDIO_BANK, 1);
    } else {
        (void)CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        (void)EV_SetObjectFlagByIndex(ARSF_NIGHT_OBJECT_INDEX, 0);
        (void)EV_LoadAudio(ARSF_AUDIO_BANK, 0);
    }
}

// @verified - p
void eQTB1_EVT_mainloop_Callback(void *) {
    (void)ryoNotBusy();
}

const GameplayAreaDefinition areaARSFGameplayDefinition{
    .areaId = AREA_ARSF,
    .padding04 = 0,
    .init = &AreaARSFInit,
    .main = &AreaARSFMain,
    .cleanup = &AreaARSFCleanup,
};

} // namespace shenmue::s2::areas
