#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_api.h"
#include "s2/event_queued_dc.h"
#include "s2/areas/area_jukebox.hpp"
static shen_name QA22_FANS[4] = {SHEN_NAME4('F', 'A', 'N', '0'), SHEN_NAME4('F', 'A', 'N', '1'), SHEN_NAME4('F', 'A', 'N', '2'), SHEN_NAME4('F', 'A', 'N', '3')};
enum { QA22_PLAYER = SHEN_NAME4('R', 'Y', 'O', '_'), QA22_LIGHT = SHEN_NAME4('0', '0', '0', '1') };
void AreaCleanup(int afternoon);
void QA22LoadAudio(int afternoon);
void QA22MainloopCallback(void *taskData);
void QA22TryStartEvent(void);
void QA22FanCallback(void *taskData);
#else
#include "s2/areas/area_qa22.hpp"
#include "s2/areas/area_jukebox.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"

namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name QA22_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name QA22_LIGHT{SHEN_NAME(0001)};

void QA22MainloopCallback(void *taskData);
void QA22FanCallback(void *taskData);
} // namespace

void AreaQA22Init() {
}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    EventDateTime dateTime;
    FTBL.HLib_EnqueueTaskWithoutParameter(QA22MainloopCallback, 4, 0x0b, TASK_EVNT);
#else
void AreaQA22Main(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(QA22MainloopCallback, 4, 0x0b, static_cast<std::int32_t>(AREA_JUKEBOX_TASK.name));
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetObjectFlagByIndex(1, 0);
    FTBL.EV_SetWeatherInstruction(0x780, 0);
    FTBL.EV_SetWeatherInstruction(0x0c, 0);
#else
    EV_SetObjectFlagByIndex(1, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x0c, 0);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if (FTBL.EV_GetShenmueFlag(0xff))
        AreaCleanup(1);
    else
        AreaCleanup(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(QA22FanCallback, 5, 0x0b, TASK_EVNT);
    FTBL.HLib_EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, TASK_EVNT);
#else
    DateTime dateTime;
    shenGetCurrentDateTime(dateTime);
    (void)EV_GetShenmueFlag(0xff);
    EV_lightLoad(QA22_LIGHT);
    PushGlobalLightingProgram();
    SetupLightConfig();
    SetPluralControlFor(ctrlGetCharaByName(QA22_PLAYER), 1);
    HLib::EnqueueTaskWithoutParameter(QA22FanCallback, 5, 0x0b, static_cast<std::int32_t>(AREA_JUKEBOX_TASK.name));
    HLib::EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, static_cast<std::int32_t>(AREA_JUKEBOX_TASK.name));
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaQA22Cleanup(std::int32_t, std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_lightLoad(QA22_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.SetupLightConfig();
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QA22_PLAYER), 1);
#else
    EV_lightLoad(QA22_LIGHT);
    PushGlobalLightingProgram();
    SetupLightConfig();
    SetPluralControlFor(ctrlGetCharaByName(QA22_PLAYER), 1);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    QA22LoadAudio(afternoon);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void QA22LoadAudio(int afternoon) {
}
#else
namespace {
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void QA22MainloopCallback(void *taskData) {
    if (FTBL.ryoNotBusy())
        QA22TryStartEvent();
}

void QA22TryStartEvent(void) {
    struct { unsigned int type, index; } authorParameter = {0, 37};
#else
void QA22MainloopCallback(void *taskData) {
    if (!ryoNotBusy())
        return;
    std::uint64_t authorParameter = 0x2500000000ULL;
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.shenGetMinutesUntilNextFrameMinute())
        return;
    if (!FTBL.EV_GetPlayerFlag(11, 1809))
        return;
#else
    if (shenGetMinutesUntilNextFrameMinute())
        return;
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1809))
        return;
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_ControlPackageResources(0, 5, "KHIN", 0, 0))
        return;
#else
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("KHIN"), 0))
        return;
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
#else
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (int)"KHIN", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "KHIN", 0, 0);
    FTBL.ExecuteCurrAreaMainFunc("KHIN.EV3", &authorParameter);
#else
    const char *const directory = Filepath_Gen(FilepathGenMode::PackDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("KHIN"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KHIN"), 0);
    ExecuteCurrAreaMainFunc("KHIN.EV3", reinterpret_cast<std::uintptr_t>(&authorParameter));
#endif
}

void QA22FanCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3i rotation;
    int index;
    for (index = 0; index < 4; ++index) {
        FTBL.GetScenarioCharacterEulerAngles(QA22_FANS[index], &rotation, 0x40000000);
        rotation.y -= 728;
        FTBL.EV_AT_SetAngY_0(QA22_FANS[index], 0x78000000, &rotation, 0);
    }
#else
    for (shen_name fan : QA22_FANS) {
        Vector3i rotation;
        GetScenarioCharacterEulerAngles(fan, &rotation, EventTransformFlags::World);
        rotation.y -= 728;
        EV_AT_SetAngY(fan, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
    }
#endif
}
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace

const GameplayAreaDefinition areaQA22GameplayDefinition{
    .areaId = SHEN_NAME(QA22),
    .padding04 = 0,
    .init = &AreaQA22Init,
    .main = &AreaQA22Main,
    .cleanup = &AreaQA22Cleanup,
};

} // namespace shenmue::s2::areas

#endif
