#include "s2/areas/area_btel.hpp"
#include "s2/pause.hpp"
#include "s2/hand.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/elevator.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name event{SHEN_NAME(EVNT)}, ryo{SHEN_NAME(RYO_)}, lift{SHEN_NAME(R599)}, propA{SHEN_NAME(a512)}, propB{SHEN_NAME(a516)};
constexpr shen_name names[]{ryo, SHEN_NAME(BF1_), SHEN_NAME(BF7_), SHEN_NAME(BF8_), SHEN_NAME(KF5_), SHEN_NAME(BF9_), SHEN_NAME(KR3_), shen_name{0xffffffffU}};
constexpr Vector3 positions[]{{245.56f, 164.8f, 119.24f}, {248.17999f, 164.8f, 118.83f}, {247.25f, 164.8f, 120.96f}, {248.05f, 164.8f, 117.6f},
    {245.72f, 164.8f, 117.73f}, {248.5f, 164.8f, 120.19f}, {245.38f, 164.8f, 120.55f}};
constexpr std::int32_t yaws[]{52308, 16384, 5679, 23185, 28253, 12743, 1092};
constexpr std::uint16_t motions[]{0x132, 0xe30, 0xe34, 0xe38, 0xe30, 0xe34, 0xe38};
constexpr std::uint16_t health[]{0, 120, 80, 60, 60, 80, 100};
constexpr std::uint8_t styles[]{0, 2, 3, 4, 2, 3, 4};
struct BattleWork {
    HLib::Task *self;
    std::uint8_t result, phase, retries, reserved0b;
    std::int32_t delay;
    EventReadCharactersWork *characters;
};
struct AuthorWork {
    HLib::Task *self;
    std::uint32_t reserved08;
    shen_name author;
    std::uint16_t phase;
    std::byte reserved12[6];
};
static_assert(sizeof(BattleWork) == 0x18 && offsetof(BattleWork, characters) == 0x10);
static_assert(sizeof(AuthorWork) == 0x18 && offsetof(AuthorWork, phase) == 0x10);
constexpr std::int32_t liftFrames[][2]{
    {0, 0}, {30, 1}, {60, 2}, {90, 3}, {120, 4}, {240, 4}, {270, 5}, {300, 6}, {330, 7}, {360, 8}, {410, 16}, {440, 17}, {470, 18}};
void RestoreAuthorAlpha(bool enabled) {
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 6, enabled ? 0 : 2);
    EV_SetCharacterAlphaEnabled(ryo, enabled);
    EV_SetCharacterAlphaEnabled(propA, enabled);
    EV_SetCharacterAlphaEnabled(propB, enabled);
    for (int i = 1; i < 7; ++i) {
        mtwkSetExtraFlag(scenFindMotionwork(names[i]), 6, enabled ? 0 : 2);
        EV_SetCharacterAlphaEnabled(names[i], enabled);
    }
}
void AuthorLoop_Callback(void *data) {
    auto *owner = *static_cast<AuthorWork **>(data);
    const std::int16_t frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    for (const auto &row : liftFrames)
        if (row[0] == frame)
            SetCurrentElevatorFloor(lift, static_cast<std::int16_t>(row[1]));
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetCurrentElevatorFloor(lift, 18);
        RestoreAuthorAlpha(true);
        owner->phase = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void AuthorMain_Callback(void *data) {
    auto *owner = static_cast<AuthorWork *>(data);
    if (owner->phase == 1) {
        auto *task = HLib::EnqueueTaskWithParameter(AuthorLoop_Callback, 4, 0x0b, sizeof(AuthorWork *), event);
        *static_cast<AuthorWork **>(HLib::GetTaskParameter(task)) = owner;
        EV_spmBeginSampleExecute();
        RestoreAuthorAlpha(false);
        adfsSetCameraIdxForChara(names[1], SHEN_NAME(damy));
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(owner->author.name));
        owner->phase = 2;
    } else if (owner->phase == 3) {
        EV_FreeAuthorPak(0, "0309");
        HLib::DestroyTask(owner->self);
    }
}
void PreparePackage() {
    SetPlayerControlEnabled(0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("BTEL"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0309A000");
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("BTEL"), 0);
    EV_LoadAudio("0309A000", 0);
    EV_spmBeginSampleExecute();
}
void Exit(BattleWork *work) {
    SetPluralControlFor(FindCharacterByScenarioName(ryo), 0);
    flddSetPointFlagOnGrid(SHEN_NAME(BTEL), 0);
    flddSetPointFlagOnGrid(SHEN_NAME(0000), 1);
    EV_EnsureCharacterDraw(propA);
    EV_DisableCharacterDisplay(propA);
    EV_EnsureCharacterDraw(propB);
    EV_DisableCharacterDisplay(propB);
    ET_AT_KillWork(work->characters);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BTEL"), 0);
    LoadCutsceneAndSoundProgramForArea();
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->self);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
    scenFreeEvent("BTEL.EV3");
}

void Battle_Callback(void *data) {
    auto *work = static_cast<BattleWork *>(data);
    if (work->delay) {
        --work->delay;
        return;
    }
    switch (work->phase) {
    case 0:
        if (NpcWait())
            return;
        work->phase = 1;
        [[fallthrough]];
    case 1: {
        shenStartFGST(10, 10, 0, 0);
        vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("BTEL"), 1);
        shenInitFightData(FindCharacterByScenarioName(ryo), nullptr, 0, 0, static_cast<std::uint16_t>(100 * (work->retries + 2)), 0);
        for (int i = 1; i < 7; ++i)
            shenInitFightData(FindCharacterByScenarioName(names[i]), nullptr, 1, 1, health[i], styles[i]);
        LoadBlurModelForCharacter(ryo, "RYO_B.MT7", 1);
        SetDoorSubjectEnabled(lift, 0);
        auto *task = HLib::EnqueueTaskWithParameter(AuthorMain_Callback, 4, 0x0b, sizeof(AuthorWork), event);
        auto *author = static_cast<AuthorWork *>(HLib::GetTaskParameter(task));
        author->self = task;
        author->phase = 1;
        author->author = SHEN_NAME(AU_0);
        SetPlayerControlEnabled(0);
        EV_FreeAuthorPak(1, "0309");
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (work->retries)
            EV_SetAuthorActiveTo(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
        work->phase = 2;
        break;
    }
    case 2:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->phase = 3;
        break;
    case 3:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->phase = 4;
        break;
    case 4:
        SetDoorSubjectEnabled(lift, 1);
        for (int i = 0; i < 7; ++i)
            EV_AT_SetPos(names[i], EventTransformFlags::World | EventTransformFlags::AllAxes, positions[i]);
        for (int i = 0; i < 7; ++i)
            EV_AT_SetAngY(names[i], EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, yaws[i], 0});
        for (int i = 0; i < 7; ++i)
            mtwkRequestMotion(scenFindMotionwork(names[i]), motions[i], MotionRegion::LowerBody);
        vfCopyPosFromEntity(0, nullptr, 244.37f, 121.8f, 6.8f);
        EV_LoadCutsceneFromCurrentScene("0309E000");
        EV_LoadAudio("0309E000", 0);
        EVT_AT_SetCam(1000, ryo, ryo, 0, 0, 0);
        camCtrlDisableFilmBars();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
        work->phase = 5;
        break;
    case 5:
        if (camCtrlCheckIsEventCam())
            return;
        FinishLoading();
        for (int i = 0; i < 7; ++i) {
            auto *character = FindCharacterByScenarioName(names[i]);
            SetPluralControlFor(character, 1);
            SetScenarioCharacterType(names[i], CharacterTypeDefault);
            SetFaceFlagsForChara_0(character, FacialNodeOriginal);
        }
        SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 0, 0);
        EV_DisableCharacterDisplay(propA);
        EV_ToggleCharacterFlag(propA);
        EV_DisableCharacterDisplay(propB);
        EV_ToggleCharacterFlag(propB);
        flddSetPointFlagOnGrid(SHEN_NAME(0000), 0);
        flddSetPointFlagOnGrid(SHEN_NAME(BTEL), 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        vfmQueueFightStart(7, 2, 1.0f, 1, -1);
        vfmDispatchControl(FightControlOp::CameraParameter, shen_name{}, 12, 1);
        work->phase = 6;
        break;
    case 6:
        if (IsFightActive())
            return;
        work->result = static_cast<std::uint8_t>(GetBtlStatus(FindCharacterByScenarioName(ryo)));
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        if (work->result == 3) {
            work->phase = 9;
            work->delay = 30;
        } else {
            Queue_ST_LoadTitleFadeIn(SHEN_NAME(RTR2));
            work->phase = 7;
            work->delay = 60;
        }
        break;
    case 7:
        ST_LoadTitleFade();
        work->phase = 8;
        work->delay = 30;
        break;
    case 8:
        EV_LoadCutsceneFromCurrentScene("0309A000");
        EV_LoadAudio("0309A000", 0);
        if (++work->retries > 10)
            work->retries = 10;
        work->phase = 1;
        break;
    case 9:
        EV_AT_SetPos(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{248.69f, 164.8f, 118.81f});
        EV_AT_SetAngY(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 50112, 0});
        work->phase = 10;
        work->delay = 5;
        break;
    case 10:
        Exit(work);
        break;
    default:
        break;
    }
}
} // namespace
void AreaBTELInit() {
}

void AreaBTELMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    auto *task = HLib::EnqueueTaskWithParameter(Battle_Callback, 4, 0x0b, sizeof(BattleWork), event);
    auto *work = static_cast<BattleWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->phase = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, names);
    work->delay = 0;
    work->retries = 0;
    PreparePackage();
    SetRenderClockAndCompassTo(1);
}
const ScriptAreaDefinition areaBTELScriptDefinition{.areaId = SHEN_NAME(BTEL), .field04 = 0, .init = AreaBTELInit, .main = AreaBTELMain};
} // namespace shenmue::s2::areas
