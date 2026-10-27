#include "s2/areas/area_0015.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <array>
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name characters0[]{SHEN_NAME(JOP_), SHEN_NAME(HCR_), SHEN_NAME(KRY_), SHEN_NAME(KRN_), SHEN_NAME(KAC_), SHEN_NAME(MKR_), SHEN_NAME(SYS_),
    SHEN_NAME(SHU_), AREA_AUTHOR_INVALID};
constexpr shen_name characters1[]{SHEN_NAME(JOP_), AREA_AUTHOR_INVALID};
constexpr shen_name characters2[]{SHEN_NAME(TNZ_), AREA_AUTHOR_INVALID};
constexpr const shen_name *characters[]{characters0, characters1, characters2};
constexpr std::array<shen_name, 5> props2{
    SHEN_NAME(a020) | 0x10000U, SHEN_NAME(a822) | 0x4000000U, SHEN_NAME(a005) | 0x30000U, SHEN_NAME(a003) | 0x30000U, SHEN_NAME(ab11) | 0x20000U};
struct Placement {
    shen_name name;
    std::uint16_t motion;
    Vector3 position;
    Vector3i rotation;
};
constexpr Placement placements[]{{SHEN_NAME(HCR_), 32788, {71.69999694824219f, 100, 169.8800048828125f}, {0, 31743, 0}},
    {SHEN_NAME(KRY_), 32813, {59.560001373291016f, 100, 171.8699951171875f}, {0, 44031, 0}},
    {SHEN_NAME(KRN_), 33024, {70.29000091552734f, 100, 179.39999389648438f}, {0, 58367, 0}},
    {SHEN_NAME(KAC_), 32947, {83.18000030517578f, 100, 172.5f}, {0, 16384, 0}},
    {SHEN_NAME(MKR_), 32772, {62.36000061035156f, 100, 179.1300048828125f}, {0, 0, 0}},
    {SHEN_NAME(SYS_), 32799, {52.220001220703125f, 100, 179.3000030517578f}, {0, 0, 0}},
    {SHEN_NAME(SHU_), 32809, {83.12999725341797f, 100, 177.75f}, {0, 16384, 0}}};
void MainCallback(void *);
void LoopCallback(void *);
// @verified - p
void InitNullsub() {
}
// @verified - p
void ConfigureEnvironment(AreaImmediateAuthorTaskParameter *parent) {
    shenSetClockAndCompassRenderTo(1);
    EV_SetNewSceneIndex();
    if (g_currentMap.name == SHEN_NAME(0015)) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(parent->selection == 2 ? "0015A2" : "0015A1"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        if (parent->selection != 2) {
            DateTime time;
            GetCurrentDateTime(time);
            if (time.hour < 19) {
                EV_ReinitLights(1);
                EV_SetLightFlagForIndex(0, 2);
                EV_SetIndexForLightEntry(0, 1);
            } else {
                EV_lightLoad(SHEN_NAME(0000));
                PushGlobalLightingProgram();
            }
        }
    } else if (g_currentMap == AREA_WSY0) {
        EV_DisableCharacterDisplay(SHEN_NAME(fun0));
        EV_DisableCharacterDisplay(SHEN_NAME(rado));
    }
    EV_LoadCutsceneFromCurrentScene(parent->selection == 2 ? "0015A020" : "0015A000");
    EV_FreeAuthorPak(1, "0015");
}
void SetCharacters(std::int16_t selection, bool restore) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, restore ? 0 : 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, restore);
    const shen_name actor = selection == 2 ? characters2[0] : characters1[0];
    mtwkSetExtraFlag(scenFindMotionwork(actor), 6, restore ? 0 : 2);
    EV_SetCharacterAlphaEnabled(actor, restore);
    if (selection == 2) {
        for (shen_name name : props2)
            EV_SetCharacterAlphaEnabled(name, restore);
    } else {
        const shen_name prop{SHEN_NAME(aa39) | 0x40000U};
        EV_SetCharacterAlphaEnabled(prop, restore);
        if (!restore && !EV_GetShenmueFlag(255)) {
            Vector3 position{0, -1000, 1000};
            EV_scenApplyPosOrRotToPartOf(prop, 0xca, &position, nullptr, EventCharacterPartTransformMode::Absolute);
            EV_scenApplyPosOrRotToPartOf(prop, 0xcb, &position, nullptr, EventCharacterPartTransformMode::Absolute);
        }
    }
}
// @verified - p
void SetupCharacters(std::int16_t selection) {
    SetCharacters(selection, false);
}
// @verified - p
void RestoreCharacters(std::int16_t selection) {
    SetCharacters(selection, true);
}
// @verified - p
void QueueLoop(AreaImmediateAuthorTaskParameter *parent) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    *static_cast<AreaImmediateAuthorTaskParameter **>(HLib::GetTaskParameter(task)) = parent;
    if (g_currentMap.name == SHEN_NAME(0015)) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(parent->selection == 2 ? "WSY0" : "WS00"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    if (parent->selection == 2)
        EV_LoadAudio("0015A020", 0);
    else if (parent->selection == 1)
        shenPlayAudioWithFlags("0015D001", 1);
    else
        for (const Placement &placement : placements) {
            EV_AT_SetPos(placement.name, EventTransformFlags::AllAxes, placement.position);
            EV_AT_SetAngY(placement.name, EventTransformFlags::AllAxes, placement.rotation);
            mtwkRequestMotion(scenFindMotionwork(placement.name), placement.motion, MotionRegion::LowerBody);
        }
    EV_spmBeginSampleExecute();
    SetupCharacters(parent->selection);
    constexpr AuthSceneCtrl scenes[]{
        static_cast<AuthSceneCtrl>(SHEN_NAME(JOY1)), static_cast<AuthSceneCtrl>(SHEN_NAME(JOY2)), static_cast<AuthSceneCtrl>(SHEN_NAME(HOTE))};
    EV_authSceneCtrl(scenes[parent->selection]);
    parent->state = 2;
}
// @verified - p
void Cleanup(AreaImmediateAuthorTaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *name = characters[parent->selection]; name->name != AREA_AUTHOR_INVALID.name; ++name)
        if (FindCharacterByScenarioName(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(parent->selection == 2 ? "0015A2" : "0015A1"), 0);
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0015");
    EV_CleanupMapControlTask();
    if (g_currentMap.name == SHEN_NAME(0015)) {
        for (const char *name : {"WS00", "WSY0"})
            if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(name), 0))
                EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(name), 0);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        return;
    }
    if (parent->selection == 0) {
        HLib::DestroyTask(parent->self);
        LoadCutsceneAndSoundProgramForArea();
        EV_stepSetFlags(0, 190);
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {79.19999694824219f, 99.9000015258789f, 164.8800048828125f}, 0};
        EVT_AT_SetPosRot(&position);
        EVT_AT_SetAng_Enqueue(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0, 0, 0, 0);
        SetNpcColdResetMode(1);
        shenSetClockAndCompassRenderTo(0);
    } else if (parent->selection == 1) {
        SetNpcColdResetMode(1);
        HLib::DestroyTask(parent->self);
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(WSY0), 1);
    } else if (parent->selection == 2) {
        HLib::DestroyTask(parent->self);
        ExecuteCleanupFuncForAreaWithCachedScene();
        EV_NPC_NewLoadSw(1);
        EV_stepSetFlags(0, 16);
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {65, 98.19999694824219f, 120.3499984741211f}, 0};
        EVT_AT_SetPosRot(&position);
        EVT_AT_SetAng_Enqueue(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, 0, 270, 0, 0);
        EV_EnableCharacterDisplay(SHEN_NAME(fun0));
        EV_EnableCharacterDisplay(SHEN_NAME(rado));
        EV_SetWeatherInstruction(12, 1);
        EV_SetWeatherInstruction(0x780, 1);
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        shenSetClockAndCompassRenderTo(0);
    }
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    SetPlayerControlEnabled(1);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
// @verified - p
void LoopCallback(void *data) {
    auto *parent = *static_cast<AreaImmediateAuthorTaskParameter **>(data);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 302 && parent->selection == 0)
        shenPlayAudioWithFlags("0015D001", 1);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    parent->state = 3;
    RestoreCharacters(parent->selection);
    HLib::DestroyCurrentTask();
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<AreaImmediateAuthorTaskParameter *>(data);
    if (parent->state == 1)
        QueueLoop(parent);
    else if (parent->state == 3)
        Cleanup(parent);
}
// @verified - p
void Main(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    auto *parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    if (g_currentMap.name == SHEN_NAME(0015))
        parent->selection = static_cast<std::int16_t>(g_entryPoint > 2 ? 0 : g_entryPoint);
    else {
        parent->selection = *reinterpret_cast<const std::int16_t *>(argument);
        if (parent->selection <= 1) {
            ReleaseNpcScriptRouteOwner();
            SetScenarioFollowCharacter(shen_name{0});
            SetNpcColdResetMode(0);
        }
    }
    ConfigureEnvironment(parent);
    Queue_EVT_UTL_LoadNPCChara_3(characters[parent->selection], parent->self);
}
} // namespace
const ScriptAreaDefinition area0015ScriptDefinition{.areaId = SHEN_NAME(0015), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
