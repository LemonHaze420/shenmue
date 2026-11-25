#include "s2/areas/area_aks0.hpp"
#if defined(SHENMUE_EVENT_OVERLAY)
ConditionalNpcLoadEntry areaAKS0DartsNpcLoads[3] = {
    {SHEN_NAME4('X','H','O','_'),{10000.0f,0,0},0,0xf002,-1,AreConditionalNpcLoadsPending},
    {SHEN_NAME4('H','L','N','_'),{10000.0f,0,0},0,0xf002,-1,AreConditionalNpcLoadsPending},
    {0xffffffffU}
};
EventDartsPlacement dartsPlacements[3] = {
    {0,{0,0x4000,0},{265.9f,100.0f,254.7f},SHEN_NAME4('D','A','R','1'),SHEN_NAME4('D','A','R','2')},
    {1,{0,0x4000,0},{265.9f,100.0f,253.7f},SHEN_NAME4('D','A','R','2'),SHEN_NAME4('D','A','R','2')},
    {1,{0,0x4000,0},{265.9f,100.0f,254.7f},SHEN_NAME4('D','A','R','1'),SHEN_NAME4('D','A','R','1')}
};
EventDartsEnemy primaryEnemy={SHEN_NAME4('X','H','O','_'),4,0,0};
EventDartsEnemy alternateEnemy={SHEN_NAME4('H','L','N','_'),2,0,0};
#else
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

#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
void StartPrimaryDarts() {
    EVT_SetEnemyDarts(&dartsPlacements[0], &primaryEnemy);
    EVT_DartsEasySingle(1, &dartsPlacements[1]);
    QueueConditionalNpcLoads(areaAKS0DartsNpcLoads);
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
#include "area_aks0_media.inl"
} // namespace

void AreaAKS0Init() {
}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int argument) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    EventDateTime dateTime;
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if ((unsigned char)(dateTime.hour - 19) <= 5)
#else
    if (FTBL.EV_GetShenmueFlag(255))
#endif
        AreaCleanup(1);
    else
        AreaCleanup(0);
    FTBL.EV_SetWeatherInstruction(0x780,0);
    FTBL.EV_SetWeatherInstruction(0xc,0);
    FTBL.AssignLightParams(1,1);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(0);
    areaClearAreaEntities();
    if (FTBL.EV_GetShenmueFlag(0x393)) {
        EVT_SetEnemyDarts(dartsPlacements,&primaryEnemy,0);
        EVT_DartsEasySingle(dartsPlacements+1,1);
        QueueConditionalNpcLoads(areaAKS0DartsNpcLoads);
    } else if (FTBL.EV_GetShenmueFlag(0x33e)) {
        EVT_SetEnemyDarts(dartsPlacements,&alternateEnemy,0);
        EVT_DartsEasySingle(dartsPlacements+1,1);
        QueueConditionalNpcLoads(areaAKS0DartsNpcLoads);
    } else {
        EVT_DartsEasySingle(dartsPlacements+1,2);
        FTBL.ToggleFlagOnFieldEntity(SHEN_NAME4('D','A','R','T'),0);
    }
}
#else
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
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
#include "area_aks0_media.inl"
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
void AreaAKS0Cleanup(std::int32_t, std::uint32_t) {
    ApplyAKS0Media();
}

const GameplayAreaDefinition areaAKS0GameplayDefinition{SHEN_NAME(AKS0), 0, AreaAKS0Init, AreaAKS0Main, AreaAKS0Cleanup};
} // namespace shenmue::s2::areas
#endif
