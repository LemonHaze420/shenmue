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

void eKWW4_EVT_KWW4_wait_Callback(void *) {
    if (!IsScenarioBusy())
        return;
    shenToggleAreaFlags(0);
    HLib::DestroyCurrentTask();
}

void eKWW4_EVT_KWW4_start_Callback(void *) {
    const Vector3 position{195.60001f, 92.0f, 144.10001f};
    const Vector3i rotation{0, 30401, 0};

    EV_ActivatePartyCompanion(sin);
    SetPlayerControlEnabled(0);
    EV_PTYCUtils(sin, 3, 1);
    EV_AT_SetPos(sin, relativeXyz, position);
    EV_AT_SetAngY(sin, relativeXyz, rotation, nullptr);
    mtwkSetExtraFlag(scenFindMotionwork(sin), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(sin), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(sin), 13, 0);
    mtwkRequestMotion(scenFindMotionwork(sin), 0x9502, MotionRegion::LowerBody);
    EV_PTYCUtils(sin, 3, 0);
    shenToggleAreaFlags(3);
    SetPlayerGameFlags(PlayerGameFlagMode::RetainMovementMode, reinterpret_cast<void *>(1));
    HLib::SetCurrentTaskTo(eKWW4_EVT_KWW4_wait_Callback);
    HLib::SetupTaskDelay(60);
}

void eKWW4_EVT_WAVE_KWW4_Callback(void *) {
    SetTextureAnimationUV(0.03f, 0.0f);
    ApplyTextureAnimationResource(12);
}

void eKWW4_EVT_nonsleep_ready_Callback(void *) {
    EV_setHoldStatesFlags(0);
    HLib::DestroyCurrentTask();
}

void eKWW4_EVT_SetKago_Callback(void *) {
    if (DoesScenarioCharacterExist(sin)) {
        if (GetMotionAnimationId(scenFindMotionwork(sin), MotionRegion::RightArm) != 0x9505) {
            SetNpcRouteControlHold(sin, 1);
            mtwkRequestMotion(scenFindMotionwork(sin), 0x9505, MotionRegion::RightArm);
            SetHighModelXYZ(ctrlGetCharaByName(sin), 1, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(sin), 1, 0, 0);
        }
    }

    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1854))
        return;

    SetMotionRegionFlag(scenFindMotionwork(sin), 73, 0, MotionRegion::RightArm);
    SetHighModelXYZ(ctrlGetCharaByName(sin), 0, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(sin), 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

void AreaKWW4Init() {
}

void AreaKWW4Main(std::uint32_t startParameters) {
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
}

void AreaKWW4Cleanup(std::int32_t, std::uint32_t) {
    EV_lightLoad(light0000);
    AssignLightProgram(0);
    (void)CheckIsArgEquZero(1);

    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
}

const GameplayAreaDefinition areaKWW4GameplayDefinition{SHEN_NAME(KWW4), 0, AreaKWW4Init, AreaKWW4Main, AreaKWW4Cleanup};

} // namespace shenmue::s2::areas
