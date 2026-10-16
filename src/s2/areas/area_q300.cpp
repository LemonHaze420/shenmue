#include "s2/debug.hpp"
#include "s2/field_collision.hpp"
#include "s2/sound_program.hpp"
#include "s2/areas/area_q300.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/pati.hpp"
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
namespace {
std::int32_t initializing{};
void SetBike() {
    constexpr shen_name bike{SHEN_NAME(BIKE)};
    if (EV_GetShenmueFlag(0x460) || EV_GetShenmueFlag(0x3c0)) {
        if (DoesScenarioCharacterExist(bike)) {
            EnsureScenarioCharacterDraw(bike);
            EnableScenarioCharacterDisplay(bike);
            ToggleFlagOnFieldEntity(bike, 1);
        }
    } else if (DoesScenarioCharacterExist(bike)) {
        DisableScenarioCharacterDisplay(bike);
        ToggleScenarioCharacterFlag(bike);
        ToggleFlagOnFieldEntity(bike, 0);
    }
}
void ApplyLighting(std::int32_t night) {
    if (night) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
    } else {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
    EV_SetObjectFlagByIndex(4, night ? 1 : 0);
    if (!initializing) {
        EV_LoadCutsceneFromCurrentScene("Q300");
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015))
            EV_LoadAudio("Q300_000", night ? 1 : 0);
    }
    SetBike();
}
void Q300SpringLoop(void *) {
    SetTextureAnimationUV(0.0f, 0.054999999701976776f);
    ApplyTextureAnimationResource(3);
}
} // namespace
void AreaQ300Init() {
}
void AreaQ300Main(std::uint32_t) {
    initializing = 1;
    ApplyLighting(EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED));
    HLib::EnqueueTaskWithoutParameter(Q300SpringLoop, 3, 2, static_cast<std::int32_t>(SHEN_NAME(fntn)));
    InitPati();
    EV_debug_log("common\\et_setgamble.c", 157, "Gamble Init Start\n");
    GambleInit();
    scenFreeEvent("GSET.EV3");
    EV_debug_log("common\\et_setgamble.c", 161, "Gamble Init End\n");
    areaClearAreaEntities();
    initializing = 0;
}
void AreaQ300Cleanup(std::int32_t night, std::uint32_t) {
    ApplyLighting(night);
}
const GameplayAreaDefinition areaQ300GameplayDefinition{SHEN_NAME(Q300), 0, AreaQ300Init, AreaQ300Main, AreaQ300Cleanup};
} // namespace shenmue::s2::areas
