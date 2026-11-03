#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_tbak.hpp"

void AreaMain(unsigned int startParameters)
{
    EventDateTime dateTime;
    if (IsOutsideQTB()) {
        FTBL.SetMapControlStatusFlag(0, 0);
        FTBL.EV_SetWeatherInstruction(15, 0);
        FTBL.EV_SetWeatherInstruction(1920, 0);
        FTBL.EV_SetWeatherInstruction(6144, 0);
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
    }
    FTBL.BeginPrimarySampleExecution(0);
    FTBL.BeginSecondarySampleExecution(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(TBAKMainLoopNullsub, 4, 11, TASK_EVNT);
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if ((unsigned char)(dateTime.hour - 19) <= 5)
        AreaCleanup(1);
    else
        AreaCleanup(0);
    SetGamble();
}

void AreaCleanup(int afternoon)
{
    if (!IsOutsideQTB()) {
        if (afternoon) {
            FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
            FTBL.PushGlobalLightingProgram();
            FTBL.CheckIsArgEquZero(1, 1);
        } else {
            FTBL.CheckIsArgEquZero(1, 1);
            FTBL.ReinitializeLights(1);
            FTBL.SetLightFlagForIndex(0, 2);
            FTBL.SetLightEntryIndex(0, 1);
        }
    }
    AreaTBAKCleanupNullsub(afternoon);
}

void AreaTBAKCleanupNullsub(int afternoon)
{
}

void TBAKMainLoopNullsub(void *taskData)
{
}

int IsOutsideQTB(void)
{
    int outside = 1;
    if (g_currentMap == AREA_QTB1 || g_currentMap == AREA_QTB3)
        outside = 0;
    return outside;
}
#else
#include "s2/debug.hpp"
#include "s2/areas/area_tbak.hpp"
#include "s2/areas/area_akt0.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/lighting.hpp"
#include "s2/sound_program.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
namespace {
void TBAKMainLoopNullsub(void *) {
}
bool UsesQTBLighting() {
    return g_currentMap == AREA_QTB1 || g_currentMap.name == SHEN_NAME(QTB3);
}
} // namespace
void AreaTBAKInit() {
}
void AreaTBAKMain(std::uint32_t) {
    if (!UsesQTBLighting()) {
        SetMapControlStatusFlag(0, false);
        EV_SetWeatherInstruction(0x0f, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_SetWeatherInstruction(0x1800, 0);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
    }
    (void)BeginPrimarySampleExecution(0);
    (void)BeginSecondarySampleExecution(0);
    HLib::EnqueueTaskWithoutParameter(TBAKMainLoopNullsub, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    AreaAKTSharedCleanup(EV_GetShenmueFlag(0xff));
    SetGamble();
}
void AreaTBAKCleanup(std::int32_t afternoon, std::uint32_t) {
    AreaAKTSharedCleanup(afternoon);
}
const GameplayAreaDefinition areaTBAKGameplayDefinition{SHEN_NAME(TBAK), 0, AreaTBAKInit, AreaTBAKMain, AreaTBAKCleanup};
} // namespace shenmue::s2::areas
#endif
