#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_wrsq.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void WrsqMain(void)
{
    FTBL.SetPlayerControlEnabled(0);
    if (g_currentMap == WRSQ_AREA)
        FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    QueueClearNpcs(QueueWrsq, 0);
}
#else
static shen_name characterNames[] = {WRSQ_RYO, WRSQ_SAY, WRSQ_III, WRSQ_SEE, WRSQ_SJN, WRSQ_GNB, WRSQ_MYU, WRSQ_INVALID};
static short sequence;

void QueueWrsq(void *data)
{
    HLibTask *task;
    WrsqWork *work;
    FTBL.SetLoadingBusy();
    task = FTBL.HLib_EnqueueTaskWithParameter(WrsqMainCallback, 5, 11, sizeof(WrsqWork), TASK_EVNT);
    work = (WrsqWork *)FTBL.HLib_GetTaskParameter(task);
    *(int *)&work->phase = 0;
    work->characters = 0;
    *(int *)&work->won = 0;
    work->self = task;
    work->phase = 0;
    work->won = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, characterNames);
    EventDebugRegister(WRSQ_DEBUG_ID, WrsqDebugExitCallback, work, 100000);
}

int InitializeWrsq(WrsqWork *work)
{
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WRSQ", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "WRSQ", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "WRSQ", 0, 0);
    FTBL.CreateEventCharacter(3, 0, WRSQ_BASE);
    FTBL.EV_FreeAuthorPak(1, "0045");
    EVT_AT_SetPos(WRSQ_BASE, WRSQ_INVALID, 181.3f, 102.25f, 196.6f, 0);
    EVT_AT_SetAngY(WRSQ_BASE, WRSQ_INVALID, 270.0f, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0045A000");
    FTBL.EV_LoadAudio("0045A000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.qteBattleUtil(8, 10, 6, 24, 0);
    FTBL.qteBattleUtil(9, 256, 1024, 512, 0);
    FTBL.qteBattleUtil(10, 0, 0, 0, 0);
    FTBL.qteBattleUtil(1, 0, 0, 0, 0);
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
    work->phase = 1;
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WRSQ_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WRSQ_RYO), 1, 1, 0);
    FTBL.EV_SetPlayerFlag(12, 481, 1);
    return 0;
}

void WrsqDebugExitCallback(void *data)
{
    WrsqEndCallback(data);
}

int FinishWrsq(WrsqWork *work)
{
    if (g_currentMap != WRSQ_AREA) {
        FTBL.spmFadeOut(0);
        FTBL.HLib_SetCurrentTaskTo(WrsqEndCallback);
    } else {
        work->step = 0;
        work->phase = 1;
    }
    return 30;
}

void WrsqEndCallback(void *data)
{
    WrsqWork *work = (WrsqWork *)data;
    if (!work->won)
        FTBL.EV_stepSetFlags(79, 0);
    else
        FTBL.EV_stepSetFlags(79, 1);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WRSQ_RYO), 0, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WRSQ_RYO), 0, 1, 0);
    FTBL.SetScenarioCharacterKillMode(WRSQ_BASE, 1);
    EventDebugUnregister(WRSQ_DEBUG_ID);
    ET_AT_KillWork(work->characters);
    FTBL.EV_HLKillOtherHierarchyTask(work->self);
    FTBL.FinishLoading();
    FTBL.EV_FreeAuthorPak(0, "0045");
    FTBL.EV_ControlPackageResources(0, 6, "WRSQ", 0, 0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_SetPlayerFlag(12, 481, 0);
    FTBL.scenFreeEvent("WRSQ.EV2");
    EVT_AT_SetPos(WRSQ_RYO, WRSQ_INVALID, 180.4f, 102.25f, 195.0f, 0);
    EVT_AT_SetAngY(WRSQ_RYO, WRSQ_INVALID, 180.0f, 0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}

void WrsqMainCallback(void *data)
{
    WrsqWork *work = (WrsqWork *)data;
    int delay = 0;
    switch (work->phase) {
    case 0:
        delay = InitializeWrsq(work);
        break;
    case 1:
        delay = IntroWrsq(work);
        break;
    case 2:
        delay = LoseWrsq(work);
        break;
    case 3:
        delay = WinWrsq(work);
        break;
    case 4:
        delay = AdvanceWrsqTime(work);
        break;
    case 5:
        delay = BattleWrsq(work);
        break;
    case 6:
        delay = FinishWrsq(work);
        break;
    }
    if (delay > 0)
        FTBL.HLib_SetupTaskDelay(delay);
}

int IntroWrsq(WrsqWork *work)
{
    int loaded;
    if (!work->step) {
        EventDebugCheck(WRSQ_DEBUG_ID, 4);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_EnableCharacterDisplay(WRSQ_A244);
        FTBL.EV_EnableCharacterDisplay(WRSQ_A245);
        FTBL.EV_EnableCharacterDisplay(WRSQ_GNB);
        FTBL.EV_EnableCharacterDisplay(WRSQ_MYU);
        FTBL.EV_authSceneCtrl(-11);
        FTBL.EV_authSceneCtrl(WRSQ_AUTHOR_FIGHT);
        work->step++;
    } else {
        loaded = FTBL.EV_authSceneCtrl(-1);
        if (!loaded) {
            FTBL.EV_authSceneCtrl(-10);
            FTBL.qteBattleUtil(3, loaded, 0, 0, 0);
            work->phase = 5;
            work->step = loaded;
        }
    }
    return 0;
}

int LoseWrsq(WrsqWork *work)
{
    FTBL.EV_EnableCharacterDisplay(WRSQ_GNB);
    FTBL.EV_EnableCharacterDisplay(WRSQ_MYU);
    QueueSkAuthorStart(work->self, WRSQ_AUTHOR_LOSE, WrsqLoseAuthorCallback, 0, 0);
    work->won = 0;
    work->phase = 6;
    work->step = 0;
    return 0;
}

void WrsqLoseAuthorCallback(void *data)
{
    switch ((short)FTBL.EV_authSceneCtrl(-6)) {
    case 1:
        FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(WRSQ_SJN), 0, 1, 0);
        break;
    case 180:
        FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(WRSQ_SJN), 0, 8, 16);
        break;
    }
}

int WinWrsq(WrsqWork *work)
{
    int delay = 0;
    if (work->step == 0) {
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_EnableCharacterDisplay(WRSQ_GNB);
        FTBL.EV_EnableCharacterDisplay(WRSQ_MYU);
        QueueSkAuthorStart(work->self, WRSQ_AUTHOR_WIN, 0, 0, delay);
        work->won = 1;
        work->step++;
    } else {
        work->phase = 4;
        work->step = 0;
    }
    delay = 0;
    return delay;
}

int AdvanceWrsqTime(WrsqWork *work)
{
    EventDateTime date;
    int delay = 0;
    switch (work->step) {
    case 0:
        FTBL.GetCurrentDateTime((DateTime *)&date);
        date.hour += 2;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
        FTBL.Queue_ST_LoadTitleFadeIn(0);
        delay = 120;
        work->step++;
        break;
    case 1:
        FTBL.ST_LoadTitleFade();
        delay = 30;
        work->step++;
        break;
    case 2:
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EVT_AT_SetCam(2000, WRSQ_BASE, WRSQ_BASE, 0, 0, 0);
        EVT_AT_SetMot(WRSQ_RYO, 6990, 200.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetFaceLooks(WRSQ_RYO, 6, 10, 1.0f, 0);
        EVT_AT_SetFaceLooks(WRSQ_RYO, 0, 10, 1.0f, 220);
        delay = 220;
        work->step++;
        break;
    case 3:
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        delay = 30;
        work->step++;
        work->phase = 6;
        break;
    }
    return delay;
}

int BattleWrsq(WrsqWork *work)
{
    int count;
    int delay = 0;
    if (!FTBL.qteBattleUtil(4, 0, 0, 0, 0))
        return 0;
    FTBL.EV_LoadAudio("0045A000", 1);
    EVT_AT_SetFaceLooks(WRSQ_RYO, 1, 0, 1.0f, 0);
    EVT_AT_SetFaceLooks(WRSQ_III, 1, 0, 1.0f, 0);
    EVT_AT_SetFaceLooks(WRSQ_SEE, 1, 0, 1.0f, 0);
    EVT_AT_SetFaceLooks(WRSQ_SJN, 1, 0, 1.0f, 0);
    FTBL.EV_DisableCharacterDisplay(WRSQ_A244);
    FTBL.EV_DisableCharacterDisplay(WRSQ_A245);
    FTBL.EV_DisableCharacterDisplay(WRSQ_GNB);
    FTBL.EV_DisableCharacterDisplay(WRSQ_MYU);
    Queue_EVT_AT_CtrlHandUp_1(WRSQ_RYO, 0, (void *)areaLoopHandPoses[WRSQ_QTE_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(WRSQ_RYO, 1, (void *)areaLoopHandPoses[WRSQ_QTE_HAND_POSE], 0, 0);
    EVT_AT_SetPos(WRSQ_RYO, WRSQ_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(WRSQ_RYO, WRSQ_BASE, 180.0f, 0);
    EVT_AT_SetPos(WRSQ_SAY, WRSQ_BASE, -0.2f, 0.0f, -2.3f, 0);
    EVT_AT_SetAngY(WRSQ_SAY, WRSQ_BASE, 180.0f, 0);
    EVT_AT_SetPos(WRSQ_III, WRSQ_RYO, 0.1f, 0.0f, -1.4f, 0);
    EVT_AT_SetAngY(WRSQ_III, WRSQ_RYO, 0.0f, 0);
    EVT_AT_SetPos(WRSQ_SEE, WRSQ_RYO, -1.1f, 0.0f, -0.6f, 0);
    EVT_AT_SetAngY(WRSQ_SEE, WRSQ_RYO, 0.0f, 0);
    EVT_AT_SetPos(WRSQ_SJN, WRSQ_RYO, 1.3f, 0.0f, -0.9f, 0);
    EVT_AT_SetAngY(WRSQ_SJN, WRSQ_RYO, 0.0f, 0);
    count = FTBL.qteBattleUtil(6, 0, 0, 0, 0);
    if (count > 0)
        delay += WrsqSuccessA(delay);
    if (count > 1)
        delay += WrsqSuccessB(delay);
    if (count > 2)
        delay += WrsqSuccessC(delay);
    if (count == 0)
        delay += WrsqFailureA(delay);
    if (count == 1)
        delay += WrsqFailureB(delay);
    if (count == 2)
        delay += WrsqFailureC(delay);
    sequence = (sequence + 1) % 4;
    if (count == 3)
        work->phase = 2;
    else
        work->phase = 3;
    return delay;
}

int WrsqSuccessA(int delay)
{
    EVT_AT_SetPos(WRSQ_RYO, WRSQ_BASE, 0.0f, 0.0f, 0.0f, delay);
    EVT_AT_SetAngY(WRSQ_RYO, WRSQ_BASE, 180.0f, delay);
    EVT_AT_SetPos(WRSQ_III, WRSQ_RYO, 0.1f, 0.0f, -1.45f, delay);
    EVT_AT_SetAngY(WRSQ_III, WRSQ_RYO, 0.0f, delay);
    EVT_AT_SetMot(WRSQ_RYO, 6986, 20.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(WRSQ_III, 6980, 20.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(WRSQ_SEE, 6982, 0.0f, 0.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(WRSQ_SJN, 6978, 0.0f, 0.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetCam(1000, WRSQ_BASE, WRSQ_BASE, 0, 0, delay);
    EVT_AT_SetFaceLooks(WRSQ_III, 6, 10, 1.0f, delay + 20);
    EVT_AT_SetVoice("0045B012", delay + 20);
    return 25;
}

int WrsqSuccessB(int delay)
{
    EVT_AT_SetPos(WRSQ_III, WRSQ_BASE, 0.1f, 0.0f, 1.8f, delay);
    EVT_AT_SetAngY(WRSQ_III, WRSQ_BASE, 180.0f, delay);
    EVT_AT_SetPos(WRSQ_SEE, WRSQ_RYO, -1.1f, 0.0f, -0.3f, delay);
    EVT_AT_SetAngY(WRSQ_SEE, WRSQ_RYO, 0.0f, delay);
    EVT_AT_SetMot(WRSQ_RYO, 6987, 50.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(WRSQ_SEE, 6982, 50.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetCam(1001, WRSQ_BASE, WRSQ_BASE, 0, 0, delay);
    EVT_AT_SetVoice("0045C004", delay);
    EVT_AT_SetVoice("0045C007", delay + 40);
    return 60;
}

int WrsqSuccessC(int delay)
{
    EVT_AT_SetPos(WRSQ_SEE, WRSQ_BASE, 1.1f, 0.0f, 1.0f, delay);
    EVT_AT_SetAngY(WRSQ_SEE, WRSQ_BASE, 180.0f, delay);
    EVT_AT_SetPos(WRSQ_SJN, WRSQ_RYO, 1.3f, 0.0f, -0.9f, delay);
    EVT_AT_SetAngY(WRSQ_SJN, WRSQ_RYO, 0.0f, delay);
    EVT_AT_SetMot(WRSQ_RYO, 6988, 130.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(WRSQ_SJN, 6978, 130.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetCam(1002, WRSQ_BASE, WRSQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(1007, WRSQ_BASE, WRSQ_BASE, 0, 0, delay + 40);
    EVT_AT_SetFaceLooks(WRSQ_SJN, 6, 10, 1.0f, 20);
    EVT_AT_SetVoice("0045E012", delay + 10);
    EVT_AT_SetVoice("0045E013", delay + 35);
    return 80;
}

int WrsqFailureA(int delay)
{
    EVT_AT_SetPos(WRSQ_III, WRSQ_RYO, 0.0f, 0.0f, -1.1f, delay);
    EVT_AT_SetAngY(WRSQ_III, WRSQ_RYO, 0.0f, delay);
    EVT_AT_SetMot(WRSQ_RYO, 6983, 40.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(WRSQ_III, 6979, 40.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetCam(1003, WRSQ_BASE, WRSQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(1008, WRSQ_BASE, WRSQ_BASE, 0, 0, delay + 40);
    EVT_AT_SetFaceLooks(WRSQ_RYO, 6, 10, 1.0f, 10);
    Queue_EVT_AT_CtrlHandUp_1(WRSQ_RYO, 0, (void *)areaLoopHandPoses[WRSQ_QTE_REACTION_HAND_POSE], 8, 0);
    Queue_EVT_AT_CtrlHandUp_1(WRSQ_RYO, 1, (void *)areaLoopHandPoses[WRSQ_QTE_REACTION_HAND_POSE], 8, 0);
    EVT_AT_SetVoice("0045B008", delay);
    EVT_AT_SetVoice("0045B010", delay + 40);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, delay + 140);
    return 170;
}

int WrsqFailureB(int delay)
{
    EVT_AT_SetPos(WRSQ_RYO, WRSQ_BASE, 0.2f, 0.0f, -0.1f, delay);
    EVT_AT_SetPos(WRSQ_SEE, WRSQ_RYO, -1.1f, 0.0f, -0.53f, delay);
    EVT_AT_SetAngY(WRSQ_SEE, WRSQ_RYO, 0.0f, delay);
    EVT_AT_SetMot(WRSQ_RYO, 6984, 90.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(WRSQ_SEE, 6981, 90.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetCam(1004, WRSQ_BASE, WRSQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(1005, WRSQ_BASE, WRSQ_BASE, 0, 0, delay + 40);
    EVT_AT_SetFaceLooks(WRSQ_RYO, 6, 10, 1.0f, delay + 40);
    Queue_EVT_AT_CtrlHandUp_1(WRSQ_RYO, 0, (void *)areaLoopHandPoses[WRSQ_QTE_REACTION_HAND_POSE], 8, delay);
    Queue_EVT_AT_CtrlHandUp_1(WRSQ_RYO, 1, (void *)areaLoopHandPoses[WRSQ_QTE_REACTION_HAND_POSE], 8, delay);
    EVT_AT_SetVoice("0045C004", delay + 10);
    EVT_AT_SetVoice("0045C003", delay + 40);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, delay + 120);
    return 150;
}

int WrsqFailureC(int delay)
{
    EVT_AT_SetPos(WRSQ_SEE, WRSQ_BASE, 1.1f, 0.0f, 1.0f, delay);
    EVT_AT_SetAngY(WRSQ_SEE, WRSQ_BASE, 180.0f, delay);
    EVT_AT_SetPos(WRSQ_SJN, WRSQ_RYO, 0.9f, 0.0f, -0.98f, delay);
    EVT_AT_SetAngY(WRSQ_SJN, WRSQ_RYO, 0.0f, delay);
    EVT_AT_SetMot(WRSQ_RYO, 6985, 70.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(WRSQ_SJN, 6977, 70.0f, 1.0f, WRSQ_MOTION_FLAGS, delay);
    EVT_AT_SetCam(1006, WRSQ_BASE, WRSQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(1009, WRSQ_BASE, WRSQ_BASE, 0, 0, delay + 35);
    EVT_AT_SetFaceLooks(WRSQ_RYO, 6, 10, 1.0f, delay + 10);
    Queue_EVT_AT_CtrlHandUp_1(WRSQ_RYO, 0, (void *)areaLoopHandPoses[WRSQ_QTE_REACTION_HAND_POSE], 8, delay);
    Queue_EVT_AT_CtrlHandUp_1(WRSQ_RYO, 1, (void *)areaLoopHandPoses[WRSQ_QTE_REACTION_HAND_POSE], 8, delay);
    EVT_AT_SetVoice("0045E010", delay);
    EVT_AT_SetVoice("0045E011", delay + 50);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, delay + 120);
    return 150;
}
#endif
#else
#include "s2/areas/area_wrsq.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, base{SHEN_NAME(BASE)}, iii{SHEN_NAME(III_)}, see{SHEN_NAME(SEE_)}, sjn{SHEN_NAME(SJN_)};
constexpr shen_name say{SHEN_NAME(SAY_)}, gnb{SHEN_NAME(GNB_)}, myu{SHEN_NAME(MYU_)}, a244{SHEN_NAME(a244)}, a245{SHEN_NAME(a245)};
constexpr shen_name event{SHEN_NAME(EVNT)};
constexpr shen_name characterNames[]{ryo, say, iii, see, sjn, gnb, myu, shen_name{0xffffffffU}};
std::int16_t sequence{};
void Hand(std::int32_t side, std::int32_t *command, std::int32_t ticks, std::int32_t delay) {
    Queue_EVT_AT_CtrlHandUp_1(ryo, static_cast<std::uint32_t>(side), reinterpret_cast<std::byte *>(command), static_cast<std::uint32_t>(ticks), delay);
}
void Fade(std::int32_t frames, std::int32_t start, std::int32_t target) {
    SetScreenFilmFade(frames, start, 0, 0, 0, target, 0, 0, 0);
}
void Mot(shen_name name, std::uint32_t motion, float frame, float speed, std::int32_t delay) {
    (void)EVT_AT_SetMot(name, motion, frame, speed, 0x44000000U, delay);
}
void Cam(std::int32_t id, std::int32_t delay) {
    EVT_AT_SetCam(id, base, base, 0, 0, delay);
}
void Voice(const char *name, std::int32_t delay) {
    EVT_AT_SetVoice(name, 1, delay);
}
void TimedFade(std::int32_t delay) {
    EventFilmControl work{};
    work.frames = 30;
    work.targetAlpha = 255;
    work.delay = static_cast<float>(delay);
    EVT_AT_MenCtrl(work);
}
void SetPose(shen_name name, shen_name parent, const Vector3 &position, float yaw, std::int32_t delay) {
    EVT_AT_SetPos(name, parent, position, delay);
    EVT_AT_SetAngY(name, parent, yaw, delay);
}
std::int32_t SuccessA() {
    SetPose(ryo, base, {0, 0, 0}, 180, 0);
    SetPose(iii, ryo, {.1f, 0, -1.45f}, 0, 0);
    Mot(ryo, 0x1b4a, 20, 1, 0);
    Mot(iii, 0x1b44, 20, 1, 0);
    Mot(see, 0x1b46, 0, 0, 0);
    Mot(sjn, 0x1b42, 0, 0, 0);
    Cam(1000, 0);
    EVT_AT_SetFaceLooks(iii, FacialExpressionId::Suffer, 10, 1, 20);
    Voice("0045B012", 20);
    return 25;
}
std::int32_t SuccessB(std::int32_t delay) {
    SetPose(iii, base, {.1f, 0, 1.8f}, 180, delay);
    SetPose(see, ryo, {-1.1f, 0, -.3f}, 0, delay);
    Mot(ryo, 0x1b4b, 50, 1, delay);
    Mot(see, 0x1b46, 50, 1, delay);
    Cam(1001, delay);
    Voice("0045C004", delay);
    Voice("0045C007", delay + 40);
    return 60;
}
std::int32_t SuccessC(std::int32_t delay) {
    SetPose(see, base, {1.1f, 0, 1}, 180, delay);
    SetPose(sjn, ryo, {1.3f, 0, -.9f}, 0, delay);
    Mot(ryo, 0x1b4c, 130, 1, delay);
    Mot(sjn, 0x1b42, 130, 1, delay);
    Cam(1002, delay);
    Cam(1007, delay + 40);
    EVT_AT_SetFaceLooks(sjn, FacialExpressionId::Suffer, 10, 1, 20);
    Voice("0045E012", delay + 10);
    Voice("0045E013", delay + 35);
    return 80;
}
std::int32_t FailureA(std::int32_t delay) {
    SetPose(iii, ryo, {0, 0, -1.1f}, 0, delay);
    Mot(ryo, 0x1b47, 40, 1, delay);
    Mot(iii, 0x1b43, 40, 1, delay);
    Cam(1003, delay);
    Cam(1008, delay + 40);
    EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Suffer, 10, 1, 10);
    Hand(0, ryoQteReactionHandPose, 8, 0);
    Hand(1, ryoQteReactionHandPose, 8, 0);
    Voice("0045B008", delay);
    Voice("0045B010", delay + 40);
    TimedFade(delay + 140);
    return 170;
}
std::int32_t FailureB(std::int32_t delay) {
    EVT_AT_SetPos(ryo, base, {.2f, 0, -.1f}, delay);
    SetPose(see, ryo, {-1.1f, 0, -.52999997f}, 0, delay);
    Mot(ryo, 0x1b48, 90, 1, delay);
    Mot(see, 0x1b45, 90, 1, delay);
    Cam(1004, delay);
    Cam(1005, delay + 40);
    EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Suffer, 10, 1, delay + 40);
    Hand(0, ryoQteReactionHandPose, 8, delay);
    Hand(1, ryoQteReactionHandPose, 8, delay);
    Voice("0045C004", delay + 10);
    Voice("0045C003", delay + 40); // @text EN: "Take this!" | JA: "くらえっ！"
    TimedFade(delay + 120);
    return 150;
}
std::int32_t FailureC(std::int32_t delay) {
    SetPose(see, base, {1.1f, 0, 1}, 180, delay);
    SetPose(sjn, ryo, {.9f, 0, -.98000002f}, 0, delay);
    Mot(ryo, 0x1b49, 70, 1, delay);
    Mot(sjn, 0x1b41, 70, 1, delay);
    Cam(1006, delay);
    Cam(1009, delay + 35);
    EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Suffer, 10, 1, delay + 10);
    Hand(0, ryoQteReactionHandPose, 8, delay);
    Hand(1, ryoQteReactionHandPose, 8, delay);
    Voice("0045E010", delay);
    Voice("0045E011", delay + 50);
    TimedFade(delay + 120);
    return 150;
}
std::int32_t Initialize(WrsqWork &work) {
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("WRSQ"));
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("WRSQ"), 0);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("WRSQ"), 0);
    (void)CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    (void)EV_FreeAuthorPak(1, "0045");
    SetPose(base, shen_name{0xffffffffU}, {181.3f, 102.25f, 196.60001f}, 270, 0);
    (void)EV_LoadCutsceneFromCurrentScene("0045A000");
    (void)EV_LoadAudio("0045A000", 0);
    (void)EV_spmBeginSampleExecute();
    (void)qteBattleUtil(BattleQteMode::SetWait, 10, 6, 24);
    (void)qteBattleUtil(BattleQteMode::SetFirstCommands, 256, 1024, 512);
    (void)qteBattleUtil(BattleQteMode::SetLastCommands);
    (void)qteBattleUtil(BattleQteMode::ResetStates, 1);
    (void)qteBattleUtil(BattleQteMode::Load);
    (void)qteBattleUtil(BattleQteMode::Queue);
    work.phase = 1;
    (void)SetHighModelXYZ(ctrlGetScenarioChara(ryo), 1, 0, 0);
    (void)SetHighModelXYZ(ctrlGetScenarioChara(ryo), 1, 1, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 481, 1);
    return 0;
}
std::int32_t Intro(WrsqWork &work) {
    if (work.step) {
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            (void)EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            (void)qteBattleUtil(BattleQteMode::Start);
            work.phase = 5;
        }
    } else {
        Fade(30, 255, 0);
        for (auto name : {a244, a245, gnb, myu})
            EV_EnableCharacterDisplay(name);
        (void)EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(FIST)));
        ++work.step;
    }
    return 0;
}
std::int32_t Win(WrsqWork &work) {
    if (work.step)
        work.phase = 4;
    else {
        Fade(30, 255, 0);
        EV_EnableCharacterDisplay(gnb);
        EV_EnableCharacterDisplay(myu);
        QueueSkAuthorStart(work.self, static_cast<std::int32_t>(SHEN_NAME(WIN_)), nullptr, nullptr, 0);
        ++work.step;
        work.won = 1;
    }
    return 0;
}
std::int32_t AdvanceTime(WrsqWork &work) {
    std::int32_t delay = 0;
    switch (work.step) {
    case 0: {
        DateTime now{};
        shenGetCurrentDateTime(now);
        now.hour = static_cast<std::int8_t>(now.hour + 2);
        SetCurrentDateTimeTo(now);
        Queue_ST_LoadTitleFadeIn(shen_name{});
        delay = 120;
        break;
    }
    case 1:
        ST_LoadTitleFade();
        delay = 30;
        break;
    case 2:
        Fade(30, 255, 0);
        (void)EV_LoadAudio("0045A000", 0);
        (void)EV_spmBeginSampleExecute();
        Cam(125, 0);
        (void)EVT_AT_SetMot(ryo, 0x1b4e, 200, 1, 0x40000000U, 0);
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(ryo), 6, 10, 1);
        EVT_AT_SetFaceLooks(ryo, FacialExpressionId::Normal, 10, 1, 220);
        delay = 220;
        break;
    case 3:
        Fade(30, 0, 255);
        delay = 30;
        work.phase = 6;
        break;
    default:
        return 0;
    }
    ++work.step;
    return delay;
}
std::int32_t Battle(WrsqWork &work) {
    const auto result = qteBattleUtil(BattleQteMode::GetResult);
    if (!result)
        return 0;
    (void)EV_LoadAudio("0045A000", 1);
    for (auto name : {ryo, iii, see, sjn})
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(name), 1, 0, 1);
    for (auto name : {a244, a245, gnb, myu})
        EV_DisableCharacterDisplay(name);
    Hand(0, ryoQteHandPose, 0, 0);
    Hand(1, ryoQteHandPose, 0, 0);
    SetPose(ryo, base, {0, 0, 0}, 180, 0);
    SetPose(say, base, {-.2f, 0, -2.3f}, 180, 0);
    SetPose(iii, ryo, {.1f, 0, -1.4f}, 0, 0);
    SetPose(see, ryo, {-1.1f, 0, -.6f}, 0, 0);
    SetPose(sjn, ryo, {1.3f, 0, -.9f}, 0, 0);
    const auto count = qteBattleUtil(BattleQteMode::GetCompletedCount);
    std::int32_t delay = 0;
    if (count > 0)
        delay = SuccessA();
    if (count > 1)
        delay += SuccessB(delay);
    if (count > 2)
        delay += SuccessC(delay);
    if (count == 0)
        delay += FailureA(delay);
    else if (count == 1)
        delay += FailureB(delay);
    else if (count == 2)
        delay += FailureC(delay);
    sequence = static_cast<std::int16_t>((sequence + 1) % 4);
    work.phase = count == 3 ? 2 : 3;
    return delay;
}
} // namespace
// @verified - p
void WrsqInit() {
    sequence = 0;
}
void WrsqMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    if (g_currentMap.name == AREA_WRSQ.name)
        Fade(1, 0, 255);
    SetPlayerControlEnabled(0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), event);
    RegisterEventTask(task);
    auto *work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = QueueWrsq;
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
void QueueWrsq(void *) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(WrsqMainCallback, 5, 0x0b, sizeof(WrsqWork), event);
    auto *work = static_cast<WrsqWork *>(HLib::GetTaskParameter(task));
    *work = {};
    work->self = task;
    work->characters = Queue_EVT_AT_ReadChr(task, characterNames);
}
// @verified - p
void WrsqLoseAuthorCallback(void *) {
    const auto frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (frame == 1)
        hmdlSetFlagsFor(ctrlGetScenarioChara(sjn), 0, 1, 0);
    else if (frame == 180)
        hmdlSetFlagsFor(ctrlGetScenarioChara(sjn), 0, 8, 16);
}
// @verified - p
void WrsqMainCallback(void *data) {
    auto &work = *static_cast<WrsqWork *>(data);
    std::int32_t delay = 0;
    switch (work.phase) {
    case 0:
        delay = Initialize(work);
        break;
    case 1:
        delay = Intro(work);
        break;
    case 2:
        EV_EnableCharacterDisplay(gnb);
        EV_EnableCharacterDisplay(myu);
        QueueSkAuthorStart(work.self, static_cast<std::int32_t>(SHEN_NAME(LOSE)), WrsqLoseAuthorCallback, nullptr, 0);
        work.won = 0;
        work.phase = 6;
        return;
    case 3:
        delay = Win(work);
        break;
    case 4:
        delay = AdvanceTime(work);
        break;
    case 5:
        delay = Battle(work);
        break;
    case 6:
        if (g_currentMap.name == AREA_WRSQ.name)
            work.phase = 1;
        else {
            spmFadeOut();
            HLib::SetCurrentTaskTo(WrsqEndCallback);
        }
        HLib::SetupTaskDelay(30);
        return;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
// @verified - p
void WrsqEndCallback(void *data) {
    auto &work = *static_cast<WrsqWork *>(data);
    EV_stepSetFlags(work.won != 0, 0x4f);
    (void)SetHighModelXYZ(ctrlGetScenarioChara(ryo), 0, 0, 0);
    (void)SetHighModelXYZ(ctrlGetScenarioChara(ryo), 0, 1, 0);
    SetCharaKill(base, 1);
    ET_AT_KillWork(work.characters);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    FinishLoading();
    (void)EV_FreeAuthorPak(0, "0045");
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WRSQ"), 0);
    (void)sbtLoadProgram();
    (void)EV_spmBeginSampleExecute();
    (void)EV_NPC_NewLoadSw(1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 481, 0);
    scenFreeEvent("WRSQ.EV2");
    SetPose(ryo, shen_name{0xffffffffU}, {180.399994f, 102.25f, 195}, 180, 0);
    Fade(30, 255, 0);
}
const ScriptAreaDefinition wrsqScriptDefinition{.areaId = AREA_WRSQ, .field04 = 0, .init = WrsqInit, .main = WrsqMain};
} // namespace shenmue::s2
#endif
