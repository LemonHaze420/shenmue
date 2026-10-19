#include "s2/areas/area_ara0.hpp"
#include "s2/areas/area_jukebox.hpp"
#include "s2/lighting.hpp"

#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t nightTimeFlag = 0xff;
constexpr std::int32_t disableAreaWeatherMask = 0x780;
constexpr std::int32_t disableGlobalLightingMask = 0x0c;
constexpr std::int32_t specialEnemyDartsFlag = 0x395;
constexpr std::int32_t storyEnemyDartsFlag = 0x33c;
constexpr std::int16_t audioSuppressedPlayerFlag = 1015;
constexpr EventDartsEnemy primaryEnemy{SHEN_NAME(JON_), 3, 0, 0};
constexpr EventDartsEnemy alternateEnemy{SHEN_NAME(FCJ_), 0, 0, 0};
constexpr EventDartsPlacement dartsPlacements[]{
    {0, {0, 49152, 0}, {249.60000610351562f, 100.2750015258789f, 415.0f}, SHEN_NAME(DAR1), SHEN_NAME(DAR2)},
    {1, {0, 49152, 0}, {249.60000610351562f, 100.2750015258789f, 416.0f}, SHEN_NAME(DAR2), SHEN_NAME(DAR2)},
    {1, {0, 49152, 0}, {249.60000610351562f, 100.2750015258789f, 415.0f}, SHEN_NAME(DAR1), SHEN_NAME(DAR1)},
};

void ARA0PostUpdateCallback(void *) {
}

} // namespace

void AreaARA0Init() {
}

void AreaARA0Main(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(ARA0PostUpdateCallback, 4, 0x0b, static_cast<std::int32_t>(ARA0_EVENT_TASK.name));

    if (EV_GetShenmueFlag(nightTimeFlag)) {
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(3, 1);
    } else {
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(3, 0);
    }

    EV_LoadCutsceneFromCurrentScene("ARA0");
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, audioSuppressedPlayerFlag)) {
        EV_LoadAudio("ARA0_000", 0);
    }

    EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    EV_AssignLightParams(1, 1);
    EV_lightLoad(ARA0_DEFAULT_LIGHT);
    AssignLightProgram(0);
    SetupLightConfig();
    (void)CheckIsArgEquZero(1);

    HLib::EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, static_cast<std::int32_t>(ARA0_EVENT_TASK.name));

    if (EV_GetShenmueFlag(specialEnemyDartsFlag)) {
        EVT_SetEnemyDarts(dartsPlacements, &primaryEnemy);
        EVT_DartsEasySingle(1, dartsPlacements + 1);
        QueueConditionalNpcLoads(areaARA0DartsNpcLoads);
        return;
    }
    if (EV_GetShenmueFlag(storyEnemyDartsFlag)) {
        EVT_SetEnemyDarts(dartsPlacements, &alternateEnemy);
        EVT_DartsEasySingle(1, dartsPlacements + 1);
        QueueConditionalNpcLoads(areaARA0DartsNpcLoads);
        return;
    }

    EVT_DartsEasySingle(2, dartsPlacements + 1);
    ToggleFlagOnFieldEntity(ARA0_DARTS_ENTITY, 0);
}

void AreaARA0Cleanup(std::int32_t afternoon, std::uint32_t) {
    if (afternoon) {
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(3, 1);
    } else {
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(3, 0);
    }
}

const GameplayAreaDefinition areaARA0GameplayDefinition{SHEN_NAME(ARA0), 0, AreaARA0Init, AreaARA0Main, AreaARA0Cleanup};

} // namespace shenmue::s2::areas
