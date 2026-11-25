#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
void AreaCleanup(int afternoon);
void AreaQAE1MainLoop(void *taskData);
void AreaQAE1LoadAudio(int afternoon);
static int areaQAE1Initializing;
#else
#include "s2/areas/area_qae1.hpp"
#include "s2/event_characters.hpp"

#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {



// @verified - p
void AreaQAE1Init() {
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaQAE1Main(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    areaQAE1Initializing = 1;
    FTBL.SetMapControlStatusFlag(0, 0);
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQAE1MainLoop, 4, 0x0b, TASK_EVNT);
    areaQAE1Initializing = 0;
#else
    (void)FTBL.SetMapControlStatusFlag(0, 0);
    if (FTBL.EV_GetShenmueFlag(0xffU) != 0) {
        FTBL.EV_SetWeatherInstruction(0x0cU, 0);
        FTBL.EV_SetWeatherInstruction(0x780U, 0);
        FTBL.EV_SetWeatherInstruction(0x1800U, 0);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
        (void)FTBL.SetAreaFogParams();
        FTBL.EV_LoadCutsceneFromCurrentScene("QAE1");
        (void)FTBL.EV_LoadAudio("QAE1_000", 0);
        FTBL.AssignLightParams(1, 8);
    } else {
        AreaQAE1Cleanup(0, 0);
    }
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQAE1MainLoop, 4, 0x0b, TASK_EVNT);
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQAE1Cleanup(std::int32_t afternoon, std::uint32_t) {
#endif
    FTBL.EV_SetWeatherInstruction(0x0cU, 0);
    FTBL.EV_SetWeatherInstruction(0x780U, 0);
    FTBL.EV_SetWeatherInstruction(0x1800U, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
    (void)FTBL.SetAreaFogParams();
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaQAE1LoadAudio(afternoon);
#else
    FTBL.EV_LoadCutsceneFromCurrentScene("QAE1");
    (void)FTBL.EV_LoadAudio("QAE1_000", 0);
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
    if (afternoon == 0)
#endif
        FTBL.SetDefaultVSByIndex(8);
    FTBL.AssignLightParams(1, 8);
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaQAE1LoadAudio(int afternoon) {
    FTBL.EV_LoadCutsceneFromCurrentScene("QAE1");
    FTBL.EV_LoadAudio("QAE1_000", 0);
}
#endif

// @verified - p
void AreaQAE1MainLoop(void *taskData) {
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQAE1GameplayDefinition{
    .areaId = AREA_QAE1,
    .padding04 = 0,
    .init = &AreaQAE1Init,
    .main = &AreaQAE1Main,
    .cleanup = &AreaQAE1Cleanup,
};

} // namespace shenmue::s2::areas

#endif
