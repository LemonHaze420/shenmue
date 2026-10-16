#include "s2/areas/area_bjmp.hpp"
#include "s2/pause.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <array>

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, ren{SHEN_NAME(REN_)}, tgy{SHEN_NAME(TGY_)}, dummy{SHEN_NAME(DUM_)}, event{SHEN_NAME(EVNT)};
constexpr shen_name names[]{tgy, ren, shen_name{0xffffffffU}};
struct JumpWork {
    HLib::Task *self;
    std::uint32_t reserved08;
    std::uint16_t phase, retries;
    std::byte reserved10[16];
};
static_assert(sizeof(JumpWork) == 0x20 && offsetof(JumpWork, phase) == 0x0c);
std::array<std::int32_t, 57> hands{0, 0, 46606, 0, 2182, 64444, 0, 0, 1456, 0, 64080, 49154, 0, 908, 44239, 0, 0, 53340, 0, 0, 0, 0, 1456, 54796, 0, 0, 43692,
    0, 727, 50974, 0, 0, 0, 0, 2912, 54797, 0, 182, 44421, 0, 1456, 50245, 0, 0, 0, 2912, 910, 51336, 726, 726, 47517, 0, 2003, 50974, 65518, 65427, 0};
void Auth1_Callback(void *);
void Main_Callback(void *);
void Exit_Callback(void *);
void Camera(std::int32_t camera, std::int32_t delay = 0) {
    EVT_AT_SetCam(camera, dummy, dummy, 0, 0, delay);
}
void Position(shen_name name, const Vector3 &position, float angle, std::int32_t delay = 0) {
    EVT_AT_SetPos(name, dummy, position, delay);
    EVT_AT_SetAngY(name, dummy, angle, delay);
}
void LandingPositions() {
    Position(ryo, Vector3{-0.36f, -0.15f, -1.28f}, 180.0f);
    Position(ren, Vector3{0.5f, -0.15f, -0.37f}, 180.0f);
}
void LandingMotions() {
    EVT_AT_SetMot(ryo, 9575, 0, 1, 0, 0);
    EVT_AT_SetMot(ren, 9543, 0, 1, 0, 0);
}
void ResetLights() {
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
}
void BeginRun() {
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(SHEN_NAME(0000));
    AssignLightProgram(0);
    SetScenarioCharacterDrawControl(ryo, 2, 0);
    SetScenarioCharacterDrawControl(ren, 2, 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(ren), 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(tgy), 0);
    EVT_AT_SetMot(tgy, 9611, 0, 1, 0, 0);
    EVT_AT_SetMot(ryo, 9577, 0, 1, 0, 0);
    EVT_AT_SetMot(ren, 9546, 0, 1, 0, 0);
    Camera(10);
    camCtrlSetFovTransitionTo(true);
}
int JumpSuccess() {
    ResetLights();
    EVT_AT_SetVoice("0218A009", 1, 20); // @text EN: " " | JA: "うわあっ！"
    Camera(110);
    LandingMotions();
    Position(tgy, Vector3{0, -100, 0}, 180);
    LandingPositions();
    EV_DisableCharacterDisplay(tgy);
    return 50;
}
int BeginLanding() {
    Camera(100);
    LandingMotions();
    LandingPositions();
    return 50;
}
void FallingPosition_Callback(void *) {
    AT_SetLocalBaseChara(ryo, dummy);
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    AT_SetLocalBaseChara(ryo, shen_name{0xffffffffU});
    EVT_AT_SetPos(ryo, ryo, Vector3{0, -0.015f, -0.03f}, 0);
    EVT_AT_SetPos(ren, ren, Vector3{0, -0.015f, -0.03f}, 0);
    if (position.y <= -1.08f)
        HLib::DestroyCurrentTask();
}
int EndLanding() {
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(tgy), 1, 10.0f, 1.0f);
    SetLowQualityHands(FindCharacterByScenarioName(tgy));
    SetHighModelXYZ(FindCharacterByScenarioName(tgy), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(tgy), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(tgy), 0, reinterpret_cast<std::byte *>(hands.data()), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(tgy), 1, reinterpret_cast<std::byte *>(hands.data()), 0);
    EV_EnableCharacterDisplay(tgy);
    Camera(20);
    Camera(25, 39);
    Camera(30, 80);
    Camera(35, 170);
    Camera(40, 230);
    Camera(50, 480);
    EVT_AT_SetSound(1405, 60);
    EVT_AT_MenCtrl(EventFilmControl{1, 90, 0, 0, 0, 0, 255, 0, 0, 0, 510});
    EVT_AT_FadeBGM(90, 1, 510);
    LandingMotions();
    EVT_AT_SetMot(ryo, 9577, 0, 1, 0, 280);
    EVT_AT_SetMot(ren, 9546, 0, 1, 0, 280);
    EVT_AT_SetMot(tgy, 9634, 180, 1, 0, 80);
    LandingPositions();
    Position(tgy, Vector3{0, -0.2f, 0.8f}, 180, 80);
    Position(ryo, Vector3{0, -3.8f, 6.4f}, 140, 280);
    Position(ren, Vector3{0.8f, -3.8f, 6.9f}, 140, 280);
    EVT_AT_SetVoice("0218A013", 1, 25);
    EVT_AT_SetVoice("0218B010", 1, 120); // @text EN: "Don't ever think that\nyou can catch me so easily!" | JA: "俺様を捕まえようなんざ、\n１０年早ぇんだよ！"
    EVT_AT_SetVoice("0218D005", 1, 280); // @text EN: "Damn!\nI'll get you for this!" | JA: "くっそぉ！\n覚えてやがれ！"
    SetScenarioCharacterDrawControl(tgy, 0, 0);
    HLib::EnqueueTaskWithoutParameter(FallingPosition_Callback, 4, 0x0b, event.name);
    return 600;
}
void Retry(JumpWork *work) {
    EV_DisableCharacterDisplay(tgy);
    ResetLights();
    HLib::SetCurrentTaskTo(Auth1_Callback);
    QueueSkAuthorStart(work->self, SHEN_NAME(QJP1), nullptr, nullptr, 0);
    if (work->retries)
        EV_SetAuthorActiveTo(1);
    if (work->retries <= 2)
        ++work->retries;
}

void Auth1_Callback(void *data) {
    auto *work = static_cast<JumpWork *>(data);
    work->phase = 0;
    EV_cuffsAttach(ryo, 1, ren, 0, 0.3f);
    EV_EnableCharacterDisplay(tgy);
    EV_LoadAudio("0218A000", 0);
    EV_spmBeginSampleExecute();
    DateTime time{};
    GetCurrentDateTime(time);
    time.hour = 8;
    time.minute = 30;
    SetCurrentDateTimeTo(time);
    EV_SetModelDataDefaultVertexShader();
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(SHEN_NAME(BJMP));
    AssignLightProgram(0);
    EV_AssignLightParams(1, 1);
    EV_AssignLightParams(1, 2);
    EV_AssignLightParams(1, 3);
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 2});
    HLib::SetCurrentTaskTo(Main_Callback);
    QueueSkAuthorStart(work->self, SHEN_NAME(QJP0), nullptr, nullptr, 0);
    if (work->retries)
        EV_SetAuthorActiveTo(1);
}

void Main_Callback(void *data) {
    auto *work = static_cast<JumpWork *>(data);
    int delay = 0;
    switch (work->phase) {
    case 0: {
        const Vector4 board{1, 1, 1, 1};
        for (shen_name name : {ryo, ren, tgy}) {
            SetScenarioReflectorBoardValues(name, &board);
            EV_SetRefBoardParamsFor(name, 1);
            EV_SetRefBoardParamsFor(name, 1);
        }
        Position(ryo, Vector3{-0.4f, 0, -5.4f}, 180);
        Position(ren, Vector3{0.4f, 0, -4.9f}, 180);
        Position(tgy, Vector3{0.1f, -0.2f, -8.9f}, 180);
        qteBattleUtil(BattleQteMode::SetWait, 2, 6, 16);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 1, 1, 256);
        qteBattleUtil(BattleQteMode::SetLastCommands);
        qteBattleUtil(BattleQteMode::ResetStates);
        BeginRun();
        ++work->phase;
        HLib::SetupTaskDelay(28);
        return;
    }
    case 1:
        EVT_AT_SetMot(tgy, 0xf002, 0, 1, 0, 40);
        qteBattleUtil(BattleQteMode::SetWait, 2, work->retries + 6, 4 * work->retries + 16);
        qteBattleUtil(BattleQteMode::Start);
        ++work->phase;
        return;
    case 2:
        if (!qteBattleUtil(BattleQteMode::GetResult))
            return;
        if (qteBattleUtil(BattleQteMode::GetCompletedCount) == 3)
            delay = JumpSuccess();
        else {
            Retry(work);
            return;
        }
        break;
    case 3:
        delay = BeginLanding();
        break;
    case 4:
        delay = EndLanding();
        break;
    case 5:
        HLib::SetCurrentTaskTo(Exit_Callback);
        return;
    default:
        return;
    }
    ++work->phase;
    if (delay)
        HLib::SetupTaskDelay(delay);
}

void Exit_Callback(void *data) {
    auto *work = static_cast<JumpWork *>(data);
    hndlSetFlags(FindCharacterByScenarioName(tgy));
    qteBattleUtil(BattleQteMode::RetireAndFree);
    FinishLoading();
    EV_cuffsSetFlags();
    EV_stepSetFlags(0, 272);
    EV_stepSetFlags(0, 317);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity | CharacterControlUpdateHeight, NpcCharacterControllerState::Enabled);
    for (shen_name name : {ryo, tgy, ren}) {
        EV_npcFree(name);
        EV_npcLockUnlock(name);
    }
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    SetCharaKill(dummy, 1);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QJMP"), 0);
    SetPlayerControlEnabled(1);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->self);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(0221), 0);
}
} // namespace

void AreaBJMPInit() {
}

void AreaBJMPMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    auto *task = HLib::EnqueueTaskWithParameter(Auth1_Callback, 5, 0x0b, sizeof(JumpWork), event);
    auto *work = static_cast<JumpWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->retries = 0;
    CreateEventCharacter(CharacterCreationHighModel, nullptr, dummy);
    EVT_AT_SetPos(dummy, shen_name{0xffffffffU}, Vector3{215.3f, 111, 197}, 0);
    EVT_AT_SetAngY(dummy, shen_name{0xffffffffU}, 270, 0);
    Queue_EVT_AT_ReadChr(work->self, names);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QJMP"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity | CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    SetRenderClockAndCompassTo(1);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QJMP"), 0);
    EV_LoadCutsceneFromCurrentScene("0218A000");
    EV_FreeAuthorPak(1, "0218");
}
const ScriptAreaDefinition areaBJMPScriptDefinition{.areaId = SHEN_NAME(BJMP), .field04 = 0, .init = AreaBJMPInit, .main = AreaBJMPMain};
} // namespace shenmue::s2::areas
