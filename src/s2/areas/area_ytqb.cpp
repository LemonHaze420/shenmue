#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_ytqb.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void YtqbMain(void *argument)
{
    QueueYtqb();
}
#else
const shen_name ytqbFirstNames[] = {SHEN_NAME4('S', 'B', 'O', '_'), SHEN_NAME4('Y', 'O', 'H', '_'), SHEN_NAME4('S', 'C', 'X', '_'),
                                    SHEN_NAME4('E', 'S', 'Y', '_'), SHEN_NAME4('G', 'E', 'N', '_'), YTQB_CHO, YTQB_INVALID};
const shen_name ytqbSecondNames[] = {SHEN_NAME4('S', 'B', 'K', '_'), SHEN_NAME4('V', 'U', 'N', '_'), SHEN_NAME4('I', 'K', 'O', '_'), YTQB_INVALID};
const shen_name ytqbWaterNames[] = {YTQB_KIC, SHEN_NAME4('R', 'I', 'K', '_'), SHEN_NAME4('T', 'H', 'G', '_'), SHEN_NAME4('S', 'O', 'Y', '_'), YTQB_INVALID};
const shen_name ytqbThirdNames[] = {SHEN_NAME4('K', 'T', '2', '_'), SHEN_NAME4('R', 'N', 'T', '_'), SHEN_NAME4('K', 'T', '3', '_'), YTQB_INVALID};
const shen_name ytqbLastNames[] = {SHEN_NAME4('E', 'H', 'Y', '_'), SHEN_NAME4('A', 'N', 'C', '_'), SHEN_NAME4('K', 'T', '5', '_'),
                                   SHEN_NAME4('C', 'J', 'U', '_'), SHEN_NAME4('K', 'T', '7', '_'), YTQB_INVALID};
const shen_name ytqbFinalNames[] = {SHEN_NAME4('M', 'R', 'C', '_'), SHEN_NAME4('S', 'E', 'E', '_'), SHEN_NAME4('I', 'I', 'I', '_'),
                                    SHEN_NAME4('R', 'K', 'N', '_'), SHEN_NAME4('E', 'S', 'Y', '_'), SHEN_NAME4('G', 'E', 'N', '_'),
                                    YTQB_CHO, YTQB_SYE, SHEN_NAME4('Y', 'U', 'A', '_'), YTQB_INVALID};
const shen_name ytqbPlayerNames[] = {YTQB_RYO, YTQB_INVALID};
int ytqbAttackState = 0;
Vector3 ytqbBucketClingPosition = {-0.433791f, -2.32006f, -0.176954f};
Vector3i ytqbBucketClingRotation = {-364, 22573, -2730};

void QueueYtqb(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(YtqbStart, 4, 11, sizeof(YtqbWork), TASK_EVNT);
    YtqbWork *work = (YtqbWork *)FTBL.HLib_GetTaskParameter(task);
    HLibTask *keeper = FTBL.HLib_EnqueueTaskWithParameter(YtqbKeeperCallback, 3, 11, sizeof(YtqbWork *), TASK_EVNT);
    *(YtqbWork **)FTBL.HLib_GetTaskParameter(keeper) = work;
    work->keeper = keeper;
    work->task = task;
    InitializeYtqb(work);
    EventDebugRegister(YTQB_DEBUG_ID, YtqbDebugExitCallback, work, 2000000000);
}

void YtqbDebugExitCallback(void *data)
{
    YtqbExit((YtqbWork *)data);
}

void YtqbExit(YtqbWork *work)
{
    CleanupYtqb(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->keeper);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    EventDebugUnregister(YTQB_DEBUG_ID);
    if (g_currentMap == YTQB_AREA) {
        FTBL.scenSetTaskFlags(8);
        FTBL.EV_scenQueueLoad(g_actNumber, YTQB_RETURN_AREA, 2);
    }
}

void YtqbKeeperCallback(void *data)
{
}

void YtqbResetSequence(YtqbSequence *sequence)
{
    sequence->continuation = YtqbSboAttack;
    sequence->parameters.player = YTQB_RYO;
    sequence->parameters.firstOpponent = YTQB_INVALID;
    sequence->parameters.secondOpponent = YTQB_INVALID;
    sequence->parameters.motionFlags = 0;
    sequence->phase = 0;
    sequence->result = 0;
    sequence->prompt = 0;
    sequence->authored = 0;
}

int YtqbPrompt(YtqbSequence *sequence, int index, int delay)
{
    sequence->prompt = &ytqbPromptQte[index];
    sequence->authored = &ytqbAuthoredQte[index];
    return Queue_EVT_AT_ResultQTE(&sequence->result, &sequence->parameters, sequence->prompt, delay);
}

void YtqbApplyResult(YtqbSequence *sequence)
{
    EventQteAuthoredCallback success = sequence->authored->resultSuccess;
    EventQteAuthoredCallback failure = sequence->authored->resultFailure;
    if (sequence->result) {
        if (success)
            success(&sequence->parameters, 0);
    } else if (failure)
        failure(&sequence->parameters, 0);
}

void YtqbStart(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    if (!work->npcs)
        work->npcs = QueueEventNpcRead();
    else
        ReleaseEventNpcRequests(work->npcs);
    RequestEventNpcCharacters(work->npcs, ytqbFirstNames);
    FTBL.AT_SetLocalBaseChara(YTQB_DUMR, YTQB_RYO);
    EVT_AT_SetPos(YTQB_DUMR, YTQB_RYO, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(YTQB_DUMR, YTQB_RYO, 0.0f, 0);
    EVT_AT_SetPos(YTQB_BASE, YTQB_INVALID, 180.6f, 102.25f, 198.2f, 0);
    EVT_AT_SetAngY(YTQB_BASE, YTQB_INVALID, 0.0f, 0);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(YTQB_RYO), 8, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(YTQB_RYO));
    YtqbResetSequence(sequence);
    YtqbSetupLantern(YTQB_CAG0, YTQB_INVALID);
    YtqbSetupWater(YTQB_SUI0, YTQB_SUI1);
    YtqbSetupBucket(YTQB_KBK0, YTQB_KBK1);
    YtqbSetupStallObjects(YTQB_YAD1, YTQB_YAT1);
    YtqbSetupNearStall(YTQB_YAD3, YTQB_YAT3);
    YtqbSetupFrontStall(YTQB_YAD2, YTQB_YAT2);
    FTBL.HLib_SetCurrentTaskTo(YtqbStartIntroAuthor);
}

void YtqbStartIntroAuthor(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    if (!IsEventNpcReadBusy(work->npcs)) {
        EventDebugCheck(YTQB_DEBUG_ID, 4);
        FTBL.EV_LoadAudio("0048A000", 1);
        EVT_AT_SetSample(0, 0, 0);
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_lightLoad(SHEN_NAME4('l', '2', '0', '1'));
        FTBL.EV_authSceneCtrl(SHEN_NAME4('A', 'T', 'H', '1'));
        work->sequence.continuation = YtqbSboAttack;
        FTBL.HLib_SetCurrentTaskTo(YtqbSboAttack);
    }
}

void YtqbStartWaterAuthor(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    if (!IsEventNpcReadBusy(work->npcs)) {
        FTBL.EV_authSceneCtrl(-8);
        FTBL.EV_authSceneCtrl(-11);
        FTBL.EV_authSceneCtrl(SHEN_NAME4('S', 'T', 'R', '0'));
        FTBL.camCtrlSetNearDistance(12.0f);
        Queue_EVT_AT_Cling(YTQB_SUI2, YTQB_KIC, 14, &ytqbBucketClingPosition, &ytqbBucketClingRotation, 0, 0);
        EVT_AT_CharaDisp(YTQB_SUI2, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(YtqbWaitWaterAuthor);
    }
}

void YtqbWaitWaterAuthor(void *data)
{
    int frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == 1)
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(YTQB_RYO), frame);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.EV_authSceneCtrl(-10);
        FTBL.EV_authSceneCtrl(-7);
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 0, (void *)areaLoopHandPoses[YTQB_POSE_GUARD], 0, 0);
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 1, (void *)areaLoopHandPoses[YTQB_POSE_GUARD], 0, 0);
        EVT_AT_SetFaceLooks(YTQB_RYO, 1, 1, 1.0f, 0);
        FTBL.qteBattleUtil(9, 16, 16, 1024, 0);
        EVT_AT_QTE(0, 24, 0, 0, 4);
        FTBL.HLib_SetCurrentTaskTo(YtqbWaterResult);
    }
}

void YtqbWaterResult(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    int delay;
    if (FTBL.qteBattleUtil(4, 0, 0, 0, 0)) {
        Queue_EVT_AT_Cling(YTQB_SUI2, 0, 0, 0, 0, 0, 1);
        EVT_AT_CharaDisp(YTQB_SUI2, 0, 1);
        YtqbResetWater(YTQB_SUI0, YTQB_SUI1);
        if (FTBL.qteBattleUtil(6, 0, 0, 0, 0) == 3)
            delay = YtqbWaterSuccess();
        else
            delay = YtqbWaterFailure();
        RetireEventNpcRequests(work->npcs);
        FTBL.HLib_SetCurrentTaskTo(sequence->continuation);
        FTBL.HLib_SetupTaskDelay(delay);
    }
}

void YtqbStartFinalAuthor(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    if (!IsEventNpcReadBusy(work->npcs)) {
        FTBL.LoadBlurModelForCharacter_0(YTQB_SYE, "SYE_B.MT7", 1.0f);
        EVT_AT_SetSample(0, 0, 0);
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_lightLoad(SHEN_NAME4('l', '3', '0', '1'));
        FTBL.EV_authSceneCtrl(-11);
        FTBL.EV_authSceneCtrl(SHEN_NAME4('A', 'T', 'H', '3'));
        FTBL.HLib_SetCurrentTaskTo(YtqbWaitFinalAuthor);
    }
}

void YtqbWaitFinalAuthor(void *data)
{
    int index;
    if (FTBL.EV_authSceneCtrl(-6) == 1)
        for (index = 0; index < 6; index++)
            FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ytqbFinalNames[index]), 1);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.EV_authSceneCtrl(-10);
        FTBL.qteBattleUtil(9, 16, 4, 256, 0);
        EVT_AT_QTE(0, 24, 0, 0, 4);
        FTBL.HLib_SetCurrentTaskTo(YtqbFinalResult);
    }
}

void YtqbFinalResult(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    if (FTBL.qteBattleUtil(4, 0, 0, 0, 0)) {
        if (FTBL.qteBattleUtil(6, 0, 0, 0, 0) == 3) {
            FTBL.EV_lightLoad(SHEN_NAME4('l', '4', '0', '1'));
            FTBL.EV_authSceneCtrl(SHEN_NAME4('A', 'T', 'H', '4'));
            work->sequence.continuation = YtqbFinish;
        } else {
            FTBL.EV_lightLoad(SHEN_NAME4('l', '5', '0', '1'));
            FTBL.EV_authSceneCtrl(SHEN_NAME4('A', 'T', 'H', '5'));
            EVT_IncrementQteFailureCount();
            RestoreEventDateTime();
            sequence->continuation = YtqbStartFinalAuthor;
        }
        FTBL.HLib_SetCurrentTaskTo(YtqbWaitResultAuthor);
    }
}

void YtqbWaitResultAuthor(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    int index;
    if (FTBL.EV_authSceneCtrl(-6) == 1)
        for (index = 0; index < 6; index++)
            FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(ytqbFinalNames[index]), 1);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        EVT_AT_SetSample(0, 1, 0);
        FTBL.HLib_SetCurrentTaskTo(sequence->continuation);
        FTBL.HLib_SetupTaskDelay(60);
    }
}

void YtqbFinish(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    if (!IsEventNpcReadBusy(work->npcs)) {
        ReleaseEventNpcRequests(work->npcs);
        YtqbExit(work);
    }
}

void YtqbSboAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    if (FTBL.EV_authSceneCtrl(-1))
        return;
    switch (sequence->phase++) {
    case 0:
        RequestEventNpcCharacters(work->npcs, ytqbSecondNames);
        parameters->firstOpponent = ytqbFirstNames[0];
        parameters->motionFlags = 0x4000000;
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(YTQB_CHO), 0, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(YTQB_CHO), 0, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(YTQB_RYO), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(YTQB_RYO), 1, 1, 0);
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 0, (void *)areaLoopHandPoses[YTQB_POSE_GUARD], 0, 0);
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 1, (void *)areaLoopHandPoses[YTQB_POSE_GUARD], 0, 0);
        EVT_AT_SetFaceLooks(YTQB_RYO, 1, 1, 1.0f, 0);
        EVT_AT_SetPos(YTQB_RYO, YTQB_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(YTQB_RYO, YTQB_BASE, 0.0f, 0);
        EVT_AT_SetPos(ytqbFirstNames[1], YTQB_BASE, -1.36f, 0.0f, -3.79f, 0);
        EVT_AT_SetAngY(ytqbFirstNames[1], YTQB_BASE, 180.0f, 0);
        EVT_AT_SetPos(ytqbFirstNames[2], YTQB_BASE, 0.736f, 0.0f, -3.66f, 0);
        EVT_AT_SetAngY(ytqbFirstNames[2], YTQB_BASE, 180.0f, 0);
        EVT_AT_SetMot(YTQB_RYO, 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbFirstNames[0], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbFirstNames[1], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbFirstNames[2], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetCam(7905, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 0, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7906, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 1, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7932, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbYohAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbYohAttack);
            FTBL.HLib_SetupTaskDelay(40);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbKnockdown(-70.0f, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7908, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbYohAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbYohAttack);
            FTBL.HLib_SetupTaskDelay(40);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbYohAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbFirstNames[1];
        EVT_AT_SetCam(7907, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        EVT_AT_SetPos(parameters->firstOpponent, parameters->player, -0.00079f, 0.0f, -2.30879f, 0);
        EVT_AT_SetAngY(parameters->firstOpponent, parameters->player, 180.0f, 0);
        delay = YtqbPrompt(sequence, 11, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7905, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 12, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7909, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbScxAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbScxAttack);
            FTBL.HLib_SetupTaskDelay(30);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbKnockdown(-70.0f, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7908, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbScxAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbScxAttack);
            FTBL.HLib_SetupTaskDelay(70);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbScxAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbFirstNames[2];
        EVT_AT_SetCam(7900, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 5, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        if (!sequence->result) {
            YtqbApplyResult(sequence);
            EVT_AT_SetCam(7906, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 6, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetPos(YTQB_RYO, YTQB_BASE, -0.68f, 0.0f, -0.53f, 0);
            EVT_AT_SetAngY(YTQB_RYO, YTQB_BASE, 0.0f, 0);
            YtqbApplyResult(sequence);
            EVT_AT_SetCam(7903, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            EVT_AT_SetCam(7930, YTQB_DUMR, YTQB_DUMR, 0, 0, 30);
            EVT_AT_SetCamRot(0.2f, 0.4f, 0.7f, 37);
            YtqbBreakBirdStall(YTQB_YAD1, YTQB_YAT1, 37);
            sequence->continuation = YtqbSbkAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbNextQte);
            FTBL.HLib_SetupTaskDelay(77);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbKnockdown(-80.0f, 15);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7905, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbSbkAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbNextQte);
            FTBL.HLib_SetupTaskDelay(98);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbSbkAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        RequestEventNpcCharacters(work->npcs, ytqbWaterNames);
        parameters->firstOpponent = ytqbSecondNames[0];
        parameters->motionFlags = 0x24000000;
        EVT_AT_SetPos(YTQB_RYO, YTQB_BASE, 2.0f, 0.0f, -10.5f, 0);
        EVT_AT_SetAngY(YTQB_RYO, YTQB_BASE, -45.0f, 0);
        EVT_AT_SetPos(ytqbSecondNames[1], YTQB_BASE, 4.65f, 0.0f, -12.5f, 0);
        EVT_AT_SetAngY(ytqbSecondNames[1], YTQB_BASE, 135.0f, 0);
        EVT_AT_SetPos(ytqbSecondNames[2], YTQB_BASE, 3.14f, 0.0f, -13.5f, 0);
        EVT_AT_SetAngY(ytqbSecondNames[2], YTQB_BASE, 135.0f, 0);
        EVT_AT_SetMot(YTQB_RYO, 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbSecondNames[0], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbSecondNames[1], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbSecondNames[2], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_CharaDisp(ytqbSecondNames[0], 0, 0);
        EVT_AT_CharaDisp(ytqbSecondNames[1], 0, 0);
        EVT_AT_SetCam(7911, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        EVT_AT_SetPos(parameters->firstOpponent, parameters->player, 0.00079f, 0.0f, -2.30879f, 0);
        EVT_AT_SetAngY(parameters->firstOpponent, parameters->player, 180.0f, 0);
        delay = YtqbPrompt(sequence, 11, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7945, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 12, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7939, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbVunAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbVunAttack);
            FTBL.HLib_SetupTaskDelay(30);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbKnockdown(70.0f, 0);
            EVT_AT_SetCam(7946, YTQB_DUMR, YTQB_DUMR, 0, (int)0x80000000U, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7907, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbVunAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbVunAttack);
            FTBL.HLib_SetupTaskDelay(70);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbVunAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbSecondNames[1];
        parameters->motionFlags = 0x4000000;
        EVT_AT_SetCam(7912, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 0, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7906, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 1, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7940, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbIkoAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbIkoAttack);
            FTBL.HLib_SetupTaskDelay(40);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_CharaDisp(ytqbSecondNames[0], 0, 1);
            YtqbKnockdown(-60.0f, 0);
            EVT_AT_SetCam(7905, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7908, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbIkoAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbIkoAttack);
            FTBL.HLib_SetupTaskDelay(40);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbIkoAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbSecondNames[2];
        parameters->motionFlags = 0x24000000;
        EVT_AT_SetCam(7900, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 17, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        if (!sequence->result) {
            YtqbApplyResult(sequence);
            EVT_AT_SetCam(7908, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 18, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetPos(YTQB_RYO, YTQB_BASE, 3.7f, 0.0f, -12.1f, 0);
            EVT_AT_SetAngY(YTQB_RYO, YTQB_BASE, -45.0f, 0);
            YtqbApplyResult(sequence);
            EVT_AT_SetCam(7914, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            YtqbBreakNearStall(YTQB_YAD3, YTQB_YAT3, 20);
            sequence->continuation = YtqbStartSecondGroup;
            FTBL.HLib_SetCurrentTaskTo(YtqbStartWaterAuthor);
            FTBL.HLib_SetupTaskDelay(43);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_CharaDisp(ytqbSecondNames[1], 0, 1);
            YtqbKnockdown(-65.0f, 15);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7905, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbStartSecondGroup;
            FTBL.HLib_SetCurrentTaskTo(YtqbStartWaterAuthor);
            FTBL.HLib_SetupTaskDelay(73);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbStartSecondGroup(void *data)
{
    YtqbWork *work;
    ytqbAttackState = 0;
    work = (YtqbWork *)data;
    work->sequence.phase = 0;
    SHENMUE_EVENT_BOUNDARY();
    work->sequence.continuation = YtqbRikThgAttack;
    FTBL.HLib_SetCurrentTaskTo(YtqbRikThgAttack);
}

void YtqbRikThgAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        RequestEventNpcCharacters(work->npcs, ytqbThirdNames);
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 0, (void *)areaLoopHandPoses[YTQB_POSE_OPEN], 30, 0);
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 1, (void *)areaLoopHandPoses[YTQB_POSE_OPEN], 30, 0);
        parameters->firstOpponent = ytqbWaterNames[1];
        parameters->secondOpponent = ytqbWaterNames[2];
        parameters->motionFlags = 0x24000000;
        EVT_AT_SetPos(YTQB_RYO, YTQB_BASE, 7.0f, 0.0f, -16.3f, 0);
        EVT_AT_SetAngY(YTQB_RYO, YTQB_BASE, -45.0f, 0);
        EVT_AT_SetPos(ytqbWaterNames[0], parameters->player, -0.15f, 0.0f, -3.6f, 0);
        EVT_AT_SetAngY(ytqbWaterNames[0], parameters->player, 180.0f, 0);
        EVT_AT_SetPos(ytqbWaterNames[3], parameters->player, 0.0f, 0.0f, 2.0f, 0);
        EVT_AT_SetAngY(ytqbWaterNames[3], parameters->player, 0.0f, 0);
        EVT_AT_SetMot(YTQB_RYO, 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbWaterNames[0], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbWaterNames[1], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbWaterNames[2], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbWaterNames[3], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetCam(7900, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 7, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbBackKnockdown(-70.0f, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7916, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 8, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbSideKnockdown(0.0f, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7917, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbSoyAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbSoyAttack);
            FTBL.HLib_SetupTaskDelay(50);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbSoyAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbWaterNames[3];
        parameters->motionFlags = 0x4000000;
        EVT_AT_SetPosAng(ytqbWaterNames[1], YTQB_INVALID, 189.5f, 182.7f, 45.0f, 0);
        EVT_AT_SetCam(7918, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 2, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7920, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            FTBL.HLib_SetupTaskDelay(10);
        } else {
            EVT_AT_SetCam(7919, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbKicAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbKicAttack);
            FTBL.HLib_SetupTaskDelay(10);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbFinalKnockdown(0.0f, 0);
        sequence->continuation = YtqbRetry;
        FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        sequence->phase = 0;
        break;
    }
}

void YtqbKicAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbWaterNames[0];
        parameters->motionFlags = 0x24000000;
        EVT_AT_SetCam(7901, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        EVT_AT_SetAngY(YTQB_RYO, YTQB_RYO, 20.0f, 0);
        delay = YtqbPrompt(sequence, 3, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7921, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 4, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetSound(649, 98);
            EVT_AT_SetVoice("S2BVB023", 108);
            EVT_AT_SetCam(7904, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            EVT_AT_SetCam(7941, YTQB_DUMR, YTQB_DUMR, 0, 0, 45);
            EVT_AT_SetCamRot(0.2f, 0.4f, 0.7f, 37);
            YtqbBreakFrontStall(YTQB_YAD2, YTQB_YAT2, 30);
            sequence->continuation = YtqbKt2Attack;
            FTBL.HLib_SetCurrentTaskTo(YtqbNextQte);
            FTBL.HLib_SetupTaskDelay(160);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbKnockdown(-70.0f, 15);
            EVT_AT_SetCam(7909, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7922, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbKt2Attack;
            FTBL.HLib_SetCurrentTaskTo(YtqbNextQte);
            FTBL.HLib_SetupTaskDelay(75);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbKt2Attack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        RequestEventNpcCharacters(work->npcs, ytqbLastNames);
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 0, (void *)areaLoopHandPoses[YTQB_POSE_GUARD], 10, 0);
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 1, (void *)areaLoopHandPoses[YTQB_POSE_GUARD], 10, 0);
        parameters->firstOpponent = ytqbThirdNames[0];
        parameters->motionFlags = 0x4000000;
        EVT_AT_SetPos(YTQB_RYO, YTQB_BASE, 12.0f, 0.0f, -22.45f, 0);
        EVT_AT_SetAngY(YTQB_RYO, YTQB_BASE, -45.0f, 0);
        EVT_AT_SetPos(ytqbThirdNames[1], YTQB_BASE, 14.5f, 0.0f, -25.7f, 0);
        EVT_AT_SetAngY(ytqbThirdNames[1], YTQB_BASE, 135.0f, 0);
        EVT_AT_SetPos(ytqbThirdNames[2], YTQB_BASE, 14.9f, 0.0f, -24.4f, 0);
        EVT_AT_SetAngY(ytqbThirdNames[2], YTQB_BASE, 135.0f, 0);
        EVT_AT_SetMot(YTQB_RYO, 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbThirdNames[1], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbThirdNames[2], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetCam(7923, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 3, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7906, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 4, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7942, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            YtqbBreakLantern(YTQB_CAG0, YTQB_INVALID, 52);
            sequence->continuation = YtqbRntAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbRntAttack);
            FTBL.HLib_SetupTaskDelay(20);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbKnockdown(-60.0f, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7909, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbRntAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbRntAttack);
            FTBL.HLib_SetupTaskDelay(20);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbRntAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbThirdNames[1];
        parameters->motionFlags = 0x4000000;
        EVT_AT_SetCam(7924, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        EVT_AT_SetPos(parameters->firstOpponent, parameters->player, -0.00079f, 0.0f, -2.30879f, 0);
        EVT_AT_SetAngY(parameters->firstOpponent, parameters->player, 180.0f, 0);
        delay = YtqbPrompt(sequence, 11, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7944, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 12, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7944, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbKt3Attack;
            FTBL.HLib_SetCurrentTaskTo(YtqbKt3Attack);
            FTBL.HLib_SetupTaskDelay(20);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbKnockdown(-70.0f, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7907, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbKt3Attack;
            FTBL.HLib_SetCurrentTaskTo(YtqbKt3Attack);
            FTBL.HLib_SetupTaskDelay(40);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbKt3Attack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbThirdNames[2];
        parameters->motionFlags = 0x4000000;
        EVT_AT_SetCam(7924, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 0, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        EVT_AT_SetPos(YTQB_RYO, YTQB_BASE, 13.5f, 0.0f, -23.9f, 0);
        EVT_AT_SetAngY(YTQB_RYO, YTQB_BASE, -45.0f, 0);
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7908, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 1, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7925, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbAncAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbNextQte);
            FTBL.HLib_SetupTaskDelay(84);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbKnockdown(-70.0f, 15);
            EVT_AT_SetCam(7931, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7925, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbAncAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbNextQte);
            FTBL.HLib_SetupTaskDelay(73);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbAncAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbLastNames[1];
        parameters->motionFlags = 0x24000000;
        EVT_AT_SetPos(YTQB_RYO, YTQB_BASE, 13.42f, 0.0f, -30.56f, 0);
        EVT_AT_SetAngY(YTQB_RYO, YTQB_BASE, 0.0f, 0);
        EVT_AT_SetPos(ytqbLastNames[0], YTQB_BASE, 13.1f, 0.0f, -35.2f, 0);
        EVT_AT_SetAngY(ytqbLastNames[0], YTQB_BASE, 180.0f, 0);
        EVT_AT_SetPos(ytqbLastNames[2], YTQB_BASE, 12.2f, 0.0f, -31.8f, 0);
        EVT_AT_SetAngY(ytqbLastNames[2], YTQB_BASE, -135.0f, 0);
        EVT_AT_SetPos(ytqbLastNames[3], YTQB_BASE, 13.6f, 0.0f, -28.5f, 0);
        EVT_AT_SetAngY(ytqbLastNames[3], YTQB_BASE, 0.0f, 0);
        EVT_AT_SetPos(ytqbLastNames[4], YTQB_BASE, 14.0f, 0.0f, -34.3f, 0);
        EVT_AT_SetAngY(ytqbLastNames[4], YTQB_BASE, 160.0f, 0);
        EVT_AT_SetMot(YTQB_RYO, 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbLastNames[0], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbLastNames[2], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbLastNames[3], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetMot(ytqbLastNames[4], 13, 2.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_CharaDisp(ytqbLastNames[1], 0, 0);
        EVT_AT_CharaDisp(ytqbLastNames[3], 0, 0);
        EVT_AT_CharaDisp(ytqbLastNames[4], 0, 0);
        EVT_AT_SetPos(parameters->firstOpponent, parameters->player, -0.00988f, 0.0f, -2.29381f, 0);
        EVT_AT_SetAngY(parameters->firstOpponent, parameters->player, 180.0f, 0);
        EVT_AT_SetCam(7909, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 9, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 0, (void *)areaLoopHandPoses[YTQB_POSE_OPEN], 10, 0);
            Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 1, (void *)areaLoopHandPoses[YTQB_POSE_OPEN], 10, 0);
            EVT_AT_SetCam(7907, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 10, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7907, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbKt5Attack;
            FTBL.HLib_SetCurrentTaskTo(YtqbKt5Attack);
            FTBL.HLib_SetupTaskDelay(10);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbKnockdown(70.0f, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7908, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbKt5Attack;
            FTBL.HLib_SetCurrentTaskTo(YtqbKt5Attack);
            FTBL.HLib_SetupTaskDelay(20);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbKt5Attack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbLastNames[2];
        parameters->motionFlags = 0x24000000;
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 0, (void *)areaLoopHandPoses[YTQB_POSE_GUARD], 10, 0);
        Queue_EVT_AT_CtrlHandUp_1(YTQB_RYO, 1, (void *)areaLoopHandPoses[YTQB_POSE_GUARD], 10, 0);
        EVT_AT_SetCam(7900, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
        delay = YtqbPrompt(sequence, 15, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7908, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 16, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7947, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            YtqbBreakBucket(YTQB_KBK0, YTQB_KBK1, 32);
            sequence->continuation = YtqbCjuAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbCjuAttack);
            FTBL.HLib_SetupTaskDelay(20);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_CharaDisp(ytqbLastNames[1], 0, 1);
            YtqbKnockdown(-65.0f, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7926, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbCjuAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbCjuAttack);
            FTBL.HLib_SetupTaskDelay(20);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbCjuAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbLastNames[3];
        parameters->motionFlags = 0x4000000;
        delay = YtqbPrompt(sequence, 19, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_CharaDisp(ytqbLastNames[4], 0, 1);
            YtqbFinalKnockdown(0.0f, 0);
            EVT_AT_SetCam(7920, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_CharaDisp(ytqbLastNames[1], 0, 1);
            EVT_AT_SetCam(7927, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbKt7Attack;
            FTBL.HLib_SetCurrentTaskTo(YtqbKt7Attack);
            FTBL.HLib_SetupTaskDelay(10);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbKt7Attack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbLastNames[4];
        parameters->motionFlags = 0x4000000;
        delay = YtqbPrompt(sequence, 20, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_CharaDisp(ytqbLastNames[3], 0, 1);
            YtqbFinalKnockdown(0.0f, 0);
            EVT_AT_SetCam(7920, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7928, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbFinalEhyAttack;
            FTBL.HLib_SetCurrentTaskTo(YtqbFinalEhyAttack);
            FTBL.HLib_SetupTaskDelay(10);
        }
        sequence->phase = 0;
        break;
    }
}

void YtqbFinalEhyAttack(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    YtqbSequence *sequence = &work->sequence;
    YtqbResultParameters *parameters = &sequence->parameters;
    int delay = 90;
    ytqbAttackState = 0;
    switch (sequence->phase++) {
    case 0:
        parameters->firstOpponent = ytqbLastNames[0];
        parameters->motionFlags = 0x4000000;
        delay = YtqbPrompt(sequence, 13, 0);
        FTBL.HLib_SetupTaskDelay(delay);
        break;
    case 1:
        EVT_AT_SetPos(YTQB_RYO, YTQB_BASE, 12.9f, 0.0f, -32.2f, 0);
        EVT_AT_SetAngY(YTQB_RYO, YTQB_BASE, 0.0f, 0);
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            EVT_AT_SetCam(7901, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            delay = YtqbPrompt(sequence, 14, 0);
            FTBL.HLib_SetupTaskDelay(delay);
        } else {
            EVT_AT_SetCam(7929, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            EVT_AT_SetSample(0, 1, delay);
            EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, delay);
            EVT_SetQteRetryFrames(36);
            sequence->continuation = YtqbLoadFinalCharacters;
            FTBL.HLib_SetCurrentTaskTo(YtqbLoadFinalCharacters);
            FTBL.HLib_SetupTaskDelay(120);
            sequence->phase = 0;
        }
        break;
    default:
        YtqbApplyResult(sequence);
        if (!sequence->result) {
            YtqbBackKnockdown(0.0f, 0);
            EVT_AT_SetCam(7929, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            sequence->continuation = YtqbRetry;
            FTBL.HLib_SetCurrentTaskTo(YtqbWaitMotion);
        } else {
            EVT_AT_SetCam(7905, YTQB_DUMR, YTQB_DUMR, 0, 0, 0);
            EVT_AT_SetSample(0, 1, 90);
            EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 90);
            EVT_SetQteRetryFrames(36);
            sequence->continuation = YtqbLoadFinalCharacters;
            FTBL.HLib_SetCurrentTaskTo(YtqbLoadFinalCharacters);
            FTBL.HLib_SetupTaskDelay(120);
        }
        sequence->phase = 0;
        break;
    }
}

int YtqbPlayRetryVoices(int delay)
{
    int start = delay;
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
        delay += 225;
    } else {
        EVT_AT_SetVoice("S2BVC007", start + 21);
        EVT_AT_SetVoice("S2BVC009", start + 57);
        EVT_AT_SetVoice("S2BVA012", start + 105);
        EVT_AT_SetSound(649, start + 62);
        EVT_AT_SetSound(650, start + 76);
        EVT_AT_SetSound(132, start + 87);
        EVT_AT_SetSound(653, start + 98);
        EVT_AT_SetSound(651, start + 114);
        EVT_AT_SetSound(652, start + 130);
        EVT_AT_SetSound(649, start + 140);
        EVT_AT_SetSound(628, start + 155);
        EVT_AT_SetSound(652, start + 160);
        EVT_AT_SetSound(650, start + 187);
        EVT_AT_SetSound(628, start + 52);
        EVT_AT_SetSound(629, start + 58);
        EVT_AT_SetSound(132, start + 127);
        EVT_AT_SetSound(133, start + 165);
        EVT_AT_SetSound(649, start + 194);
        EVT_AT_SetSound(628, start + 160);
        EVT_AT_SetSound(629, start + 166);
        delay += 260;
    }
    return delay;
}

void YtqbWaitMotion(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(YTQB_RYO), 65, 1, 0))
        if (!IsEventNpcReadBusy(work->npcs))
            FTBL.HLib_SetCurrentTaskTo(work->sequence.continuation);
}

void YtqbNextQte(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    if (!IsEventNpcReadBusy(work->npcs)) {
        RetireEventNpcRequests(work->npcs);
        FTBL.HLib_SetCurrentTaskTo(work->sequence.continuation);
    }
}

void YtqbRetry(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    int delay;
    if (!IsEventNpcReadBusy(work->npcs)) {
        delay = YtqbPlayRetryVoices(0);
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        EVT_AT_SetSample(0, 1, delay - 60);
        EVT_IncrementQteFailureCount();
        RestoreEventDateTime();
        FTBL.HLib_SetCurrentTaskTo(YtqbStart);
        FTBL.HLib_SetupTaskDelay(delay);
    }
}

void YtqbLoadFinalCharacters(void *data)
{
    YtqbWork *work = (YtqbWork *)data;
    ytqbAttackState = 0;
    RequestEventNpcCharacters(work->npcs, ytqbFinalNames);
    RetireEventNpcRequests(work->npcs);
    FTBL.HLib_SetCurrentTaskTo(YtqbStartFinalAuthor);
}

void InitializeYtqb(YtqbWork *work)
{
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    EVT_SetQteRetryFrames(24);
    SaveEventDateTime();
    work->characters = Queue_EVT_AT_ReadChr(work->task, ytqbPlayerNames);
    work->npcs = 0;
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"YTQB", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0048", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"048E", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "YTQB", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "YTQB", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0048");
    FTBL.EV_FreeAuthorPak(1, "S2BV");
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0048A000");
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.EV_SetWeatherInstruction(1920, 0);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
    QueueYtqbAmbientSmoke(0);
}

void CleanupYtqb(YtqbWork *work)
{
    FTBL.RemoveInventoryItem(36, 1);
    FTBL.RemoveInventoryItem(37, 1);
    FTBL.RemoveInventoryItem(38, 1);
    FTBL.RemoveInventoryItem(40, 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(YTQB_RYO), 0);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(YTQB_RYO));
    FTBL.qteBattleUtil(5, 0, 0, 0, 0);
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "S2BV");
    FTBL.EV_FreeAuthorPak(0, "0048");
    FTBL.EV_ControlPackageResources(0, 6, "YTQB", 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "0048", 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "048E", 0, 0);
    ET_AT_KillWork(work->characters);
}
#endif
#else
#include "s2/areas/area_q7qb.hpp"
#include "s2/areas/area_ytqb.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/load_title.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"

namespace shenmue::s2::areas {
std::int32_t ytqbAmbientSmoke = -1, ytqbFinalAuthorActive = -1;
namespace {
void InitializeYtqb() {
    ytqbAmbientSmoke = -1;
}
void RunYtqb(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(YtqbStart, 4, 11, sizeof(YtqbWork), YTQB_EVNT);
    YtqbWork *work = static_cast<YtqbWork *>(HLib::GetTaskParameter(task));
    work->reserved10 = 0;
    work->task = task;
    YtqbSetup(*work);
    GlowParameters glow{.index = 5, .intensity = .3f, .values = {.3f, .3f, .3f, .2f}};
    SelectPostFxBank(1, 1);
    UploadGlowParams(glow);
    SelectPostFxBank(3, 1);
    UploadBlurStrength(1);
    ytqbFinalAuthorActive = 0;
}
void FilmBlack() {
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
}
void RetireNpcRequests(EventNpcReadWork &work) {
    for (std::int32_t i = 0; i < 32; ++i)
        if (work.states[i] == 2 || work.requests[i] == 2)
            work.requests[i] = 3;
}
void EnableFinalPluralControl() {
    for (std::int32_t i = 0; i < 6; ++i)
        SetPluralControlFor(ctrlGetScenarioChara(YTQB_FinalNames[i]), 1);
}
void StartCombinationPrompt(std::int32_t second, std::int32_t third) {
    qteBattleUtil(BattleQteMode::SetLastCommands, 16, second, third);
    std::int16_t frames = 24;
    ReInitElement(UIElement::GameQte);
    if (g_eventQteRetryFrames > 0)
        frames = static_cast<std::int16_t>(static_cast<std::int16_t>(EVT_GetQteFailureCount()) * ((g_eventQteRetryFrames - 24) / 2) + 24);
    EventQtePromptTaskData prompt{.autoCleanupTask = 0, .requiredStatus = 0, .maximumFrames = frames, .reserved08 = 0, .displayState = 4, .delayFrames = 0};
    EVT_AT_Qte_Callback(&prompt);
}
} // namespace
void YtqbPosition(shen_name character, shen_name parent, const Vector3 &position) {
    EventSetPosition request{.cleanup = 0, .character = character, .parent = parent, .position = position, .delay = 0};
    EVT_AT_SetPosRot(&request);
}
void YtqbAngle(shen_name character, shen_name parent, float degrees) {
    EventSetAngle request{.cleanup = 0, .character = character, .parent = parent, .degrees = degrees, .delay = 0};
    EVT_AT_SetAngY_Callback(&request);
}
void YtqbStand(shen_name character) {
    EventSetMotion request{.cleanup = 0, .character = character, .animation = 13, .frame = 2, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&request);
    GetMotionFrameCount(13);
}
void YtqbCamera(std::int32_t camera, std::uint8_t context, float angle) {
    EventSetCamera request{.cleanup = 0, .camera = camera, .eye = YTQB_DUMR, .target = YTQB_DUMR, .context = context, .angle = angle, .delay = 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void YtqbPrompt(YtqbWork &work, const EventQteData &prompt, const EventQteData &authored) {
    work.prompt = &prompt;
    work.authored = &authored;
    HLib::SetupTaskDelay(static_cast<std::int32_t>(Queue_EVT_AT_ResultQTE(&work.result, &work.parameters, &prompt)));
}
void YtqbApplyResult(YtqbWork &work) {
    EventQteAuthoredCallback callback = work.result ? work.authored->resultSuccess : work.authored->resultFailure;
    if (callback)
        callback(&work.parameters, 0);
}
void YtqbTransition(YtqbWork &work, HLib::TaskCallback continuation, std::int32_t delay, HLib::TaskCallback callback) {
    work.continuation = continuation;
    HLib::SetCurrentTaskTo(callback ? callback : continuation);
    if (delay)
        HLib::SetupTaskDelay(delay);
}
void YtqbSetup(YtqbWork &work) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EVT_SetQteRetryFrames(24);
    EVT_SetQteFailureCount(0);
    shenGetCurrentDateTime(eventTemporaryDateTime);
    work.characters = Queue_EVT_AT_ReadChr(work.task, YTQB_PlayerNames);
    work.npcs = nullptr;
    for (const char *package : {"YTQB", "0048", "048E"}) {
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(package));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("YTQB"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("YTQB"), 0);
    EV_FreeAuthorPak(1, "S2BV");
    EV_FreeAuthorPak(1, "0048");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0048A000");
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_UpdateWeather();
    SetWeatherInstruction(12, false);
    SetWeatherInstruction(0x780, false);
    EV_lightLoad(YTQB_0000);
    PushGlobalLightingProgram();
    shenSetClockAndCompassRenderTo(1);
    nullsub();
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EventTickFunction_Callback, 6, 11, 0x20, YTQB_EVNT);
    RegisterEventTask(task);
    EventTickFunctionWork *call = static_cast<EventTickFunctionWork *>(HLib::GetTaskParameter(task));
    call->callback = YtqbAmbientSmoke;
    call->maxTime = 200000000;
    call->currentTicks = 0;
    call->paramSize = 0;
    call->delay = 0;
}
void YtqbStart(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs)
        for (std::int32_t i = 0; i < 32; ++i)
            work.npcs->requests[i] = 3;
    else
        work.npcs = QueueEventNpcRead();
    RequestEventNpcCharacters(work.npcs, YTQB_FirstNames);
    AT_SetLocalBaseChara(YTQB_DUMR, YTQB_RYO_);
    YtqbPosition(YTQB_DUMR, YTQB_RYO_, {});
    YtqbAngle(YTQB_DUMR, YTQB_RYO_, 0);
    YtqbPosition(YTQB_BASE, YTQB_INVALID, YTQB_WorldBase);
    YtqbAngle(YTQB_BASE, YTQB_INVALID, 0);
    mtwkSetFrameTime(scenFindMotionwork(YTQB_RYO_), 8, MotionRegion::LowerBody);
    SetLowQualityHands(ctrlGetScenarioChara(YTQB_RYO_));
    work.parameters.player = YTQB_RYO_;
    work.continuation = YtqbSboAttack;
    work.parameters.firstOpponent = YTQB_INVALID;
    work.parameters.secondOpponent = YTQB_INVALID;
    work.parameters.motionFlags = 0;
    work.phase = 0;
    work.result = 0;
    work.prompt = nullptr;
    work.authored = nullptr;
    YtqbSetupLantern();
    YtqbSetupWater();
    YtqbSetupBucket();
    YtqbSetupStallObjects();
    for (shen_name character : {YTQB_YAD3, YTQB_YAD2}) {
        const shen_name broken{character.name | 0x100000U};
        EV_DisableCharacterDisplay(character);
        EV_EnableCharacterDisplay(broken);
        SetScenarioCharacterDrawControl(character, 0, 0);
        SetScenarioCharacterDrawControl(broken, 0, 0);
    }
    HLib::SetCurrentTaskTo(YtqbStartIntroAuthor);
}
void YtqbStartIntroAuthor(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    SelectPostFxBank(3, 1);
    UploadBlurStrength(1);
    EV_LoadAudio("0048A000", 1);
    EV_spmBeginSampleExecute();
    FilmBlack();
    EV_lightLoad(YTQB_l201);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_ATH1.name));
    work.continuation = YtqbSboAttack;
    HLib::SetCurrentTaskTo(work.continuation);
}
void YtqbStartWaterAuthor(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_STR0.name));
    camCtrlSetNearDistance(12);
    EventCling cling{.cleanup = 0,
        .target = YTQB_SUI2,
        .character = YTQB_KIC_,
        .bone = 14,
        .position = {.x = -.43379101157188416f, .y = -2.3200600147247314f, .z = -.1769540011882782f},
        .rotation = {.x = -364, .y = 22573, .z = -2730},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    EV_EnableCharacterDisplay(YTQB_SUI2);
    HLib::SetCurrentTaskTo(YtqbWaitWaterAuthor);
}
void YtqbWaitWaterAuthor(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        SetPluralControlFor(ctrlGetScenarioChara(YTQB_RYO_), 1);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(YTQB_RYO_), 0, reinterpret_cast<std::byte *>(ryoQteHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(YTQB_RYO_), 1, reinterpret_cast<std::byte *>(ryoQteHandPose), 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(YTQB_RYO_), static_cast<std::int32_t>(FacialExpressionId::Angry), 1, 1);
    StartCombinationPrompt(16, 1024);
    HLib::SetCurrentTaskTo(YtqbWaterResult);
}
void YtqbWaterResult(void *data) {
    if (!qteBattleUtil(BattleQteMode::GetResult))
        return;
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    EventCling cling;
    cling.cleanup = 0;
    cling.target = YTQB_SUI2;
    cling.character = YTQB_KIC_;
    cling.mode = 1;
    cling.delay = 0;
    EVT_AT_Cling(&cling);
    EV_DisableCharacterDisplay(YTQB_SUI2);
    YtqbResetWater();
    const std::int32_t delay = qteBattleUtil(BattleQteMode::GetCompletedCount) == 3 ? YtqbWaterSuccess() : YtqbWaterFailure();
    RetireNpcRequests(*work.npcs);
    HLib::SetCurrentTaskTo(work.continuation);
    HLib::SetupTaskDelay(delay);
}
void YtqbStartFinalAuthor(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    LoadBlurModelForCharacter(YTQB_SYE_, "SYE_B.MT7", 1);
    if (ytqbFinalAuthorActive)
        EV_SetAuthorActiveTo(1);
    EV_spmBeginSampleExecute();
    FilmBlack();
    EV_lightLoad(YTQB_l203);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_ATH3.name));
    HLib::SetCurrentTaskTo(YtqbWaitFinalAuthor);
}
void YtqbWaitFinalAuthor(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        EnableFinalPluralControl();
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    StartCombinationPrompt(4, 256);
    HLib::SetCurrentTaskTo(YtqbFinalResult);
}
void YtqbFinalResult(void *data) {
    if (!qteBattleUtil(BattleQteMode::GetResult))
        return;
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (qteBattleUtil(BattleQteMode::GetCompletedCount) == 3) {
        EV_lightLoad(YTQB_l204);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_ATH4.name));
        work.continuation = YtqbFinish;
    } else {
        EV_lightLoad(YTQB_l205);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_ATH5.name));
        if (EVT_GetQteFailureCount() < 2)
            EVT_SetQteFailureCount(EVT_GetQteFailureCount() + 1);
        SetCurrentDateTimeTo(eventTemporaryDateTime);
        shenSetTimeSimulationTo(1);
        work.continuation = YtqbStartFinalAuthor;
        if (ytqbFinalAuthorActive == 1)
            EV_SetAuthorActiveTo(1);
        ytqbFinalAuthorActive = 1;
    }
    HLib::SetCurrentTaskTo(YtqbWaitResultAuthor);
}
void YtqbWaitResultAuthor(void *data) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        EnableFinalPluralControl();
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    spmFadeOut();
    HLib::SetCurrentTaskTo(static_cast<YtqbWork *>(data)->continuation);
    HLib::SetupTaskDelay(60);
}
void YtqbFinish(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    for (std::int32_t i = 0; i < 32; ++i)
        work.npcs->requests[i] = 3;
    YtqbExit(work);
}
void YtqbWaitMotion(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (mtwkSetRegionFlag(scenFindMotionwork(YTQB_RYO_), 65, 1, MotionRegion::LowerBody) && work.npcs->characters[24].name == 0)
        HLib::SetCurrentTaskTo(work.continuation);
}
void YtqbNextQte(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    RetireNpcRequests(*work.npcs);
    HLib::SetCurrentTaskTo(work.continuation);
}
void YtqbRetry(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
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
    EV_SetAuthorActiveTo(1);
    HLib::SetCurrentTaskTo(YtqbStart);
    HLib::SetupTaskDelay(delay);
}
void YtqbLoadFinalCharacters(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    RequestEventNpcCharacters(work.npcs, YTQB_FinalNames);
    RetireNpcRequests(*work.npcs);
    SelectPostFxBank(3, 0);
    HLib::SetCurrentTaskTo(YtqbStartFinalAuthor);
}
void YtqbCleanup(YtqbWork &work) {
    RemoveInventoryItem(36, 1);
    RemoveInventoryItem(37, 1);
    RemoveInventoryItem(38, 1);
    RemoveInventoryItem(40, 1);
    SetPluralControlFor(ctrlGetScenarioChara(YTQB_RYO_), 0);
    hndlSetFlags(ctrlGetScenarioChara(YTQB_RYO_));
    qteBattleUtil(BattleQteMode::RetireAndFree);
    FinishLoading();
    shenSetClockAndCompassRenderTo(0);
    sbtLoadProgram();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "S2BV");
    EV_FreeAuthorPak(0, "0048");
    for (const char *package : {"YTQB", "0048", "048E"})
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    ET_AT_KillWork(work.characters);
}
void YtqbExit(YtqbWork &work) {
    SelectPostFxBank(1, 0);
    SelectPostFxBank(3, 0);
    UploadBlurStrength(1);
    YtqbCleanup(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (g_currentMap.name == YTQB_YTQB.name) {
        scenSetTaskFlags(8);
        EV_scenQueueLoad(g_actNumber, YTQB_0048, 2);
    }
}
const ScriptAreaDefinition ytqbScriptDefinition{.areaId = YTQB_YTQB, .field04 = 0, .init = InitializeYtqb, .main = RunYtqb};
} // namespace shenmue::s2::areas
#endif
