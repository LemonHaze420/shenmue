#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_wtkq.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void WtkqMain(void *argument)
{
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(0);
    QueueClearNpcs(QueueWtkq, 0);
}
#else
static shen_name wtkqCharacters[] = {WTKQ_RYO, WTKQ_KUN, WTKQ_III, WTKQ_SEE, WTKQ_SYG, WTKQ_TKM, EVENT_CHARACTER_INVALID};

void QueueWtkq(void *argument)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(WtkqMainCallback, 4, 11, sizeof(WtkqWork), TASK_EVNT);
    WtkqWork *work = (WtkqWork *)FTBL.HLib_GetTaskParameter(task);
    *(int *)&work->phase = 0;
    work->characters = 0;
    work->qte[0] = 0;
    work->qte[1] = 0;
    work->qte[2] = 0;
    work->qte[3] = 0;
    work->task = task;
    work->phase = 0;
    work->step = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, wtkqCharacters);
    EventDebugRegister(WTKQ_DEBUG_ID, WtkqEndCallback, work, 100000);
}

int WtkqInitialize(WtkqWork *work)
{
    EventDateTime date;
    int slot;
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WTKQ", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "WTKQ", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "WTKQ", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0146");
    FTBL.EV_LoadCutsceneFromCurrentScene("0146A000");
    FTBL.qteBattleUtil(8, 0, 6, 16, 0);
    FTBL.qteBattleUtil(9, 16, 1024, 0, 0);
    FTBL.qteBattleUtil(10, 0, 0, 0, 0);
    FTBL.qteBattleUtil(1, 0, 0, 0, 0);
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
    FTBL.SetDoorSubjectEnabled(WTKQ_DOOR, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(WTKQ_RYO));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(WTKQ_KUN));
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(WTKQ_RYO));
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(WTKQ_KUN));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WTKQ_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WTKQ_RYO), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WTKQ_KUN), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WTKQ_KUN), 1, 1, 0);
    EVT_AT_CharaDisp(WTKQ_TABLE0, 0, 1);
    EVT_AT_CharaDisp(WTKQ_TABLE1, 0, 1);
    EVT_AT_CharaDisp(WTKQ_CHAIR0, 0, 1);
    EVT_AT_CharaDisp(WTKQ_CHAIR1, 0, 1);
    EVT_AT_CharaDisp(WTKQ_CHAIR2, 0, 1);
    EVT_AT_CharaDisp(WTKQ_CHAIR3, 0, 1);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.GetCurrentDateTime((DateTime *)&date);
    if (date.hour >= 19 && date.hour <= 24) {
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
    }
    FTBL.EV_lightLoad(WTKQ_LIGHT);
    slot = 0;
    while (FTBL.HasMapModelReader(slot))
        slot++;
    FTBL.AssignLightProgram(slot - 1);
    FTBL.CreateEventCharacter(3, 0, WTKQ_BASE);
    FTBL.CreateEventCharacter(3, 0, WTKQ_DUMMY);
    EVT_AT_SetPos(WTKQ_BASE, EVENT_CHARACTER_INVALID, 266.7f, 115.3f, 329.2f, 0);
    EVT_AT_SetAngY(WTKQ_BASE, EVENT_CHARACTER_INVALID, 0.0f, 0);
    work->phase = 4;
    EventDebugCheck(WTKQ_DEBUG_ID, 4);
    EVT_UTL_SaveDateTime();
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    return 0;
}

int WtkqFinish(WtkqWork *work)
{
    FTBL.spmFadeOut(0);
    FTBL.HLib_SetCurrentTaskTo(WtkqEndCallback);
    return 30;
}

void WtkqEndCallback(void *data)
{
    WtkqWork *work = (WtkqWork *)data;
    FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(WTKQ_RYO));
    FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(WTKQ_KUN));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(WTKQ_RYO));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(WTKQ_KUN));
    FTBL.qteBattleUtil(5, 0, 0, 0, 0);
    FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
    FTBL.SetDoorSubjectEnabled(WTKQ_DOOR, 1);
    EVT_AT_CharaDisp(WTKQ_TABLE0, 0, 0);
    EVT_AT_CharaDisp(WTKQ_TABLE1, 0, 0);
    EVT_AT_CharaDisp(WTKQ_CHAIR0, 0, 0);
    EVT_AT_CharaDisp(WTKQ_CHAIR1, 0, 0);
    EVT_AT_CharaDisp(WTKQ_CHAIR2, 0, 0);
    EVT_AT_CharaDisp(WTKQ_CHAIR3, 0, 0);
    FTBL.SetScenarioCharacterKillMode(WTKQ_BASE, 1);
    FTBL.SetScenarioCharacterKillMode(WTKQ_DUMMY, 1);
    ET_AT_KillWork(work->characters);
    EventDebugUnregister(WTKQ_DEBUG_ID);
    FTBL.EV_FreeAuthorPak(0, "0146");
    FTBL.EV_ControlPackageResources(0, 6, "WTKQ", 0, 0);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.scenFreeEvent("WTKQ.EV3");
    FTBL.EnterDoorSubscene(0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_stepSetFlags(512, 0);
}

void WtkqMainCallback(void *data)
{
    WtkqWork *work = (WtkqWork *)data;
    int delay = 0;
    switch (work->phase) {
    case 0:
        delay = WtkqInitialize(work);
        break;
    case 4:
        delay = WtkqStartAuthor(work);
        break;
    case 2:
        delay = WtkqBattle(work);
        break;
    case 3:
        delay = WtkqRetry(work);
        break;
    case 1:
        delay = WtkqWin(work);
        break;
    case 5:
        delay = WtkqEndAuthor(work);
        break;
    case 6:
        delay = WtkqFinish(work);
        break;
    }
    if (delay > 0)
        FTBL.HLib_SetupTaskDelay(delay);
}

int WtkqBattle(WtkqWork *work)
{
    int delay = 0;
    int result;
    if (work->step == 0) {
        FTBL.EV_LoadAudio("0146A000", 1);
        EVT_AT_SetPos(WTKQ_RYO, WTKQ_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(WTKQ_RYO, WTKQ_BASE, 180.0f, 0);
        EVT_AT_SetPos(WTKQ_III, WTKQ_RYO, 0.4f, 0.0f, -1.7f, 0);
        EVT_AT_SetAngY(WTKQ_III, WTKQ_RYO, 0.0f, 0);
        EVT_AT_SetMot(WTKQ_RYO, 9227, 0.0f, 0.8f, 0x40000000U, 0);
        EVT_AT_SetMot(WTKQ_III, 9228, 0.0f, 0.8f, 0x40000000U, 0);
        EVT_AT_SetCam(4000, WTKQ_BASE, WTKQ_BASE, 0, delay, delay);
        EVT_AT_QTE(0, 16, 0, 20, 4);
        work->step++;
        delay = 20;
    } else {
        result = FTBL.qteBattleUtil(4, 0, 0, 0, 0);
        if (result) {
            if (result == 1)
                work->phase = 1;
            else
                work->phase = 3;
            work->step = 0;
        }
    }
    return delay;
}

int WtkqRetry(WtkqWork *work)
{
    EVT_AT_SetPos(WTKQ_III, WTKQ_RYO, 0.4f, 0.0f, -1.7f, 0);
    EVT_AT_SetAngY(WTKQ_III, WTKQ_RYO, 0.0f, 0);
    EVT_AT_SetMot(WTKQ_RYO, 9227, 15.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(WTKQ_III, 9228, 15.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetCam(4500, WTKQ_BASE, WTKQ_BASE, 0, 0, 0);
    EVT_AT_SetVoice("0146A012", 5);
    EVT_AT_SetVoice("0146C011", 30);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 60);
    work->phase = 4;
    return 150;
}

int WtkqWin(WtkqWork *work)
{
    EVT_AT_CharaDisp(WTKQ_OTAM, 0, 1);
    EVT_AT_CharaDisp(WTKQ_FLYP, 0, 1);
    EVT_AT_CharaDisp(WTKQ_SYG, 0, 1);
    EVT_AT_CharaDisp(WTKQ_TKM, 0, 1);
    Queue_EVT_AT_Cling(WTKQ_OTAM, 0, 0, 0, 0, 0, 1);
    Queue_EVT_AT_Cling(WTKQ_FLYP, 0, 0, 0, 0, 0, 1);
    EVT_AT_SetPos(WTKQ_RYO, WTKQ_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(WTKQ_RYO, WTKQ_BASE, 180.0f, 0);
    EVT_AT_SetPos(WTKQ_III, WTKQ_RYO, 0.1f, 0.0f, -2.6f, 0);
    EVT_AT_SetAngY(WTKQ_III, WTKQ_RYO, 0.0f, 0);
    EVT_AT_SetPos(WTKQ_SEE, WTKQ_RYO, 0.5f, 0.0f, -1.3f, 40);
    EVT_AT_SetAngY(WTKQ_SEE, WTKQ_RYO, 0.0f, 40);
    EVT_AT_SetMot(WTKQ_RYO, 9223, 40.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(WTKQ_RYO, 9224, 140.0f, 1.0f, 0x40000000U, 40);
    EVT_AT_SetMot(WTKQ_III, 9219, 40.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(WTKQ_SEE, 9222, 100.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetCam(1000, WTKQ_BASE, WTKQ_BASE, 0, 0, 0);
    EVT_AT_SetCam(1001, WTKQ_BASE, WTKQ_BASE, 0, 0, 40);
    EVT_AT_SetCam(1002, WTKQ_BASE, WTKQ_BASE, 0, 0, 70);
    EVT_AT_DispBlur(WTKQ_RYO, 1, 10, 3, 0, 50, 10);
    EVT_AT_SetFaceLooks(WTKQ_RYO, 1, 16, 1.0f, 0);
    Queue_EVT_AT_CtrlHandUp_1(WTKQ_RYO, 0, (void *)areaLoopHandPoses[WTKQ_RYO_QTE_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(WTKQ_RYO, 1, (void *)areaLoopHandPoses[WTKQ_RYO_QTE_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(WTKQ_KUN, 0, (void *)areaLoopHandPoses[WTKQ_KUN_GRIP_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(WTKQ_KUN, 1, (void *)areaLoopHandPoses[WTKQ_KUN_GRIP_HAND_POSE], 0, 0);
    EVT_AT_SetVoice("0146A013", 10);
    EVT_AT_SetVoice("0146D015", 50);
    QueueSkQteWait(work->qte, 256, 24, 20, WtkqQteSuccess, WtkqQteFailure, 70);
    work->phase = 5;
    return 0;
}

int WtkqQteSuccess(void)
{
    Vector3 position;
    Vector3i rotation;
    position.x = 0.058f;
    position.y = -0.037f;
    position.z = -0.023f;
    rotation.x = 26068;
    rotation.y = 0xb778;
    rotation.z = 0xc78a;
    Queue_EVT_AT_Cling(WTKQ_FLYP, WTKQ_KUN, 14, &position, &rotation, 0, 0);
    EVT_AT_CharaDisp(WTKQ_FLYP, 0, 0);
    EVT_AT_SetAngY(WTKQ_RYO, WTKQ_RYO, -75.5f, 0);
    EVT_AT_SetPos(WTKQ_III, WTKQ_RYO, 1.7f, 0.0f, 0.8f, 0);
    EVT_AT_SetAngY(WTKQ_III, WTKQ_RYO, 0.0f, 0);
    EVT_AT_SetPos(WTKQ_KUN, WTKQ_RYO, 2.6f, 0.0f, 1.3f, 0);
    EVT_AT_SetAngY(WTKQ_KUN, WTKQ_RYO, 0.0f, 0);
    EVT_AT_SetMot(WTKQ_RYO, 9225, 60.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(WTKQ_III, 9220, 60.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(WTKQ_KUN, 9217, 60.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetVoice("0146F021", 145);
    EVT_AT_SetCam(2000, WTKQ_BASE, WTKQ_BASE, 0, 0, 0);
    EVT_AT_SetCam(2001, WTKQ_BASE, WTKQ_BASE, 0, 0, 60);
    EVT_AT_SetCam(2002, WTKQ_BASE, WTKQ_BASE, 0, 0, 110);
    EVT_AT_DispBlur(WTKQ_RYO, 4, 20, 1, 0, 30, 0);
    EVT_AT_SetFaceLooks(WTKQ_KUN, 0, 15, 1.0f, 110);
    EVT_AT_SetFaceLooks(WTKQ_KUN, 19, 30, 1.0f, 145);
    EVT_AT_SetVoice("0146C015", 15);
    EVT_AT_SetVoice("0146C013", 30);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 200);
    return 230;
}

int WtkqQteFailure(void)
{
    Vector3 position;
    Vector3i rotation;
    position.x = 0.05f;
    position.y = -0.04f;
    position.z = -0.04f;
    rotation.x = 20534;
    rotation.y = 0xe432;
    rotation.z = 0xe38e;
    Queue_EVT_AT_Cling(WTKQ_OTAM, WTKQ_KUN, 14, &position, &rotation, 0, 0);
    EVT_AT_CharaDisp(WTKQ_OTAM, 0, 0);
    EVT_AT_SetAngY(WTKQ_RYO, WTKQ_RYO, 112.5f, 0);
    EVT_AT_SetPos(WTKQ_III, WTKQ_RYO, -1.6f, 0.0f, -0.7f, 0);
    EVT_AT_SetAngY(WTKQ_III, WTKQ_RYO, 0.0f, 0);
    EVT_AT_SetPos(WTKQ_KUN, WTKQ_RYO, -3.8f, 0.0f, 0.4f, 0);
    EVT_AT_SetAngY(WTKQ_KUN, WTKQ_RYO, 0.0f, 0);
    EVT_AT_SetPos(WTKQ_DUMMY, WTKQ_BASE, 0.0f, 1.9f, -0.6f, 0);
    EVT_AT_SetAngY(WTKQ_DUMMY, WTKQ_BASE, 0.0f, 0);
    EVT_AT_SetMot(WTKQ_RYO, 9226, 60.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(WTKQ_III, 9221, 60.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(WTKQ_KUN, 9218, 60.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetVoice("0146C012", 10);
    EVT_AT_SetVoice("0146F019", 115);
    EVT_AT_SetVoice("0146C015", 170);
    EVT_AT_SetVoice("0146F020", 240);
    Queue_EVT_AT_CtrlHandUp_1(WTKQ_KUN, 0, (void *)areaLoopHandPoses[WTKQ_KUN_FAILURE_HAND_POSE], 8, 0);
    EVT_AT_SetCam(3000, WTKQ_BASE, WTKQ_BASE, 0, 0, 0);
    EVT_AT_SetCam(3001, WTKQ_BASE, WTKQ_BASE, 0, 0, 75);
    EVT_AT_SetCam(3002, WTKQ_BASE, WTKQ_BASE, 0, 0, 150);
    EVT_AT_SetCam(3003, WTKQ_BASE, WTKQ_BASE, 0, 0, 240);
    EVT_AT_DispBlur(WTKQ_RYO, 1, 20, 2, 0, 20, 160);
    EVT_AT_SetFaceLooks(WTKQ_RYO, 6, 16, 1.0f, 30);
    EVT_AT_SetFaceLooks(WTKQ_RYO, 1, 16, 1.0f, 160);
    EVT_AT_SetFaceLooks(WTKQ_KUN, 1, 16, 1.0f, 60);
    EVT_AT_SetFaceLooks(WTKQ_KUN, 19, 16, 1.0f, 240);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 315);
    return 345;
}

int WtkqStartAuthor(WtkqWork *work)
{
    FTBL.EV_LoadCutsceneFromCurrentScene("0146A000");
    FTBL.EV_LoadAudio("0146A000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    EVT_AT_CharaDisp(WTKQ_SYG, 0, 0);
    EVT_AT_CharaDisp(WTKQ_TKM, 0, 0);
    EVT_UTL_RestoreDateTime();
    QueueSkAuthorStart(work->task, WTKQ_AUTHOR_START, 0, 0, 0);
    work->phase = 2;
    return 0;
}

int WtkqEndAuthor(WtkqWork *work)
{
    EventDateTime date;
    int slot;
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.GetCurrentDateTime((DateTime *)&date);
    if (date.hour >= 19 && date.hour <= 24) {
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
    }
    FTBL.EV_lightLoad(WTKQ_END_LIGHT);
    slot = 0;
    while (FTBL.HasMapModelReader(slot))
        slot++;
    FTBL.AssignLightProgram(slot - 1);
    if (date.hour < 19 || date.hour > 24)
        FTBL.ReinitializeLights(1);
    else
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.EV_LoadCutsceneFromCurrentScene("0146A020");
    FTBL.EV_LoadAudio("0146A000", 2);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_CharaDisp(WTKQ_OTAM, 0, 1);
    EVT_AT_CharaDisp(WTKQ_FLYP, 0, 1);
    Queue_EVT_AT_Cling(WTKQ_OTAM, 0, 0, 0, 0, 0, 1);
    Queue_EVT_AT_Cling(WTKQ_FLYP, 0, 0, 0, 0, 0, 1);
    QueueSkAuthorStart(work->task, WTKQ_AUTHOR_END, 0, 0, 0);
    work->phase = 6;
    return 0;
}
#endif
#else
#include "s2/areas/area_wtkq.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, kun{SHEN_NAME(KUN_)}, iii{SHEN_NAME(III_)}, see{SHEN_NAME(SEE_)};
constexpr shen_name base{SHEN_NAME(BASE)}, dummy{SHEN_NAME(DUM_)}, otam{SHEN_NAME(OTAM)}, flyp{SHEN_NAME(FLYP)};
constexpr shen_name syg{SHEN_NAME(SYG_)}, tkm{SHEN_NAME(TKM_)}, event{SHEN_NAME(EVNT)};
constexpr shen_name names[]{ryo, kun, iii, shen_name{1598375251U}, syg, tkm, shen_name{0xffffffffU}};
constexpr shen_name furniture[]{SHEN_NAME(tbl0), SHEN_NAME(tbl1), SHEN_NAME(isu0), SHEN_NAME(isu1), SHEN_NAME(isu2), SHEN_NAME(isu3)};
void Fade(std::int32_t frames, std::int32_t from, std::int32_t to) {
    SetScreenFilmFade(frames, from, 0, 0, 0, to, 0, 0, 0);
}
void Voice(const char *voice, std::int32_t delay) {
    EVT_AT_SetVoice(voice, 1, delay);
}
void Cam(std::int32_t id, std::int32_t delay) {
    EVT_AT_SetCam(id, base, base, 0, 0, delay);
}
void Mot(shen_name character, std::uint32_t animation, float frame, float speed, std::uint32_t flags, std::int32_t delay) {
    (void)EVT_AT_SetMot(character, animation, frame, speed, flags, delay);
}
void Pose(shen_name character, shen_name parent, const Vector3 &position, float yaw, std::int32_t delay = 0) {
    EVT_AT_SetPos(character, parent, position, delay);
    EVT_AT_SetAngY(character, parent, yaw, delay);
}
void Hand(shen_name character, std::int32_t side, std::int32_t *command, std::int32_t ticks) {
    (void)ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(character), side, reinterpret_cast<std::byte *>(command), static_cast<std::int16_t>(ticks));
}
void Film(std::int32_t delay) {
    EventFilmControl request{};
    request.frames = 30;
    request.targetAlpha = 255;
    request.delay = static_cast<float>(delay);
    EVT_AT_MenCtrl(request);
}
void Detach(shen_name object) {
    EventCling work{};
    work.target = object;
    work.mode = 1;
    EVT_AT_Cling(&work);
}
void Attach(shen_name object, const Vector3 &position, const Vector3i &rotation) {
    EventCling work{};
    work.target = object;
    work.character = kun;
    work.bone = 14;
    work.position = position;
    work.rotation = rotation;
    EVT_AT_Cling(&work);
}
bool Daytime(const DateTime &date) {
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(date.hour) - 19) > 5;
}
void SetupLights(shen_name light) {
    EV_SetModelDataDefaultVertexShader();
    DateTime now{};
    shenGetCurrentDateTime(now);
    if (Daytime(now)) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    } else {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
    }
    EV_lightLoad(light);
    std::uint32_t index = 0;
    while (GetMapModelSlots()[index].reader)
        ++index;
    AssignLightProgram(index - 1);
}
std::int32_t Initialize(WtkqWork &work) {
    (void)EV_SetNewSceneIndex();
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("WTKQ"));
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("WTKQ"), 0);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("WTKQ"), 0);
    (void)EV_FreeAuthorPak(1, "0146");
    (void)EV_LoadCutsceneFromCurrentScene("0146A000");
    (void)qteBattleUtil(BattleQteMode::SetWait, 0, 6, 16);
    (void)qteBattleUtil(BattleQteMode::SetFirstCommands, 16, 1024);
    (void)qteBattleUtil(BattleQteMode::SetLastCommands);
    (void)qteBattleUtil(BattleQteMode::ResetStates);
    (void)qteBattleUtil(BattleQteMode::Load);
    (void)qteBattleUtil(BattleQteMode::Queue);
    (void)SetDoorSubjectEnabled(SHEN_NAME(R709), 0);
    (void)SetLowQualityHands(ctrlGetScenarioChara(ryo));
    (void)SetLowQualityHands(ctrlGetScenarioChara(kun));
    (void)FindMotionWork(ctrlGetScenarioChara(ryo));
    (void)FindMotionWork(ctrlGetScenarioChara(kun));
    for (auto character : {ryo, kun})
        for (std::int32_t side = 0; side < 2; ++side)
            (void)SetHighModelXYZ(ctrlGetScenarioChara(character), 1, side, 0);
    for (auto name : furniture)
        EV_DisableCharacterDisplay(name);
    SetupLights(SHEN_NAME(l002));
    (void)CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    (void)CreateEventCharacter(CharacterCreationHighModel, nullptr, dummy);
    Pose(base, shen_name{0xffffffffU}, {266.700012f, 115.300003f, 329.200012f}, 0);
    work.phase = 4;
    shenGetCurrentDateTime(eventSavedCurrentDateTime);
    work.retried = 0;
    return 0;
}
std::int32_t StartAuthor(WtkqWork &work) {
    (void)EV_LoadCutsceneFromCurrentScene("0146A000");
    (void)EV_LoadAudio("0146A000", 0);
    (void)EV_spmBeginSampleExecute();
    EV_EnableCharacterDisplay(syg);
    EV_EnableCharacterDisplay(tkm);
    SetCurrentDateTimeTo(eventSavedCurrentDateTime);
    if (work.retried > 0)
        EV_SetAuthorActiveTo(1);
    QueueSkAuthorStart(work.self, 1414681683, nullptr, nullptr, 0);
    work.phase = 2;
    snapSetDesc(0);
    return 0;
}
std::int32_t Battle(WtkqWork &work) {
    if (work.step) {
        const auto result = qteBattleUtil(BattleQteMode::GetResult);
        if (result) {
            work.phase = result == 1 ? 1 : 3;
            work.step = 0;
        }
        return 0;
    }
    (void)EV_LoadAudio("0146A000", 1);
    Pose(ryo, base, {0, 0, 0}, 180);
    Pose(iii, ryo, {.4f, 0, -1.7f}, 0);
    Mot(ryo, 9227, 0, .8f, 0x40000000U, 0);
    Mot(iii, 9228, 0, .8f, 0x40000000U, 0);
    Cam(4000, 0);
    EVT_AT_QTE(0, 16, 4, 20);
    ++work.step;
    return 20;
}
std::int32_t Retry(WtkqWork &work) {
    Pose(iii, ryo, {.4f, 0, -1.7f}, 0);
    Mot(ryo, 9227, 15, 1, 0x44000000U, 0);
    Mot(iii, 9228, 15, 1, 0x44000000U, 0);
    Cam(4500, 0);
    Voice("0146A012", 5);
    Voice("0146C011", 30); // @text EN: "Look at him." | JA: "へっ、ザマねぇな。"
    Film(60);
    snapSetDesc(1);
    work.retried = 1;
    work.phase = 4;
    return 150;
}
std::int32_t Win(WtkqWork &work) {
    for (auto character : {otam, flyp, syg, tkm})
        EV_DisableCharacterDisplay(character);
    Detach(otam);
    Detach(flyp);
    Pose(ryo, base, {0, 0, 0}, 180);
    Pose(iii, ryo, {.1f, 0, -2.6f}, 0);
    Pose(see, ryo, {.5f, 0, -1.3f}, 0, 40);
    Mot(ryo, 9223, 40, 1, 0x40000000U, 0);
    Mot(ryo, 9224, 140, 1, 0x40000000U, 40);
    Mot(iii, 9219, 40, 1, 0x40000000U, 0);
    Mot(see, 9222, 100, 1, 0x40000000U, 0);
    Cam(1000, 0);
    Cam(1001, 40);
    Cam(1002, 70);
    EVT_AT_DispBlur(ryo, 1, 10, 3, 50, 10);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(ryo), 1, 16, 1);
    Hand(ryo, 0, ryoQteHandPose, 0);
    Hand(ryo, 1, ryoQteHandPose, 0);
    Hand(kun, 0, crateGripHandPose, 0);
    Hand(kun, 1, crateGripHandPose, 0);
    Voice("0146A013", 10);
    Voice("0146D015", 50);
    QueueSkQteWait(&work.qteElapsed, 256, 24, 20, WtkqQteSuccess, WtkqQteFailure, 70);
    work.phase = 5;
    return 0;
}
std::int32_t EndAuthor(WtkqWork &work) {
    SetupLights(SHEN_NAME(WTS3));
    DateTime now{};
    shenGetCurrentDateTime(now);
    if (Daytime(now))
        EV_ReinitLights(1);
    else
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    (void)EV_LoadCutsceneFromCurrentScene("0146A020");
    (void)EV_LoadAudio("0146A000", 2);
    Fade(30, 255, 0);
    EV_DisableCharacterDisplay(otam);
    EV_DisableCharacterDisplay(flyp);
    Detach(otam);
    Detach(flyp);
    QueueSkAuthorStart(work.self, 1598312005, nullptr, nullptr, 0);
    work.phase = 6;
    return 0;
}
} // namespace
// @verified - p
void WtkqInit() {
}
void WtkqMain(std::uintptr_t) {
    Fade(1, 0, 255);
    SetPlayerControlEnabled(0);
    SetPlayerControlEnabled(0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), event);
    RegisterEventTask(task);
    auto *work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = QueueWtkq;
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
void QueueWtkq(void *) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(WtkqMainCallback, 4, 0x0b, sizeof(WtkqWork), event);
    auto *work = static_cast<WtkqWork *>(HLib::GetTaskParameter(task));
    *work = {};
    work->self = task;
    work->characters = Queue_EVT_AT_ReadChr(task, names);
}
// @verified - p
void WtkqMainCallback(void *data) {
    auto &work = *static_cast<WtkqWork *>(data);
    std::int32_t delay = 0;
    switch (work.phase) {
    case 0:
        delay = Initialize(work);
        break;
    case 1:
        delay = Win(work);
        break;
    case 2:
        delay = Battle(work);
        break;
    case 3:
        delay = Retry(work);
        break;
    case 4:
        delay = StartAuthor(work);
        break;
    case 5:
        delay = EndAuthor(work);
        break;
    case 6:
        spmFadeOut();
        HLib::SetCurrentTaskTo(WtkqEndCallback);
        HLib::SetupTaskDelay(30);
        return;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
// @verified - p
std::int32_t WtkqQteSuccess() {
    Attach(flyp, {.058f, -.037f, -.023f}, {26068, 46968, 51082});
    EV_EnableCharacterDisplay(flyp);
    EVT_AT_SetAngY(ryo, ryo, -75.5f, 0);
    Pose(iii, ryo, {1.7f, 0, .8f}, 0);
    Pose(kun, ryo, {2.6f, 0, 1.3f}, 0);
    Mot(ryo, 9225, 60, 1, 0x40000000U, 0);
    Mot(iii, 9220, 60, 1, 0x40000000U, 0);
    Mot(kun, 9217, 60, 1, 0x40000000U, 0);
    Voice("0146F021", 145); // @text EN: " " | JA: "えへっ。"
    Cam(2000, 0);
    Cam(2001, 60);
    Cam(2002, 110);
    EVT_AT_DispBlur(ryo, 4, 20, 1, 30, 0);
    EVT_AT_SetFaceLooks(kun, FacialExpressionId::Normal, 15, 1, 110);
    EVT_AT_SetFaceLooks(kun, FacialExpressionId::SmileKun, 30, 1, 145);
    Voice("0146C015", 15);
    Voice("0146C013", 30);
    Film(200);
    return 230;
}
// @verified - p
std::int32_t WtkqQteFailure() {
    Attach(otam, {.05f, -.04f, -.04f}, {20534, 58418, 58254});
    EV_EnableCharacterDisplay(otam);
    EVT_AT_SetAngY(ryo, ryo, 112.5f, 0);
    Pose(iii, ryo, {-1.6f, 0, -.7f}, 0);
    Pose(kun, ryo, {-3.8f, 0, .4f}, 0);
    Pose(dummy, base, {0, 1.9f, -.6f}, 0);
    Mot(ryo, 9226, 60, 1, 0x44000000U, 0);
    Mot(iii, 9221, 60, 1, 0x44000000U, 0);
    Mot(kun, 9218, 60, 1, 0x44000000U, 0);
    Voice("0146C012", 10); // @text EN: "You!" | JA: "このおっ！"
    Voice("0146F019", 115); // @text EN: "You=@you=@you!" | JA: "このこのこのこのぉ！"
    Voice("0146C015", 170);
    Voice("0146F020", 240); // @text EN: "All right!" | JA: "やったね！！"
    Hand(kun, 0, handPoseReplacementCommand[0], 8);
    Cam(3000, 0);
    Cam(3001, 75);
    Cam(3002, 150);
    Cam(3003, 240);
    EVT_AT_DispBlur(ryo, 1, 20, 2, 20, 160);
    EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Suffer, 16, 1, 30);
    EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Angry, 16, 1, 160);
    EVT_AT_SetFaceLooks(kun, FacialExpressionId::Angry, 16, 1, 60);
    EVT_AT_SetFaceLooks(kun, FacialExpressionId::SmileKun, 16, 1, 240);
    Film(315);
    return 345;
}
// @verified - p
void WtkqEndCallback(void *data) {
    auto &work = *static_cast<WtkqWork *>(data);
    (void)FindMotionWork(ctrlGetScenarioChara(ryo));
    (void)FindMotionWork(ctrlGetScenarioChara(kun));
    (void)hndlSetFlags(ctrlGetScenarioChara(ryo));
    (void)hndlSetFlags(ctrlGetScenarioChara(kun));
    (void)qteBattleUtil(BattleQteMode::RetireAndFree);
    (void)ExecuteCleanupFuncForAreaWithCachedScene();
    (void)SetDoorSubjectEnabled(SHEN_NAME(R709), 1);
    for (auto character : furniture)
        EV_EnableCharacterDisplay(character);
    SetCharaKill(base, 1);
    SetCharaKill(dummy, 1);
    ET_AT_KillWork(work.characters);
    (void)EV_FreeAuthorPak(0, "0146");
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WTKQ"), 0);
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    scenFreeEvent("WTKQ.EV3");
    (void)doorLoadAuthorWithAudio(shen_name{});
    Fade(30, 255, 0);
    (void)sbtLoadProgram();
    (void)EV_spmBeginSampleExecute();
    (void)EV_NPC_NewLoadSw(1);
    EV_stepSetFlags(0, 512);
    FinishLoading();
}
const ScriptAreaDefinition wtkqScriptDefinition{.areaId = AREA_WTKQ, .field04 = 0, .init = WtkqInit, .main = WtkqMain};
} // namespace shenmue::s2
#endif
