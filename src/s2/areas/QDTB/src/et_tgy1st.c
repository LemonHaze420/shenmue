#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_qdtb.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void QdtbMain(void)
{
    QueueQdtbEvent();
}
#elif defined(SHENMUE_EVENT_QDTB_AUTHOR)
const int qdtbTentParts[] = {175, 170, 152, 163, 158, 172, 167, 174, 169, 155, 162, 157, 166, 154, 173, 168, 153, 165, 160, 176, 171, 164, 159, 161, 156,
    192, 193, 180, 181, 182, 189, 190, 191, 186, 187, 188, 183, 184, 185, 177, 178, 179, -1};
shen_name qdtbOpponents[] = {QDTB_TGY, QDTB_YOH, QDTB_RKN, QDTB_III, QDTB_SEE, QDTB_SYE, QDTB_INVALID};

void QueueBoutAuthor(shen_name author)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(BoutAuthorCallback, 4, 11, sizeof(QdtbAuthorWork), QDTB_TASK);
    QdtbAuthorWork *work = (QdtbAuthorWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->state = 1;
    work->author = author;
    DisableBoutAuthorPlayerControl();
}

void BoutAuthorCallback(void *data)
{
    QdtbAuthorWork *work = (QdtbAuthorWork *)data;
    switch (work->state) {
    case 1:
        QueueBoutAuthorLoop(work);
        break;
    case 3:
        DestroyBoutAuthor(work);
        break;
    }
}

void QueueBoutAuthorLoop(QdtbAuthorWork *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(BoutAuthorLoopCallback, 4, 11, sizeof(QdtbAuthorLoopWork), QDTB_TASK);
    QdtbAuthorLoopWork *work = (QdtbAuthorLoopWork *)FTBL.HLib_GetTaskParameter(task);
    work->parent = parent;
    work->frame = 0;
    FTBL.EV_LoadAudio("0249A000", 0);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.DebugLogToFileRemoved("ID : [%c%c%c%c]\n", parent->author & 255, parent->author >> 8 & 255, parent->author >> 16 & 255, parent->author >> 24);
    FTBL.EV_authSceneCtrl(parent->author);
    parent->state = 2;
}

void BoutAuthorLoopCallback(void *data)
{
    QdtbAuthorLoopWork *work = (QdtbAuthorLoopWork *)data;
    work->frame = FTBL.EV_authSceneCtrl(-6);
    switch (work->frame) {
    case 1:
        BeginBoutAuthorCharacters();
        if (work->parent->author == QDTB_AUTHOR_1) {
            FTBL.EV_DisableCharacterDisplay(QDTB_YOH);
            FTBL.EV_DisableCharacterDisplay(QDTB_RKN);
            FTBL.EV_DisableCharacterDisplay(QDTB_III);
            FTBL.EV_DisableCharacterDisplay(QDTB_SEE);
            FTBL.EV_DisableCharacterDisplay(QDTB_SYE);
            FTBL.EV_DisableCharacterDisplay(QDTB_A235);
        } else if (work->parent->author == QDTB_AUTHOR_2) {
            FTBL.EV_DisableCharacterDisplay(QDTB_YOH);
            FTBL.EV_DisableCharacterDisplay(QDTB_RKN);
            FTBL.EV_DisableCharacterDisplay(QDTB_III);
            FTBL.EV_DisableCharacterDisplay(QDTB_SEE);
        }
        if (work->parent->author == QDTB_AUTHOR_1)
            Queue_EVT_AT_PlayDynamix(QDTB_A401, "TJMP", 2.0f, 135.0f, 42, qdtbTentParts, 2.0f, 175);
        else if (work->parent->author == QDTB_AUTHOR_2) {
            FTBL.EV_DisableCharacterDisplay(QDTB_A352);
            FTBL.EV_DisableCharacterDisplay(QDTB_TENT);
        }
        break;
    case 22:
        if (work->parent->author == QDTB_AUTHOR_0)
            FTBL.AssignIMGMParams(FTBL.ctrlGetCharaByName(QDTB_TENT), 1, 1, 1.0f, 0);
        break;
    case 171:
        if (work->parent->author == QDTB_AUTHOR_1) {
            FTBL.EV_EnableCharacterDisplay(QDTB_A235);
            FTBL.EV_DisableCharacterDisplay(QDTB_TENT);
            FTBL.EV_DisableCharacterDisplay(QDTB_A352);
        }
        break;
    case 470:
        if (work->parent->author == QDTB_AUTHOR_0)
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        EndBoutAuthorCharacters();
        work->parent->state = 3;
        FTBL.HLib_DestroyCurrentEventTask();
    }
}

void DisableBoutAuthorPlayerControl(void)
{
    FTBL.SetPlayerControlEnabled(0);
}

void DestroyBoutAuthor(QdtbAuthorWork *work)
{
    FTBL.HLib_DestroyTask(work->task);
}

void BeginBoutAuthorCharacters(void)
{
    int index;
    HideAuthorCharacter(QDTB_RYO);
    HideAuthorProp(QDTB_A235);
    HideAuthorProp(QDTB_A352);
    HideAuthorProp(QDTB_A397);
    HideAuthorProp(QDTB_A398);
    HideAuthorProp(QDTB_A401);
    HideAuthorProp(QDTB_TENT);
    FTBL.LoadBlurModelForCharacter_0(QDTB_RYO, "RYO_B.MT7", 1.0f);
    FTBL.LoadBlurModelForCharacter_0(QDTB_SYE, "SYE_B.MT7", 1.0f);
    FTBL.LoadBlurModelForCharacter_0(QDTB_TGY, "TGY_B.MT7", 1.0f);
    for (index = 0; qdtbOpponents[index] != QDTB_INVALID; index++)
        HideAuthorCharacter(qdtbOpponents[index]);
}

void EndBoutAuthorCharacters(void)
{
    int index;
    ShowAuthorCharacter(QDTB_RYO);
    ShowAuthorProp(QDTB_A235);
    ShowAuthorProp(QDTB_A352);
    ShowAuthorProp(QDTB_A397);
    ShowAuthorProp(QDTB_A398);
    ShowAuthorProp(QDTB_A401);
    for (index = 0; qdtbOpponents[index] != QDTB_INVALID; index++)
        ShowAuthorCharacter(qdtbOpponents[index]);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"
#elif defined(SHENMUE_EVENT_QDTB_SKY)
HLibTask *qdtbSkyFadeTask;

void QueueQdtbSkyFade(unsigned int minutes, int red, int green, int blue)
{
    QdtbSkyFadeWork *work;
    unsigned char index;
    qdtbSkyFadeTask = FTBL.HLib_EnqueueTaskWithParameter(QdtbSkyFadeCallback, 3, 11, sizeof(QdtbSkyFadeWork), QDTB_TASK);
    work = (QdtbSkyFadeWork *)FTBL.HLib_GetTaskParameter(qdtbSkyFadeTask);
    work->state = 0;
    work->task = qdtbSkyFadeTask;
    work->started = 0;
    for (index = 0; index < 3; index++) {
        work->alpha[index] = 0;
        work->red[index] = 0;
        work->green[index] = 0;
        work->blue[index] = 0;
    }
    work->cloud = 0;
    work->sky = 0;
    work->cloudStep[0] = 0.0f;
    work->cloudValue[0] = 0.0f;
    work->cloudStep[1] = 0.0f;
    work->cloudValue[1] = 0.0f;
    work->cloudStep[2] = 0.0f;
    work->cloudValue[2] = 0.0f;
    work->cloudStep[3] = 0.0f;
    work->cloudValue[3] = 0.0f;
    work->skyStep[0] = 0.0f;
    work->skyValue[0] = 0.0f;
    work->skyStep[1] = 0.0f;
    work->skyValue[1] = 0.0f;
    work->skyStep[2] = 0.0f;
    work->skyValue[2] = 0.0f;
    work->skyStep[3] = 0.0f;
    work->skyValue[3] = 0.0f;
    work->frames = minutes * 30;
    work->alpha[2] = 255;
    work->red[2] = red;
    work->green[2] = green;
    work->blue[2] = blue;
}

void QdtbSkyFadeCallback(void *data)
{
    QdtbSkyFadeWork *work = (QdtbSkyFadeWork *)data;
    switch (work->state) {
    case 0:
        BeginQdtbSkyFade(work);
        break;
    case 1:
        UpdateQdtbSkyFade(work);
        break;
    case 2:
        FTBL.HLib_DestroyTaskInternal(work->task);
        break;
    }
}

void BeginQdtbSkyFade(QdtbSkyFadeWork *work)
{
    int cloud, sky;
    FTBL.SetWeatherUpdateEnabledTo(0);
    cloud = FTBL.GetWeatherCloudOrSkyColour(0);
    sky = FTBL.GetWeatherCloudOrSkyColour(1);
    work->alpha[0] = cloud >> 24 & 255;
    work->red[0] = cloud >> 16 & 255;
    work->green[0] = cloud >> 8 & 255;
    work->blue[0] = cloud & 255;
    work->alpha[1] = sky >> 24 & 255;
    work->red[1] = sky >> 16 & 255;
    work->green[1] = sky >> 8 & 255;
    work->blue[1] = sky & 255;
    work->state++;
}

void UpdateQdtbSkyFade(QdtbSkyFadeWork *work)
{
    if (!work->started) {
        float red = work->red[2] * 2.0f, green = work->green[2] * 2.0f, blue = work->blue[2] * 2.0f;
        work->cloudValue[0] = work->alpha[0];
        work->cloudValue[1] = work->red[0];
        work->cloudValue[3] = work->blue[0];
        work->cloudValue[2] = work->green[0];
        work->skyValue[0] = work->alpha[1];
        work->skyValue[1] = work->red[1];
        work->skyValue[2] = work->green[1];
        work->skyValue[3] = work->blue[1];
        work->cloudStep[1] = (red - work->cloudValue[1]) / work->frames;
        work->cloudStep[2] = (green - work->cloudValue[2]) / work->frames;
        work->cloudStep[3] = (blue - work->cloudValue[3]) / work->frames;
        work->skyStep[1] = (red - work->skyValue[1]) / work->frames;
        work->skyStep[2] = (green - work->skyValue[2]) / work->frames;
        work->skyStep[3] = (blue - work->skyValue[3]) / work->frames;
        work->started = 1;
    }
    if (!work->frames) {
        work->alpha[0] = work->alpha[2];
        work->red[0] = work->red[2];
        work->green[0] = work->green[2];
        work->blue[0] = work->blue[2];
        work->alpha[1] = work->alpha[2];
        work->red[1] = work->red[2];
        work->green[1] = work->green[2];
        work->blue[1] = work->blue[2];
        work->state++;
    } else if (work->frames) {
        work->cloudValue[1] += work->cloudStep[1];
        work->cloudValue[2] += work->cloudStep[2];
        work->cloudValue[3] += work->cloudStep[3];
        work->alpha[0] = (int)work->cloudValue[0] << 24;
        work->red[0] = (int)work->cloudValue[1] << 16 & 0xff0000;
        work->green[0] = (int)work->cloudValue[2] << 8 & 0xff00;
        work->blue[0] = (int)work->cloudValue[3] & 0xff;
        work->cloud = work->alpha[0] | work->red[0] | work->green[0] | work->blue[0];
        work->skyValue[0] += work->skyStep[0];
        work->skyValue[1] += work->skyStep[1];
        work->skyValue[2] += work->skyStep[2];
        work->skyValue[3] += work->skyStep[3];
        work->alpha[1] = (int)work->skyValue[0] << 24;
        work->red[1] = (int)work->skyValue[1] << 16 & 0xff0000;
        work->green[1] = (int)work->skyValue[2] << 8 & 0xff00;
        work->blue[1] = (int)work->skyValue[3] & 0xff;
        work->sky = work->alpha[1] | work->red[1] | work->green[1] | work->blue[1];
    }
    FTBL.SetCloudAndSkyColour(work->cloud, work->sky);
    work->frames--;
}
#else
const shen_name qdtbCharacters[] = {QDTB_RYO, QDTB_TGY, QDTB_YOH, QDTB_RKN, QDTB_III, QDTB_SEE, QDTB_SYE, QDTB_INVALID};
Vector3 qdtbRyoPosition = {176.13f, 128.6f, 58.06f};
Vector3 qdtbTgyPosition = {176.1f, 128.6f, 59.47f};
Vector3 qdtbRknPosition = {178.58f, 128.6f, 56.97f};
Vector3 qdtbSeePosition = {173.47f, 128.6f, 57.3f};
Vector3 qdtbYohPosition = {177.01f, 128.6f, 55.52f};
Vector3 qdtbIiiPosition = {175.29f, 128.6f, 55.78f};
Vector3i qdtbRyoRotation = {0, 34588, 0};
Vector3i qdtbTgyRotation = {0, 109, 0};
Vector3i qdtbRknRotation = {0, 15561, 0};
Vector3i qdtbSeeRotation = {0, 49519, 0};
Vector3i qdtbYohRotation = {0, 26460, 0};
Vector3i qdtbIiiRotation = {0, 45219, 0};
unsigned char qdtbGangFightLost = 0;

void QueueQdtbEvent(void)
{
    HLibTask *task;
    QdtbEventWork *work;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    task = FTBL.HLib_EnqueueTaskWithParameter(QdtbEventCallback, 4, 11, sizeof(QdtbEventWork), QDTB_TASK);
    work = (QdtbEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->mode = 0;
    work->reserved10 = 0;
    work->fightFrames = 0;
    work->frames = 0;
    work->fade = 0;
    work->exitFade = 0;
    work->retries = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, qdtbCharacters);
    InitializeQdtbEvent(work);
    EventDebugRegister(QDTB_DEBUG_ID, QdtbDebugExitCallback, work, 36000);
    FTBL.DebugLogToFileRemoved("gw : %x\n", work);
    FTBL.DebugLogToFileRemoved("htask : %x\n", work->task);
    FTBL.DebugLogToFileRemoved("End of initialyze\n");
}

void InitializeQdtbEvent(QdtbEventWork *work)
{
    EventDateTime date;
    if (FTBL.IsFightActive())
        FTBL.check_dead_status(1);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"QDTB", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "QDTB", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "QDTB", 0, 0);
    FTBL.CreateEventCharacter(3, 0, QDTB_BASE);
    FTBL.EV_FreeAuthorPak(1, "0249");
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0249A000");
    FTBL.EV_LoadAudio("0249A000", 0);
    FTBL.SetMapControlStatusFlag(0, 1);
    FTBL.SetWeatherUpdateEnabledTo(1);
    FTBL.RefreshWeatherForCurrentTime(0);
    FTBL.UpdateWeather();
    FTBL.GetCurrentDateTime((DateTime *)&date);
    if (date.hour >= 19 && date.hour <= 24) {
        FTBL.EV_lightLoad(0);
        FTBL.PushGlobalLightingProgram();
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
    }
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(1);
    work->reserved10 = 0;
}

void QdtbDebugExitCallback(void *data)
{
    FinishQdtbEvent((QdtbEventWork *)data);
}

void FinishQdtbEvent(QdtbEventWork *work)
{
    EventDateTime date;
    int index;
    EventDebugUnregister(QDTB_DEBUG_ID);
    ET_AT_KillWork(work->characters);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScenarioCharacterKillMode(QDTB_BASE, 1);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(QDTB_RYO), 71, 0, 0);
    for (index = 0; work->motions[index]; index++)
        FTBL.DeleteMotion(work->motions[index]);
    FTBL.EV_FreeAuthorPak(0, "0249");
    FTBL.EV_ControlPackageResources(0, 6, "QDTB", 0, 0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.HLib_DestroyCurrentTask();
    FTBL.GetCurrentDateTime((DateTime *)&date);
    date.day += 3;
    date.hour = 8;
    date.minute = 0;
    date.second = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
    FTBL.EV_scenQueueLoad(g_actNumber, QDTB_RETURN_AREA, 0);
}

void QdtbEventCallback(void *data)
{
    QdtbEventWork *work = (QdtbEventWork *)data;
    switch (work->mode) {
    case 0:
        work->mode = 3;
        break;
    case 3:
        FTBL.EV_ControlPackageResources(2, 2, "QDTB", 0, 0);
        FTBL.LoadBlurModelForCharacter_0(QDTB_RYO, "RYO_B.MT7", 1.0f);
        FTBL.LoadBlurModelForCharacter_0(QDTB_SYE, "SYE_B.MT7", 1.0f);
        FTBL.LoadBlurModelForCharacter_0(QDTB_TGY, "TGY_B.MT7", 1.0f);
        FTBL.SetLoadingBusy();
        work->thread = ThrCreate(QdtbSequenceThread, QDTB_SEQUENCE, work);
        work->threadActive = 1;
        FTBL.DebugLogToFileRemoved("MODE_MAIN\n");
        work->mode = 4;
        break;
    case 4:
        if (!work->threadActive) {
            FTBL.DebugLogToFileRemoved("MODE_MAIN2\n");
            work->mode = 5;
        }
        break;
    case 5:
        FTBL.DebugLogToFileRemoved("MODE_EXIT\n");
        work->mode = 6;
        break;
    case 6:
        FTBL.FinishLoading();
        FTBL.DebugLogToFileRemoved("MODE_EXIT2\n");
        work->mode = 7;
        break;
    case 7:
        FTBL.EV_stepSetFlags(286, 0);
        FTBL.EV_stepSetFlags(287, 0);
        FinishQdtbEvent(work);
        break;
    default:
        FTBL.DebugLogToFileRemoved("mode error ! : %d\n", work->mode);
        FTBL.EventAssertNullsub(__FILE__, 368);
        break;
    }
}

void QdtbSequenceThread(void *data)
{
    Thr *thr = (Thr *)data;
    QdtbEventWork *work = (QdtbEventWork *)ThrGetWork(thr);
    ThrSetDebugId(ThrGetId(thr));
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            QueueBoutAuthor(QDTB_AUTHOR_0);
            ThrWaitAuthor(thr, 101, 0);
            break;
        case 101:
            ThrMakeChild(thr, QdtbGangFightThread, QDTB_BATTLE, work, 0);
            ThrWaitChildExit(thr, 102, 1, 0);
            break;
        case 102:
            if (!qdtbGangFightLost)
                ThrWaitChildExit(thr, 103, 1, 0);
            else {
                work->retries++;
                ThrWaitChildExit(thr, 0, 1, 0);
            }
            break;
        case 103:
            FTBL.EV_authSceneCtrl(-11);
            QueueBoutAuthor(QDTB_AUTHOR_1);
            ThrWaitAuthor(thr, 104, 0);
            break;
        case 104:
            FTBL.EV_authSceneCtrl(-10);
            FTBL.EV_SetCharaControllerFlags(QDTB_RYO, 1, 0);
            FTBL.EV_SetCharaControllerFlags(QDTB_TGY, 1, 0);
            FTBL.EV_SetCharaControllerFlags(QDTB_RYO, 2, 0);
            FTBL.EV_SetCharaControllerFlags(QDTB_RYO, 3, 0);
            SHENMUE_EVENT_BOUNDARY();
            FTBL.EV_SetCharaControllerFlags(QDTB_TGY, 2, 0);
            FTBL.EV_SetCharaControllerFlags(QDTB_TGY, 3, 0);
            SHENMUE_EVENT_BOUNDARY();
            FTBL.EV_AT_SetPos(QDTB_RYO, 0x78000000, &qdtbRyoPosition);
            FTBL.EV_AT_SetPos(QDTB_TGY, 0x78000000, &qdtbTgyPosition);
            FTBL.EV_AT_SetPos(QDTB_RKN, 0x78000000, &qdtbRknPosition);
            FTBL.EV_AT_SetPos(QDTB_SEE, 0x78000000, &qdtbSeePosition);
            FTBL.EV_AT_SetPos(QDTB_YOH, 0x78000000, &qdtbYohPosition);
            FTBL.EV_AT_SetPos(QDTB_III, 0x78000000, &qdtbIiiPosition);
            FTBL.EV_AT_SetAngY_0(QDTB_RYO, 0x38000000, &qdtbRyoRotation, 0);
            FTBL.EV_AT_SetAngY_0(QDTB_TGY, 0x38000000, &qdtbTgyRotation, 0);
            FTBL.EV_AT_SetAngY_0(QDTB_RKN, 0x38000000, &qdtbRknRotation, 0);
            FTBL.EV_AT_SetAngY_0(QDTB_SEE, 0x38000000, &qdtbSeeRotation, 0);
            FTBL.EV_AT_SetAngY_0(QDTB_YOH, 0x38000000, &qdtbYohRotation, 0);
            FTBL.EV_AT_SetAngY_0(QDTB_III, 0x38000000, &qdtbIiiRotation, 0);
            EVT_AT_SetMot(QDTB_RYO, 306, 0.0f, 1.0f, 0x40000000, 0);
            EVT_AT_SetMot(QDTB_TGY, 2070, 0.0f, 1.0f, 0x40000000, 0);
            EVT_AT_SetMot(QDTB_YOH, 3632, 0.0f, 1.0f, 0x40000000, 0);
            EVT_AT_SetMot(QDTB_III, 3636, 0.0f, 1.0f, 0x40000000, 0);
            EVT_AT_SetMot(QDTB_SEE, 3640, 0.0f, 1.0f, 0x40000000, 0);
            EVT_AT_SetMot(QDTB_RKN, 3644, 0.0f, 1.0f, 0x40000000, 0);
            ThrMakeChild(thr, QdtbTgyFightThread, QDTB_BATTLE, work, 0);
            ThrWaitChildExit(thr, 105, 1, 0);
            break;
        case 105:
            QueueBoutAuthor(QDTB_AUTHOR_2);
            ThrWaitAuthor(thr, 107, 0);
            break;
        case 106:
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(QDTB_RYO), 0);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(QDTB_TGY), 0);
            FTBL.camCtrlSetFovTransitionTo(1);
            break;
        case 107:
            ThrChangeSeq(thr, 110, 1);
            break;
        case 108:
            FTBL.dbgPrintEventMessage("チャイの不意打ちオーサ\n");
            ThrWaitAuthor(thr, 109, 0);
            break;
        case 109:
            ThrWaitFrames(thr, 110, 1, 122);
            break;
        case 110:
            ThrWaitFrames(thr, 111, 1, 1);
            break;
        case 111:
            work->threadActive = 0;
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}

void QdtbGangFightThread(void *data)
{
    Thr *thr = (Thr *)data;
    QdtbEventWork *work = (QdtbEventWork *)ThrGetWork(thr);
    ThrSetDebugId(ThrGetId(thr));
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            EVT_AT_SetCam(100, QDTB_RYO, QDTB_RYO, 0, 0, 0);
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            FTBL.EV_EnableCharacterDisplay(QDTB_RYO);
            FTBL.EV_EnableCharacterDisplay(QDTB_TGY);
            FTBL.EV_EnableCharacterDisplay(QDTB_YOH);
            FTBL.EV_EnableCharacterDisplay(QDTB_RKN);
            FTBL.EV_EnableCharacterDisplay(QDTB_III);
            FTBL.EV_EnableCharacterDisplay(QDTB_SEE);
            FTBL.shenStartFGST(10, 10, 0, 0);
            FTBL.vfmDispatchControl(2, 0, "QDTB", 1);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_RYO), 0, 0, 0, work->retries * 100 + 200, 0);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_YOH), 0, 1, 1, 100, 2);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_III), 0, 1, 1, 100, 3);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_SEE), 0, 1, 1, 80, 4);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_RKN), 0, 1, 1, 80, 5);
            FTBL.vfCopyPosFromEntity(0, 0, 175.6f, 57.9f, 5.0f);
            ThrWaitFrames(thr, 100, 1, 30);
            break;
        case 100:
            work->frames = 30;
            FTBL.EV_vfBattleUtil(3, 2048, 1);
            FTBL.vfmQueueFightStart(5, 2, 1.0f, 1, -1);
            ThrChangeSeq(thr, 101, 1);
            break;
        case 101:
            if (FTBL.IsFightActive()) {
                qdtbGangFightLost = 1;
                if ((unsigned int)FTBL.vfmDispatchControl(19, 0, 0, -1) > 2) {
                    int status = FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QDTB_RYO));
                    if (status == 1) {
                        qdtbGangFightLost = 0;
                        FTBL.EV_vfBattleUtil(9, QDTB_RYO, status);
                        if (!work->fade) {
                            work->fade = 1;
                            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                        }
                        if (--work->frames == 0 && work->fade == 1) {
                            FTBL.check_dead_status(1);
                            ThrChangeSeq(thr, 12, 1);
                        }
                    }
                }
            } else {
                qdtbGangFightLost = 1;
                ThrChangeSeq(thr, 12, 1);
            }
            break;
        case 12:
            if (!FTBL.IsFightActive())
                ThrChangeSeq(thr, 104, 1);
            break;
        case 104:
            if (qdtbGangFightLost == 1)
                FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            ThrWaitFade(thr, 105, 1);
            break;
        case 105:
            FTBL.figpAssignModeFor(QDTB_RYO, 0);
            FTBL.figpAssignModeFor(QDTB_TGY, 0);
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}

void QdtbTgyFightThread(void *data)
{
    Thr *thr = (Thr *)data;
    QdtbEventWork *work = (QdtbEventWork *)ThrGetWork(thr);
    ThrSetDebugId(ThrGetId(thr));
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            EVT_AT_SetCam(101, QDTB_RYO, QDTB_RYO, 0, 0, 0);
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            FTBL.EV_EnableCharacterDisplay(QDTB_RYO);
            FTBL.EV_EnableCharacterDisplay(QDTB_TGY);
            FTBL.EV_EnableCharacterDisplay(QDTB_YOH);
            FTBL.EV_EnableCharacterDisplay(QDTB_III);
            FTBL.EV_EnableCharacterDisplay(QDTB_SEE);
            FTBL.EV_DisableCharacterDisplay(QDTB_RKN);
            FTBL.EV_ToggleCharacterFlag(QDTB_RKN);
            FTBL.EV_DisableCharacterDisplay(QDTB_SYE);
            FTBL.EV_ToggleCharacterFlag(QDTB_SYE);
            FTBL.EV_EnableCharacterDisplay(QDTB_A235);
            FTBL.EV_DisableCharacterDisplay(QDTB_A352);
            FTBL.EV_DisableCharacterDisplay(QDTB_A401);
            FTBL.EV_ToggleCharacterFlag(QDTB_A352);
            FTBL.EV_ToggleCharacterFlag(QDTB_A401);
            FTBL.SetScenarioCharacterRadius(QDTB_A235, 10.0f);
            work->fade = 0;
            work->exitFade = 0;
            work->frames = 360;
            FTBL.shenStartFGST(10, 10, 0, 0);
            FTBL.vfmDispatchControl(2, 0, "QDTB", 1);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_RYO), 0, 0, 0, 0, 0);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_TGY), 0, 1, 1, 2000, 8);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_YOH), 0, 1, 1, 100, 2);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_III), 0, 1, 1, 100, 3);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QDTB_SEE), 0, 1, 1, 80, 4);
            FTBL.vfCopyPosFromEntity(0, 0, 175.6f, 57.9f, 5.0f);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QDTB_RYO), 0, 0x20000, 80);
            ThrWaitFrames(thr, 100, 1, 30);
            break;
        case 100:
            FTBL.EV_vfBattleUtil(3, 2048, 1);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(QDTB_RYO), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(QDTB_TGY), 0);
            FTBL.vfmQueueFightStart(6, 2, 1.0f, 1, -1);
            ThrChangeSeq(thr, 101, 1);
            break;
        case 101:
            if (work->fightFrames++ > 900) {
                if (!work->fade) {
                    FTBL.vfmDispatchControl(10, 0, 0, 1);
                    FTBL.SetScreenFilmFade(360, 0, 0, 0, 0, 185, 35, 20, 0);
                    QueueQdtbSkyFade(12, 35, 20, 0);
                    work->fade = 1;
                }
                if (--work->frames == 0) {
                    work->frames = 210;
                    ThrChangeSeq(thr, 102, 1);
                }
            }
            break;
        case 102:
            if (!work->exitFade) {
                FTBL.SetScreenFilmFade(210, 185, 35, 20, 0, 255, 0, 0, 0);
                work->exitFade = 1;
            }
            if (--work->frames == 0) {
                FTBL.check_dead_status(1);
                FTBL.EV_EnsureCharacterDraw(QDTB_SYE);
                FTBL.SetWeatherUpdateEnabledTo(1);
                ThrChangeSeq(thr, 12, 1);
            } else if (work->frames == 30)
                FTBL.spmFadeOut(0);
            break;
        case 12:
            if (!FTBL.IsFightActive())
                ThrChangeSeq(thr, 104, 1);
            break;
        case 104:
            if (!FTBL.filmCheckFadeTrackColour())
                ThrWaitFade(thr, 105, 1);
            break;
        case 105:
            FTBL.figpAssignModeFor(QDTB_RYO, 0);
            FTBL.figpAssignModeFor(QDTB_TGY, 0);
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
#endif
#endif
