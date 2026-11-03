#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_q7qb.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void StartQ7Qb(void *argument)
{
    QueueQ7Qb();
}
#else
static const shen_name q7qbCharacters[] = {Q7QB_RYO, Q7QB_REN, Q7QB_KK9, Q7QB_KK8, Q7QB_KN1, Q7QB_RIK, Q7QB_SBK, Q7QB_KKA, Q7QB_KN2, Q7QB_KK7, Q7QB_INVALID};

void QueueQ7Qb(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Q7QbStartCallback, 4, 11, sizeof(Q7QbWork), TASK_EVNT);
    Q7QbWork *work = (Q7QbWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    Q7QbSetup(work);
    EventDebugRegister(Q7QB_DEBUG_ID, Q7QbDebugExitCallback, work, 2000000000);
}

void Q7QbDebugExitCallback(void *data)
{
    Q7QbFinish((Q7QbWork *)data);
}

void Q7QbFinish(Q7QbWork *work)
{
    Q7QbCleanup(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    EventDebugUnregister(Q7QB_DEBUG_ID);
    if (g_currentMap == Q7QB_AREA) {
        FTBL.scenSetTaskFlags(6);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

void Q7QbResetSequence(Q7QbSequence *sequence)
{
    sequence->continuation = Q7QbFirstAttackCallback;
    sequence->result.player = Q7QB_RYO;
    sequence->result.firstOpponent = Q7QB_KK9;
    sequence->result.secondOpponent = Q7QB_KK7;
    sequence->result.motionFlags = 0x44000000U;
    sequence->phase = 0;
}

void Q7QbStartCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    Q7QbSequence *sequence = &work->sequence;
    EventDebugCheck(Q7QB_DEBUG_ID, 4);
    Q7QbResetSequence(sequence);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(Q7QB_RYO));
    EVT_AT_SetPos(Q7QB_BASE, Q7QB_INVALID, 198.1f, work->baseHeight, 161.9f, 0);
    EVT_AT_SetAngY(Q7QB_BASE, Q7QB_INVALID, 180.0f, 0);
    EVT_AT_SetPos(Q7QB_RYO, Q7QB_BASE, -1.0f, 0.0f, -0.5f, 0);
    EVT_AT_SetAngY(Q7QB_RYO, Q7QB_BASE, 0.0f, 0);
    EVT_AT_SetPos(Q7QB_REN, Q7QB_BASE, -10000.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_REN, Q7QB_BASE, 0.0f, 0);
    EVT_AT_SetPos(Q7QB_CAMERA_DUMMY, Q7QB_BASE, -2.55f, 0.0f, 5.2f, 0);
    EVT_AT_SetAngY(Q7QB_CAMERA_DUMMY, Q7QB_BASE, -90.0f, 0);
    EVT_AT_SetPos(Q7QB_KK9, Q7QB_BASE, 100.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_KK9, Q7QB_BASE, 0.0f, 0);
    EVT_AT_SetPos(Q7QB_KK7, Q7QB_BASE, 100.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_KK7, Q7QB_BASE, 0.0f, 0);
    EVT_AT_SetPos(Q7QB_KK8, Q7QB_BASE, 100.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_KK8, Q7QB_BASE, 0.0f, 0);
    EVT_AT_SetPos(Q7QB_KKA, Q7QB_BASE, 100.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_KKA, Q7QB_BASE, 0.0f, 0);
    EVT_AT_SetPos(Q7QB_KN1, Q7QB_BASE, 100.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_KN1, Q7QB_BASE, 180.0f, 0);
    EVT_AT_SetPos(Q7QB_KN2, Q7QB_BASE, 100.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_KN2, Q7QB_BASE, 0.0f, 0);
    EVT_AT_SetPos(Q7QB_RIK, Q7QB_BASE, 100.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_RIK, Q7QB_BASE, 180.0f, 0);
    EVT_AT_SetPos(Q7QB_SBK, Q7QB_BASE, 100.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_SBK, Q7QB_BASE, 180.0f, 0);
    FTBL.AT_SetLocalBaseChara(Q7QB_DUMMY, Q7QB_RYO);
    EVT_AT_SetPos(Q7QB_DUMMY, Q7QB_RYO, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(Q7QB_DUMMY, Q7QB_RYO, 0.0f, 0);
    EVT_AT_SetMot(Q7QB_RYO, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(Q7QB_KK9, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(Q7QB_KK7, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(Q7QB_KK8, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(Q7QB_KKA, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(Q7QB_KN1, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(Q7QB_KN2, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(Q7QB_RIK, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(Q7QB_SBK, 13, 0.0f, 1.0f, 0x40000000U, 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KK9), 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KK7), 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KK8), 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KKA), 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KN1), 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KN2), 0);
    FTBL.EV_LoadAudio("0290A000", 1);
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_authSceneCtrl(Q7QB_AUTHOR_INTRO);
    FTBL.HLib_SetCurrentTaskTo(Q7QbWaitIntroCallback);
}

void Q7QbWaitIntroCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    int playing = FTBL.EV_authSceneCtrl(-1);
    if (!playing) {
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(Q7QB_RYO), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(Q7QB_RYO), 1, 1, 0);
        Queue_EVT_AT_CtrlHandUp_1(Q7QB_RYO, 0, (void *)areaLoopHandPoses[Q7QB_QTE_HAND_POSE], 0, playing);
        Queue_EVT_AT_CtrlHandUp_1(Q7QB_RYO, 1, (void *)areaLoopHandPoses[Q7QB_QTE_HAND_POSE], 0, playing);
        EVT_AT_SetFaceLooks(Q7QB_RYO, 1, 0, 1.0f, 0);
        EVT_AT_SetPosAng(Q7QB_KK8, Q7QB_BASE, -1000.0f, 0.0f, 0.0f, 0);
        FTBL.HLib_SetCurrentTaskTo(work->sequence.continuation);
    }
}

void Q7QbFirstAttackCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    Q7QbSequence *sequence = &work->sequence;
    int delay;
    switch (sequence->phase++) {
    case 0:
        sequence->result.firstOpponent = Q7QB_KK9;
        delay = Queue_EVT_AT_ResultQTE(&work->succeeded, &sequence->result, &q7qbQteData[0], 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        if (!work->succeeded) {
            FTBL.HLib_SetCurrentTaskTo(Q7QbEndCallback);
            FTBL.HLib_SetupTaskDelay(60);
        }
        break;
    default:
        sequence->phase = 0;
        sequence->continuation = Q7QbSecondAttackCallback;
        FTBL.HLib_SetCurrentTaskTo(sequence->continuation);
        FTBL.HLib_SetupTaskDelay(15);
        break;
    }
}

void Q7QbSecondAttackCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    Q7QbSequence *sequence = &work->sequence;
    int delay;
    switch (sequence->phase++) {
    case 0:
        sequence->result.firstOpponent = Q7QB_KK7;
        delay = Queue_EVT_AT_ResultQTE(&work->succeeded, &sequence->result, &q7qbQteData[1], 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        if (!work->succeeded) {
            FTBL.HLib_SetCurrentTaskTo(Q7QbEndCallback);
            FTBL.HLib_SetupTaskDelay(60);
        }
        break;
    default:
        sequence->phase = 0;
        sequence->continuation = Q7QbThirdAttackCallback;
        FTBL.HLib_SetCurrentTaskTo(sequence->continuation);
        FTBL.HLib_SetupTaskDelay(30);
        break;
    }
}

void Q7QbThirdAttackCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    Q7QbSequence *sequence = &work->sequence;
    int delay;
    switch (sequence->phase++) {
    case 0:
        sequence->result.firstOpponent = Q7QB_KK8;
        delay = Queue_EVT_AT_ResultQTE(&work->succeeded, &sequence->result, &q7qbQteData[2], 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        if (!work->succeeded) {
            FTBL.HLib_SetCurrentTaskTo(Q7QbEndCallback);
            FTBL.HLib_SetupTaskDelay(60);
        }
        break;
    default:
        sequence->phase = 0;
        sequence->continuation = Q7QbFourthAttackCallback;
        FTBL.HLib_SetCurrentTaskTo(sequence->continuation);
        FTBL.HLib_SetupTaskDelay(30);
        break;
    }
}

void Q7QbFourthAttackCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    Q7QbSequence *sequence = &work->sequence;
    int delay;
    switch (sequence->phase++) {
    case 0:
        sequence->result.firstOpponent = Q7QB_KKA;
        delay = Queue_EVT_AT_ResultQTE(&work->succeeded, &sequence->result, &q7qbQteData[3], 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        if (!work->succeeded) {
            FTBL.HLib_SetCurrentTaskTo(Q7QbEndCallback);
            FTBL.HLib_SetupTaskDelay(60);
        }
        break;
    default:
        sequence->phase = 0;
        sequence->continuation = Q7QbFifthAttackCallback;
        FTBL.HLib_SetCurrentTaskTo(sequence->continuation);
        FTBL.HLib_SetupTaskDelay(30);
        break;
    }
}

void Q7QbFifthAttackCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    Q7QbSequence *sequence = &work->sequence;
    int delay;
    switch (sequence->phase++) {
    case 0:
        sequence->result.firstOpponent = Q7QB_KN1;
        delay = Queue_EVT_AT_ResultQTE(&work->succeeded, &sequence->result, &q7qbQteData[4], 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        if (!work->succeeded) {
            FTBL.HLib_SetCurrentTaskTo(Q7QbEndCallback);
            FTBL.HLib_SetupTaskDelay(60);
        }
        break;
    default:
        sequence->phase = 0;
        sequence->continuation = Q7QbSixthAttackCallback;
        FTBL.HLib_SetCurrentTaskTo(sequence->continuation);
        FTBL.HLib_SetupTaskDelay(60);
        break;
    }
}

void Q7QbSixthAttackCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    Q7QbSequence *sequence = &work->sequence;
    int delay;
    switch (sequence->phase++) {
    case 0:
        sequence->result.firstOpponent = Q7QB_KN1;
        sequence->result.secondOpponent = Q7QB_KN2;
        delay = Queue_EVT_AT_ResultQTE(&work->succeeded, &sequence->result, &q7qbQteData[5], 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        if (!work->succeeded) {
            FTBL.HLib_SetCurrentTaskTo(Q7QbEndCallback);
            FTBL.HLib_SetupTaskDelay(60);
        }
        break;
    default:
        sequence->phase = 0;
        sequence->continuation = Q7QbFinalAttackCallback;
        FTBL.HLib_SetCurrentTaskTo(sequence->continuation);
        FTBL.HLib_SetupTaskDelay(60);
        break;
    }
}

void Q7QbFinalAttackCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    Q7QbSequence *sequence = &work->sequence;
    int delay;
    switch (sequence->phase++) {
    case 0:
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KK9), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KK7), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KK8), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KKA), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KN1), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(Q7QB_KN2), 1);
        sequence->result.firstOpponent = Q7QB_RIK;
        sequence->result.secondOpponent = Q7QB_SBK;
        delay = Queue_EVT_AT_ResultQTE(&work->succeeded, &sequence->result, &q7qbQteData[6], 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        if (work->succeeded) {
            delay = Queue_EVT_AT_ResultQTE(&work->succeeded, &sequence->result, &q7qbQteData[7], 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            FTBL.HLib_SetCurrentTaskTo(Q7QbEndCallback);
            FTBL.HLib_SetupTaskDelay(120);
        }
        break;
    case 2:
        if (!work->succeeded) {
            FTBL.HLib_SetCurrentTaskTo(Q7QbEndCallback);
            FTBL.HLib_SetupTaskDelay(120);
        } else
            FTBL.HLib_SetupTaskDelay(25);
        break;
    case 3:
        FTBL.qteBattleUtil(9, 1, 256, 0, 0);
        EVT_AT_QTE(0, 16, 0, 0, 4);
        break;
    default:
        if (FTBL.qteBattleUtil(4, 0, 0, 0, 0)) {
            if (FTBL.qteBattleUtil(6, 0, 0, 0, 0) == 2) {
                work->succeeded = 1;
                sequence->phase = 0;
                sequence->continuation = Q7QbStartResultAuthorCallback;
                Q7QbBattleSuccess(&sequence->result, 0);
                EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 70);
                FTBL.HLib_SetCurrentTaskTo(Q7QbWaitMotionCallback);
                FTBL.HLib_SetupTaskDelay(100);
            } else {
                work->succeeded = 0;
                Q7QbBattleFailure(&sequence->result, 0);
                FTBL.HLib_SetCurrentTaskTo(Q7QbEndCallback);
                FTBL.HLib_SetupTaskDelay(120);
            }
        }
        break;
    }
}

void Q7QbWaitMotionCallback(void *data)
{
    if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(Q7QB_RYO), 65, 1, 0))
        FTBL.HLib_SetCurrentTaskTo(((Q7QbWork *)data)->sequence.continuation);
}

void Q7QbStartResultAuthorCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    RestoreEventCharacter(work->characters);
    RestoreEventCharacter(work->characters);
    RestoreEventCharacter(work->characters);
    FTBL.EV_authSceneCtrl(Q7QB_AUTHOR_RESULT);
    work->sequence.continuation = Q7QbEndCallback;
    FTBL.HLib_SetCurrentTaskTo(Q7QbWaitResultAuthorCallback);
}

void Q7QbWaitResultAuthorCallback(void *data)
{
    if (!FTBL.EV_authSceneCtrl(-1)) {
        EVT_AT_SetSample(0, 1, 0);
        FTBL.HLib_SetCurrentTaskTo(((Q7QbWork *)data)->sequence.continuation);
        FTBL.HLib_SetupTaskDelay(60);
    }
}

void Q7QbEndCallback(void *data)
{
    Q7QbWork *work = (Q7QbWork *)data;
    int delay;
    if (work->succeeded)
        Q7QbFinish(work);
    else {
        delay = PlayStandingQteRetryVoices(0);
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        EVT_AT_SetSample(0, 1, delay - 60);
        EVT_IncrementQteFailureCount();
        RestoreEventDateTime();
        FTBL.HLib_SetCurrentTaskTo(Q7QbStartCallback);
        FTBL.HLib_SetupTaskDelay(delay);
    }
}

int PlayStandingQteRetryVoices(int delay)
{
    int frames = delay;
    if (FTBL.GetRandomNumberInRange(100.0f) < 50.0f) {
        EVT_AT_SetVoice("S2BVB010", delay + 23);
        EVT_AT_SetVoice("S2BVA010", delay + 92);
        EVT_AT_SetSound(653, delay + 53);
        EVT_AT_SetSound(651, delay + 85);
        EVT_AT_SetSound(652, delay + 115);
        EVT_AT_SetSound(652, delay + 131);
        EVT_AT_SetSound(650, delay + 142);
        EVT_AT_SetSound(649, delay + 158);
        EVT_AT_SetSound(628, delay + 45);
        EVT_AT_SetSound(629, delay + 51);
        EVT_AT_SetSound(649, delay + 58);
        EVT_AT_SetSound(650, delay + 70);
        frames += 225;
    } else {
        EVT_AT_SetVoice("S2BVC007", delay + 21);
        EVT_AT_SetVoice("S2BVC009", delay + 57);
        EVT_AT_SetVoice("S2BVA012", delay + 105);
        EVT_AT_SetSound(649, delay + 62);
        EVT_AT_SetSound(650, delay + 76);
        EVT_AT_SetSound(132, delay + 87);
        EVT_AT_SetSound(653, delay + 98);
        EVT_AT_SetSound(651, delay + 114);
        EVT_AT_SetSound(652, delay + 130);
        EVT_AT_SetSound(649, delay + 140);
        EVT_AT_SetSound(628, delay + 155);
        EVT_AT_SetSound(652, delay + 160);
        EVT_AT_SetSound(650, delay + 187);
        EVT_AT_SetSound(628, delay + 52);
        EVT_AT_SetSound(629, delay + 58);
        EVT_AT_SetSound(132, delay + 127);
        EVT_AT_SetSound(133, delay + 165);
        EVT_AT_SetSound(649, delay + 194);
        EVT_AT_SetSound(628, delay + 160);
        EVT_AT_SetSound(629, delay + 166);
        frames += 260;
    }
    return frames;
}

void Q7QbSetup(Q7QbWork *work)
{
    Vector3 position;
    EVT_SetQteRetryFrames(24);
    SaveEventDateTime();
    FTBL.GetCharaPosition(Q7QB_RYO, -1, &position, 0x40000000);
    work->baseHeight = position.y;
    work->characters = Queue_EVT_AT_ReadChr(work->task, q7qbCharacters);
    work->succeeded = 1;
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0290", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"Q7QB", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "Q7QB", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "Q7QB", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0290");
    FTBL.EV_FreeAuthorPak(1, "S2BV");
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0290A000");
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
}

void Q7QbCleanup(Q7QbWork *work)
{
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(Q7QB_RYO));
    FTBL.qteBattleUtil(5, 0, 0, 0, 0);
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0290");
    FTBL.EV_FreeAuthorPak(0, "S2BV");
    FTBL.EV_ControlPackageResources(0, 6, "Q7QB", 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "0290", 0, 0);
    ET_AT_KillWork(work->characters);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_NPC_NewLoadSw_0(1);
}

const EventQteData q7qbQteData[] = {
    {0x400, 23, 16, 2, 0, 15, 0, Q7QbFirstAttackBegin, Q7QbFirstAttackSuccess, Q7QbFirstAttackFailure},
    {0x100, 23, 16, 2, 0, 8, 0, Q7QbSecondAttackBegin, Q7QbSecondAttackSuccess, Q7QbSecondAttackFailure},
    {0x200, 23, 16, 3, 12, 0, 0, Q7QbThirdAttackBegin, Q7QbThirdAttackSuccess, Q7QbThirdAttackFailure},
    {0x100, 23, 16, 4, 0, 8, 0, Q7QbFourthAttackBegin, Q7QbFourthAttackSuccess, Q7QbFourthAttackFailure},
    {0x800, 23, 16, 0, 0, 5, 0, Q7QbFifthAttackBegin, Q7QbFifthAttackSuccess, Q7QbFifthAttackFailure},
    {0x100, 23, 16, 4, 0, 6, 0, Q7QbSixthAttackBegin, Q7QbSixthAttackSuccess, Q7QbSixthAttackFailure},
    {0x800, 23, 16, 0, 0, 10, 0, Q7QbFinalAttackBegin, Q7QbFinalAttackSuccess, Q7QbFinalAttackFailure},
    {0x100, 14, 14, 0, 0, 0, 0, 0, Q7QbFinalCounterSuccess, Q7QbFinalCounterFailure},
    {0x100, 23, 16, 0, 0, 0, 0, 0, Q7QbBattleSuccess, Q7QbBattleFailure},
};
#endif
#else
#include "s2/areas/area_q7qb.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/load_title.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
namespace {
void Q7QbFirstAttackCallback(void *);
void Q7QbSecondAttackCallback(void *);
void Q7QbThirdAttackCallback(void *);
void Q7QbFourthAttackCallback(void *);
void Q7QbFifthAttackCallback(void *);
void Q7QbSixthAttackCallback(void *);
void Q7QbFinalAttackCallback(void *);
void Q7QbStartCallback(void *);
void Q7QbStartResultAuthorCallback(void *);
void Q7QbWaitResultAuthorCallback(void *);
void Q7QbWaitMotionCallback(void *);
void Q7QbWaitIntroCallback(void *);
void Q7QbEndCallback(void *);

// @verified - p
void Q7QbSetupNullsub() {
}

void Q7QbSetup(Q7QbWork &work) {
    EVT_SetQteRetryFrames(24);
    EVT_SetQteFailureCount(0);
    shenGetCurrentDateTime(eventTemporaryDateTime);
    Vector3 position;
    EV_GetCharaPosition(Q7QB_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    work.baseHeight = position.y;
    work.characters = Queue_EVT_AT_ReadChr(work.task, Q7QB_CHARACTERS);
    work.succeeded = 1;
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("0290"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("Q7QB"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("Q7QB"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("Q7QB"), 0);
    EV_FreeAuthorPak(1, "S2BV");
    EV_FreeAuthorPak(1, "0290");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0290A000");
    shenSetClockAndCompassRenderTo(1);
    Q7QbSetupNullsub();
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
}
void Q7QbStartCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    work.resultParameters = {.player = Q7QB_RYO, .firstOpponent = Q7QB_KK9, .secondOpponent = Q7QB_KK7, .motionFlags = 0x44000000U};
    work.continuation = Q7QbFirstAttackCallback;
    work.phase = 0;
    SetLowQualityHands(ctrlGetScenarioChara(Q7QB_RYO));
    EventSetPosition position{
        .cleanup = 0, .character = Q7QB_BASE, .parent = Q7QB_INVALID, .position = {.x = 198.1f, .y = work.baseHeight, .z = 161.9f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = Q7QB_BASE, .parent = Q7QB_INVALID, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = Q7QB_RYO, .parent = Q7QB_BASE, .position = Q7QB_RYO_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = Q7QB_RYO, .parent = Q7QB_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = Q7QB_REN;
    position.position = Q7QB_REN_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = Q7QB_REN;
    EVT_AT_SetAngY_Callback(&angle);
    position.character = Q7QB_CAMERA_DUMMY;
    position.position = Q7QB_CAMERA_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = Q7QB_CAMERA_DUMMY;
    angle.degrees = -90;
    EVT_AT_SetAngY_Callback(&angle);
    for (shen_name character : Q7QB_HIDDEN_CHARACTERS) {
        position.character = character;
        position.position = Q7QB_HIDDEN_POSITION;
        EVT_AT_SetPosRot(&position);
        angle.character = character;
        angle.degrees = character.name == Q7QB_KN1.name || character.name == Q7QB_RIK.name || character.name == Q7QB_SBK.name ? 180.0f : 0.0f;
        EVT_AT_SetAngY_Callback(&angle);
    }
    AT_SetLocalBaseChara(Q7QB_DUMMY, Q7QB_RYO);
    position = {.cleanup = 0, .character = Q7QB_DUMMY, .parent = Q7QB_RYO, .position = {}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = Q7QB_DUMMY, .parent = Q7QB_RYO, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    for (shen_name character : Q7QB_STANDING_CHARACTERS) {
        EventSetMotion motion{.cleanup = 0, .character = character, .animation = 13, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(13);
    }
    for (shen_name character : Q7QB_PLURAL_CHARACTERS)
        SetPluralControlFor(ctrlGetScenarioChara(character), 0);
    EV_LoadAudio("0290A000", 1);
    EV_spmBeginSampleExecute();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(ATH1)));
    HLib::SetCurrentTaskTo(Q7QbWaitIntroCallback);
}
void Q7QbWaitIntroCallback(void *data) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    Character *character = ctrlGetScenarioChara(Q7QB_RYO);
    SetHighModelXYZ(character, 1, 0, 0);
    SetHighModelXYZ(character, 1, 1, 0);
    ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(ryoQteHandPose), 0);
    ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(ryoQteHandPose), 0);
    SetHighModelAndFaceFlagsForChara(character, static_cast<std::int32_t>(FacialExpressionId::Angry), 0, 1);
    EventSetPositionAngle position{
        .cleanup = 0, .character = Q7QB_KK8, .parent = Q7QB_BASE, .position = {.x = -1000, .y = 0, .z = 0}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    HLib::SetCurrentTaskTo(work.continuation);
}
void Q7QbFirstAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KK9;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[0]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbSecondAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(15);
    }
}
void Q7QbSecondAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KK7;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[1]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbThirdAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(30);
    }
}
void Q7QbThirdAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KK8;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[2]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbFourthAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(30);
    }
}
void Q7QbFourthAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KKA;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[3]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbFifthAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(30);
    }
}
void Q7QbFifthAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KN1;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[4]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbSixthAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(60);
    }
}
void Q7QbSixthAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KN1;
        work.resultParameters.secondOpponent = Q7QB_KN2;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[5]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbFinalAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(60);
    }
}
void Q7QbFinalAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        for (shen_name character : Q7QB_PLURAL_CHARACTERS)
            SetPluralControlFor(ctrlGetScenarioChara(character), 1);
        work.resultParameters.firstOpponent = Q7QB_RIK;
        work.resultParameters.secondOpponent = Q7QB_SBK;
        HLib::SetupTaskDelay(static_cast<std::int32_t>(Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[6])));
    } else if (phase == 1) {
        if (work.succeeded) {
            HLib::SetupTaskDelay(static_cast<std::int32_t>(Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[7])));
        } else {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(120);
        }
    } else if (phase == 2) {
        if (work.succeeded)
            HLib::SetupTaskDelay(25);
        else {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(120);
        }
    } else if (phase == 3) {
        qteBattleUtil(BattleQteMode::SetFirstCommands, 1, 256);
        ReInitElement(UIElement::GameQte);
        std::int16_t maximum = 16;
        if (g_eventQteRetryFrames > 0)
            maximum = static_cast<std::int16_t>(static_cast<std::int16_t>(EVT_GetQteFailureCount()) * ((g_eventQteRetryFrames - 16) / 2) + 16);
        EventQtePromptTaskData prompt{
            .autoCleanupTask = 0, .requiredStatus = 0, .maximumFrames = maximum, .reserved08 = 0, .displayState = 4, .delayFrames = 0};
        EVT_AT_Qte_Callback(&prompt);
    } else if (qteBattleUtil(BattleQteMode::GetResult)) {
        if (qteBattleUtil(BattleQteMode::GetCompletedCount) == 2) {
            work.succeeded = 1;
            work.phase = 0;
            work.continuation = Q7QbStartResultAuthorCallback;
            Q7QbBattleSuccess(&work.resultParameters, 0);
            HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(EventFilmControl), Q7QB_TASK);
            RegisterEventTask(task);
            EventFilmControl *film = static_cast<EventFilmControl *>(HLib::GetTaskParameter(task));
            *film = {.cleanup = 1,
                .frames = 30,
                .startAlpha = 0,
                .startRed = 0,
                .startGreen = 0,
                .startBlue = 0,
                .targetAlpha = 255,
                .targetRed = 0,
                .targetGreen = 0,
                .targetBlue = 0,
                .delay = 70};
            HLib::SetCurrentTaskTo(Q7QbWaitMotionCallback);
            HLib::SetupTaskDelay(100);
        } else {
            work.succeeded = 0;
            Q7QbFinalCounterFailure(&work.resultParameters, 0);
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(120);
        }
    }
}
void Q7QbWaitMotionCallback(void *data) {
    if (mtwkSetRegionFlag(scenFindMotionwork(Q7QB_RYO), 65, 1, MotionRegion::LowerBody))
        HLib::SetCurrentTaskTo(static_cast<Q7QbWork *>(data)->continuation);
}
void Q7QbStartResultAuthorCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    RestoreEventCharacter(work.characters);
    RestoreEventCharacter(work.characters);
    RestoreEventCharacter(work.characters);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(ATH2)));
    work.continuation = Q7QbEndCallback;
    HLib::SetCurrentTaskTo(Q7QbWaitResultAuthorCallback);
}
void Q7QbWaitResultAuthorCallback(void *data) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(static_cast<Q7QbWork *>(data)->continuation);
        HLib::SetupTaskDelay(60);
    }
}
void Q7QbCleanup(Q7QbWork &work) {
    hndlSetFlags(ctrlGetScenarioChara(Q7QB_RYO));
    qteBattleUtil(BattleQteMode::RetireAndFree);
    FinishLoading();
    shenSetClockAndCompassRenderTo(0);
    sbtLoadProgram();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0290");
    EV_FreeAuthorPak(0, "S2BV");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("Q7QB"), 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0290"), 0);
    ET_AT_KillWork(work.characters);
    EventFilmControl film{.cleanup = 0,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0};
    EVT_AT_MenCtrl_Callback(&film);
    EV_NPC_NewLoadSw(1);
}
void Q7QbEndCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    if (work.succeeded) {
        Q7QbCleanup(work);
        EV_CleanupMapControlTask();
        HLib::EV_HLKillOtherHierarchyTask(work.task);
        if (g_currentMap.name == Q7QB_AREA.name) {
            scenSetTaskFlags(6);
            EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        }
    } else {
        const std::int32_t delay = PlayStandingQteRetryVoices();
        EventFilmControl film{.cleanup = 0,
            .frames = 30,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 0};
        EVT_AT_MenCtrl_Callback(&film);
        EVT_AT_FadeBGM(0, 1, delay - 60);
        if (EVT_GetQteFailureCount() < 2)
            EVT_SetQteFailureCount(EVT_GetQteFailureCount() + 1);
        SetCurrentDateTimeTo(eventTemporaryDateTime);
        shenSetTimeSimulationTo(1);
        HLib::SetCurrentTaskTo(Q7QbStartCallback);
        HLib::SetupTaskDelay(delay);
    }
}
} // namespace
std::int32_t PlayStandingQteRetryVoices() {
    if (GetRandomNumberInRange(100) >= 50) {
        EVT_AT_SetVoice("S2BVC007", 1, 21);
        EVT_AT_SetVoice("S2BVC009", 1, 57);
        EVT_AT_SetVoice("S2BVA012", 1, 105);
        EVT_AT_SetSound(649, 62);
        EVT_AT_SetSound(650, 76);
        EVT_AT_SetSound(132, 87);
        EVT_AT_SetSound(653, 98);
        EVT_AT_SetSound(651, 114);
        EVT_AT_SetSound(652, 130);
        EVT_AT_SetSound(649, 140);
        EVT_AT_SetSound(628, 155);
        EVT_AT_SetSound(652, 160);
        EVT_AT_SetSound(650, 187);
        EVT_AT_SetSound(628, 52);
        EVT_AT_SetSound(629, 58);
        EVT_AT_SetSound(132, 127);
        EVT_AT_SetSound(133, 165);
        EVT_AT_SetSound(649, 194);
        EVT_AT_SetSound(628, 160);
        EVT_AT_SetSound(629, 166);
        return 260;
    }
    EVT_AT_SetVoice("S2BVB010", 1, 23);
    EVT_AT_SetVoice("S2BVA010", 1, 92);
    EVT_AT_SetSound(653, 53);
    EVT_AT_SetSound(651, 85);
    EVT_AT_SetSound(652, 115);
    EVT_AT_SetSound(652, 131);
    EVT_AT_SetSound(650, 142);
    EVT_AT_SetSound(649, 158);
    EVT_AT_SetSound(628, 45);
    EVT_AT_SetSound(629, 51);
    EVT_AT_SetSound(649, 58);
    EVT_AT_SetSound(650, 70);
    return 225;
}
// @verified - p
void InitializeQ7QbEvent() {
}
void RunQ7QbEvent(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Q7QbStartCallback, 4, 0x0b, sizeof(Q7QbWork), Q7QB_TASK);
    Q7QbWork *work = static_cast<Q7QbWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    Q7QbSetup(*work);
}
const ScriptAreaDefinition q7qbScriptDefinition{.areaId = Q7QB_AREA, .field04 = 0, .init = InitializeQ7QbEvent, .main = RunQ7QbEvent};
} // namespace shenmue::s2::areas
#endif
