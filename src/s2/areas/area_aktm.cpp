#include "s2/areas/area_aktm.hpp"
#include "s2/npc_package.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name event{SHEN_NAME(EVNT)}, base{SHEN_NAME(BASE)}, prop{SHEN_NAME(a015)}, invalid{0xffffffffU};
constexpr shen_name names[]{SHEN_NAME(RYO_), SHEN_NAME(TGI_), SHEN_NAME(HVA_), SHEN_NAME(HV6_), SHEN_NAME(HV7_), SHEN_NAME(HV8_), SHEN_NAME(KEI_), invalid};
constexpr Vector3 retryPositions[]{{-2.5f, 0, 4.4f}, {2, 0, 4.4f}, {2.8f, 0, 3.3f}, {1.5f, 0, 1.4f}, {4.3f, 0, 3.9f}, {1.9f, 0, 2.4f}, {.9f, 0, 2.9f}};
constexpr float retryYaws[]{292.5f, 90, 112.5f, 112.5f, 90, 112.5f, 135};
constexpr float initialYaws[]{0, 180, 150, 220, 90, 250, 120};
struct BattleWork {
    HLib::Task *self;
    std::uint16_t phase, reserved0a;
    std::uint32_t reserved0c;
    EventReadCharactersWork *characters;
};
static_assert(sizeof(BattleWork) == 0x18 && offsetof(BattleWork, characters) == 0x10);
std::int32_t motionHandle = -1, retries;
DateTime savedTime{};
void Battle_Callback(void *);

void AuthorFrame_Callback(void *) {
    if (Author_GetCurrentFrame() == 1) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        EVT_AT_MenCtrl(EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 60});
    }
    if (Author_GetCurrentFrame() == 91) {
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (retries > 0)
            EV_SetAuthorActiveTo(1);
    }
}

void ResetOpponent(shen_name name) {
    SetMotionRegionFlag(scenFindMotionwork(name), 69, 2, MotionRegion::LowerBody);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 0, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(name, false);
    mtwkSetExtraFlag(scenFindMotionwork(name), 55, 2);
    UpdateMotionWorkWithPosition(scenFindMotionwork(name), nullptr, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(name), nullptr, 3);
}

void End_Callback(void *data) {
    auto *work = static_cast<BattleWork *>(data);
    EV_stepSetFlags(0, 136);
    SetCharaKill(base, 1);
    SetCharaKill(prop, 1);
    if (motionHandle != -1)
        DeleteMotion(motionHandle);
    ET_AT_KillWork(work->characters);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->self);
    EV_FreeAuthorPak(0, "0053");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("AKTM"), 0);
    LoadCutsceneAndSoundProgramForArea();
    EV_NPC_NewLoadSw(1);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(ABLB), 0);
    scenFreeEvent("AKTM.EV3");
}

void Setup(BattleWork *work) {
    EV_SetNewSceneIndex();
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("AKTM"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("AKTM"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("AKTM"), 0);
    motionHandle = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    EV_FreeAuthorPak(1, "0053");
    EV_FreeAuthorPak(1, "0138");
    CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, prop);
    EVT_AT_SetPos(base, invalid, Vector3{267.4f, 100, 230.9f}, 0);
    EVT_AT_SetAngY(base, invalid, 0, 0);
    GetCurrentDateTime(savedTime);
    work->phase = 1;
    retries = 0;
}

void InitializeFight(BattleWork *work) {
    for (int i = 0; i < 7; ++i)
        EVT_AT_SetAngY(names[i], invalid, initialYaws[i], 0);
    shenStartFGST(10, 10, 0, 0);
    shenInitFightData(FindCharacterByScenarioName(names[0]), nullptr, 0, 0, static_cast<std::uint16_t>(100 * (retries + 2)), 0);
    vfCopyPosFromEntity(1, FindCharacterByScenarioName(names[0]), 0, 0, 10);
    for (int i = 1; i < 7; ++i) {
        auto *character = FindCharacterByScenarioName(names[i]);
        shenInitFightData(character, nullptr, 1, 1, i == 1 ? 80 : i == 2 ? 60 : 40, 2);
        if (i >= 3) {
            vfUpdateFlagsFor(character, 0, 0x400000, 128);
            vfUpdateFlagsFor(character, 0, 0x200000, 2048);
        }
    }
    for (int i = 0; i < 7; ++i)
        EVT_AT_SetMot(names[i], static_cast<std::uint16_t>(i == 0 ? 306 : 3632), 0, 1, 0x40000000U, 0);
    work->phase = 3;
}

void Battle_Callback(void *data) {
    auto *work = static_cast<BattleWork *>(data);
    std::int32_t delay = 0;
    switch (work->phase) {
    case 0:
        Setup(work);
        break;
    case 1:
        EV_LoadCutsceneFromCurrentScene("0053A000");
        EV_LoadAudio("0053A000", 0);
        EV_spmBeginSampleExecute();
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        QueueSkAuthorStart(work->self, SHEN_NAME(STRT), AuthorFrame_Callback, nullptr, 0);
        work->phase = 2;
        break;
    case 2:
        InitializeFight(work);
        break;
    case 3:
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        camCtrlSetState(CameraState::Event);
        camCtrlSetNextPosition(Vector3{270.909912109375f, 100.92680358886719f, 235.43089294433594f});
        camCtrlSetTargetPosition(Vector3{268.4183044433594f, 100.7426986694336f, 232.3896026611328f});
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        vfmQueueFightStart(2, 2, 1, 1, -1);
        vfmDispatchControl(FightControlOp::CameraParameter, shen_name{}, 13, 1);
        SetPluralControlFor(FindCharacterByScenarioName(names[0]), 1);
        work->phase = 4;
        break;
    case 4:
        if (IsFightActive())
            return;
        if (GetBtlStatus(FindCharacterByScenarioName(names[0])) == 3)
            work->phase = 7;
        else {
            work->phase = 6;
            if (retries < 15)
                ++retries;
            Queue_ST_LoadTitleFadeIn(SHEN_NAME(RTR2));
        }
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        delay = 30;
        break;
    case 5:
        for (int i = 0; i < 7; ++i) {
            EVT_AT_SetPos(names[i], base, retryPositions[i], 0);
            EVT_AT_SetAngY(names[i], base, retryYaws[i], 0);
        }
        SetCurrentDateTimeTo(savedTime);
        work->phase = 6;
        delay = 30;
        break;
    case 6:
        ST_LoadTitleFade();
        work->phase = 1;
        delay = 30;
        break;
    case 7:
        ResetOpponent(names[1]);
        ResetOpponent(names[2]);
        for (int i = 3; i < 7; ++i)
            EV_DisableCharacterDisplay(names[i]);
        QueueSkAuthorStart(work->self, SHEN_NAME(WIN_), nullptr, nullptr, 0);
        work->phase = 8;
        break;
    case 8:
        spmFadeOut();
        HLib::SetCurrentTaskTo(End_Callback);
        delay = 30;
        break;
    default:
        break;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}

void QueueBattle_Callback(void *) {
    auto *task = HLib::EnqueueTaskWithParameter(Battle_Callback, 4, 0x0b, sizeof(BattleWork), event);
    auto *work = static_cast<BattleWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->phase = 0;
    work->reserved0a = 0;
    work->reserved0c = 0;
    work->characters = nullptr;
    work->characters = Queue_EVT_AT_ReadChr(task, names);
}
} // namespace
void AreaAKTMInit() {
    motionHandle = -1;
    savedTime = {};
    retries = 0;
}

void AreaAKTMMain(std::uintptr_t) {
    EVT_AT_MenCtrl(EventFilmControl{1, 1, 0, 0, 0, 0, 255, 0, 0, 0, 1});
    SetPlayerControlEnabled(0);
    SetPlayerControlEnabled(0);
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), event);
    RegisterEventTask(task);
    auto *work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = QueueBattle_Callback;
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
const ScriptAreaDefinition areaAKTMScriptDefinition{.areaId = SHEN_NAME(AKTM), .field04 = 0, .init = AreaAKTMInit, .main = AreaAKTMMain};
} // namespace shenmue::s2::areas
