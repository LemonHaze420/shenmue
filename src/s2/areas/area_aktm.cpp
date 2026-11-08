#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_aktm.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void AreaAKTMMain(void)
{
    EVT_AT_MenCtrl(1, 0, 0, 0, 0, 255, 0, 0, 0, 1);
    FTBL.SetPlayerControlEnabled(0);
    QueueClearNpcs(QueueBattle_Callback, 0);
}
#else
#if defined(SHENMUE_EVENT_AKTM_RETRIES)
static void *aktmMotion = (void *)-1;
#endif
static shen_name aktmNames[] = {AKTM_RYO, AKTM_TGI, AKTM_HVA, AKTM_HV6, AKTM_HV7, AKTM_HV8, AKTM_KEI, EVENT_CHARACTER_INVALID};
static Vector3 aktmCameraPosition = {270.909912109375f, 100.92680358886719f, 235.43089294433594f};
static Vector3 aktmCameraTarget = {268.4183044433594f, 100.7426986694336f, 232.3896026611328f};
#if !defined(SHENMUE_EVENT_AKTM_RETRIES)
static void *aktmMotion;
#endif
static EventDateTime aktmSavedTime;
#if defined(SHENMUE_EVENT_AKTM_RETRIES)
static int aktmRetries;
#endif

void QueueBattle_Callback(void *argument)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Battle_Callback, 4, 11, sizeof(AktmBattleWork), TASK_EVNT);
    AktmBattleWork *work = (AktmBattleWork *)FTBL.HLib_GetTaskParameter(task);
    *(int *)&work->phase = 0;
    work->characters = 0;
    work->self = task;
    work->phase = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, aktmNames);
    EventDebugRegister(AKTM_DEBUG_ID, End_Callback, work, 100000);
}

#if defined(SHENMUE_EVENT_AKTM_RETRIES)
int SetupAktmBattle(AktmBattleWork *work)
{
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"AKTM", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "AKTM", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "AKTM", 0, 0);
    aktmMotion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    FTBL.EV_FreeAuthorPak(1, "0053");
    FTBL.EV_FreeAuthorPak(1, "0138");
    FTBL.CreateEventCharacter(3, 0, AKTM_BASE);
    FTBL.CreateEventCharacter(3, 0, AKTM_PROP);
    EVT_AT_SetPos(AKTM_BASE, EVENT_CHARACTER_INVALID, 267.4f, 100.0f, 230.9f, 0);
    EVT_AT_SetAngY(AKTM_BASE, EVENT_CHARACTER_INVALID, 0.0f, 0);
    FTBL.GetCurrentDateTime((DateTime *)&aktmSavedTime);
    work->phase = 1;
    aktmRetries = 0;
    return 0;
}
#else
int SetupAktmBattle(AktmBattleWork *work)
{
    FTBL.CheckIsArgEquZero(1, 1);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"AKTM", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "AKTM", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "AKTM", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0053");
    FTBL.EV_FreeAuthorPak(1, "0138");
    FTBL.CreateEventCharacter(3, 0, AKTM_BASE);
    FTBL.CreateEventCharacter(3, 0, AKTM_PROP);
    EVT_AT_SetPos(AKTM_BASE, EVENT_CHARACTER_INVALID, 267.4f, 100.0f, 230.9f, 0);
    EVT_AT_SetAngY(AKTM_BASE, EVENT_CHARACTER_INVALID, 0.0f, 0);
    FTBL.GetCurrentDateTime((DateTime *)&aktmSavedTime);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(AKTM_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 1);
    work->phase = 1;
    return 0;
}
#endif

int FinishAktmBattle(AktmBattleWork *work)
{
    FTBL.spmFadeOut(0);
    FTBL.HLib_SetCurrentTaskTo(End_Callback);
    return 30;
}

void End_Callback(void *data)
{
    AktmBattleWork *work = (AktmBattleWork *)data;
    FTBL.EV_stepSetFlags(136, 0);
    FTBL.SetScenarioCharacterKillMode(AKTM_BASE, 1);
    FTBL.SetScenarioCharacterKillMode(AKTM_PROP, 1);
#if defined(SHENMUE_EVENT_AKTM_RETRIES)
    if (aktmMotion != (void *)-1)
        FTBL.DeleteMotion(aktmMotion);
#endif
    ET_AT_KillWork(work->characters);
    FTBL.EV_HLKillOtherHierarchyTask(work->self);
    EventDebugUnregister(AKTM_DEBUG_ID);
    FTBL.EV_FreeAuthorPak(0, "0053");
    FTBL.EV_ControlPackageResources(0, 6, "AKTM", 0, 0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_scenQueueLoad(g_actNumber, AKTM_ABLB, 0);
    FTBL.scenFreeEvent("AKTM.EV3");
}

void Battle_Callback(void *data)
{
    AktmBattleWork *work = (AktmBattleWork *)data;
    int delay = 0;
    switch (work->phase) {
    case 0:
        delay = SetupAktmBattle(work);
        break;
    case 1:
        delay = StartAktmAuthor(work);
        break;
    case 2:
        delay = InitializeFight(work);
        break;
#if defined(SHENMUE_EVENT_AKTM_RETRIES)
    case 3:
        delay = StartAktmFight(work);
        break;
    case 4:
        delay = WaitAktmFight(work);
        break;
    case 5:
        delay = RetryAktmFight(work);
        break;
    case 6:
        delay = ReloadAktmRetryTitle(work);
        break;
    case 7:
        delay = StartAktmWinAuthor(work);
        break;
    case 8:
        delay = FinishAktmBattle(work);
        break;
#else
    case 3:
        delay = WaitAktmFight(work);
        break;
    case 4:
        delay = RetryAktmFight(work);
        break;
    case 5:
        delay = StartAktmWinAuthor(work);
        break;
    case 6:
        delay = FinishAktmBattle(work);
        break;
#endif
    }
    if (delay > 0)
        FTBL.HLib_SetupTaskDelay(delay);
}

int StartAktmAuthor(AktmBattleWork *work)
{
#if defined(SHENMUE_EVENT_AKTM_RETRIES)
    FTBL.EV_LoadCutsceneFromCurrentScene("0053A000");
    FTBL.EV_LoadAudio("0053A000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#else
    FTBL.EV_LoadCutsceneFromCurrentScene("0138A000");
    FTBL.EV_LoadAudio("0138A000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_authSceneCtrl(-8);
#endif
    QueueSkAuthorStart(work->self, AKTM_START_AUTHOR, 0, 0, 0);
    EventDebugCheck(AKTM_DEBUG_ID, 4);
    work->phase = 2;
    return 0;
}

int StartAktmWinAuthor(AktmBattleWork *work)
{
#if !defined(SHENMUE_EVENT_AKTM_RETRIES)
    FTBL.EV_LoadCutsceneFromCurrentScene("0138A000");
    FTBL.EV_LoadAudio("0138A000", 0);
#endif
    ResetOpponent(AKTM_TGI);
    ResetOpponent(AKTM_HVA);
    EVT_AT_CharaDisp(AKTM_HV6, 0, 1);
    EVT_AT_CharaDisp(AKTM_HV7, 0, 1);
    EVT_AT_CharaDisp(AKTM_HV8, 0, 1);
    EVT_AT_CharaDisp(AKTM_KEI, 0, 1);
#if defined(SHENMUE_EVENT_AKTM_RETRIES)
    QueueSkAuthorStart(work->self, AKTM_WIN_AUTHOR, 0, 0, 0);
    work->phase = 8;
#else
    FTBL.EV_authSceneCtrl(-7);
    QueueSkAuthorStart(work->self, AKTM_WIN_AUTHOR, 0, 0, 0);
    work->phase = 6;
#endif
    return 0;
}

#if defined(SHENMUE_EVENT_AKTM_RETRIES)
int InitializeFight(AktmBattleWork *work)
{
    EVT_AT_SetAngY(AKTM_RYO, EVENT_CHARACTER_INVALID, 0.0f, 0);
    EVT_AT_SetAngY(AKTM_TGI, EVENT_CHARACTER_INVALID, 180.0f, 0);
    EVT_AT_SetAngY(AKTM_HVA, EVENT_CHARACTER_INVALID, 150.0f, 0);
    EVT_AT_SetAngY(AKTM_HV6, EVENT_CHARACTER_INVALID, 220.0f, 0);
    EVT_AT_SetAngY(AKTM_HV7, EVENT_CHARACTER_INVALID, 90.0f, 0);
    EVT_AT_SetAngY(AKTM_HV8, EVENT_CHARACTER_INVALID, 250.0f, 0);
    EVT_AT_SetAngY(AKTM_KEI, EVENT_CHARACTER_INVALID, 120.0f, 0);
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_RYO), 0, 0, 0, aktmRetries * 100 + 200, 0);
    FTBL.vfCopyPosFromEntity(1, FTBL.ctrlGetCharaByName(AKTM_RYO), 0.0f, 0.0f, 10.0f);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_TGI), 0, 1, 1, 80, 2);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_HVA), 0, 1, 1, 60, 2);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_HV6), 0, 1, 1, 40, 2);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(AKTM_HV6), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(AKTM_HV6), 0, 0x200000, 2048);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_HV7), 0, 1, 1, 40, 2);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(AKTM_HV7), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(AKTM_HV7), 0, 0x200000, 2048);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_HV8), 0, 1, 1, 40, 2);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(AKTM_HV8), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(AKTM_HV8), 0, 0x200000, 2048);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_KEI), 0, 1, 1, 40, 2);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(AKTM_KEI), 0, 0x400000, 128);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(AKTM_KEI), 0, 0x200000, 2048);
    EVT_AT_SetMot(AKTM_RYO, 306, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(AKTM_TGI, 3632, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(AKTM_HVA, 3632, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(AKTM_HV6, 3632, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(AKTM_HV7, 3632, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(AKTM_HV8, 3632, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(AKTM_KEI, 3632, 0.0f, 1.0f, 0x40000000, 0);
    work->phase = 3;
    return 0;
}

int StartAktmFight(AktmBattleWork *work)
{
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.camCtrlSetState(9);
    FTBL.camCtrlSetNextPosition(&aktmCameraPosition);
    FTBL.camCtrlSetTargetPosition(&aktmCameraTarget);
    FTBL.EV_vfBattleUtil(3, 0x800, 1);
    FTBL.vfmQueueFightStart(2, 2, 1.0f, 1, -1);
    FTBL.vfmDispatchControl(8, 0, (const void *)13, 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(AKTM_RYO), 1);
    work->phase = 4;
    return 0;
}

int WaitAktmFight(AktmBattleWork *work)
{
    int delay = 0;
    if (!FTBL.IsFightActive()) {
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(AKTM_RYO)) == 3)
            work->phase = 7;
        else {
            work->phase = 6;
            if (aktmRetries < 15)
                aktmRetries++;
            FTBL.Queue_ST_LoadTitleFadeIn(SHEN_NAME4('R', 'T', 'R', '2'));
        }
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        delay = 30;
    }
    return delay;
}
#else
int InitializeFight(AktmBattleWork *work)
{
    FTBL.EV_LoadCutsceneFromCurrentScene("0053A000");
    FTBL.EV_LoadAudio("0053A000", 1);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.camCtrlSetState(9);
    FTBL.camCtrlSetNextPosition(&aktmCameraPosition);
    FTBL.camCtrlSetTargetPosition(&aktmCameraTarget);
    EVT_AT_SetAng(AKTM_RYO, EVENT_CHARACTER_INVALID, 0.0f, 0.0f, 0.0f, 0);
    FTBL.EV_LoadAudio("0138A000", 3);
    aktmMotion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_RYO), 0, 0, 0, 0, 0);
    FTBL.vfCopyPosFromEntity(1, FTBL.ctrlGetCharaByName(AKTM_RYO), 0.0f, 0.0f, 10.0f);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_TGI), 0, 1, 1, 80, 2);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_HVA), 0, 1, 1, 60, 2);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_HV6), 0, 1, 1, 40, 2);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_HV7), 0, 1, 1, 40, 2);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_HV8), 0, 1, 1, 40, 2);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(AKTM_KEI), 0, 1, 1, 40, 2);
    FTBL.vfmQueueFightStart(2, 2, 1.0f, 1, -1);
    FTBL.vfmDispatchControl(8, 0, (const void *)13, 1);
    work->phase = 3;
    return 0;
}

int WaitAktmFight(AktmBattleWork *work)
{
    int delay = 0;
    if (!FTBL.IsFightActive()) {
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(AKTM_RYO)) == 3)
            work->phase = 5;
        else
            work->phase = 1;
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        delay = 30;
    }
    return delay;
}
#endif

int RetryAktmFight(AktmBattleWork *work)
{
    EVT_AT_SetPos(AKTM_RYO, AKTM_BASE, -2.5f, 0.0f, 4.4f, 0);
    EVT_AT_SetAngY(AKTM_RYO, AKTM_BASE, 292.5f, 0);
    EVT_AT_SetPos(AKTM_TGI, AKTM_BASE, 2.0f, 0.0f, 4.4f, 0);
    EVT_AT_SetAngY(AKTM_TGI, AKTM_BASE, 90.0f, 0);
    EVT_AT_SetPos(AKTM_HVA, AKTM_BASE, 2.8f, 0.0f, 3.3f, 0);
    EVT_AT_SetAngY(AKTM_HVA, AKTM_BASE, 112.5f, 0);
    EVT_AT_SetPos(AKTM_HV6, AKTM_BASE, 1.5f, 0.0f, 1.4f, 0);
    EVT_AT_SetAngY(AKTM_HV6, AKTM_BASE, 112.5f, 0);
    EVT_AT_SetPos(AKTM_HV7, AKTM_BASE, 4.3f, 0.0f, 3.9f, 0);
    EVT_AT_SetAngY(AKTM_HV7, AKTM_BASE, 90.0f, 0);
    EVT_AT_SetPos(AKTM_HV8, AKTM_BASE, 1.9f, 0.0f, 2.4f, 0);
    EVT_AT_SetAngY(AKTM_HV8, AKTM_BASE, 112.5f, 0);
    EVT_AT_SetPos(AKTM_KEI, AKTM_BASE, 0.9f, 0.0f, 2.9f, 0);
    EVT_AT_SetAngY(AKTM_KEI, AKTM_BASE, 135.0f, 0);
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&aktmSavedTime);
#if defined(SHENMUE_EVENT_AKTM_RETRIES)
    work->phase = 6;
#else
    work->phase = 2;
#endif
    return 30;
}

#if defined(SHENMUE_EVENT_AKTM_RETRIES)
int ReloadAktmRetryTitle(AktmBattleWork *work)
{
    FTBL.ST_LoadTitleFade();
    work->phase = 1;
    return 30;
}
#endif

void ResetOpponent(shen_name name)
{
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 69, 2, 0);
    FTBL.EV_SetCharaControllerFlags(name, 3, 0);
    FTBL.EV_SetCharaControllerFlags(name, 0, 0);
    FTBL.EV_SetCharaControllerFlags(name, 2, 0);
    FTBL.EV_SetCharaControllerFlags(name, 3, 0);
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 55, 2);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(name), 0, 0);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(name), 0, 3);
}
#endif
#else
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
    mtwkSetRegionFlag(scenFindMotionwork(name), 69, 2, MotionRegion::LowerBody);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 0, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(name, false);
    mtwkSetExtraFlag(scenFindMotionwork(name), 55, 2);
    mtwkUpdateWithPosition(scenFindMotionwork(name), nullptr, 0);
    mtwkUpdateWithPosition(scenFindMotionwork(name), nullptr, 3);
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
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("AKTM"), 0);
    sbtLoadProgram();
    EV_NPC_NewLoadSw(1);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(ABLB), 0);
    scenFreeEvent("AKTM.EV3");
}

void Setup(BattleWork *work) {
    EV_SetNewSceneIndex();
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("AKTM"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("AKTM"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("AKTM"), 0);
    motionHandle = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    EV_FreeAuthorPak(1, "0053");
    EV_FreeAuthorPak(1, "0138");
    CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, prop);
    EVT_AT_SetPos(base, invalid, Vector3{267.4f, 100, 230.9f}, 0);
    EVT_AT_SetAngY(base, invalid, 0, 0);
    shenGetCurrentDateTime(savedTime);
    work->phase = 1;
    retries = 0;
}

void InitializeFight(BattleWork *work) {
    for (int i = 0; i < 7; ++i)
        EVT_AT_SetAngY(names[i], invalid, initialYaws[i], 0);
    shenStartFGST(10, 10, 0, 0);
    shenInitFightData(ctrlGetScenarioChara(names[0]), nullptr, 0, 0, static_cast<std::uint16_t>(100 * (retries + 2)), 0);
    vfCopyPosFromEntity(1, ctrlGetScenarioChara(names[0]), 0, 0, 10);
    for (int i = 1; i < 7; ++i) {
        auto *character = ctrlGetScenarioChara(names[i]);
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
    using enum CameraState;
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
        camCtrlSetState(Event);
        camCtrlSetNextPosition(Vector3{270.909912109375f, 100.92680358886719f, 235.43089294433594f});
        camCtrlSetTargetPosition(Vector3{268.4183044433594f, 100.7426986694336f, 232.3896026611328f});
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        vfmQueueFightStart(2, 2, 1, 1, -1);
        vfmDispatchControl(FightControlOp::CameraParameter, shen_name{}, 13, 1);
        SetPluralControlFor(ctrlGetScenarioChara(names[0]), 1);
        work->phase = 4;
        break;
    case 4:
        if (IsFightActive())
            return;
        if (GetBtlStatus(ctrlGetScenarioChara(names[0])) == 3)
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
#endif
