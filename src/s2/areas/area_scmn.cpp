#include "s2/areas/area_scmn.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/pati.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/street_fight_conversation.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {
Vector3 scmnSavedPlayerPosition{};
Vector3i scmnSavedPlayerRotation{};
std::uint32_t scmnSavedFloor{};
std::int32_t scmnEntryMode{};
std::int32_t scmnEyeFrame{};
std::int32_t scmnSequenceFinished{};
std::uint32_t scmnChoicePhase{};
std::int32_t scmnDefaultYaw{}, scmnQjbtYaw{}, scmnQsfaYaw{}, scmnPlayerLocalYaw{};
Vector3 scmnPlayerLocalPosition{};
// @verified - p
void ScmnInit() {
    scmnSavedFloor = 0;
    scmnEntryMode = 0;
    scmnEyeFrame = 0;
}
// @verified - p
void ScmnMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    scmnEntryMode = static_cast<std::int32_t>(argument);
    (void)HLib::EnqueueTaskWithoutParameter(scmnEntryMode ? ScmnWaitVoiceCallback : QueueScmnSceneCallback, 5, 0x0b, static_cast<std::int32_t>(SCMN_TASK.name));
}
// @verified - p
void ScmnWaitVoiceCallback(void *) {
    if (!cvsHasValidConverseData(nullptr)) {
        HLib::SetCurrentTaskTo(ScmnFadeOutCallback);
        HLib::SetupTaskDelay(40);
    }
}
// @verified - p
void ScmnFadeOutCallback(void *) {
    HLib::SetCurrentTaskTo(QueueScmnSceneCallback);
    HLib::SetupTaskDelay(120);
}
// @verified - p
void QueueScmnSceneCallback(void *) {
    SetPlayerControlEnabled(0);
    (void)EV_NPC_NewLoadSw(0);
    (void)EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ScmnMainCallback, 5, 0x0b, sizeof(ScmnEventWork), SCMN_TASK);
    ScmnEventWork *const work = static_cast<ScmnEventWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->state = 0;
    work->motions[0] = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(50);
    work->characters = Queue_EVT_AT_ReadChr(task, SCMN_CHARACTERS);
    work->changeScene = 0;
    LoadScmnScene();
    (void)HLib::DestroyCurrentTask();
}
// @verified - p
void LoadScmnScene() {
    (void)EV_FreeAuthorPak(1, "0255");
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SCMN"), 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("SCMN"), 0);
    (void)CreateEventCharacter(CharacterCreationHighModel, nullptr, SCMN_BASE);
    (void)EV_SetNewSceneIndex();
    (void)EV_LoadCutsceneFromCurrentScene("0255E000");
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
}
// @verified - p
void FinishScmnSequence() {
    scmnSequenceFinished = 1;
}
// @verified - p
void ScmnMainCallback(void *parameter) {
    ScmnEventWork *const work = static_cast<ScmnEventWork *>(parameter);
    switch (work->state) {
    case 0:
        (void)EV_NPC_NewLoadSw(1);
        work->state = 1;
        break;
    case 1:
        if (!filmCheckFadeTrackColour())
            work->state = 3;
        break;
    case 2:
        scmnSequenceFinished = 0;
        if (GetCompletedGameCount()) {
            scmnConversationSkipEnabled = 1;
            scmnConversationSkipCallback = FinishScmnSequence;
        }
        QueueScmnStreetFightConversation(PlayScmnInvitationVoice, work);
        work->state = 3;
        break;
    case 3:
        scmnSequenceFinished = 0;
        PrepareScmnCharacters();
        if (GetCompletedGameCount()) {
            scmnConversationSkipEnabled = 1;
            scmnConversationSkipCallback = FinishScmnSequence;
        }
        QueueScmnStreetFightConversation(PlayScmnInvitationScene, work);
        work->state = 4;
        break;
    case 4:
        if (scmnSequenceFinished) {
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetupTaskDelay(30);
        }
        work->state = 5;
        break;
    case 5:
        if (scmnSequenceFinished)
            EV_DisableCharacterDisplay(SCMN_CHUNYAN);
        (void)EV_PlayDialogue("0255A006");
        (void)EVT_AT_KsIcon_Enqueue(-1, 251, 253, -1, 10800, 0xffff, 0, 0);
        scmnChoicePhase = 0;
        work->state = 6;
        break;
    case 6: {
        const std::uint16_t choice = static_cast<std::uint16_t>(PollScmnChoice(1));
        if (!choice)
            break;
        if (choice != 251) {
            work->state = 8;
            break;
        }
        spmFadeOut();
        DateTime date;
        GetCurrentDateTime(date);
        date.minute = static_cast<std::uint8_t>(date.minute + 30U);
        SetCurrentDateTimeTo(date);
        work->changeScene = 1;
        work->state = 8;
        break;
    }
    case 7:
        if (!filmCheckFadeTrackColour())
            work->state = 8;
        break;
    case 8:
        HLib::SetCurrentTaskTo(ScmnCleanupCallback);
        break;
    default:
        break;
    }
}
// @verified - p
void PrepareScmnCharacters() {
    EV_EnableCharacterDisplay(SCMN_CHUNYAN);
    EV_EnsureCharacterDraw(SCMN_CHUNYAN);
    EV_GetCharaPosition(SCMN_PLAYER, CHARACTER_TRANSLATION_ROOT, &scmnSavedPlayerPosition, EventTransformFlags::World);
    (void)GetScenarioCharacterEulerAngles(SCMN_PLAYER, &scmnSavedPlayerRotation, EventTransformFlags::World);
    if (g_currentMap.name == SCMN_QJBT.name) {
        scmnSavedFloor = static_cast<std::uint32_t>(GetQronNextStream());
        (void)RepositionPlayerForQronFloor(0);
    }
    const Vector3 *position = &SCMN_QTB1_POSITION;
    const std::int32_t *yaw = &scmnDefaultYaw;
    switch (g_currentMap.name) {
    case SCMN_QSFA.name:
        position = &SCMN_QSFA_POSITION;
        yaw = &scmnQsfaYaw;
        break;
    case SCMN_QSFB.name:
        position = &SCMN_QSFB_POSITION;
        yaw = &patiGameplayPlacements[1].yaw;
        break;
    case SCMN_QSFC.name:
        position = &SCMN_QSFC_POSITION;
        yaw = &patiGameplayPlacements[5].yaw;
        break;
    case SCMN_QSFD.name:
        position = &SCMN_QSFD_POSITION;
        yaw = &patiGameplayPlacements[2].yaw;
        break;
    case SCMN_QGBT.name:
        position = &SCMN_QGBT_POSITION;
        yaw = &patiGameplayPlacements[3].yaw;
        break;
    case SCMN_QJBT.name:
        position = &SCMN_QJBT_POSITION;
        yaw = &scmnQjbtYaw;
        break;
    case SCMN_QLBT.name:
        position = &SCMN_QLBT_POSITION;
        yaw = &patiGameplayPlacements[4].yaw;
        break;
    default:
        break;
    }
    EventSetPosition base{0, SCMN_BASE, SCMN_INVALID, *position, 0};
    EVT_AT_SetPosRot(&base);
    EventSetAngle angle{0, SCMN_BASE, SCMN_INVALID, static_cast<float>(*yaw), 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(SCMN_PLAYER, SCMN_BASE);
    AT_SetLocalBaseChara(SCMN_CHUNYAN, SCMN_BASE);
    SetScenarioCharacterType(SCMN_PLAYER, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SCMN_CHUNYAN, CharacterTypeStencilShadow);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(SCMN_CHUNYAN), FacialNodeReplacement);
}
// @verified - p
std::int32_t PlayScmnInvitationVoice(void *) {
    (void)EV_PlayDialogue("0255A005");
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SCMN"), 0);
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 90.0F});
    return 210;
}
// @verified - p
std::int32_t PlayScmnInvitationScene(void *) {
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 2.0F});
    EventSetPosition ryoPosition{0, SCMN_PLAYER, SCMN_BASE, scmnPlayerLocalPosition, 0};
    EVT_AT_SetPosRot(&ryoPosition);
    EventSetAngle ryoAngle{0, SCMN_PLAYER, SCMN_BASE, static_cast<float>(scmnPlayerLocalYaw), 0};
    EVT_AT_SetAngY_Callback(&ryoAngle);
    EventSetPosition chunyanPosition{0, SCMN_CHUNYAN, SCMN_BASE, SCMN_CHUNYAN_LOCAL_POSITION, 0};
    EVT_AT_SetPosRot(&chunyanPosition);
    EventSetAngle chunyanAngle{0, SCMN_CHUNYAN, SCMN_BASE, 270.0F, 0};
    EVT_AT_SetAngY_Callback(&chunyanAngle);
    EventSetMotion ryoTurn{0, SCMN_PLAYER, 9672, 0, 1.0F, 0x60000000U, 0};
    EVT_AT_SetMot_Callback(&ryoTurn);
    (void)GetMotionFrameCount(9672);
    EVT_AT_SetMot(SCMN_PLAYER, 9673, 0, 1.0F, 0x20000000U, 150);
    EventSetMotion chunyanWalk{0, SCMN_CHUNYAN, 9671, 0, 1.0F, 0x40000000U, 0};
    EVT_AT_SetMot_Callback(&chunyanWalk);
    (void)GetMotionFrameCount(9671);
    EVT_AT_SetMot(SCMN_CHUNYAN, 9669, 0, 1.0F, 0x40000000U, 100);
    EVT_AT_SetMot(SCMN_CHUNYAN, 9670, 0, 1.0F, 0x40000000U, 200);
    EV_DisableCharacterDisplay(SCMN_CHUNYAN);
    HLib::Task *const displayTask = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), SCMN_TASK);
    (void)RegisterEventTask(displayTask);
    *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(displayTask)) = {1, SCMN_CHUNYAN, 0, 50.0F};
    HLib::Task *const hideTask = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), SCMN_TASK);
    (void)RegisterEventTask(hideTask);
    *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(hideTask)) = {1, SCMN_CHUNYAN, 1, 290.0F};
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SCMN"), 0);
    EVT_AT_SetCam(2051, SCMN_BASE, SCMN_BASE, 0, 0, 1);
    EVT_AT_SetCam(2054, SCMN_BASE, SCMN_BASE, 0, 0, 50);
    EVT_AT_SetCam(2052, SCMN_BASE, SCMN_BASE, 0, 0, 100);
    if (g_currentMap.name == SCMN_QGBT.name || g_currentMap.name == SCMN_QTB1.name || g_currentMap.name == SCMN_QSFA.name ||
        g_currentMap.name == SCMN_QSFB.name || g_currentMap.name == SCMN_QJBT.name || g_currentMap.name == SCMN_QLBT.name)
        EVT_AT_SetCam(2053, SCMN_BASE, SCMN_BASE, 0, 0, 290);
    EVT_AT_SetVoice("0255B001", 1, 120); // @text EN: "Come to Dragon St at sundown." | JA: "陽が落ちてから龍影街に来な…"
    EVT_AT_SetVoice("0255A004", 1, 250); // @text EN: "Dragon St=@" | JA: "龍影街…"
    scmnEyeFrame = 0;
    (void)HLib::EnqueueTaskWithoutParameter(ScmnEyeControlCallback, 5, 0x0b, static_cast<std::int32_t>(SCMN_TASK.name));
    EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 350.0F});
    return 380;
}
// @verified - p
std::uint32_t PollScmnChoice(std::int32_t mode) {
    if (!mode) {
        scmnChoicePhase = 0;
        return 0;
    }
    switch (scmnChoicePhase) {
    case 0:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return 0;
        ++scmnChoicePhase;
        return 0;
    case 1:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return 0;
        ++scmnChoicePhase;
        return 0;
    case 2: {
        const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
        scmnChoicePhase = 0;
        return choice;
    }
    default:
        return 0;
    }
}
// @verified - p
void ScmnEyeControlCallback(void *) {
    if (scmnEyeFrame == 95) {
        Vector3 position;
        EV_GetCharaPosition(SCMN_CHUNYAN, 7, &position, EventTransformFlags::World);
        SetEyeToChara(FindCharacterByScenarioName(SCMN_PLAYER), &position, 15, 2);
    } else if (scmnEyeFrame == 379) {
        SetEyeToChara(FindCharacterByScenarioName(SCMN_PLAYER), nullptr, 16, 0);
        (void)HLib::DestroyCurrentTask();
    }
    ++scmnEyeFrame;
}
void ScmnCleanupCallback(void *parameter) {
    using enum CameraState;
    ScmnEventWork *const work = static_cast<ScmnEventWork *>(parameter);
    ET_AT_KillWork(work->characters);
    SetPlayerControlEnabled(0);
    SetScenarioCharacterKillMode(SCMN_BASE, 1);
    SetEyeToChara(FindCharacterByScenarioName(SCMN_PLAYER), nullptr, 16, 0);
    if (!scmnEntryMode) {
        EV_AT_SetPos(SCMN_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, scmnSavedPlayerPosition);
        (void)EV_AT_SetAngY(SCMN_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, scmnSavedPlayerRotation, nullptr);
        if (g_currentMap.name == SCMN_QJBT.name)
            (void)RepositionPlayerForQronFloor(static_cast<std::int32_t>(scmnSavedFloor));
    }
    (void)mtwkRequestMotion(scenFindMotionwork(SCMN_PLAYER), 0xf004, MotionRegion::LowerBody);
    (void)SetMotionRegionFlag(scenFindMotionwork(SCMN_PLAYER), 71, 0, MotionRegion::LowerBody);
    (void)GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::uint32_t *motion = work->motions; *motion; ++motion)
        (void)DeleteMotion(*motion);
    (void)EV_FreeAuthorPak(0, "0255");
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("SCMN"), 0);
    shenSetClockAndCompassRenderTo(0);
    EV_CleanupMapControlTask();
    HLKillOtherHierarchyTask(work->self);
    (void)EV_ResetFreeTalkFlags();
    (void)EV_NPC_NewLoadSw(1);
    if (work->changeScene)
        EV_scenQueueLoad(g_actNumber, SCMN_QR00, 81);
    else {
        (void)LoadCutsceneAndSoundProgramForArea();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetPlayerControlEnabled(1);
        (void)camCtrlSetState(Init);
        (void)camCtrlSetFovTransitionTo(true);
    }
}
const ScriptAreaDefinition scmnScriptDefinition{.areaId = AREA_SCMN, .field04 = 0, .init = ScmnInit, .main = ScmnMain};
} // namespace shenmue::s2
