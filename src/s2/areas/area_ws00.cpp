#include "s2/debug.hpp"
#include "s2/field_collision.hpp"
#include "s2/areas/area_ws00.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/pati.hpp"
namespace shenmue::s2::areas {
namespace {
void ApplyLighting(std::int32_t night) {
    if (night) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(11, 1);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(11, 0);
    }
    EV_LoadAudio("WS00_000", night ? 1 : 0);
}
} // namespace
void AreaWS00Init() {
}
void AreaWS00Main(std::uint32_t) {
    ApplyLighting(EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED));
    EV_debug_log("common\\et_setgamble.c", 157, "Gamble Init Start\n");
    GambleInit();
    scenFreeEvent("GSET.EV3");
    EV_debug_log("common\\et_setgamble.c", 161, "Gamble Init End\n");
    InitPati();
    areaClearAreaEntities();
    constexpr shen_name board{SHEN_NAME(brdX)};
    if (EV_GetShenmueFlag(0x4a8)) {
        SetCharacterMapEventRegionEnabled(SHEN_NAME(RYO_), 76, 1);
        ToggleFlagOnFieldEntity(board, 0);
        DisableScenarioCharacterDisplay(board);
    } else {
        SetCharacterMapEventRegionEnabled(SHEN_NAME(RYO_), 76, 0);
        ToggleFlagOnFieldEntity(board, 1);
        EnableScenarioCharacterDisplay(board);
    }
    const Vector3 origin{95.21690368652344f, 98.19999694824219f, 130.5460968017578f};
    Vector3i rotation{0, 24576, 0};
    Vector3 position{1.2300000190734863f, 3.2300000190734863f, 0.0f};
    ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 29))
        rotation.z = 1092;
    constexpr shen_name placedBoard{SHEN_NAME(brd_)};
    EV_AT_SetPos(placedBoard, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(placedBoard, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
    EnableScenarioCharacterDisplay(placedBoard);
    SetMainMapMinimumModelHeight(100.0f);
    SetMainMapProjectedShadowDepth(30.0f);
}
void AreaWS00Cleanup(std::int32_t night, std::uint32_t) {
    ApplyLighting(night);
}
const GameplayAreaDefinition areaWS00GameplayDefinition{SHEN_NAME(WS00), 0, AreaWS00Init, AreaWS00Main, AreaWS00Cleanup};
} // namespace shenmue::s2::areas
