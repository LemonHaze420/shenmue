#include "s2/areas/area_aks1.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/areas/area_jukebox.hpp"
#include "s2/help.hpp"
#include "s2/map_control.hpp"
#include "s2/lighting.hpp"
#include "s2/field_collision.hpp"

#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t nightTimeFlag = 0xff;
constexpr std::int32_t enemyDartsFlag = 0x397;
constexpr std::int16_t audioSuppressedPlayerFlag = 1015;
constexpr shen_name defaultLight{SHEN_NAME(0000)};
constexpr shen_name dartsEntity{SHEN_NAME(DART)};
constexpr std::int32_t eventTaskToken = static_cast<std::int32_t>(SHEN_NAME(EVNT));
constexpr EventDartsEnemy enemy{SHEN_NAME(HCY_), 1, 0, 0};
constexpr EventDartsPlacement dartsPlacements[]{
    {0, {0, 32768, 0}, {276.79998779296875f, 106.30000305175781f, 269.79998779296875f}, SHEN_NAME(DAR1), SHEN_NAME(DAR2)},
    {1, {0, 32768, 0}, {275.79998779296875f, 106.30000305175781f, 269.79998779296875f}, SHEN_NAME(DAR2), SHEN_NAME(DAR2)},
    {1, {0, 32768, 0}, {276.79998779296875f, 106.30000305175781f, 269.79998779296875f}, SHEN_NAME(DAR1), SHEN_NAME(DAR1)},
};

} // namespace

void AreaAKS1Init() {
}

void AreaAKS1Main(std::uint32_t) {
    if (EV_GetShenmueFlag(nightTimeFlag)) {
        EV_lightLoad(defaultLight);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(0, 0);
        EV_SetObjectFlagByIndex(1, 1);
        EV_LoadCutsceneFromCurrentScene("AKS1");
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, audioSuppressedPlayerFlag)) {
            EV_LoadAudio("AKS1_000", 1);
        }
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(0, 1);
        EV_SetObjectFlagByIndex(1, 0);
        EV_LoadCutsceneFromCurrentScene("AKS1");
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, audioSuppressedPlayerFlag)) {
            EV_LoadAudio("AKS1_000", 0);
        }
    }

    BeginSecondarySampleExecution(0);
    HLib::EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, eventTaskToken);
    areaClearAreaEntities();

    if (EV_GetShenmueFlag(enemyDartsFlag)) {
        EVT_SetEnemyDarts(dartsPlacements, &enemy);
        EVT_DartsEasySingle(1, dartsPlacements + 1);
        QueueConditionalNpcLoads(areaAKS1DartsNpcLoads);
        EV_EventSetUpHelpIcon_0(0, 0);
        return;
    }

    EVT_DartsEasySingle(2, dartsPlacements + 1);
    ToggleFlagOnFieldEntity(dartsEntity, 0);
    EV_EventSetUpHelpIcon_0(0, 0);
}

void AreaAKS1Cleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon) {
        EV_lightLoad(defaultLight);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(0, 0);
        EV_SetObjectFlagByIndex(1, 1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(0, 1);
        EV_SetObjectFlagByIndex(1, 0);
    }

    EV_LoadCutsceneFromCurrentScene("AKS1");
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, audioSuppressedPlayerFlag)) {
        EV_LoadAudio("AKS1_000", afternoon ? 1 : 0);
    }
}

const GameplayAreaDefinition areaAKS1GameplayDefinition{SHEN_NAME(AKS1), 0, AreaAKS1Init, AreaAKS1Main, AreaAKS1Cleanup};

} // namespace shenmue::s2::areas
