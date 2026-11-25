#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
void AreaCleanup(int afternoon);
static int areaQDKJInitializing;
#define CHARACTER_QDKJ_TARGET SHEN_NAME4('a','3','2','1')
#else
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
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaQDKJMain(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime dateTime;
#endif
    areaQDKJInitializing = 1;
    (void)FTBL.SetMapControlStatusFlag(0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
#else
    DateTime dateTime{};
    shenGetCurrentDateTime(dateTime);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    AreaQDKJCleanup(FTBL.EV_GetShenmueFlag(0xffU) != 0 ? 1 : 0, 0);
#endif
    areaQDKJInitializing = 0;
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQDKJCleanup(std::int32_t, std::uint32_t) {
#endif
    FTBL.EV_SetWeatherInstruction(0x0cU, 0);
    FTBL.EV_SetWeatherInstruction(0x780U, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
    (void)FTBL.SetAreaFogParams();
    if (areaQDKJInitializing == 0) {
        FTBL.EV_LoadCutsceneFromCurrentScene("QDKJ");
        (void)FTBL.EV_LoadAudio("QDKJ_000", 0);
    }
    FTBL.SetDefaultVSByIndex(1);
    FTBL.AssignLightParams(1, 1);
    (void)FTBL.SetScenarioCharacterRadius(CHARACTER_QDKJ_TARGET, 10.0f);
    (void)FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(CHARACTER_QDKJ_TARGET), 0);
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQDKJGameplayDefinition{
    .areaId = AREA_QDKJ,
    .padding04 = 0,
    .init = &AreaQDKJInit,
    .main = &AreaQDKJMain,
    .cleanup = &AreaQDKJCleanup,
};

} // namespace shenmue::s2::areas

#endif
