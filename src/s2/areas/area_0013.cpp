#include "s2/debug.hpp"
#include "s2/pause.hpp"
#include "s2/areas/area_0013.hpp"
#include "s2/pak.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>

namespace shenmue::s2::areas {
namespace {
std::array<std::byte, 0x100> distance0013{}, kernel0013First{}, kernel0013Second{};
void E0013MainCallback(void *);
void E0013LoopCallback(void *);
// @verified - p
void E0013Init() {
    distance0013.fill(std::byte{});
    kernel0013First.fill(std::byte{});
    kernel0013Second.fill(std::byte{});
}
// @verified - p
void E0013EnvironmentNullsub() {
}
// @verified - p
void E0013WaveCallback(void *) {
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.36070001125335693f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.3725000023841858f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.4352000057697296f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.2549000084400177f, static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3799999952316284f,
            static_cast<float>(specular & 255U) / 255.0f * 0.28619998693466187f},
        42.540000915527344f, 1.0f);
    ApplyWaveTextureMeshResource(15);
    ApplyWaveReflectionResource(16);
}
// @verified - p
void SetupE0013Environment() {
    const Vector3 center{220.63f, 100.0f, 343.79999f};
    SetPlayerControlEnabled(0);
    EV_stepSetFlags(0, 27);
    if (npcSetAreaName(AREA_0013_ANP1, &center, 5.0f, 1) == 1)
        EV_debug_log("0013\\e0013_et_0013.c", 221, "NPC_AREA1 : TRUE\n");
    EV_SetNewSceneIndex();
    if (g_currentMap.name == AREA_0013_AREA.name) {
        GenerateFFT(2.0f);
        FillFftDistance(FillFftKernel(23.52899932861328f, GenerateDistanceFalloffRowMajor(2.0820000171661377f, distance0013)));
        void *const second = FillFftKernel(0.03500000014901161f, GenerateDistanceFalloffKernel8x8(0.9520000219345093f, kernel0013Second));
        void *const first = FillFftKernel(0.05640000104904175f, GenerateDistanceFalloffKernel8x8(0.9399999976158142f, kernel0013First));
        ApplyFft(first, second);
        FinalizeFft(0.0020000000949949026f, 0.0020000000949949026f);
        SetWaveLighting(0.1f, {0.059799999f, 0.97680002f, 0.2054f});
        HLib::EnqueueTaskWithoutParameter(E0013WaveCallback, 3, 2, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
    }
    EV_LoadCutsceneFromCurrentScene("0013A000");
    EV_FreeAuthorPak(1, "0013");
    E0013EnvironmentNullsub();
}
// @verified - p
void SetupE0013Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0013_CLJ), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0013_CLJ, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0013_WON), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0013_WON, false);
    EV_SetCharacterAlphaEnabled(AREA_0013_A014, false);
}
// @verified - p
void RestoreE0013Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0013_WON), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0013_WON, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0013_CLJ), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0013_CLJ, true);
    EV_SetCharacterAlphaEnabled(AREA_0013_A014, true);
}
// @verified - p
void E0013Main(std::uintptr_t) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0013MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    SetupE0013Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0013_CHARACTERS, parent->self);
}
// @verified - p
void QueueE0013Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0013LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    *static_cast<AreaImmediateAuthorTaskParameter **>(HLib::GetTaskParameter(task)) = parent;
    EV_LoadAudio("0013A000", 0);
    EV_spmBeginSampleExecute();
    EV_NPC_NewLoadSw(1);
    SetupE0013Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0013_AREA.name));
    parent->state = 2;
}
void CleanupE0013(AreaImmediateAuthorTaskParameter *parent) {
    using enum CameraState;
    EV_FreeAuthorPak(0, "0013");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0013A"), 0);
    npcSetAreaName(AREA_0013_ANP1, nullptr, 0.0f, 0);
    EV_CleanupMapControlTask();
    if (g_currentMap.name == AREA_0013_AR02.name) {
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {220.52000427246094f, 100.19999694824219f, 341.3299865722656f}, 0};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 267, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        LoadCutsceneAndSoundProgramForArea();
        EV_ResetFreeTalkFlags();
        EV_NPC_NewLoadSw(0);
        EV_ForceLoadNpc(AREA_0013_WON);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        EV_CheckWarmStartNpcRoute(AREA_0013_WON);
        EV_NPC_NewLoadSw(1);
        mtwkRequestMotion(scenFindMotionwork(AREA_AUTHOR_RYO), 0xf003, MotionRegion::LowerBody);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        parent->state = 4;
    } else {
        HLib::DestroyTask(parent->self);
        FinishLoading();
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
void E0013MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0013Loop(parent);
    else if (parent->state == 3)
        CleanupE0013(parent);
    else if (parent->state == 4 && !filmCheckFadeTrackColour()) {
        SetPlayerControlEnabled(1);
        FinishLoading();
        EV_npcLockUnlock(AREA_0013_WON);
        SetScenarioFollowCharacter(AREA_0013_WON);
        HLib::DestroyTask(parent->self);
    } else if (parent->state == 0) {
        EV_npcLockUnlock(AREA_0013_WON);
        EV_npcFree(AREA_0013_WON);
        EV_npcLockUnlock(AREA_0013_CLJ);
        EV_npcFree(AREA_0013_CLJ);
        EV_LoadNPCRoute("NPC_AR02.BIN");
        HLib::SetupTimedTask(parent->self, 30);
        parent->state = 3;
    }
}
// @verified - p
void E0013LoopCallback(void *taskData) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    RestoreE0013Characters();
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    AreaImmediateAuthorTaskParameter *const parent = *static_cast<AreaImmediateAuthorTaskParameter **>(taskData);
    parent->state = 0;
    HLib::SetupTimedTask(parent->self, 10);
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0013ScriptDefinition{.areaId = AREA_0013_AREA, .field04 = 0, .init = E0013Init, .main = E0013Main};
} // namespace shenmue::s2::areas
