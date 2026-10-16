#include "s2/areas/area_uede.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2::areas {
std::int32_t uedeOpponentIndex, uedeOpponentPose, uedeStartingCash, uedeInfoPlayed, uedeWinStreak, uedeDiceSpeaker, uedeVoiceCount;
char uedeVoicePrefixes[4][16], uedeVoices[16][12];
shen_name uedeCharacterList[]{UEDE_RYO, UEDE_INVALID, UEDE_INVALID, UEDE_INVALID};
void InitializeUede() {
    uedeCharacterList[0] = UEDE_RYO;
    uedeCharacterList[1] = UEDE_INVALID;
    uedeCharacterList[2] = UEDE_INVALID;
    uedeCharacterList[3] = UEDE_INVALID;
    uedeOpponentPose = 0;
    uedeOpponentIndex = 0;
    uedeDiceSpeaker = 0;
    uedeStartingCash = 0;
    uedeInfoPlayed = 0;
    uedeWinStreak = 0;
    uedeVoiceCount = 0;
    std::memset(uedeVoicePrefixes, 0, sizeof(uedeVoicePrefixes));
    std::memset(uedeVoices, 0, sizeof(uedeVoices));
}
// @verified - p
void RunUede(std::uintptr_t argument) {
    uedeOpponentIndex = static_cast<std::int32_t>(argument);
    SetPlayerControlEnabled(0);
    HLib::Task *filmTask = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(EventFilmControl), UEDE_TASK);
    RegisterEventTask(filmTask);
    EventFilmControl *film = static_cast<EventFilmControl *>(HLib::GetTaskParameter(filmTask));
    *film = {.cleanup = 1,
        .frames = 1,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 1};
    HLib::Task *task = HLib::EnqueueTaskWithParameter(UedeInitializeCharacters, 4, 0x0b, sizeof(UedeWork), UEDE_TASK);
    UedeWork *work = static_cast<UedeWork *>(HLib::GetTaskParameter(task));
    uedeInfoPlayed = 0;
    uedeWinStreak = 0;
    work->self = task;
    work->state = 0;
    if (g_currentMap.name == UEDE_AREA.name) {
        work->opponent = uedeOpponents[6];
        uedeOpponentIndex = 8;
    } else
        work->opponent = uedeOpponents[uedeOpponentIndex];
    work->previousBase = UEDE_INVALID;
    work->audio = "01020000";
    uedeCharacterList[1] = work->opponent;
    uedeCharacterList[2] = work->previousBase;
    uedeCharacterList[3] = UEDE_INVALID;
    work->characters = Queue_EVT_AT_ReadChr(task, uedeCharacterList);
}
void UedePrepareResources(UedeWork &work) {
    FinishLoading();
    EV_EventSetUpHelpIcon_0(0x47, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("UEDE"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("UEDE"), 0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene(work.audio);
    EV_LoadAudio(work.audio, 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0102");
    if (((g_currentMap.name - SHEN_NAME(AKT0)) & 0xfcffffffU) == 0) {
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(UEDE_RYO), 0);
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 0);
    }
    SetLowQualityHands(ctrlGetCharaByName(UEDE_RYO));
    SetLowQualityHands(ctrlGetCharaByName(work.opponent));
    SetHighModelXYZ(ctrlGetCharaByName(UEDE_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(UEDE_RYO), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(UEDE_RYO), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 8);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(UEDE_RYO), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 8);
    SetHighModelXYZ(ctrlGetCharaByName(work.opponent), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(work.opponent), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(work.opponent), 0, reinterpret_cast<std::byte *>(uedeCupHandPose), 8);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(work.opponent), 1, reinterpret_cast<std::byte *>(uedeCupHandPose), 8);
    work.played = 0;
    uedeStartingCash = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash));
    uedeOpponentPose = GetMotionRigBoneId(scenFindMotionwork(work.opponent)) == 6;
    std::strcpy(uedeVoicePrefixes[0], "0102A");
    switch (work.opponent.name) {
    case SHEN_NAME(AHE_):
        std::strcpy(uedeVoicePrefixes[1], "0102F");
        break;
    case SHEN_NAME(SSA_):
        std::strcpy(uedeVoicePrefixes[1], "0102E");
        break;
    case SHEN_NAME(SNN_):
        std::strcpy(uedeVoicePrefixes[1], "0102B");
        break;
    case SHEN_NAME(SYY_):
        std::strcpy(uedeVoicePrefixes[1], "0102C");
        break;
    case SHEN_NAME(EMI_):
        std::strcpy(uedeVoicePrefixes[1], "0102D");
        break;
    case SHEN_NAME(KCH_):
        std::strcpy(uedeVoicePrefixes[1], "0102G");
        break;
    case SHEN_NAME(KKK_):
        std::strcpy(uedeVoicePrefixes[1], "0102H");
        break;
    case SHEN_NAME(DS2_):
        std::strcpy(uedeVoicePrefixes[1], "0102I");
        break;
    default:
        break;
    }
}
void UedeInitializeCharacters(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    SetLoadingDrawFlags(3);
    UedePrepareResources(work);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, UEDE_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, UEDE_CAMERA);
    const UedeVenue &venue = uedeVenues[uedeOpponentIndex];
    work.maximumBet = venue.maximumBet;
    work.minimumBet = venue.minimumBet;
    EventSetPosition basePosition{.character = UEDE_BASE, .parent = UEDE_INVALID, .position = venue.position};
    EVT_AT_SetPosRot(&basePosition);
    EventSetAngle baseAngle{.character = UEDE_BASE, .parent = UEDE_INVALID, .degrees = venue.angle};
    EVT_AT_SetAngY_Callback(&baseAngle);
    npcSetAreaName(UEDE_REGION, &venue.position, 2, 1);
    EventSetPosition ryoPosition{.character = UEDE_RYO, .parent = UEDE_BASE, .position = {0, 0, 1.3f}};
    EVT_AT_SetPosRot(&ryoPosition);
    EventSetAngle ryoAngle{.character = UEDE_RYO, .parent = UEDE_BASE};
    EVT_AT_SetAngY_Callback(&ryoAngle);
    EventSetPosition opponentPosition{.character = work.opponent, .parent = UEDE_BASE, .position = {0, 0, -.9f}};
    EVT_AT_SetPosRot(&opponentPosition);
    EventSetAngle opponentAngle{.character = work.opponent, .parent = UEDE_BASE, .degrees = 200};
    EVT_AT_SetAngY_Callback(&opponentAngle);
    EventSetPosition potPosition{.character = UEDE_POT, .parent = UEDE_BASE, .position = {-.12f, .755f, .07f}};
    EVT_AT_SetPosRot(&potPosition);
    EventSetAngle potAngle{.character = UEDE_POT, .parent = UEDE_BASE};
    EVT_AT_SetAngY_Callback(&potAngle);
    EventSetPosition cameraPosition{.character = UEDE_CAMERA, .parent = UEDE_BASE, .position = {0, 0, 1.3f}};
    EVT_AT_SetPosRot(&cameraPosition);
    EventSetAngle cameraAngle{.character = UEDE_CAMERA, .parent = UEDE_BASE};
    EVT_AT_SetAngY_Callback(&cameraAngle);
    EventSetMotion motion{.character = work.opponent, .animation = uedeIdleMotions[uedeOpponentPose], .speed = 1, .flags = 0x40000000U};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(static_cast<std::uint16_t>(motion.animation));
    if (DoesScenarioCharacterExist(UEDE_TABLE)) {
        const Vector3 scale{.5f, 1, .5f};
        SetMotionworkFlagForCharaById(UEDE_TABLE, &scale);
        EventSetPosition tablePosition{.character = UEDE_TABLE, .parent = UEDE_BASE, .position = {-.1f, 0, 0}};
        EVT_AT_SetPosRot(&tablePosition);
        EventSetAngle tableAngle{.character = UEDE_TABLE, .parent = UEDE_BASE};
        EVT_AT_SetAngY_Callback(&tableAngle);
        AT_SetLocalBaseChara(UEDE_TABLE, UEDE_BASE);
    }
    AT_SetLocalBaseChara(UEDE_DICE[0], UEDE_BASE);
    AT_SetLocalBaseChara(UEDE_DICE[1], UEDE_BASE);
    if (g_currentMap.name != UEDE_AREA.name)
        EV_DisableCharacterDisplay(UEDE_POT);
    uedeOpponentPose = GetMotionRigBoneId(scenFindMotionwork(work.opponent)) == 6;
    EV_NPC_NewLoadSw(1);
    HLib::SetCurrentTaskTo(UedeWaitLoading);
    QueueSkNpcWait(work.self);
}
void UedeWaitLoading(void *) {
    HLib::SetCurrentTaskTo(((g_currentMap.name - SHEN_NAME(AKT0)) & 0xfdffffffU) != 0 ? UedeTalk : UedeIntroductionCamera);
    ResetLoadingDrawState();
}
void UedeIntroductionCamera(void *) {
    EV_DisableCharacterDisplay(UEDE_RYO);
    HLib::Task *displayTask = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), UEDE_TASK);
    RegisterEventTask(displayTask);
    StepObjectDisplayWork *display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(displayTask));
    display->cleanup = 1;
    display->character = UEDE_RYO;
    display->delay = 430;
    EventSetCamera camera{.camera = 2000, .eye = UEDE_BASE, .target = UEDE_BASE};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    HLib::Task *cameraTask = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 0x0b, sizeof(EventSetCamera), UEDE_TASK);
    RegisterEventTask(cameraTask);
    EventSetCamera *queued = static_cast<EventSetCamera *>(HLib::GetTaskParameter(cameraTask));
    queued->cleanup = 1;
    queued->camera = 2001;
    queued->eye = UEDE_BASE;
    queued->target = UEDE_BASE;
    queued->context = 0;
    queued->angle = 0;
    queued->delay = 250;
    EventFilmControl film{.frames = 30, .startAlpha = 255};
    EVT_AT_MenCtrl_Callback(&film);
    HLib::SetCurrentTaskTo(UedeTalk);
    HLib::SetupTaskDelay(430);
}
void UedeStop(void *) {
    if (g_currentMap.name == UEDE_AREA.name)
        HLib::SetCurrentTaskTo(UedeTalk);
    else {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        HLib::SetCurrentTaskTo(UedeEnd);
        HLib::SetupTaskDelay(30);
    }
}
// @verified - p
void UedeEnd(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    if (((g_currentMap.name - SHEN_NAME(AKT0)) & 0xfcffffffU) == 0) {
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(UEDE_RYO), 1);
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 1);
    }
    npcSetAreaName(UEDE_REGION, nullptr, 0, 0);
    EventSetPoint ryoPoint{.character = UEDE_RYO, .trackingMode = 3, .flags = 0x10000000U};
    EVT_AT_SetPoint_Callback(&ryoPoint);
    EventSetPoint opponentPoint{.character = work.opponent, .trackingMode = 3, .flags = 0x10000000U};
    EVT_AT_SetPoint_Callback(&opponentPoint);
    EV_FreeAuthorPak(0, "0102");
    SetCharaKill(UEDE_BASE, 1);
    SetCharaKill(UEDE_CAMERA, 1);
    AT_SetLocalBaseChara(UEDE_DICE[0], UEDE_INVALID);
    AT_SetLocalBaseChara(UEDE_DICE[1], UEDE_INVALID);
    EV_EventSetUpHelpIcon_0(0, 0);
    hndlSetFlags(ctrlGetCharaByName(UEDE_RYO));
    hndlSetFlags(ctrlGetCharaByName(work.opponent));
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    ET_AT_KillWork(work.characters);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("UEDE"), 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    EV_NPC_NewLoadSw(1);
}
const ScriptAreaDefinition uedeScriptDefinition{.areaId = UEDE_AREA, .field04 = 0, .init = InitializeUede, .main = RunUede};
} // namespace shenmue::s2::areas
