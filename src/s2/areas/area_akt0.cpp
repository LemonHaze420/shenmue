#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_akt0.hpp"
#include "s2/events/event_api.h"
#include "s2/areas/area_gambling.hpp"
#include "s2/event_queued_dc.h"
enum {
    disableGlobalLightingMask = 12,
    disableFogEffectMask = 0x780,
    nightTimeFlag = 0xff,
    defaultLight = LIGHT_DEFAULT_DEFINITION,
    eventTaskToken = TASK_EVNT
};
#else
#include "s2/debug.hpp"
#include "s2/areas/area_akt0.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/areas/area_jukebox.hpp"
#include "s2/sound_program.hpp"
#include "s2/lighting.hpp"

#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {
namespace {

constexpr std::int32_t disableGlobalLightingMask = 0x0c;
constexpr std::int32_t disableFogEffectMask = 0x780;
constexpr std::int32_t nightTimeFlag = 0xff;
constexpr shen_name defaultLight{SHEN_NAME(0000)};
constexpr shen_name qtb1{SHEN_NAME(QTB1)};
constexpr shen_name qtb3{SHEN_NAME(QTB3)};
constexpr std::int32_t eventTaskToken = static_cast<std::int32_t>(SHEN_NAME(EVNT));

void AKT0PostUpdateCallback(void *data);

bool UsesQTBLightOverride()
{
    return g_currentMap.name == qtb1.name || g_currentMap.name == qtb3.name;
}

} // namespace

void AreaAKTSharedCleanup(std::int32_t afternoon)
{
    if (!UsesQTBLightOverride())
        return;

    if (afternoon) {
        EV_lightLoad(defaultLight);
        PushGlobalLightingProgram();
        (void)CheckIsArgEquZero(1);
        return;
    }

    (void)CheckIsArgEquZero(1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
}

void AreaAKT0Init() {}

#endif
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters)
{
#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
    EventDateTime dateTime;
#endif
#else
void AreaAKT0Main(std::uint32_t)
{
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetMapControlStatusFlag(0, 0);
    FTBL.EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    FTBL.EV_SetWeatherInstruction(disableFogEffectMask, 0);
    FTBL.AssignLightParams(1, 1);
    FTBL.EV_lightLoad(defaultLight);
    FTBL.AssignLightProgram(0);
    (void)FTBL.CheckIsArgEquZero(1, 1);
    (void)FTBL.BeginPrimarySampleExecution(0);
    (void)FTBL.BeginSecondarySampleExecution(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(AKT0PostUpdateCallback, 4, 0x0b, eventTaskToken);

#else
    SetMapControlStatusFlag(0, false);
    EV_SetWeatherInstruction(disableGlobalLightingMask, 0);
    EV_SetWeatherInstruction(disableFogEffectMask, 0);
    EV_AssignLightParams(1, 1);
    EV_lightLoad(defaultLight);
    AssignLightProgram(0);
    (void)CheckIsArgEquZero(1);
    (void)BeginPrimarySampleExecution(0);
    (void)BeginSecondarySampleExecution(0);
    HLib::EnqueueTaskWithoutParameter(AKT0PostUpdateCallback, 4, 0x0b, eventTaskToken);

#endif

#if defined(SHENMUE_EVENT_OVERLAY)
#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if ((unsigned char)(dateTime.hour - 19) <= 5)
#else
    if (FTBL.EV_GetShenmueFlag(nightTimeFlag))
#endif
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    if (EV_GetShenmueFlag(nightTimeFlag)) {
        if (UsesQTBLightOverride()) {
            EV_lightLoad(defaultLight);
            PushGlobalLightingProgram();
            (void)CheckIsArgEquZero(1);
        }
    } else if (UsesQTBLightOverride()) {
        (void)CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }

#endif
    SetGamble();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.HLib_EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, eventTaskToken);
#else
    HLib::EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, eventTaskToken);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon)
{
    if (!IsOutsideQTB()) {
        if (afternoon) {
            FTBL.EV_lightLoad(defaultLight);
            FTBL.PushGlobalLightingProgram();
            FTBL.CheckIsArgEquZero(1, 1);
        } else {
            FTBL.CheckIsArgEquZero(1, 1);
            FTBL.ReinitializeLights(1);
            FTBL.SetLightFlagForIndex(0, 2);
            FTBL.SetLightEntryIndex(0, 1);
        }
    }
    AKTUnusedCleanup(afternoon);
}

void AKTUnusedCleanup(int afternoon) {}
#endif
#if !defined(SHENMUE_EVENT_OVERLAY)
void AreaAKT0Cleanup(std::int32_t afternoon, std::uint32_t)
{
    AreaAKTSharedCleanup(afternoon);
}

const GameplayAreaDefinition areaAKT0GameplayDefinition{SHEN_NAME(AKT0), 0, AreaAKT0Init, AreaAKT0Main, AreaAKT0Cleanup};

namespace {
void AKT0PostUpdateCallback(void *data) {}
} // namespace
} // namespace shenmue::s2::areas

#else
void AKT0PostUpdateCallback(void *data) {}

int IsOutsideQTB(void)
{
    int outside = 1;
    if (g_currentMap == SHEN_NAME4('Q', 'T', 'B', '1') || g_currentMap == SHEN_NAME4('Q', 'T', 'B', '3'))
        outside = 0;
    return outside;
}
#endif
