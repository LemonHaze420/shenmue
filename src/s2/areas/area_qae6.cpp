#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
void AreaCleanup(int afternoon);
void AreaQAE6MainLoop(void *taskData);
void AreaQAE6LoadAudio(int afternoon);
static int areaQAE6Initializing;
#else
#include "s2/areas/area_qae6.hpp"
#include "s2/event_characters.hpp"

#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {

namespace {

std::int32_t areaQAE6Initializing{};

} // namespace


// @verified - p
void AreaQAE6Init() {
}
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaQAE6Main(std::uint32_t) {
#endif
    areaQAE6Initializing = 1;
    (void)FTBL.SetMapControlStatusFlag(0, 0);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    AreaQAE6Cleanup(FTBL.EV_GetShenmueFlag(0xffU) != 0 ? 1 : 0, 0);
#endif
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQAE6MainLoop, 4, 0x0b, TASK_EVNT);
    areaQAE6Initializing = 0;
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQAE6Cleanup(std::int32_t, std::uint32_t) {
#endif
    FTBL.EV_SetWeatherInstruction(0x0fU, 0);
    FTBL.EV_SetWeatherInstruction(0x780U, 0);
    FTBL.EV_SetWeatherInstruction(0x1800U, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
    (void)FTBL.SetAreaFogParams();
    if (areaQAE6Initializing == 0) {
#if defined(SHENMUE_EVENT_OVERLAY)
        AreaQAE6LoadAudio(afternoon);
#else
        FTBL.EV_LoadCutsceneFromCurrentScene("QAE6");
        (void)FTBL.EV_LoadAudio("QAE1_000", 0);
#endif
    }
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaQAE6LoadAudio(int afternoon) {
    FTBL.EV_LoadCutsceneFromCurrentScene("QAE6");
    FTBL.EV_LoadAudio("QAE1_000", 0);
}
#endif

// @verified - p
void AreaQAE6MainLoop(void *taskData) {
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQAE6GameplayDefinition{
    .areaId = AREA_QAE6,
    .padding04 = 0,
    .init = &AreaQAE6Init,
    .main = &AreaQAE6Main,
    .cleanup = &AreaQAE6Cleanup,
};

} // namespace shenmue::s2::areas

#endif
