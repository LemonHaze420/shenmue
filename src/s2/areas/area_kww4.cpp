#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_api.h"
#include "s2/event_queued_dc.h"
#include "s2/event_utility_dc.h"
enum { sin = SHEN_NAME4('S', 'I', 'N', '_'), light0000 = SHEN_NAME4('0', '0', '0', '0'), relativeXyz = 0x38000000 };
static shen_name npcLoadCharacters[2] = {sin, 0xffffffff};
void AreaCleanup(int afternoon);
void eKWW4_EVT_KWW4_start_Callback(void *taskData);
void eKWW4_EVT_KWW4_wait_Callback(void *taskData);
void eKWW4_EVT_nonsleep_ready_Callback(void *taskData);
void eKWW4_EVT_WAVE_KWW4_Callback(void *taskData);
void eKWW4_EVT_SetKago_Callback(void *taskData);
#else
#include "s2/areas/area_kww4.hpp"

#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/world_render.hpp"

#include <array>

#include <cstdint>

namespace shenmue::s2::areas {
namespace {

constexpr shen_name sin{SHEN_NAME(SIN_)};
constexpr std::array<shen_name, 2> npcLoadCharacters{sin, shen_name{0xffffffffU}};
constexpr shen_name light0000{SHEN_NAME(0000)};
constexpr std::uint32_t relativeXyz = 0x38000000U;

void eKWW4_EVT_KWW4_start_Callback(void *taskData);
void eKWW4_EVT_KWW4_wait_Callback(void *taskData);
void eKWW4_EVT_nonsleep_ready_Callback(void *taskData);
void eKWW4_EVT_WAVE_KWW4_Callback(void *taskData);
void eKWW4_EVT_SetKago_Callback(void *taskData);
} // namespace

void AreaKWW4Init() {
}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters)
#else
void AreaKWW4Main(std::uint32_t startParameters)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    HLibTask *startTask = 0;
    if (!startParameters)
        startTask = FTBL.HLib_EnqueueTaskWithoutParameter(eKWW4_EVT_KWW4_start_Callback, 4, 11, SHEN_NAME4('k', 'r', 'm', '3'));
    FTBL.EV_SetPlayerFlag(12, 1495, 0);
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0);
    AreaCleanup(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(eKWW4_EVT_WAVE_KWW4_Callback, 3, 2, SHEN_NAME4('w', 'a', 'v', 'e'));
    FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(eKWW4_EVT_nonsleep_ready_Callback, 4, 11, SHEN_NAME4('s', 'l', 'e', 'p')), 10);
    FTBL.HLib_EnqueueTaskWithoutParameter(eKWW4_EVT_SetKago_Callback, 4, 11, SHEN_NAME4('k', 'a', 'g', 'o'));
    if (!startParameters) {
        FTBL.EV_NPC_NewLoadSw_0(0);
        Queue_EVT_UTL_LoadNPCChara_3(npcLoadCharacters, startTask);
        FTBL.HLib_SetupTimedTask(startTask, 0);
    } else
        FTBL.PTYCUtils(sin, 3, 1);
#else
    HLib::Task *startTask = nullptr;
    if (!startParameters) {
        startTask = HLib::EnqueueTaskWithoutParameter(eKWW4_EVT_KWW4_start_Callback, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(krm3)));
    }

    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_lightLoad(light0000);
    AssignLightProgram(0);
    (void)CheckIsArgEquZero(1);

    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    HLib::EnqueueTaskWithoutParameter(eKWW4_EVT_WAVE_KWW4_Callback, 3, 2, static_cast<std::int32_t>(SHEN_NAME(wave)));
    HLib::Task *const nonsleepTask = HLib::EnqueueTaskWithoutParameter(eKWW4_EVT_nonsleep_ready_Callback, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(slep)));
    HLib::SetupTimedTask(nonsleepTask, 10);
    HLib::EnqueueTaskWithoutParameter(eKWW4_EVT_SetKago_Callback, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(kago)));

    if (startParameters) {
        EV_PTYCUtils(sin, 3, 1);
        return;
    }

    EV_NPC_NewLoadSw(0);
    AssignAreaNpcLoadContext(npcLoadCharacters.data(), startTask);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (startTask)
        HLib::SetupTimedTask(startTask, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(SHEN_NAME(load)));
    HLib::SetupTimedTask(startTask, 0);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon)
#else
void AreaKWW4Cleanup(std::int32_t, std::uint32_t)
#endif
{
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
    FTBL.AssignLightProgram(0);
    FTBL.CheckIsArgEquZero(1, 1);
    FTBL.ReinitializeLights(1);
    FTBL.SetLightFlagForIndex(0, 2);
    FTBL.SetLightEntryIndex(0, 1);
#else
    EV_lightLoad(light0000);
    AssignLightProgram(0);
    (void)CheckIsArgEquZero(1);

    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
namespace {
#endif

void eKWW4_EVT_KWW4_start_Callback(void *taskData) {
    const Vector3 position = {195.60001f, 92.0f, 144.10001f};
    const Vector3i rotation = {0, 30401, 0};
#if defined(SHENMUE_EVENT_OVERLAY)

    FTBL.ActivatePartyCompanion(sin);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.PTYCUtils(sin, 3, 1);
    FTBL.EV_AT_SetPos(sin, 0x38000000, &position);
    FTBL.EV_AT_SetAngY_0(sin, 0x38000000, &rotation, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(sin), 17, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(sin), 71, 0, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(sin), 13, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(sin), 0x9502, 0);
    FTBL.PTYCUtils(sin, 3, 0);
    FTBL.shenToggleAreaFlags(3);
    FTBL.SetPlayerGameFlags(64, 1);
    FTBL.HLib_SetCurrentTaskTo(eKWW4_EVT_KWW4_wait_Callback);
    FTBL.HLib_SetupTaskDelay(60);
#else

    EV_ActivatePartyCompanion(sin);
    SetPlayerControlEnabled(0);
    EV_PTYCUtils(sin, 3, 1);
    EV_AT_SetPos(sin, relativeXyz, position);
    EV_AT_SetAngY(sin, relativeXyz, rotation, nullptr);
    mtwkSetExtraFlag(scenFindMotionwork(sin), 17, 0);
    mtwkSetRegionFlag(scenFindMotionwork(sin), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(sin), 13, 0);
    mtwkRequestMotion(scenFindMotionwork(sin), 0x9502, MotionRegion::LowerBody);
    EV_PTYCUtils(sin, 3, 0);
    shenToggleAreaFlags(3);
    SetPlayerGameFlags(PlayerGameFlagMode::RetainMovementMode, reinterpret_cast<void *>(1));
    HLib::SetCurrentTaskTo(eKWW4_EVT_KWW4_wait_Callback);
    HLib::SetupTaskDelay(60);
#endif
}

void eKWW4_EVT_KWW4_wait_Callback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    int busy = FTBL.IsScenarioBusy();
#else
    bool busy = IsScenarioBusy();
#endif
    if (!busy)
        return;
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.DebugLogToFileRemoved("player mode clear\n");
    FTBL.shenToggleAreaFlags(0);
    FTBL.HLib_DestroyCurrentTask();
#else
    shenToggleAreaFlags(0);
    HLib::DestroyCurrentTask();
#endif
}

void eKWW4_EVT_nonsleep_ready_Callback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_setHoldStatesFlags(0);
    FTBL.HLib_DestroyCurrentTask();
#else
    EV_setHoldStatesFlags(0);
    HLib::DestroyCurrentTask();
#endif
}

void eKWW4_EVT_WAVE_KWW4_Callback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.nlSetTexanimUV(.03f, 0);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(12));
#else
    SetTextureAnimationUV(0.03f, 0.0f);
    ApplyTextureAnimationResource(12);
#endif
}

void eKWW4_EVT_SetKago_Callback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 position = {0.078f, 0.015f, -0.007f};
    Vector3i rotation = {3822, 62987, 14927};
    int exists = FTBL.DoesScenarioCharacterExist(sin);
#else
    bool exists = DoesScenarioCharacterExist(sin);
#endif
    if (exists) {
#if defined(SHENMUE_EVENT_OVERLAY)
        unsigned short animation = FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(sin), 3);
#else
        auto animation = mtwkGetAnimationId(scenFindMotionwork(sin), MotionRegion::RightArm);
#endif
        if (animation != 0x9505) {
#if defined(SHENMUE_EVENT_OVERLAY)
            FTBL.SetNpcRouteUpdatePaused(sin, 1);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(sin), 0x9505, 3);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(sin), 1, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(sin), 1, 0, 0);
#else
            SetNpcRouteControlHold(sin, 1);
            mtwkRequestMotion(scenFindMotionwork(sin), 0x9505, MotionRegion::RightArm);
            SetHighModelXYZ(ctrlGetCharaByName(sin), 1, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(sin), 1, 0, 0);
#endif
        }
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_GetPlayerFlag(11, 1854)) {
#else
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1854)) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(sin), 73, 0, 3);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(sin), 0, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(sin), 0, 0, 0);
        FTBL.HLib_DestroyCurrentEventTask();
#else
        mtwkSetRegionFlag(scenFindMotionwork(sin), 73, 0, MotionRegion::RightArm);
        SetHighModelXYZ(ctrlGetCharaByName(sin), 0, 1, 0);
        SetHighModelXYZ(ctrlGetCharaByName(sin), 0, 0, 0);
        HLib::DestroyCurrentTask();
#endif
    }
}

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace

const GameplayAreaDefinition areaKWW4GameplayDefinition{SHEN_NAME(KWW4), 0, AreaKWW4Init, AreaKWW4Main, AreaKWW4Cleanup};

} // namespace shenmue::s2::areas
#endif
