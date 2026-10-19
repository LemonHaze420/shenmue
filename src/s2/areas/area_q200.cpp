#include "s2/areas/area_q200.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/sound_program.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
namespace {
void Q200MainloopNullsub(void *) {
}
void Q200FountainCallback(void *) {
    SetTextureAnimationUV(0.0f, 0.055f);
    ApplyTextureAnimationResource(4);
    SetMapModelSlotEnabled(2, 0);
    SetMapModelSlotEnabled(3, 0);
    AdvanceSpringTextureAnimation();
}
} // namespace

void AreaQ200Init() {
}

void AreaQ200Main(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(Q200MainloopNullsub, 4, 0x0b, static_cast<std::int32_t>(Q200_EVENT_TASK.name));
    if (EV_GetShenmueFlag(0xff)) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(5, 1);
        EV_LoadCutsceneFromCurrentScene("Q200");
        EV_LoadAudio("Q200_000", 1);
    } else {
        (void)CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(5, 0);
        EV_LoadCutsceneFromCurrentScene("Q200");
        EV_LoadAudio("Q200_000", 0);
    }
    HLib::EnqueueTaskWithoutParameter(Q200FountainCallback, 3, 2, static_cast<std::int32_t>(Q200_FOUNTAIN_TASK.name));
    InitializeSpringTextureAnimation(6);
}

void AreaQ200Cleanup(std::int32_t afternoon, std::uint32_t) {
    std::int32_t enabled;
    if (afternoon) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        enabled = 1;
    } else {
        (void)CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        enabled = 0;
    }
    EV_SetObjectFlagByIndex(5, enabled);
    EV_LoadCutsceneFromCurrentScene("Q200");
    EV_LoadAudio("Q200_000", afternoon ? 1 : 0);
}

const GameplayAreaDefinition areaQ200GameplayDefinition{
    .areaId = SHEN_NAME(Q200),
    .padding04 = 0,
    .init = &AreaQ200Init,
    .main = &AreaQ200Main,
    .cleanup = &AreaQ200Cleanup,
};

} // namespace shenmue::s2::areas
