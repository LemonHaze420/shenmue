#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_tgbt.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
const shen_name tgbtLoadCharacters[] = {TGBT_REN, TGBT_FIRST_FIGHTER, TGBT_SECOND_FIGHTER, TGBT_THIRD_FIGHTER, TGBT_INVALID};

void TgbtMain(void)
{
    HLibTask *task;
    TgbtEventWork *work;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    if (FTBL.DoesScenarioCharacterExist(TGBT_REN)) {
        if (FTBL.IsNpcRouteLoadedOrVisible(TGBT_REN))
            FTBL.npcLockUnlock(TGBT_REN);
        FTBL.npcFree(TGBT_REN);
    }
    task = FTBL.HLib_EnqueueTaskWithParameter(TgbtEventCallback, 4, 11, sizeof(TgbtEventWork), TASK_EVNT);
    work = (TgbtEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->state = 1;
    work->characters = Queue_EVT_AT_ReadChr(task, tgbtLoadCharacters);
    work->frames = 0;
    work->retries = 0;
    InitializeTgbtResources(work);
    FTBL.SetRenderClockAndCompassTo(1);
}
#elif defined(SHENMUE_EVENT_TGBT_AUTHOR)
static shen_name tgbtHiddenCharacters[] = {TGBT_REN, TGBT_SECOND_FIGHTER, TGBT_FIRST_FIGHTER, TGBT_THIRD_FIGHTER, TGBT_INVALID};

void QueueTgbtAuthor(shen_name author)
{
    HLibTask *task;
    TgbtAuthorWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(TgbtAuthorCallback, 4, 11, sizeof(TgbtAuthorWork), TGBT_AUTHOR_TASK);
    work = (TgbtAuthorWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->state = 1;
    BeginTgbtAuthor();
    work->author = author;
}

void TgbtAuthorCallback(void *data)
{
    TgbtAuthorWork *work = (TgbtAuthorWork *)data;
    switch (work->state) {
    case 0:
        FTBL.HLib_DestroyCurrentTask();
        break;
    case 1:
        StartTgbtAuthor(work);
        break;
    case 3:
        EndTgbtAuthor(work);
        break;
    }
}

void StartTgbtAuthor(TgbtAuthorWork *work)
{
    HLibTask *task;
    TgbtAuthorWaitWork *wait;
    task = FTBL.HLib_EnqueueTaskWithParameter(TgbtAuthorWaitCallback, 4, 11, sizeof(TgbtAuthorWaitWork), TGBT_AUTHOR_TASK);
    wait = (TgbtAuthorWaitWork *)FTBL.HLib_GetTaskParameter(task);
    wait->parent = work;
    wait->state = 0;
    PrepareTgbtAuthoredCharacters();
    FTBL.EV_authSceneCtrl(work->author);
    work->state = 2;
}

void TgbtAuthorWaitCallback(void *data)
{
    TgbtAuthorWaitWork *work = (TgbtAuthorWaitWork *)data;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        work->parent->state = 0;
        RestoreTgbtAuthoredCharacters();
        FTBL.HLib_DestroyCurrentTask();
    }
}

void BeginTgbtAuthor(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_authSceneCtrl(-8);
    FTBL.EV_SetWeatherInstruction(12, 0);
}

void EndTgbtAuthor(TgbtAuthorWork *work)
{
    FTBL.HLib_DestroyTask(work->task);
}

void PrepareTgbtAuthoredCharacters(void)
{
    int i;
    HideTgbtCharacter(TGBT_PLAYER);
    HideTgbtCharacter(TGBT_FIRST_FIGHTER);
    HideTgbtProp(TGBT_AUTHOR_CHARACTER_0);
    HideTgbtProp(TGBT_AUTHOR_CHARACTER_1);
    HideTgbtProp(TGBT_AUTHOR_CHARACTER_2);
    HideTgbtProp(TGBT_AUTHOR_CHARACTER_3);
    for (i = 0; tgbtHiddenCharacters[i] != TGBT_INVALID; i++)
        HideTgbtCharacter(tgbtHiddenCharacters[i]);
}

void RestoreTgbtAuthoredCharacters(void)
{
    int i;
    ShowTgbtCharacter(TGBT_PLAYER);
    ShowTgbtProp(TGBT_AUTHOR_CHARACTER_0);
    ShowTgbtProp(TGBT_AUTHOR_CHARACTER_1);
    ShowTgbtProp(TGBT_AUTHOR_CHARACTER_2);
    ShowTgbtProp(TGBT_AUTHOR_CHARACTER_3);
    for (i = 0; tgbtHiddenCharacters[i] != TGBT_INVALID; i++)
        ShowTgbtCharacter(tgbtHiddenCharacters[i]);
}

void HideTgbtCharacter(shen_name character)
{
    Vector3 position = {0.0f, 0.0f, 0.0f};
    FTBL.EV_AT_SetPos(character, 0x78000000U, &position);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void ShowTgbtCharacter(shen_name character)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}

void HideTgbtProp(shen_name prop)
{
    Vector3 position = {0.0f, 0.0f, 0.0f};
    FTBL.EV_AT_SetPos(prop, 0x78000000U, &position);
    FTBL.EV_SetCharacterAlphaEnabled(prop, 0);
}

void ShowTgbtProp(shen_name prop)
{
    FTBL.EV_SetCharacterAlphaEnabled(prop, 1);
}
#else
static Vector3 tgbtExitPosition = {242.03f, 118.0f, 147.9f};
static Vector3i tgbtExitRotation = {0, 0, 0};
static short tgbtChoices[] = {-1, 231, 232, -1};

void InitializeTgbtResources(TgbtEventWork *work)
{
    FTBL.EnterDoorSubscene(TGBT_AREA);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0280A000.smp");
    FTBL.EV_ControlPackageResources(2, 4, "TGBT", 0, 0);
    FTBL.SetDoorSubjectEnabled(TGBT_PROP, 0);
    FTBL.EV_LoadAudio("0280A000", 0);
    FTBL.EV_FreeAuthorPak(1, "0280");
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.CreateEventCharacter(3, 0, TGBT_BASE);
    EVT_AT_SetPos(TGBT_BASE, TGBT_INVALID, 244.77f, 118.0f, 153.15f, 0);
    EVT_AT_SetAngY(TGBT_BASE, TGBT_INVALID, 0.0f, 0);
}

void CleanupTgbtEvent(TgbtEventWork *work)
{
    int i;
    for (i = 0; tgbtLoadCharacters[i] != TGBT_INVALID; i++)
        FTBL.npcLockUnlock(tgbtLoadCharacters[i]);
    ET_AT_KillWork(work->characters);
    FTBL.EV_setHoldStatesFlags(1);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.LeaveDoorSubscene(0);
    FTBL.EV_FreeAuthorPak(0, "0280");
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.SetDoorSubjectEnabled(TGBT_PROP, 1);
    FTBL.EV_AT_SetPos(TGBT_PLAYER, 0x78000000U, &tgbtExitPosition);
    FTBL.EV_AT_SetAngY_0(TGBT_PLAYER, 0x78000000U, &tgbtExitRotation, 0);
    FTBL.SetScenarioCharacterKillMode(TGBT_BASE, 1);
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.FinishLoading();
    FTBL.scenFreeEvent("TGBT.EV3");
}

void TgbtEventCallback(void *data)
{
    TgbtEventWork *work = (TgbtEventWork *)data;
    int i;
    if (work->frames) {
        work->frames--;
        return;
    }
    switch (work->state) {
    case 0:
        FTBL.EV_LoadAudio("0280A000", 0);
        FTBL.EV_spmBeginSampleExecute(0);
        work->state = 1;
    case 1:
        if (FTBL.NpcWait())
            break;
        for (i = 0; tgbtLoadCharacters[i] != TGBT_INVALID; i++)
            FTBL.CheckWarmStartNpcRoute(tgbtLoadCharacters[i]);
        work->state = 2;
    case 2:
        FTBL.shenStartFGST(10, 10, 0, 0);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TGBT_PLAYER), 0, 0, 0, work->retries * 100 + 200, 0);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TGBT_FIRST_FIGHTER), 0, 1, 1, 100, 2);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TGBT_FIRST_FIGHTER), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TGBT_FIRST_FIGHTER), 0, 0x200000, 2048);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TGBT_SECOND_FIGHTER), 0, 1, 1, 60, 3);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TGBT_SECOND_FIGHTER), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TGBT_SECOND_FIGHTER), 0, 0x200000, 2048);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TGBT_THIRD_FIGHTER), 0, 1, 1, 60, 4);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TGBT_THIRD_FIGHTER), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TGBT_THIRD_FIGHTER), 0, 0x200000, 2048);
        FTBL.vfCopyPosFromEntity(0, 0, 243.66f, 152.77f, 3.9f);
    case 3:
        FTBL.EV_authSceneCtrl(-11);
        QueueTgbtAuthor(TGBT_AUTHOR_0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state = 4;
        break;
    case 4:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 5;
        break;
    case 5:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 6;
        else if (FTBL.EV_authSceneCtrl(-6) == 849) {
            FTBL.EV_authSceneCtrl(-2);
            work->state = 6;
        }
        break;
    case 6:
        FTBL.SetupControlSpeakAndUI(tgbtChoices, 600, 0xffff, 2);
        FTBL.FinishLoading();
        work->state = 7;
        break;
    case 7:
        if (!FTBL.GetShouldRenderIcons(2)) {
            if (FTBL.EV_authSceneCtrl(-1))
                FTBL.EV_authSceneCtrl(-4);
            FTBL.EV_authSceneCtrl(-10);
            switch (FTBL.shenGetCtrlSpeakIconFlag()) {
            case 231:
                QueueTgbtAuthor(TGBT_AUTHOR_2);
                break;
            case 232:
                QueueTgbtAuthor(TGBT_AUTHOR_3);
                break;
            default:
                QueueTgbtAuthor(TGBT_AUTHOR_1);
                break;
            }
            work->state = 8;
        }
        break;
    case 8:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 9;
        break;
    case 9:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            QueueTgbtAuthor(TGBT_AUTHOR_4);
            work->state = 10;
        }
        break;
    case 10:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 11;
        break;
    case 11:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            FTBL.SetLoadingBusy();
            FTBL.spmFadeOut(0);
            work->state = 12;
            work->frames = 30;
        }
        break;
    case 12:
        FTBL.EV_LoadAudio("0280A000", 1);
        FTBL.AT_SetLocalBaseChara(TGBT_PLAYER, TGBT_BASE);
        FTBL.AT_SetLocalBaseChara(TGBT_FIRST_FIGHTER, TGBT_BASE);
        FTBL.AT_SetLocalBaseChara(TGBT_SECOND_FIGHTER, TGBT_BASE);
        FTBL.AT_SetLocalBaseChara(TGBT_THIRD_FIGHTER, TGBT_BASE);
        EVT_AT_SetPos(TGBT_PLAYER, TGBT_BASE, -1.64f, 0.0f, -1.65f, 0);
        EVT_AT_SetAngY(TGBT_PLAYER, TGBT_BASE, 230.0f, 0);
        EVT_AT_SetPos(TGBT_FIRST_FIGHTER, TGBT_BASE, 1.31f, 0.0f, -0.89f, 0);
        EVT_AT_SetAngY(TGBT_FIRST_FIGHTER, TGBT_BASE, 75.0f, 0);
        EVT_AT_SetPos(TGBT_SECOND_FIGHTER, TGBT_BASE, -1.46f, 0.0f, 0.89f, 0);
        EVT_AT_SetAngY(TGBT_SECOND_FIGHTER, TGBT_BASE, 339.0f, 0);
        EVT_AT_SetPos(TGBT_THIRD_FIGHTER, TGBT_BASE, 0.61f, 0.0f, 0.93f, 0);
        EVT_AT_SetAngY(TGBT_THIRD_FIGHTER, TGBT_BASE, 40.0f, 0);
        EVT_AT_SetMot(TGBT_PLAYER, 0x132, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(TGBT_FIRST_FIGHTER, 0xe30, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(TGBT_SECOND_FIGHTER, 0xe34, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(TGBT_THIRD_FIGHTER, 0xe38, 0.0f, 1.0f, 0x40000000U, 0);
        UnparentAndSetExistingPosRot(TGBT_PLAYER);
        UnparentAndSetExistingPosRot(TGBT_FIRST_FIGHTER);
        UnparentAndSetExistingPosRot(TGBT_SECOND_FIGHTER);
        UnparentAndSetExistingPosRot(TGBT_THIRD_FIGHTER);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_spmBeginSampleExecute(0);
        EVT_AT_SetCam(100, TGBT_PLAYER, TGBT_PLAYER, 0, 0, 0);
        work->state = 13;
        break;
    case 13:
        if (!FTBL.camCtrlCheckIsEventCam()) {
            FTBL.flddSetPointFlagOnGrid(TGBT_MAP_PART, 0);
            FTBL.flddSetPointFlagOnGrid(TGBT_FIELD_PART, 0);
            FTBL.EV_vfBattleUtil(1, 2, 1);
            FTBL.vfmQueueFightStart(4, 99, 1.0f, 0, -1);
            FTBL.EV_vfBattleUtil(4, 0x80000000U, 1);
            EVT_AT_SetAngY(TGBT_AUTHOR_CHARACTER_0, TGBT_INVALID, 0.0f, 0);
            work->state = 14;
        }
        break;
    case 14:
        if (!FTBL.IsFightActive()) {
            if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(TGBT_PLAYER)) == 3)
                work->state = 17;
            else {
                if (++work->retries > 15)
                    work->retries = 15;
                work->state = 15;
            }
            FTBL.spmFadeOut(0);
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            work->frames = 30;
        }
        break;
    case 15:
        FTBL.Queue_ST_LoadTitleFadeIn(TGBT_RETRY_TITLE);
        work->state = 16;
        work->frames = 60;
        break;
    case 16:
        FTBL.ST_LoadTitleFade();
        work->state = 0;
        break;
    case 17:
        FTBL.flddSetPointFlagOnGrid(TGBT_MAP_PART, 1);
        FTBL.flddSetPointFlagOnGrid(TGBT_FIELD_PART, 1);
        QueueTgbtAuthor(TGBT_AUTHOR_5);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state = 18;
        break;
    case 18:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 19;
        break;
    case 19:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 20;
        break;
    case 20:
        EVT_AT_SetSound(1467, 0);
        EVT_AT_SetSound(1468, 30);
        work->frames = 45;
        work->state = 21;
        break;
    case 21:
        FTBL.spmFadeOut(0);
        CleanupTgbtEvent(work);
        break;
    }
}

void UnparentAndSetExistingPosRot(shen_name character)
{
    Vector3 position;
    Vector3i rotation;
    FTBL.GetCharaPosition(character, 0xffffffffU, &position, 0x40000000U);
    FTBL.GetScenarioCharacterEulerAngles(character, &rotation, 0x40000000U);
    FTBL.AT_SetLocalBaseChara(character, 0xffffffffU);
    FTBL.EV_AT_SetPos(character, 0x78000000U, &position);
    FTBL.EV_AT_SetAngY_0(character, 0x78000000U, &rotation, 0);
}
#endif
#else
#include "s2/areas/area_tgbt.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/street_fight_event.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2 {
namespace {
std::int16_t tgbtChoices[]{-1, 231, 232, -1};
}
// @verified - p
void TgbtInit() {
}
// @verified - p
void TgbtDormantBoundary() {
}
// @verified - p
void TgbtMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (DoesScenarioCharacterExist(TGBT_REN)) {
        if (IsNpcLoadedOrVisible(TGBT_REN))
            EV_npcLockUnlock(TGBT_REN);
        EV_npcFree(TGBT_REN);
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TgbtEventCallback, 4, 0x0b, sizeof(TgbtEventWork), TGBT_TASK);
    TgbtEventWork *const work = static_cast<TgbtEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 1;
    work->characters = Queue_EVT_AT_ReadChr(task, TGBT_LOAD_CHARACTERS);
    work->frames = 0;
    work->retries = 0;
    InitializeTgbtResources();
    shenSetClockAndCompassRenderTo(1);
}
// @verified - p
void InitializeTgbtResources() {
    doorLoadAuthorWithAudio(TGBT_PACKAGE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0280A000.smp");
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("TGBT"), 0);
    SetDoorSubjectEnabled(TGBT_PROP, 0);
    EV_LoadAudio("0280A000", 0);
    EV_FreeAuthorPak(1, "0280");
    EV_spmBeginSampleExecute();
    CreateEventCharacter(CharacterCreationHighModel, nullptr, TGBT_BASE);
    EVT_AT_SetPos(TGBT_BASE, TGBT_INVALID, {244.77f, 118.0f, 153.14999f}, 0);
    EVT_AT_SetAngY(TGBT_BASE, TGBT_INVALID, 0, 0);
}
// @verified - p
void QueueTgbtAuthor(shen_name author) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TgbtAuthorCallback, 4, 0x0b, sizeof(TgbtAuthorWork), TGBT_AUTHOR_TASK);
    TgbtAuthorWork *const work = static_cast<TgbtAuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 1;
    SetPlayerControlEnabled(0);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    SetWeatherInstruction(0x0cU, false);
    work->author = author;
}
// @verified - p
void TgbtAuthorCallback(void *data) {
    TgbtAuthorWork *const work = static_cast<TgbtAuthorWork *>(data);
    if (work->state == 0) {
        HLib::DestroyCurrentTask();
    } else if (work->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(TgbtAuthorWaitCallback, 4, 0x0b, sizeof(TgbtAuthorWaitWork), TGBT_AUTHOR_TASK);
        TgbtAuthorWaitWork *const wait = static_cast<TgbtAuthorWaitWork *>(HLib::GetTaskParameter(task));
        wait->parent = work;
        wait->state = 0;
        PrepareTgbtAuthoredCharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->author.name));
        work->state = 2;
    } else if (work->state == 3) {
        HLib::DestroyTask(work->task);
    }
}
// @verified - p
void TgbtAuthorWaitCallback(void *data) {
    TgbtAuthorWaitWork *const work = static_cast<TgbtAuthorWaitWork *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        work->parent->state = 0;
        RestoreTgbtAuthoredCharacters();
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void PrepareTgbtAuthoredCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(TGBT_PLAYER), 6, 2);
    EV_SetCharacterAlphaEnabled(TGBT_PLAYER, false);
    mtwkSetExtraFlag(scenFindMotionwork(TGBT_FIRST_FIGHTER), 6, 2);
    EV_SetCharacterAlphaEnabled(TGBT_FIRST_FIGHTER, false);
    for (const shen_name character : TGBT_AUTHOR_CHARACTERS) {
        EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{});
        EV_SetCharacterAlphaEnabled(character, false);
    }
    for (const shen_name character : TGBT_HIDDEN_CHARACTERS) {
        if (character.name == TGBT_INVALID.name)
            break;
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
        EV_SetCharacterAlphaEnabled(character, false);
    }
}
// @verified - p
void RestoreTgbtAuthoredCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(TGBT_PLAYER), 6, 0);
    EV_SetCharacterAlphaEnabled(TGBT_PLAYER, true);
    for (const shen_name character : TGBT_AUTHOR_CHARACTERS)
        EV_SetCharacterAlphaEnabled(character, true);
    for (const shen_name character : TGBT_HIDDEN_CHARACTERS) {
        if (character.name == TGBT_INVALID.name)
            break;
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
        EV_SetCharacterAlphaEnabled(character, true);
    }
}
// @verified - p
void CleanupTgbtEvent(TgbtEventWork *work) {
    for (const shen_name character : TGBT_LOAD_CHARACTERS) {
        if (character.name == TGBT_INVALID.name)
            break;
        EV_npcLockUnlock(character);
    }
    ET_AT_KillWork(work->characters);
    EV_setHoldStatesFlags(1);
    sbtLoadProgram();
    doorLoadAuthorSilently(shen_name{});
    EV_FreeAuthorPak(0, "0280");
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetDoorSubjectEnabled(TGBT_PROP, 1);
    EV_AT_SetPos(TGBT_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, TGBT_EXIT_POSITION);
    EV_AT_SetAngY(TGBT_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{});
    SetCharaKill(TGBT_BASE, 1);
    shenSetClockAndCompassRenderTo(0);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FinishLoading();
    scenFreeEvent("TGBT.EV3");
}
// @verified - p
void TgbtEventCallback(void *data) {
    TgbtEventWork *const work = static_cast<TgbtEventWork *>(data);
    if (work->frames != 0) {
        --work->frames;
        return;
    }
    switch (work->state) {
    case 0:
        EV_LoadAudio("0280A000", 0);
        EV_spmBeginSampleExecute();
        work->state = 1;
        [[fallthrough]];
    case 1:
        if (NpcWait())
            return;
        for (const shen_name character : TGBT_LOAD_CHARACTERS) {
            if (character.name == TGBT_INVALID.name)
                break;
            EV_CheckWarmStartNpcRoute(character);
        }
        work->state = 2;
        [[fallthrough]];
    case 2:
        shenStartFGST(10, 10, 0, 0);
        shenInitFightData(ctrlGetScenarioChara(TGBT_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100 * (work->retries + 2)), 0);
        shenInitFightData(ctrlGetScenarioChara(TGBT_FIRST_FIGHTER), nullptr, 1, 1, 100, 2);
        vfUpdateFlagsFor(ctrlGetScenarioChara(TGBT_FIRST_FIGHTER), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetScenarioChara(TGBT_FIRST_FIGHTER), 0, 0x200000, 2048);
        shenInitFightData(ctrlGetScenarioChara(TGBT_SECOND_FIGHTER), nullptr, 1, 1, 60, 3);
        vfUpdateFlagsFor(ctrlGetScenarioChara(TGBT_SECOND_FIGHTER), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetScenarioChara(TGBT_SECOND_FIGHTER), 0, 0x200000, 2048);
        shenInitFightData(ctrlGetScenarioChara(TGBT_THIRD_FIGHTER), nullptr, 1, 1, 60, 4);
        vfUpdateFlagsFor(ctrlGetScenarioChara(TGBT_THIRD_FIGHTER), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetScenarioChara(TGBT_THIRD_FIGHTER), 0, 0x200000, 2048);
        vfCopyPosFromEntity(0, nullptr, 243.66000366210938f, 152.77000427246094f, 3.9000000953674316f);
        [[fallthrough]];
    case 3:
        if (work->retries)
            EV_SetAuthorActiveTo(1);
        QueueTgbtAuthor(TGBT_AUTHORS[0]);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state = 4;
        break;
    case 4:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 5;
        return;
    case 5:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 6;
        else if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 849) {
            EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
            work->state = 6;
        }
        if (GetCurrentAuthorId().name == TGBT_AUTHORS[0].name) {
            const std::int64_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
            if (frame == 190)
                snapSetDesc(1);
            else if (frame == 370)
                snapSetDesc(0);
        }
        break;
    case 6:
        SetupControlSpeakAndUI(tgbtChoices, 600, -1, 2);
        FinishLoading();
        work->state = 7;
        return;
    case 7:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
            if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
                EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
            EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            if (work->retries)
                EV_SetAuthorActiveTo(1);
            const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
            if (choice == 231)
                QueueTgbtAuthor(TGBT_AUTHORS[2]);
            else if (choice == 232)
                QueueTgbtAuthor(TGBT_AUTHORS[3]);
            else
                QueueTgbtAuthor(TGBT_AUTHORS[1]);
            work->state = 8;
        }
        return;
    case 8:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 9;
        return;
    case 9:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            if (work->retries)
                EV_SetAuthorActiveTo(1);
            QueueTgbtAuthor(TGBT_AUTHORS[4]);
            work->state = 10;
        }
        return;
    case 10:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 11;
        return;
    case 11:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            TgbtDormantBoundary();
            spmFadeOut();
            work->state = 12;
            work->frames = 30;
        }
        return;
    case 12:
        EV_LoadAudio("0280A000", 1);
        AT_SetLocalBaseChara(TGBT_PLAYER, TGBT_BASE);
        AT_SetLocalBaseChara(TGBT_FIRST_FIGHTER, TGBT_BASE);
        AT_SetLocalBaseChara(TGBT_SECOND_FIGHTER, TGBT_BASE);
        AT_SetLocalBaseChara(TGBT_THIRD_FIGHTER, TGBT_BASE);
        EVT_AT_SetPos(TGBT_PLAYER, TGBT_BASE, TGBT_PLAYER_POSITION, 0);
        EVT_AT_SetAngY(TGBT_PLAYER, TGBT_BASE, 230, 0);
        EVT_AT_SetPos(TGBT_FIRST_FIGHTER, TGBT_BASE, TGBT_FIRST_FIGHTER_POSITION, 0);
        EVT_AT_SetAngY(TGBT_FIRST_FIGHTER, TGBT_BASE, 75, 0);
        EVT_AT_SetPos(TGBT_SECOND_FIGHTER, TGBT_BASE, TGBT_SECOND_FIGHTER_POSITION, 0);
        EVT_AT_SetAngY(TGBT_SECOND_FIGHTER, TGBT_BASE, 339, 0);
        EVT_AT_SetPos(TGBT_THIRD_FIGHTER, TGBT_BASE, TGBT_THIRD_FIGHTER_POSITION, 0);
        EVT_AT_SetAngY(TGBT_THIRD_FIGHTER, TGBT_BASE, 40, 0);
        EVT_AT_SetMot(TGBT_PLAYER, 0x132, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(TGBT_FIRST_FIGHTER, 0xe30, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(TGBT_SECOND_FIGHTER, 0xe34, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(TGBT_THIRD_FIGHTER, 0xe38, 0, 1, 0x40000000U, 0);
        UnparentAndSetExistingPosRot(TGBT_PLAYER);
        UnparentAndSetExistingPosRot(TGBT_FIRST_FIGHTER);
        UnparentAndSetExistingPosRot(TGBT_SECOND_FIGHTER);
        UnparentAndSetExistingPosRot(TGBT_THIRD_FIGHTER);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
        EVT_AT_SetCam(100, TGBT_PLAYER, TGBT_PLAYER, 0, 0, 0);
        camCtrlDisableFilmBars();
        work->state = 13;
        return;
    case 13:
        if (!camCtrlCheckIsEventCam()) {
            flddSetPointFlagOnGrid(TGBT_MAP_PART, 0);
            flddSetPointFlagOnGrid(TGBT_FIELD_PART, 0);
            EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
            vfmQueueFightStart(4, 99, 1, 0, -1);
            EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x80000000U}, 1);
            EVT_AT_SetAngY(TGBT_AUTHOR_CHARACTERS[0], TGBT_INVALID, 0, 0);
            work->state = 14;
        }
        return;
    case 14:
        if (!IsFightActive()) {
            if (GetBtlStatus(ctrlGetScenarioChara(TGBT_PLAYER)) == 3)
                work->state = 17;
            else {
                ++work->retries;
                if (work->retries > 15)
                    work->retries = 15;
                work->state = 15;
            }
            spmFadeOut();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            work->frames = 30;
        }
        return;
    case 15:
        Queue_ST_LoadTitleFadeIn(TGBT_RETRY_TITLE);
        work->state = 16;
        work->frames = 60;
        return;
    case 16:
        ST_LoadTitleFade();
        work->state = 0;
        return;
    case 17:
        flddSetPointFlagOnGrid(TGBT_MAP_PART, 1);
        flddSetPointFlagOnGrid(TGBT_FIELD_PART, 1);
        QueueTgbtAuthor(TGBT_AUTHORS[5]);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state = 18;
        return;
    case 18:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 19;
        return;
    case 19:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 20;
        return;
    case 20:
        Loop3DSoundEffect(0x5bb);
        EVT_AT_SetSound(0x5bc, 30);
        work->frames = 45;
        work->state = 21;
        return;
    case 21:
        spmFadeOut();
        CleanupTgbtEvent(work);
        return;
    default:
        return;
    }
}
const ScriptAreaDefinition tgbtScriptDefinition{TGBT_AREA, 0, TgbtInit, TgbtMain};
} // namespace shenmue::s2
#endif
