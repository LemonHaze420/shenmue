#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "area_chime.hpp"
void AreaCleanup(int afternoon);
void AreaQE01LoadAudio(int afternoon);
void AreaQE01MainLoop(void *taskData);
static int areaQE01Initializing;
#else
#include "s2/areas/area_qe01.hpp"
#include "s2/areas/area_chime.hpp"
#include "s2/areas/area_qe06.hpp"
#include "s2/event_characters.hpp"

#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"

#include <cstdint>

namespace shenmue::s2::areas {

// @verified - p
void AreaQE01Init() {
}


#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    areaQE01Initializing = 1;
    FTBL.SetMapControlStatusFlag(0, 0);
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
    StartAreaChime();
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaQE01MainLoop, 4, 0x0b, TASK_EVNT);
    areaQE01Initializing = 0;
#else
void AreaQE01Main(std::uint32_t) {
    (void)SetMapControlStatusFlag(0, 0);
    AreaQE01Cleanup(EV_GetShenmueFlag(0xffU) != 0 ? 1 : 0, 0);

    if (g_currentMap.name == AREA_QE00.name || g_currentMap.name == AREA_QE01.name || g_currentMap.name == AREA_QE06.name) {
        HLib::EnqueueTaskWithoutParameter(AreaChimeCallback, 4, 0x0b, static_cast<std::int32_t>(CHIME_TASK.name));
        areaChimeLastSound = 100;
    }
    HLib::EnqueueTaskWithoutParameter(AreaQE01MainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    EV_SetAlignedShadowRange(7.0f);
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQE01Cleanup(std::int32_t afternoon, std::uint32_t) {
#endif
    FTBL.EV_SetWeatherInstruction(0x0cU, 0);
    FTBL.EV_SetWeatherInstruction(0x780U, 0);
    FTBL.EV_SetWeatherInstruction(0x1800U, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    (void)FTBL.CheckIsArgEquZero(1, 1);
    (void)FTBL.SetAreaFogParams();
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaQE01LoadAudio(afternoon);
    FTBL.SetDefaultVSByIndex(7);
#else
    (void)EV_LoadCutsceneFromCurrentScene("QE01");
    (void)EV_LoadAudio("QAE1_000", 0);
    if (afternoon != 0) {
        Vector4 savedGlobalLighting;
        GetGlobalLighting(&savedGlobalLighting);
        UpdateGlobalLighting(qe01NightGlobalLighting);
        AssignLightProgram(7);
        UpdateGlobalLighting(savedGlobalLighting);
    } else {
        SetDefaultVSByIndex(7);
    }
#endif
    FTBL.AssignLightParams(1, 7);
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaQE01LoadAudio(int afternoon) {
    FTBL.EV_LoadCutsceneFromCurrentScene("QE01");
    FTBL.EV_LoadAudio("QAE1_000", 0);
}
#endif

// @verified - p
void AreaQE01MainLoop(void *taskData) {
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaQE01GameplayDefinition{
    .areaId = AREA_QE01,
    .padding04 = 0,
    .init = &AreaQE01Init,
    .main = &AreaQE01Main,
    .cleanup = &AreaQE01Cleanup,
};

} // namespace shenmue::s2::areas

#endif
