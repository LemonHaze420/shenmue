#include "s2/areas/area_qdkj.hpp"

#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/fog.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {

std::int32_t areaQDKJInitializing{};

} // namespace

// @verified - p
void AreaQDKJInit() {
    areaQDKJInitializing = 0;
}

// @verified - p
void AreaQDKJMain(std::uint32_t) {
    areaQDKJInitializing = 1;
    (void)SetMapControlStatusFlag(0, 0);
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    AreaQDKJCleanup(EV_GetShenmueFlag(0xffU) != 0 ? 1 : 0, 0);
    areaQDKJInitializing = 0;
}

// @verified - p
void AreaQDKJCleanup(std::int32_t, std::uint32_t) {
    EV_SetWeatherInstruction(0x0cU, 0);
    EV_SetWeatherInstruction(0x780U, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    (void)SetAreaFogParams();
    if (areaQDKJInitializing == 0) {
        EV_LoadCutsceneFromCurrentScene("QDKJ");
        (void)EV_LoadAudio("QDKJ_000", 0);
    }
    SetDefaultVSByIndex(1);
    EV_AssignLightParams(1, 1);
    (void)SetScenarioCharacterRadius(CHARACTER_QDKJ_TARGET, 10.0f);
    (void)SetPluralControlFor(ctrlGetCharaByName(CHARACTER_QDKJ_TARGET), 0);
}

const GameplayAreaDefinition areaQDKJGameplayDefinition{
    .areaId = AREA_QDKJ,
    .padding04 = 0,
    .init = &AreaQDKJInit,
    .main = &AreaQDKJMain,
    .cleanup = &AreaQDKJCleanup,
};

} // namespace shenmue::s2::areas
