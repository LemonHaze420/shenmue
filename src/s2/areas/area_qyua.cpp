#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_qyua.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void QyuaEntry(void *argument)
{
    QyuaMain(argument);
}
#elif defined(SHENMUE_EVENT_QYUA_SEGMENTS)
int BeginQyuaKnifeQte(int delay)
{
    int frames = delay + 60;
    qyuaSegmentState = 0;
    EVT_AT_SetVoice("0217C002", delay + 35);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, 0.0f, 0.0f, 0.0f, delay);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0.0f, delay);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, -0.15029f, 0.0f, 0.38437f, delay);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, -1.97384f, 0.0f, 0.02722f, delay);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0.0f, delay);
    FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(QYUA_PLAYER), 16, 1);
    FTBL.toggleOsagFlags(FTBL.ctrlGetCharaByName(QYUA_REN));
    EVT_AT_SetCam(7800, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, delay);
    EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 0.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(QYUA_REN, 0x2545, 0.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetMot(QYUA_YUAN, 0x2590, 0.0f, 1.0f, 0x40000000, delay);
    EVT_AT_SetFaceLooks(QYUA_PLAYER, 1, 1, 1.0f, delay);
    EVT_AT_SetFaceLooks(QYUA_REN, 1, 1, 1.0f, delay);
    EVT_AT_SetFaceLooks(QYUA_YUAN, 13, 1, 1.0f, delay);
    return frames;
}

int BeginQyuaSecondKnifeQte(int delay)
{
    int frames = delay + 70;
    qyuaSegmentState = 0;
    EVT_AT_SetVoice("0217C003", delay + 50);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, 0.0f, 0.0f, 0.0f, delay);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0.0f, delay);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, -0.15029f, 0.0f, 0.38437f, delay);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, -1.97384f, 0.0f, 0.02722f, delay);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetCam(7801, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, delay);
    EVT_AT_SetCam(7804, QYUA_BASE, QYUA_BASE, 0, 0, delay + 60);
    EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 60.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_REN, 0x2545, 60.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_YUAN, 0x2590, 60.0f, 1.0f, 0x44000000, delay);
    return frames;
}

int PlayQyuaFirstKnifeFailure(int delay)
{
    int frames = delay + 60;
    qyuaSegmentState = 0;
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, 0.0f, 0.0f, 0.0f, delay);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0.0f, delay);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, -0.17183f, 0.0f, 0.42309001f, delay);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, -1.77588f, 0.0f, -0.12269f, delay);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetCam(7802, QYUA_DUMMY_YUAN, QYUA_DUMMY_YUAN, 0, 0, delay);
    EVT_AT_SetMot(QYUA_PLAYER, 0x2562, 36.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_REN, 0x2542, 36.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_YUAN, 0x258d, 36.0f, 1.0f, 0x44000000, delay);
    return frames;
}

int HoldQyuaSecondKnifeQte(int delay)
{
    qyuaSegmentState = 0;
    return delay;
}

int BeginQyuaThirdKnifeQte(int delay)
{
    int frames = delay + 115;
    qyuaSegmentState = 0;
    EVT_AT_SetVoice("0217C004", delay + 25);
    EVT_AT_SetVoice("0217C007", delay + 80);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, 0.0f, 0.0f, 0.0f, delay);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0.0f, delay);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, -0.15029f, 0.0f, 0.38437f, delay);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, -1.97384f, 0.0f, 0.02722f, delay);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetCam(7805, QYUA_BASE, QYUA_BASE, 0, 0, delay);
    EVT_AT_SetCam(7807, QYUA_BASE, QYUA_BASE, 0, 0, delay + 20);
    EVT_AT_SetCam(7808, QYUA_DUMMY_YUAN, QYUA_DUMMY_YUAN, 0, 0, delay + 60);
    EVT_AT_SetCam(7817, QYUA_DUMMY_YUAN, QYUA_DUMMY_YUAN, 0, 0, delay + 92);
    EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 130.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_REN, 0x2545, 130.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_YUAN, 0x2590, 130.0f, 1.0f, 0x44000000, delay);
    EVT_AT_DispBlur(QYUA_REN, 7, 12, 1, 0, 25, delay + 10);
    return frames;
}

int PlayQyuaSecondKnifeFailure(int delay)
{
    int frames = delay + 45;
    qyuaSegmentState = 0;
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, 0.0f, 0.0f, 0.0f, delay);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0.0f, delay);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, -0.079530001f, 0.0f, 0.42789f, delay);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, -2.0088601f, 0.0f, 0.31832999f, delay);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetCam(7809, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, delay);
    EVT_AT_SetMot(QYUA_PLAYER, 0x2563, 112.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_REN, 0x2543, 112.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_YUAN, 0x258e, 112.0f, 1.0f, 0x44000000, delay);
    return frames;
}

int HoldQyuaThirdKnifeQte(int delay)
{
    qyuaSegmentState = 0;
    return delay;
}

int PlayQyuaKnifeSuccess(int delay)
{
    int frames = delay + 135;
    qyuaSegmentState = 0;
    EVT_AT_SetVoice("0217A002", delay + 5);
    EVT_AT_SetVoice("0217B002", delay + 40);
    EVT_AT_SetVoice("0217C006", delay + 70);
    EVT_AT_CharaDisp(QYUA_KNIFE, delay + 20, 1);
    EVT_AT_SetSound(1954, delay + 30);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, 0.0f, 0.0f, 0.0f, delay);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0.0f, delay);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, 0.0f, 0.0f, 0.0f, delay + 15);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0.0f, delay + 15);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, -0.15029f, 0.0f, 0.38437f, delay);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, -0.15029f, 0.0f, 0.38437f, delay + 15);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0.0f, delay + 15);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, -1.97384f, 0.0f, 0.02722f, delay);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, -1.97384f, 0.0f, 0.02722f, delay + 15);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0.0f, delay + 15);
    EVT_AT_SetCam(7816, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, delay);
    EVT_AT_SetCam(7811, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, delay + 15);
    EVT_AT_SetCam(7812, QYUA_DUMMY_REN, QYUA_DUMMY_REN, 0, 0, delay + 40);
    EVT_AT_SetCam(7813, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, delay + 95);
    EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 245.0f, 0.1f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 245.0f, 1.0f, 0x44000000, delay + 15);
    EVT_AT_SetMot(QYUA_REN, 0x2545, 245.0f, 0.1f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_REN, 0x2545, 245.0f, 1.0f, 0x44000000, delay + 15);
    EVT_AT_SetMot(QYUA_YUAN, 0x2590, 245.0f, 0.1f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_YUAN, 0x2590, 245.0f, 1.0f, 0x44000000, delay + 15);
    EVT_AT_DispBlur(QYUA_PLAYER, 6, 12, 1, 0, 8, delay + 15);
    EVT_AT_DispBlur(QYUA_REN, 7, 12, 1, 0, 18, delay + 38);
    return frames;
}

int PlayQyuaKnifeRecovery(int delay)
{
    int frames = delay + 50;
    qyuaSegmentState = 0;
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, 0.0f, 0.0f, 0.0f, delay);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0.0f, delay);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, 0.13912f, 0.0f, 0.36248001f, delay);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, -1.28835f, 0.0f, 0.32345f, delay);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0.0f, delay);
    EVT_AT_SetCam(7814, QYUA_BASE, QYUA_BASE, 0, 0, delay);
    EVT_AT_SetMot(QYUA_PLAYER, 0x2564, 235.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_REN, 0x2544, 235.0f, 1.0f, 0x44000000, delay);
    EVT_AT_SetMot(QYUA_YUAN, 0x258f, 235.0f, 1.0f, 0x44000000, delay);
    return frames;
}
#else
static const int qyuaDefaultArguments[1] = {0};

void QyuaMain(void *argument)
{
    HLibTask *task;
    QyuaEventWork *work;
    HLibTask *debugTask;
    task = FTBL.HLib_EnqueueTaskWithParameter(QyuaStartCallback, 4, 11, sizeof(QyuaEventWork), TASK_EVNT);
    work = (QyuaEventWork *)FTBL.HLib_GetTaskParameter(task);
    debugTask = FTBL.HLib_EnqueueTaskWithParameter(QyuaDebugDisplayCallback, 3, 11, sizeof(QyuaEventWork *), TASK_EVNT);
    *(QyuaEventWork **)FTBL.HLib_GetTaskParameter(debugTask) = work;
    work->self = task;
    work->debugTask = debugTask;
    if (argument)
        work->arguments = argument;
    else
        work->arguments = qyuaDefaultArguments;
    InitializeQyuaResources(work);
    EventDebugRegister(QYUA_DEBUG_ID, QyuaDebugExitCallback, work, 2000000000);
}

void QyuaDebugExitCallback(void *data)
{
    FinishQyuaEvent((QyuaEventWork *)data);
}

void FinishQyuaEvent(QyuaEventWork *work)
{
    RestoreQyuaResources(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->debugTask);
    FTBL.EV_HLKillOtherHierarchyTask(work->self);
    EventDebugUnregister(QYUA_DEBUG_ID);
    FTBL.EV_stepSetFlags(QYUA_FINISHED_STEP_FLAG, 0);
    if (g_currentMap == AREA_QYUA) {
        FTBL.scenSetTaskFlags(6);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

void QyuaDebugDisplayCallback(void *data)
{
    FTBL.SetDebugScreenTextPosition(2, 23);
}

void ResetQyuaFight(QyuaFight *fight)
{
    fight->nextCallback = QyuaWaitOpeningAuthorCallback;
    fight->succeeded = 0;
}

shen_name qyuaCharacters[] = {QYUA_PLAYER, QYUA_REN, QYUA_YUAN, QYUA_TOGYU, EVENT_CHARACTER_INVALID};
int qyuaSegmentState = 0;
Vector3 qyuaKnifePosition = {0.071784f, -0.023756f, -0.01023f};
Vector3i qyuaKnifeRotation = {-16690, 2613, -11462};

void QyuaStartCallback(void *data)
{
    QyuaEventWork *work = (QyuaEventWork *)data;
    EventDebugCheck(QYUA_DEBUG_ID, 4);
    ResetQyuaFight(&work->fight);
    FTBL.LoadBlurModelForCharacter_0(QYUA_REN, "REN_B.MT7", 1.0f);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(QYUA_PLAYER));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(QYUA_REN));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(QYUA_YUAN));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(QYUA_TOGYU));
    Queue_EVT_AT_Cling(QYUA_KNIFE, QYUA_YUAN, 14, &qyuaKnifePosition, &qyuaKnifeRotation, 0, 0);
    FTBL.EV_cuffsAttach(QYUA_PLAYER, 1, QYUA_REN, 0, 0.3f);
    FTBL.AT_SetLocalBaseChara(QYUA_DUMMY_RYO, QYUA_PLAYER);
    FTBL.AT_SetLocalBaseChara(QYUA_DUMMY_REN, QYUA_REN);
    FTBL.AT_SetLocalBaseChara(QYUA_DUMMY_YUAN, QYUA_YUAN);
    FTBL.AT_SetLocalBaseChara(QYUA_DUMMY_TOGYU, QYUA_TOGYU);
    EVT_AT_SetPos(QYUA_BASE, EVENT_CHARACTER_INVALID, 218.2f, 110.8f, 217.6f, 0);
    EVT_AT_SetAngY(QYUA_BASE, EVENT_CHARACTER_INVALID, 0.0f, 0);
    EVT_AT_SetPos(QYUA_DUMMY_RYO, QYUA_PLAYER, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_RYO, QYUA_PLAYER, 0.0f, 0);
    EVT_AT_SetPos(QYUA_DUMMY_REN, QYUA_REN, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_REN, QYUA_REN, 0.0f, 0);
    EVT_AT_SetPos(QYUA_DUMMY_YUAN, QYUA_YUAN, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_YUAN, QYUA_YUAN, 0.0f, 0);
    EVT_AT_SetPos(QYUA_DUMMY_TOGYU, QYUA_TOGYU, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_TOGYU, QYUA_TOGYU, 0.0f, 0);
    EVT_AT_SetPos(QYUA_TOGYU, QYUA_BASE, -1000.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QYUA_TOGYU, QYUA_BASE, 0.0f, 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QYUA_PLAYER), 0);
    EVT_AT_CharaDisp(QYUA_PLAYER, 0, 0);
    EVT_AT_CharaDisp(QYUA_REN, 0, 0);
    EVT_AT_CharaDisp(QYUA_YUAN, 0, 0);
    EVT_AT_CharaDisp(QYUA_AUTHOR_KNIFE, 0, 0);
    EVT_AT_CharaDisp(QYUA_KNIFE, 0, 1);
    FTBL.EV_LoadAudio("0217A000", 1);
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_authSceneCtrl(QYUA_AUTHOR_START);
    FTBL.HLib_SetCurrentTaskTo(QyuaWaitOpeningAuthorCallback);
}

void QyuaWaitOpeningAuthorCallback(void *data)
{
    int frames;
    int playing = FTBL.EV_authSceneCtrl(-1);
    if (playing)
        return;
    EVT_AT_CharaDisp(QYUA_AUTHOR_KNIFE, 0, 1);
    EVT_AT_CharaDisp(QYUA_KNIFE, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QYUA_PLAYER), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QYUA_PLAYER), 1, 1, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QYUA_PLAYER), 0, (void *)areaLoopHandPoses[QYUA_REPLACEMENT_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QYUA_PLAYER), 1, (void *)areaLoopHandPoses[QYUA_REPLACEMENT_HAND_POSE], 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QYUA_REN), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QYUA_REN), 1, 1, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QYUA_REN), 0, (void *)areaLoopHandPoses[QYUA_REPLACEMENT_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QYUA_REN), 1, (void *)areaLoopHandPoses[QYUA_REPLACEMENT_HAND_POSE], 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QYUA_YUAN), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QYUA_YUAN), 1, 1, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QYUA_YUAN), 0, (void *)areaLoopHandPoses[QYUA_KNIFE_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QYUA_YUAN), 1, (void *)areaLoopHandPoses[QYUA_CRATE_GRIP_HAND_POSE], 0);
    frames = BeginQyuaKnifeQte(0);
    EVT_AT_QTE(16, 16, 0, frames - 29, playing);
    FTBL.HLib_SetCurrentTaskTo(QyuaFirstResultCallback);
    FTBL.HLib_SetupTaskDelay(frames);
}

void QyuaFirstResultCallback(void *data)
{
    int frames;
    unsigned int status = FTBL.GetCurrentQteStatus() & 0xfff;
    if (status == 16) {
        frames = BeginQyuaSecondKnifeQte(0);
        frames = HoldQyuaSecondKnifeQte(frames);
        EVT_AT_QTE(4, status, 0, frames - 29, 0);
        FTBL.HLib_SetCurrentTaskTo(QyuaSecondResultCallback);
    } else {
        frames = PlayQyuaFirstKnifeFailure(0);
        FTBL.HLib_SetCurrentTaskTo(QyuaFailureAuthorCallback);
    }
    FTBL.HLib_SetupTaskDelay(frames);
}

void QyuaSecondResultCallback(void *data)
{
    int frames;
    if ((FTBL.GetCurrentQteStatus() & 0xfff) == 4) {
        frames = BeginQyuaThirdKnifeQte(0);
        frames = HoldQyuaThirdKnifeQte(frames);
        EVT_AT_QTE(256, 16, 0, frames - 29, 0);
        FTBL.HLib_SetCurrentTaskTo(QyuaThirdResultCallback);
    } else {
        frames = PlayQyuaSecondKnifeFailure(0);
        FTBL.HLib_SetCurrentTaskTo(QyuaFailureAuthorCallback);
    }
    FTBL.HLib_SetupTaskDelay(frames);
}

void QyuaThirdResultCallback(void *data)
{
    int frames;
    if ((FTBL.GetCurrentQteStatus() & 0xfff) == 256) {
        frames = PlayQyuaKnifeSuccess(0);
        FTBL.HLib_SetCurrentTaskTo(QyuaSuccessAuthorCallback);
    } else {
        frames = PlayQyuaFirstKnifeFailure(0);
        FTBL.HLib_SetCurrentTaskTo(QyuaFailureAuthorCallback);
    }
    FTBL.HLib_SetupTaskDelay(frames);
}

void QyuaSuccessAuthorCallback(void *data)
{
    QyuaEventWork *work = (QyuaEventWork *)data;
    QyuaFight *fight = &work->fight;
    EVT_AT_CharaDisp(QYUA_YUAN, 0, 0);
    EVT_AT_CharaDisp(QYUA_KNIFE, 0, 1);
    EVT_AT_CharaDisp(QYUA_AUTHOR_KNIFE, 0, 1);
    FTBL.EV_authSceneCtrl(QYUA_AUTHOR_SUCCESS);
    fight->succeeded = 1;
    work->fight.nextCallback = QyuaRetryOrFinishCallback;
    FTBL.HLib_SetCurrentTaskTo(QyuaWaitResultAuthorCallback);
}

void QyuaFailureAuthorCallback(void *data)
{
    QyuaEventWork *work = (QyuaEventWork *)data;
    QyuaFight *fight = &work->fight;
    EVT_AT_CharaDisp(QYUA_YUAN, 0, 0);
    EVT_AT_CharaDisp(QYUA_KNIFE, 0, 1);
    EVT_AT_CharaDisp(QYUA_AUTHOR_KNIFE, 0, 0);
    FTBL.EV_authSceneCtrl(QYUA_AUTHOR_FAILURE);
    fight->succeeded = 0;
    work->fight.nextCallback = QyuaRetryOrFinishCallback;
    FTBL.HLib_SetCurrentTaskTo(QyuaWaitResultAuthorCallback);
}

void QyuaWaitResultAuthorCallback(void *data)
{
    QyuaFight *fight = &((QyuaEventWork *)data)->fight;
    int frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == 1)
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QYUA_PLAYER), frame);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        EVT_AT_SetSample(0, 1, 0);
        FTBL.HLib_SetCurrentTaskTo(fight->nextCallback);
        FTBL.HLib_SetupTaskDelay(60);
    }
}

void QyuaRetryOrFinishCallback(void *data)
{
    QyuaEventWork *work = (QyuaEventWork *)data;
    if (!work->fight.succeeded || g_currentMap == AREA_QYUA) {
        EVT_IncrementQteFailureCount();
        RestoreEventDateTime();
        FTBL.HLib_SetCurrentTaskTo(QyuaStartCallback);
    } else {
        FinishQyuaEvent(work);
    }
}

void InitializeQyuaResources(QyuaEventWork *work)
{
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    SaveEventDateTime();
    EVT_SetQteRetryFrames(24);
    work->characters = Queue_EVT_AT_ReadChr(work->self, qyuaCharacters);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0217", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"QYUA", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "QYUA", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "QYUA", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0217");
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0217A000");
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    FTBL.SetObjectFlagByIndex(1, 0);
    FTBL.SetObjectFlagByIndex(3, 0);
}

void RestoreQyuaResources(QyuaEventWork *work)
{
    FTBL.EV_cuffsSetFlags();
    Queue_EVT_AT_Cling(QYUA_KNIFE, 0, 0, 0, 0, 0, 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QYUA_PLAYER), 0);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(QYUA_TOGYU));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(QYUA_YUAN));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(QYUA_REN));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(QYUA_PLAYER));
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0217");
    FTBL.EV_ControlPackageResources(0, 6, "QYUA", 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "0217", 0, 0);
    ET_AT_KillWork(work->characters);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_NPC_NewLoadSw_0(1);
}
#endif
#else
#include "s2/areas/area_qyua.hpp"
#include "s2/step_event.hpp"
#include "s2/map_control.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/model_component.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/pak.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2 {
DateTime qyuaStartDate{};
// @verified - p
void QyuaInit() {
}
// @verified - p
void QyuaMain(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QyuaStartCallback, 4, 0x0b, sizeof(QyuaEventWork), QYUA_EVENT_TASK);
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(HLib::GetTaskParameter(task));
    work->arguments = argument ? reinterpret_cast<const void *>(argument) : QYUA_DEFAULT_ARGUMENTS;
    work->self = task;
    work->reserved08 = 0;
    InitializeQyuaResources(work);
}
// @verified - p
void InitializeQyuaResources(QyuaEventWork *work) {
    (void)EV_NPC_NewLoadSw(0);
    (void)EV_ResetFreeTalkFlags();
    shenGetCurrentDateTime(qyuaStartDate);
    EVT_SetQteRetryFrames(24);
    EVT_SetQteFailureCount(0);
    work->characters = Queue_EVT_AT_ReadChr(work->self, QYUA_CHARACTERS);
    work->authorHasPlayed = 0;
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("0217"));
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QYUA"));
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QYUA"), 0);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QYUA"), 0);
    (void)EV_FreeAuthorPak(1, "0217");
    (void)EV_SetNewSceneIndex();
    (void)EV_LoadCutsceneFromCurrentScene("0217A000");
    shenSetClockAndCompassRenderTo(1);
    QyuaInit();
    (void)EV_SetObjectFlagByIndex(1, 0);
    (void)EV_SetObjectFlagByIndex(3, 0);
}
// @verified - p
void QyuaStartCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    work->nextCallback = QyuaWaitOpeningAuthorCallback;
    work->succeeded = 0;
    LoadBlurModelForCharacter(QYUA_REN, "REN_B.MT7", 1.0F);
    (void)SetLowQualityHands(ctrlGetScenarioChara(QYUA_PLAYER));
    (void)SetLowQualityHands(ctrlGetScenarioChara(QYUA_REN));
    (void)SetLowQualityHands(ctrlGetScenarioChara(QYUA_YUAN));
    (void)SetLowQualityHands(ctrlGetScenarioChara(QYUA_TOGYU));
    EventCling knife{0, QYUA_KNIFE, QYUA_YUAN, 14, QYUA_KNIFE_POSITION, QYUA_KNIFE_ROTATION, 0, 0};
    EVT_AT_Cling(&knife);
    (void)EV_cuffsAttach(QYUA_PLAYER, 1, QYUA_REN, 0, 0.30000001192092896F);
    AT_SetLocalBaseChara(QYUA_DUMMY_RYO, QYUA_PLAYER);
    AT_SetLocalBaseChara(QYUA_DUMMY_REN, QYUA_REN);
    AT_SetLocalBaseChara(QYUA_DUMMY_YUAN, QYUA_YUAN);
    AT_SetLocalBaseChara(QYUA_DUMMY_TOGYU, QYUA_TOGYU);
    EVT_AT_SetPos(QYUA_BASE, QYUA_INVALID, QYUA_BASE_POSITION, 0);
    EVT_AT_SetAngY(QYUA_BASE, QYUA_INVALID, 0, 0);
    EVT_AT_SetPos(QYUA_DUMMY_RYO, QYUA_PLAYER, {}, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_RYO, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_DUMMY_REN, QYUA_REN, {}, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_REN, QYUA_REN, 0, 0);
    EVT_AT_SetPos(QYUA_DUMMY_YUAN, QYUA_YUAN, {}, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_YUAN, QYUA_YUAN, 0, 0);
    EVT_AT_SetPos(QYUA_DUMMY_TOGYU, QYUA_TOGYU, {}, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_TOGYU, QYUA_TOGYU, 0, 0);
    EVT_AT_SetPos(QYUA_TOGYU, QYUA_BASE, {-1000.0F, 0, 0}, 0);
    EVT_AT_SetAngY(QYUA_TOGYU, QYUA_BASE, 0, 0);
    (void)SetPluralControlFor(ctrlGetScenarioChara(QYUA_PLAYER), 0);
    EV_EnableCharacterDisplay(QYUA_PLAYER);
    EV_EnableCharacterDisplay(QYUA_REN);
    EV_EnableCharacterDisplay(QYUA_YUAN);
    EV_EnableCharacterDisplay(QYUA_AUTHOR_KNIFE);
    EV_DisableCharacterDisplay(QYUA_KNIFE);
    (void)EV_LoadAudio("0217A000", 1);
    (void)EV_spmBeginSampleExecute();
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QYUA_AUTHOR_START.name));
    if (work->authorHasPlayed == 1)
        EV_SetAuthorActiveTo(1);
    HLib::SetCurrentTaskTo(QyuaWaitOpeningAuthorCallback);
}
// @verified - p
void QyuaWaitOpeningAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    EV_DisableCharacterDisplay(QYUA_AUTHOR_KNIFE);
    EV_EnableCharacterDisplay(QYUA_KNIFE);
    (void)SetHighModelXYZ(ctrlGetScenarioChara(QYUA_PLAYER), 1, 0, 0);
    (void)SetHighModelXYZ(ctrlGetScenarioChara(QYUA_PLAYER), 1, 1, 0);
    (void)ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QYUA_PLAYER), 0, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
    (void)ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QYUA_PLAYER), 1, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
    (void)SetHighModelXYZ(ctrlGetScenarioChara(QYUA_REN), 1, 0, 0);
    (void)SetHighModelXYZ(ctrlGetScenarioChara(QYUA_REN), 1, 1, 0);
    (void)ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QYUA_REN), 0, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
    (void)ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QYUA_REN), 1, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
    (void)SetHighModelXYZ(ctrlGetScenarioChara(QYUA_YUAN), 1, 0, 0);
    (void)SetHighModelXYZ(ctrlGetScenarioChara(QYUA_YUAN), 1, 1, 0);
    (void)ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QYUA_YUAN), 0, reinterpret_cast<std::byte *>(yuanKnifeHandPose), 0);
    (void)ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QYUA_YUAN), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    const std::int32_t frames = BeginQyuaKnifeQte();
    EVT_AT_QTE(0x10, 0x10, 0, frames - 29);
    HLib::SetCurrentTaskTo(QyuaFirstResultCallback);
    HLib::SetupTaskDelay(frames);
}
// @verified - p
std::int32_t BeginQyuaKnifeQte() {
    EVT_AT_SetVoice("0217C002", 1, 35);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    SetClothFlag(ctrlGetScenarioChara(QYUA_PLAYER), 16, 1);
    toggleOsagFlags(ctrlGetScenarioChara(QYUA_REN));
    EVT_AT_SetCam(7800, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 0);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 0, 1, 0x40000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 0, 1, 0x40000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 0, 1, 0x40000000U, 0);
    EVT_AT_SetFaceLooks(QYUA_PLAYER, FacialExpressionId::Angry, 1, 1, 0);
    EVT_AT_SetFaceLooks(QYUA_REN, FacialExpressionId::Angry, 1, 1, 0);
    EVT_AT_SetFaceLooks(QYUA_YUAN, FacialExpressionId::Scheme, 1, 1, 0);
    return 60;
}
// @verified - p
void QyuaFirstResultCallback(void *) {
    std::int32_t frames;
    if ((EV_GetCurrentQteStatus() & 0xfff) == 0x10) {
        frames = BeginQyuaSecondKnifeQte();
        EVT_AT_QTE(4, 16, 0, frames - 29);
        HLib::SetCurrentTaskTo(QyuaSecondResultCallback);
    } else {
        frames = PlayQyuaFirstKnifeFailure();
        HLib::SetCurrentTaskTo(QyuaFailureAuthorCallback);
    }
    HLib::SetupTaskDelay(frames);
}
// @verified - p
std::int32_t BeginQyuaSecondKnifeQte() {
    EVT_AT_SetVoice("0217C003", 1, 50);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetCam(7801, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 0);
    EVT_AT_SetCam(7804, QYUA_BASE, QYUA_BASE, 0, 0, 60);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 60, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 60, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 60, 1, 0x44000000U, 0);
    return 70;
}
// @verified - p
std::int32_t PlayQyuaFirstKnifeFailure() {
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.17183F, 0, 0.42309001F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.77588F, 0, -0.12269F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetCam(7802, QYUA_DUMMY_YUAN, QYUA_DUMMY_YUAN, 0, 0, 0);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2562, 36, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2542, 36, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x258d, 36, 1, 0x44000000U, 0);
    return 60;
}
// @verified - p
void QyuaSecondResultCallback(void *) {
    std::int32_t frames;
    if ((EV_GetCurrentQteStatus() & 0xfff) == 4) {
        frames = BeginQyuaThirdKnifeQte();
        EVT_AT_QTE(256, 16, 0, frames - 29);
        HLib::SetCurrentTaskTo(QyuaThirdResultCallback);
    } else {
        frames = PlayQyuaSecondKnifeFailure();
        HLib::SetCurrentTaskTo(QyuaFailureAuthorCallback);
    }
    HLib::SetupTaskDelay(frames);
}
// @verified - p
std::int32_t BeginQyuaThirdKnifeQte() {
    EVT_AT_SetVoice("0217C004", 1, 25);
    EVT_AT_SetVoice("0217C007", 1, 80); // @text EN: " " | JA: "甘いわね。"
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetCam(7805, QYUA_BASE, QYUA_BASE, 0, 0, 0);
    EVT_AT_SetCam(7807, QYUA_BASE, QYUA_BASE, 0, 0, 20);
    EVT_AT_SetCam(7808, QYUA_DUMMY_YUAN, QYUA_DUMMY_YUAN, 0, 0, 60);
    EVT_AT_SetCam(7817, QYUA_DUMMY_YUAN, QYUA_DUMMY_YUAN, 0, 0, 92);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 130, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 130, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 130, 1, 0x44000000U, 0);
    EVT_AT_DispBlur(QYUA_REN, 7, 12, 1, 25, 10);
    return 115;
}
// @verified - p
std::int32_t PlayQyuaSecondKnifeFailure() {
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.079530001F, 0, 0.42789F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-2.0088601F, 0, 0.31832999F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetCam(7809, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 0);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2563, 112, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2543, 112, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x258e, 112, 1, 0x44000000U, 0);
    return 45;
}
// @verified - p
void QyuaThirdResultCallback(void *) {
    std::int32_t frames;
    if ((EV_GetCurrentQteStatus() & 0xfff) == 0x100) {
        frames = PlayQyuaKnifeSuccess();
        HLib::SetCurrentTaskTo(QyuaSuccessAuthorCallback);
    } else {
        frames = PlayQyuaFirstKnifeFailure();
        HLib::SetCurrentTaskTo(QyuaFailureAuthorCallback);
    }
    HLib::SetupTaskDelay(frames);
}
// @verified - p
std::int32_t PlayQyuaKnifeSuccess() {
    EVT_AT_SetVoice("0217A002", 1, 5);
    EVT_AT_SetVoice("0217B002", 1, 40); // @text EN: " " | JA: "おりゃっ！"
    EVT_AT_SetVoice("0217C006", 1, 70);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), QYUA_EVENT_TASK);
    (void)RegisterEventTask(task);
    *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task)) = {1, QYUA_KNIFE, 1, 20.0F};
    (void)EVT_AT_SetSound(1954, 30);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 15);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 15);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 15);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 15);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 15);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 15);
    EVT_AT_SetCam(7816, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 0);
    EVT_AT_SetCam(7811, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 15);
    EVT_AT_SetCam(7812, QYUA_DUMMY_REN, QYUA_DUMMY_REN, 0, 0, 40);
    EVT_AT_SetCam(7813, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 95);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 245, 0.1F, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 245, 1, 0x44000000U, 15);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 245, 0.1F, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 245, 1, 0x44000000U, 15);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 245, 0.1F, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 245, 1, 0x44000000U, 15);
    EVT_AT_DispBlur(QYUA_PLAYER, 6, 12, 1, 8, 15);
    EVT_AT_DispBlur(QYUA_REN, 7, 12, 1, 18, 38);
    return 135;
}
// @verified - p
void QyuaFailureAuthorCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    EV_EnableCharacterDisplay(QYUA_YUAN);
    EV_DisableCharacterDisplay(QYUA_KNIFE);
    EV_EnableCharacterDisplay(QYUA_AUTHOR_KNIFE);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QYUA_AUTHOR_FAILURE.name));
    if (work->authorHasPlayed == 1)
        EV_SetAuthorActiveTo(1);
    work->authorHasPlayed = 1;
    work->nextCallback = QyuaRetryOrFinishCallback;
    work->succeeded = 0;
    HLib::SetCurrentTaskTo(QyuaWaitResultAuthorCallback);
}
// @verified - p
void QyuaSuccessAuthorCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    EV_EnableCharacterDisplay(QYUA_YUAN);
    EV_DisableCharacterDisplay(QYUA_KNIFE);
    EV_DisableCharacterDisplay(QYUA_AUTHOR_KNIFE);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QYUA_AUTHOR_SUCCESS.name));
    work->succeeded = 1;
    work->nextCallback = QyuaRetryOrFinishCallback;
    HLib::SetCurrentTaskTo(QyuaWaitResultAuthorCallback);
}
// @verified - p
void QyuaWaitResultAuthorCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        (void)SetPluralControlFor(ctrlGetScenarioChara(QYUA_PLAYER), 1);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(work->nextCallback);
        HLib::SetupTaskDelay(60);
    }
}
void QyuaRetryOrFinishCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    if (!work->succeeded || g_currentMap.name == AREA_QYUA.name) {
        const std::uint32_t failures = EVT_GetQteFailureCount();
        if (failures < 2)
            EVT_SetQteFailureCount(failures + 1);
        SetCurrentDateTimeTo(qyuaStartDate);
        (void)shenSetTimeSimulationTo(1);
        HLib::SetCurrentTaskTo(QyuaStartCallback);
    } else
        FinishQyuaEvent(work);
}
void FinishQyuaEvent(QyuaEventWork *work) {
    RestoreQyuaResources(work);
    EV_CleanupMapControlTask();
    HLKillOtherHierarchyTask(work->self);
    EV_stepSetFlags(0, 0x13c);
    if (g_currentMap.name == AREA_QYUA.name) {
        scenSetTaskFlags(6);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
// @verified - p
void RestoreQyuaResources(QyuaEventWork *work) {
    EV_cuffsSetFlags();
    EventCling knife{0, QYUA_KNIFE, shen_name{}, 0, {}, {}, 1, 0};
    EVT_AT_Cling(&knife);
    (void)SetPluralControlFor(ctrlGetScenarioChara(QYUA_PLAYER), 0);
    (void)hndlSetFlags(ctrlGetScenarioChara(QYUA_TOGYU));
    (void)hndlSetFlags(ctrlGetScenarioChara(QYUA_YUAN));
    (void)hndlSetFlags(ctrlGetScenarioChara(QYUA_REN));
    (void)hndlSetFlags(ctrlGetScenarioChara(QYUA_PLAYER));
    FinishLoading();
    shenSetClockAndCompassRenderTo(0);
    (void)sbtLoadProgram();
    (void)EV_spmBeginSampleExecute();
    (void)EV_FreeAuthorPak(0, "0217");
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QYUA"), 0);
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0217"), 0);
    ET_AT_KillWork(work->characters);
    SetPlayerControlEnabled(0);
    (void)EV_NPC_NewLoadSw(1);
}
const ScriptAreaDefinition qyuaScriptDefinition{.areaId = AREA_QYUA, .field04 = 0, .init = QyuaInit, .main = QyuaMain};
} // namespace shenmue::s2
#endif
