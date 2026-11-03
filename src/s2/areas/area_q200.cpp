#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_q200.hpp"

static int areaMainActive;

void AreaMain(unsigned int startParameters)
{
    areaMainActive = 1;
    FTBL.HLib_EnqueueTaskWithoutParameter(Q200MainloopNullsub, 4, 11, Q200_EVENT_TASK);
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(Q200FountainCallback, 3, 2, Q200_FOUNTAIN_TASK);
    if (FTBL.EV_GetShenmueFlag(322) || FTBL.EV_GetShenmueFlag(270)) {
        FTBL.EV_EnableCharacterDisplay(Q200_WORKER_1);
        FTBL.EV_EnableCharacterDisplay(Q200_WORKER_2);
        FTBL.EV_EnableCharacterDisplay(Q200_WORKER_3);
        FTBL.EV_EnableCharacterDisplay(Q200_WORKER_4);
    }
    areaMainActive = 0;
}

void AreaCleanup(int afternoon)
{
    if (afternoon) {
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
        FTBL.SetObjectFlagByIndex(5, 1);
    } else {
        FTBL.CheckIsArgEquZero(1, 1);
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        FTBL.SetObjectFlagByIndex(5, 0);
    }
    Q200LoadSceneAssets(afternoon);
}

void Q200LoadSceneAssets(int afternoon)
{
    FTBL.EV_LoadCutsceneFromCurrentScene("Q200");
    if (afternoon)
        FTBL.EV_LoadAudio("Q200_000", 1);
    else
        FTBL.EV_LoadAudio("Q200_000", 0);
}

void Q200MainloopNullsub(void *taskData)
{
}

void Q200FountainCallback(void *taskData)
{
    FTBL.nlSetTexanimUV(0.0f, 0.055f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(2));
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(3));
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(4));
}
#else
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
#endif
