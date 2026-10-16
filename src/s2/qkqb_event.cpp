#include "s2/qkqb_event.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/system_modules.hpp"

namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, ren{SHEN_NAME(REN_)}, base{SHEN_NAME(BASE)}, invalid{0xffffffffU};
constexpr std::array<std::int32_t, 6> audioVariants{0, 0, 1, 1, 2, 0};

std::array<shen_name, 8> characterNames{ryo, ren, invalid, invalid, invalid, invalid, invalid, shen_name{}};
DateTime savedDateTime{};
std::int32_t handCommandOpen[19][3] = {{0, 0, 46606}, {0, 2182, 64444}, {0, 0, 1456}, {0, 64080, 49154}, {0, 908, 44239}, {0, 0, 53340}, {0, 0, 0},
    {0, 1456, 54796}, {0, 0, 43692}, {0, 727, 50974}, {0, 0, 0}, {0, 2912, 54797}, {0, 182, 44421}, {0, 1456, 50245}, {0, 0, 0}, {2912, 910, 51336},
    {726, 726, 47517}, {0, 2003, 50974}, {65518, 65427, 0}};
std::int32_t handCommandClosed[19][3] = {{1999, 65354, 60806}, {1638, 180, 63171}, {546, 64719, 3365}, {0, 64810, 53705}, {0, 64810, 49335}, {0, 0, 60075},
    {0, 0, 64811}, {0, 0, 56800}, {0, 0, 47516}, {0, 65172, 56618}, {0, 0, 65354}, {0, 729, 55706}, {0, 65173, 49338}, {0, 64990, 56256}, {0, 0, 0},
    {0, 0, 56434}, {0, 1639, 49882}, {0, 182, 59349}, {64080, 65354, 64444}};

void Position(shen_name character, shen_name parent, const Vector3 &position) {
    EventSetPosition request{0, character, parent, position, 0};
    EVT_AT_SetPosRot(&request);
}
void Angle(shen_name character, shen_name parent, float degrees) {
    EventSetAngle request{0, character, parent, degrees, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void PositionAngle(shen_name character, shen_name parent, const Vector3 &position, float degrees) {
    EventSetPositionAngle request{0, character, parent, position, degrees, 0};
    EVT_AT_SetPosAng_Callback(&request);
}
void Motion(shen_name character, std::uint32_t animation, float frame) {
    EventSetMotion request{0, character, animation, frame, 1, 0x40000000U, 0};
    EVT_AT_SetMot_Callback(&request);
}
void Camera(std::int32_t camera) {
    EventSetCamera request{0, camera, base, base, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void FadeFromBlack() {
    EventFilmControl request{0, 30, 255, 0, 0, 0, 0, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&request);
}
std::int32_t MotionFrames(std::uint16_t motion) {
    const MotnSequence *const sequence = FindLoadedMotionSequence(motion);
    return sequence ? sequence->frameCount : 0;
}


std::int32_t Intro(shen_name enemy, std::int32_t mode) {
    Position(ryo, base, {5.2f, 0, 1.2f});
    Angle(ryo, base, 90);
    Position(enemy, base, {-1.3f, 0, -12.3f});
    Angle(enemy, base, 180);
    EVT_AT_SetCam(mode == 3 ? 7311 : 7305, base, base, 0, 0, 125);
    Camera(7301);
    EVT_AT_SetCam(7304, base, base, 0, 0, 70);
    Motion(ryo, 0xf005, 2);
    const std::int32_t delay = EVT_AT_SetMot(ryo, 8193, 2, 1, 0x20000000U, 120);
    EVT_AT_SetMot(ryo, 8194, 2, 1, 0x20000000U, delay);
    Motion(enemy, 8219, 2);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(ryo), 1, 1, 1);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(ren), 1, 1, 1);
    return delay + 80;
}

std::int32_t Failure(shen_name enemy, std::int32_t mode) {
    switch (mode) {
    case 0: EVT_AT_SetVoice("0299A003",1,55); EVT_AT_SetVoice("0299D001",1,70); EVT_AT_SetVoice("0299A007",1,95); break; // @text 0299A003 EN: " " | JA: "んっ…" ; 0299D001 EN: "Who are you!?" | JA: "なんだキサマァ！" ; 0299A007 EN: "Can't be helped!" | JA: "仕方ないっ！"
    case 1: EVT_AT_SetVoice("0299A004",1,50); EVT_AT_SetVoice("0299E001",1,70); EVT_AT_SetVoice("0299A009",1,110); break; // @text 0299A004 EN: " " | JA: "うっ…" ; 0299E001 EN: "You=@!" | JA: "おっ…テメェ！" ; 0299A009 EN: "=@gotta fight!" | JA: "ちっ…やるか…！"
    case 2: EVT_AT_SetVoice("0299A003",1,55); EVT_AT_SetVoice("0299F001",1,70); EVT_AT_SetVoice("0299A010",1,110); break; // @text 0299A003 EN: " " | JA: "んっ…" ; 0299F001 EN: "Who are you!?" | JA: "なんだテメェは！？" ; 0299A010 EN: "Damn!" | JA: "しまった！"
    case 3: EVT_AT_SetVoice("0299A003",1,55); EVT_AT_SetVoice("0299G001",1,70); EVT_AT_SetVoice("0299A011",1,110); break; // @text 0299A003 EN: " " | JA: "んっ…" ; 0299G001 EN: "What are you doing?" | JA: "ここで何してる！？" ; 0299A011 EN: "There's no choice=@" | JA: "やるしかないか…"
    default: EVT_AT_SetVoice("0299A004",1,50); EVT_AT_SetVoice("0299H001",1,70); EVT_AT_SetVoice("0299A012",1,120); break; // @text 0299A004 EN: " " | JA: "うっ…" ; 0299H001 EN: "=@Who are you?" | JA: "おっ…何者だ！？" ; 0299A012 EN: "Damn=@!" | JA: "しまった…！"
    }
    PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
    PositionAngle(enemy, ryo, {-1.608f, 0, -2.524169921875f}, 0);
    Camera(7302);
    Motion(ryo, 8208, 0);
    Motion(enemy, 8202, 0);
    return 160;
}

std::int32_t Success(shen_name enemy, std::int32_t mode) {
    Character *const character = FindCharacterByScenarioName(ryo);
    ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(handCommandOpen), 10);
    ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(handCommandClosed), 10);
    switch (mode) {
    case 0:
        PositionAngle(enemy, ryo, {1.5f, 0, -1.5f}, 90);
        EVT_AT_SetPosAng(enemy, enemy, 1, 1, 0, 50);
        EVT_AT_SetPosAng(ryo, enemy, -0.5f, -1.5f, 180, 50);
        Camera(7310);
        EVT_AT_SetCam(7303, base, base, 0, 0, 50);
        Motion(ryo, 8196, 0);
        EVT_AT_SetMot(ryo, 8195, 2, 1, 0x40000000U, 50);
        EVT_AT_SetMot(enemy, 8197, 0, 1, 0, 36);
        EVT_AT_SetMot(enemy, 8198, 2, 1, 0x40000000U, 50);
        EVT_AT_DispBlur(ryo, 4, 12, 1, 15, 20);
        EVT_AT_DispBlur(ryo, 1, 8, 1, 10, 50);
        return 100;
    case 1:
        PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
        PositionAngle(enemy, ryo, {-1.608f, 0, -2.524169921875f}, 0);
        Camera(7309);
        Motion(ryo, 8204, 60);
        Motion(enemy, 8199, 60);
        EVT_AT_DispBlur(ryo, 7, 12, 1, 12, 75);
        EVT_AT_DispBlur(ryo, 1, 8, 1, 10, 100);
        return 200;
    case 2:
        PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
        PositionAngle(enemy, ryo, {-1.49543f, 0, -2.30519f}, 0);
        Camera(7307);
        Motion(ryo, 8205, 70);
        Motion(enemy, 8200, 70);
        EVT_AT_DispBlur(ryo, 1, 6, 1, 10, 62);
        EVT_AT_DispBlur(ryo, 1, 8, 1, 12, 88);
        return 170;
    case 3:
        PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
        PositionAngle(enemy, ryo, {-1.82542f, 0, -2.21864f}, 0);
        Camera(7308);
        Motion(ryo, 8206, 125);
        Motion(enemy, 8210, 125);
        EVT_AT_DispBlur(ryo, 1, 6, 1, 10, 45);
        EVT_AT_DispBlur(ryo, 7, 12, 1, 10, 60);
        EVT_AT_DispBlur(ryo, 1, 6, 1, 10, 98);
        return 150;
    default:
        PositionAngle(ryo, ryo, {0, 0, -0.3f}, -90);
        PositionAngle(enemy, ryo, {-1.08371f, 0, -1.93784f}, 0);
        Camera(7306);
        Motion(ryo, 8207, 2);
        Motion(enemy, 8201, 2);
        EVT_AT_DispBlur(ryo, 1, 6, 1, 10, 75);
        EVT_AT_DispBlur(ryo, 4, 12, 1, 10, 105);
        EVT_AT_DispBlur(ryo, 1, 8, 1, 5, 147);
        EVT_AT_DispBlur(ryo, 2, 6, 1, 10, 201);
        return 300;
    }
}
void Setup(QKQBEventWork &work) {
    work.authored = &qkqbAuthoredEntries.data()[*work.entry];
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EVT_SetQteFailureCount(0);
    EVT_SetQteRetryFrames(work.authored->retryQteFrames);
    GetCurrentDateTime(savedDateTime);
    for (std::int32_t i = 0; i < work.authored->enemyCount; ++i)
        characterNames.data()[2 + i] = qkqbEnemyNames.data()[*work.entry];
    work.characters = Queue_EVT_AT_ReadChr(work.task, characterNames.data());
    work.motionHandle = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QKQB"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QKQB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QKQB"), 0);
    EV_FreeAuthorPak(1, "0299");
    EV_SetNewSceneIndex();
    LoadCutsceneSoundProgram("0299E000", g_actNumber);
    SetRenderClockAndCompassTo(1);
    Vector3 position{};
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    work.playerY = position.y;
}
void Cleanup(QKQBEventWork &work) {
    const auto &authored = *work.authored;
    Position(ryo, invalid, {authored.returnPosition.x, work.playerY, authored.returnPosition.z});
    Angle(ryo, invalid, authored.returnYaw);
    Motion(ryo, 0xf003, 1);
    hndlSetFlags(FindCharacterByScenarioName(ryo));
    FinishLoading();
    SetRenderClockAndCompassTo(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0299");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QKQB"), 0);
    DeleteMotion(static_cast<std::int32_t>(work.motionHandle));
    ET_AT_KillWork(work.characters);
    FadeFromBlack();
    EV_NPC_NewLoadSw(1);
}

void InitQKQB() {
}
}

const ScriptAreaDefinition qkqbScriptDefinition{SHEN_NAME(QKQB), 0, InitQKQB, Queue_eQKQB_EVT_StartQKuro};

void Queue_eQKQB_EVT_StartQKuro(std::uintptr_t entry) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(eQKQB_EVT_StartQKuro_Callback, 4, 0x0b, sizeof(QKQBEventWork), SHEN_NAME(EVNT));
    auto &work = *static_cast<QKQBEventWork *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.entry = entry ? reinterpret_cast<const std::int32_t *>(entry) : &qkqbEntryPoints.data()[g_entryPoint];
    Setup(work);
}
void eQKQB_EVT_StartQKuro_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    const auto &authored = *work.authored;
    Character *const character = FindCharacterByScenarioName(ryo);
    SetLowQualityHands(character);
    SetHighModelXYZ(character, true, 0, 0);
    SetHighModelXYZ(character, true, 1, 0);
    ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(handCommandOpen), 0);
    ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(handCommandOpen), 0);
    AT_SetLocalBaseChara(SHEN_NAME(DUMR), ryo);
    Position(base, invalid, {authored.basePosition.x, work.playerY, authored.basePosition.z});
    Angle(base, invalid, authored.baseYaw);
    Position(SHEN_NAME(DUMR), ryo, {});
    Angle(SHEN_NAME(DUMR), ryo, 0);
    Position(ren, base, {1000, 0, 0});
    Angle(ren, base, 0);
    EV_LoadAudio("0299E000", audioVariants.data()[*work.entry]);
    EV_spmBeginSampleExecute();
    FadeFromBlack();
    const std::int32_t delay = Intro(characterNames[2], authored.cutMode);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    qteBattleUtil(BattleQteMode::SetFirstCommands, authored.commands[0], authored.commands[1], authored.commands[2], authored.commands[3]);
    qteBattleUtil(BattleQteMode::SetLastCommands);
    EVT_AT_QTE(0, authored.maximumQteFrames, 4, delay);
    HLib::SetCurrentTaskTo(eQKQB_EVT_WaitQTE_Callback);
    HLib::SetupTaskDelay(delay);
}
void eQKQB_EVT_WaitQTE_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    // 27122d tests EAX for zero: FBQT publishes +1/-1 only after its result lifetime.
    if (qteBattleUtil(BattleQteMode::GetResult) == 0)
        return;
    if (qteBattleUtil(BattleQteMode::GetCompletedCount) == work.authored->requiredCommands) {
        const std::int32_t delay = Success(characterNames[2], work.authored->cutMode);
        EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, static_cast<float>(delay)});
        EVT_AT_FadeBGM(0, 1, delay);
        qteBattleUtil(BattleQteMode::RetireAndFree);
        HLib::SetCurrentTaskTo(eQKQB_EVT_WaitPA_Callback);
        work.remainingFrames = delay + 60;
    } else {
        const std::int32_t delay = Failure(characterNames[2], work.authored->cutMode);
        qteBattleUtil(BattleQteMode::RetireAndFree);
        HLib::SetCurrentTaskTo(eQKQB_EVT_StartBattle_Callback);
        HLib::SetupTaskDelay(delay);
    }
}
void eQKQB_EVT_WaitPA_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (authGetFinished() && IsScreenFilmNotShowing(control->screenFilmTask))
        work.remainingFrames = 0;
    if (work.remainingFrames-- <= 0)
        HLib::SetCurrentTaskTo(eQKQB_EVT_EndQKuro_Callback);
}
void eQKQB_EVT_StartBattle_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    const auto &authored = *work.authored;
    shenStartFGST(10, 10, 0, 0);
    PublishFightFormation(FindCharacterByScenarioName(ryo), nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    Position(ryo, ryo, {0, 0.2f, 0});
    for (std::int32_t i = 0; i < authored.enemyCount; ++i) {
        const shen_name enemy = characterNames.data()[2 + i];
        Position(enemy, enemy, {0, 0.2f, 0});
        PublishFightFormation(FindCharacterByScenarioName(enemy), nullptr, 1, 1, static_cast<std::uint16_t>(authored.enemyHp),
            static_cast<std::uint8_t>(authored.fightStyle), 0, 0, 0, 0, 0);
    }
    vfCopyPosFromEntity(0, nullptr, authored.battlePosition.x, authored.battlePosition.z, authored.battleRadius);
    g_pFightStage->battleFlags |= 0x80U;
    vfmQueueFightStart(static_cast<std::uint8_t>(authored.enemyCount + 1), 2, 1, 1, -1);
    EV_camCtrlToggleCombatCamera(2, 1);
    HLib::SetCurrentTaskTo(eQKQB_EVT_PlayBattle_Callback);
}
void eQKQB_EVT_PlayBattle_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    if (g_fightActive)
        return;
    EventFilmControl fade{0, 30, 0, 0, 0, 0, 255, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&fade);
    spmFadeOut();
    if (GetBtlStatus(FindCharacterByScenarioName(ryo)) == 3) {
        EVT_AT_SetPos(ryo, invalid, {work.authored->returnPosition.x, work.playerY, work.authored->returnPosition.z}, 30);
        EVT_AT_SetAngY(ryo, invalid, work.authored->returnYaw, 30);
        EVT_AT_SetMot(ryo, 0xf003, 1, 1, 0x40000000U, 30);
        HLib::SetCurrentTaskTo(eQKQB_EVT_EndQKuro_Callback);
    } else {
        const std::uint32_t failures = EVT_GetQteFailureCount();
        if (failures < 2)
            EVT_SetQteFailureCount(failures + 1);
        SetCurrentDateTimeTo(savedDateTime);
        shenSetTimeSimulationTo(1);
        Queue_ST_LoadTitleFadeIn(SHEN_NAME(RTR2));
        HLib::SetCurrentTaskTo(eQKQB_EVT_RetryQKuro_Callback);
    }
    HLib::SetupTaskDelay(60);
}
void eQKQB_EVT_RetryQKuro_Callback(void *) {
    ST_LoadTitleFade();
    HLib::SetCurrentTaskTo(eQKQB_EVT_StartQKuro_Callback);
    HLib::SetupTaskDelay(30);
}
void eQKQB_EVT_EndQKuro_Callback(void *data) {
    auto &work = *static_cast<QKQBEventWork *>(data);
    Cleanup(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (g_currentMap.name == SHEN_NAME(QKQB)) {
        scenSetTaskFlags(6);
        SetPlayerControlEnabled(0);
        const auto module =
            static_cast<SystemModule>(((g_actNumber >> 31) & static_cast<std::int32_t>(SystemModule::Game)) + static_cast<std::int32_t>(SystemModule::Game));
        shenQueueChangeModuleAndScene(module, g_actNumber, g_currentMap, g_entryPoint);
    }
}
} // namespace shenmue::s2
