#include "s2/areas/area_aks0.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/lighting.hpp"
#include "s2/field_collision.hpp"
#include "s2/map_control.hpp"

#include "s2/event_runtime.hpp"
#include "s2/sound_program.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t disableAreaWeatherMask = 0x780;
constexpr std::int32_t disableGlobalLightingMask = 0x0c;
constexpr std::int32_t nightTimeFlag = 0xff;
constexpr std::int32_t primaryDartsFlag = 0x393;
constexpr std::int32_t alternateDartsFlag = 0x33e;
constexpr std::int16_t suppressAreaAudioPlayerFlag = 1015;
constexpr shen_name defaultLight{SHEN_NAME(0000)};
constexpr shen_name dartsFieldEntity{SHEN_NAME(DART)};
constexpr EventDartsEnemy primaryEnemy{SHEN_NAME(XHO_), 4};
constexpr EventDartsEnemy alternateEnemy{SHEN_NAME(HLN_), 2};
constexpr std::array<EventDartsPlacement, 3> dartsPlacements{{
    {
        0,
        {0, 0x4000, 0},
        {265.8999938964844f, 100.0f, 254.6999969482422f},
        SHEN_NAME(DAR1),
        SHEN_NAME(DAR2),
    },
    {
        1,
        {0, 0x4000, 0},
        {265.8999938964844f, 100.0f, 253.6999969482422f},
        SHEN_NAME(DAR2),
        SHEN_NAME(DAR2),
    },
    {
        1,
        {0, 0x4000, 0},
        {265.8999938964844f, 100.0f, 254.6999969482422f},
        SHEN_NAME(DAR1),
        SHEN_NAME(DAR1),
    },
}};

void StartPrimaryDarts() {
    EVT_SetEnemyDarts(&dartsPlacements[0], &primaryEnemy);
    EVT_DartsEasySingle(1, &dartsPlacements[1]);
    QueueConditionalNpcLoads(areaAKS0DartsNpcLoads);
}

void ApplyAKS0Media() {
    EV_LoadCutsceneFromCurrentScene("AKS0");
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, suppressAreaAudioPlayerFlag) == 0) {
        EV_LoadAudio("AKS0_000", 0);
    }
}

} // namespace

void AreaAKS0Init() {
}

void AreaAKS0Main(std::uint32_t) {
    (void)EV_GetShenmueFlag(nightTimeFlag);
    ApplyAKS0Media();
    EV_SetWeatherInstruction(disableAreaWeatherMask, 0);
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    EV_AssignLightParams(1, 1);
    EV_lightLoad(defaultLight);
    AssignLightProgram(0);
    areaClearAreaEntities();

    if (EV_GetShenmueFlag(primaryDartsFlag) != 0) {
        StartPrimaryDarts();
    } else if (EV_GetShenmueFlag(alternateDartsFlag) != 0) {
        EVT_SetEnemyDarts(&dartsPlacements[0], &alternateEnemy);
        EVT_DartsEasySingle(1, &dartsPlacements[1]);
        QueueConditionalNpcLoads(areaAKS0DartsNpcLoads);
    } else {
        EVT_DartsEasySingle(2, &dartsPlacements[1]);
        ToggleFlagOnFieldEntity(dartsFieldEntity, 0);
    }
}

void AreaAKS0Cleanup(std::int32_t, std::uint32_t) {
    ApplyAKS0Media();
}

const GameplayAreaDefinition areaAKS0GameplayDefinition{SHEN_NAME(AKS0), 0, AreaAKS0Init, AreaAKS0Main, AreaAKS0Cleanup};

} // namespace shenmue::s2::areas
