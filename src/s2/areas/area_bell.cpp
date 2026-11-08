#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_bell.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
static BellWork *bellWork;

void BellMain(const unsigned int *argument)
{
    HLibTask *task;
    FTBL.DebugLogToFileRemoved("BELL_EVENT mode[%d]\n", *argument);
    if (*argument == 0) {
        FTBL.SetPlayerControlEnabled(0);
        FTBL.EV_SetNewSceneIndex();
        FTBL.EV_setHoldStatesFlags(0);
        FTBL.DebugLogToFileRemoved("=============== YAN_KANPOUYA_BELL ==============\n");
        task = FTBL.HLib_EnqueueTaskWithParameter(BellLoadNpcCallback, 5, 11, sizeof(BellWork), TASK_EVNT);
        bellWork = (BellWork *)FTBL.HLib_GetTaskParameter(task);
        bellWork->task = task;
        bellWork->elapsed = 0;
        bellWork->busy = 0;
        bellWork->escapeTalks = 0;
        bellWork->conversations = 0;
        FTBL.EV_SetPlayerFlag(12, 679, 1);
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.CheckWarmStartNpcRoute(BELL_REN);
        FTBL.CheckWarmStartNpcRoute(BELL_YANG);
        BELL_SET_GRAVITY(BELL_RYO, 0);
        FTBL.EV_LoadCutsceneFromCurrentScene("0202A000.smp");
        FTBL.EV_FreeAuthorPak(1, bellAuthorPak);
        FTBL.EV_FreeAuthorPak(1, bellYangAuthorPak);
        FTBL.EV_SetCanDoAreaJumpTo(0);
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)bellPackage, 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 4, bellPackage, 0, 0);
        FTBL.EV_setHoldStatesFlags(0);
    }
    bellWork->command = *argument;
}

const char bellAuthorPak[] = "0202";
const char bellYangAuthorPak[] = "0203";
const char bellPackage[] = "BELL";
#else
static BellSearchTarget bellTargets[2];

void BellLoadNpcCallback(void *data)
{
    if (FTBL.ForceLoadNpc(BELL_REN)) {
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(BELL_REN));
        FTBL.EV_SetCharacterAlphaEnabled(BELL_REN, 0);
        FTBL.HLib_SetCurrentTaskTo(BellInteractionInitCallback);
        QueueSkAuthorStart(((BellWork *)data)->task, BELL_AUTHOR_START, 0, 0, 0);
    }
}

void BellInteractionInitCallback(void *data)
{
    BellWork *work = (BellWork *)data;
    Vector3 position;
    Vector3i rotation;
    position.x = 207.65f;
    position.y = 103.6f;
    position.z = 195.1f;
    rotation.x = 0;
    rotation.y = 0xe666;
    rotation.z = 0;
    FTBL.EV_AT_SetAngY_0(BELL_REN, BELL_ROTATION_FLAGS, &rotation, 0);
    FTBL.EV_AT_SetPos(BELL_REN, BELL_POSITION_FLAGS, &position);
    position.x = 208.73f;
    position.y = 103.6f;
    position.z = 195.91f;
    rotation.x = 0;
    rotation.y = 0xe000;
    rotation.z = 0;
    FTBL.EV_AT_SetAngY_0(BELL_RYO, BELL_ROTATION_FLAGS, &rotation, 0);
    FTBL.EV_AT_SetPos(BELL_RYO, BELL_POSITION_FLAGS, &position);
    EVT_AT_SetMot(BELL_REN, 0x2182, 0.0f, 1.0f, 0x41000000U, 0);
    EVT_AT_SetMot(BELL_RYO, 0xf003, 0.0f, 1.0f, 0x40000000U, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(BELL_REN));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(BELL_REN), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(BELL_REN), 1, 1, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(BELL_REN), 0, (void *)areaLoopHandPoses[BELL_RESTING_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(BELL_REN), 1, (void *)areaLoopHandPoses[BELL_RESTING_HAND_POSE], 0);
    bellTargets[0].name = BELL_TARGET;
    bellTargets[0].flags = 0;
    bellTargets[0].field08 = 0;
    bellTargets[0].position.x = 208.64f;
    bellTargets[0].position.y = 104.5f;
    bellTargets[0].position.z = 194.73f;
    bellTargets[0].lockonAngle = 0.3f;
    bellTargets[0].yaw = -32768;
    bellTargets[0].yawLimit = -32768;
    bellTargets[1].name = 0;
    bellTargets[1].flags = 0;
    bellTargets[1].field08 = 0;
    bellTargets[1].position.x = 0.0f;
    bellTargets[1].position.y = 0.0f;
    bellTargets[1].position.z = 0.0f;
    bellTargets[1].lockonAngle = 0.0f;
    bellTargets[1].yaw = 0;
    bellTargets[1].yawLimit = 0;
    FTBL.srchSetTarget(2, bellTargets);
    position.x = 208.64f;
    position.y = 104.5f;
    position.z = 194.73f;
    rotation.x = 0;
    rotation.y = 0;
    rotation.z = 0;
    FTBL.EV_AT_SetAngY_0(BELL_ANCHOR, BELL_ROTATION_FLAGS, &rotation, 0);
    FTBL.EV_AT_SetPos(BELL_ANCHOR, BELL_POSITION_FLAGS, &position);
    FTBL.EV_DisableCharacterDisplay(BELL_SHUTTER);
    FTBL.EVS_LoopCamera(0, 0, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    work->renRouteCompletion = FTBL.IsNpcAnimationActive(BELL_REN);
    FTBL.UnknownCharaFunc(BELL_REN);
    BELL_SET_GRAVITY(BELL_RYO, 1);
    EVT_AT_MenCtrl(10, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.HLib_SetCurrentTaskTo(BellInteractionCallback);
}

void BellInteractionCallback(void *data)
{
    BellWork *work = (BellWork *)data;
    Vector3 position;
    Vector3i rotation;
    float reflector[4] = {1.0f, 0.3f, 0.3f, 0.3f};
    int duration;
    switch (work->command) {
    case 0:
        if (!work->busy && !FTBL.GetShouldRenderIcons(0))
            work->elapsed++;
        switch (work->elapsed) {
        case 900:
            FTBL.SetPlayerControlEnabled(0);
            if (FTBL.IsItemSelectable())
                FTBL.EV_SetMemoStateTo(1);
            EVT_AT_SetMot(BELL_RYO, 0xf003, 0.0f, 1.0f, 0x80000000U, 0);
            EVT_AT_SetVoice("0202C032", 0);
            EVT_AT_SetVoice("0202C026", 70);
            break;
        case 1030:
            FTBL.SetPlayerControlEnabled(1);
            break;
        case 1800:
            FTBL.SetPlayerControlEnabled(0);
            if (FTBL.IsItemSelectable())
                FTBL.EV_SetMemoStateTo(1);
            EVT_AT_SetMot(BELL_RYO, 0xf003, 0.0f, 1.0f, 0x80000000U, 0);
            EVT_AT_SetVoice("0202C033", 0);
            EVT_AT_SetVoice("0202C025", 60);
            break;
        case 1920:
            FTBL.SetPlayerControlEnabled(1);
            break;
        case 2700:
            FTBL.SetPlayerControlEnabled(0);
            if (FTBL.IsItemSelectable())
                FTBL.EV_SetMemoStateTo(1);
            EVT_AT_SetMot(BELL_RYO, 0xf003, 0.0f, 1.0f, 0x80000000U, 0);
            EVT_AT_SetVoice("0202C034", 0);
            EVT_AT_SetVoice("0202C035", 70);
            break;
        case 2840:
            FTBL.SetPlayerControlEnabled(1);
            break;
        }
        if (work->elapsed < 3600)
            return;
        if (FTBL.IsItemSelectable())
            FTBL.EV_SetMemoStateTo(1);
        FTBL.srchReset();
        FTBL.EV_refreshPadControlData(0, 1);
        FTBL.SetPlayerControlEnabled(0);
        position.x = 207.7f;
        position.y = 103.6f;
        position.z = 194.96f;
        rotation.x = 0;
        rotation.y = 0xe666;
        rotation.z = 0;
        FTBL.EV_AT_SetAngY_0(BELL_REN, BELL_ROTATION_FLAGS, &rotation, 0);
        FTBL.EV_AT_SetPos(BELL_REN, BELL_POSITION_FLAGS, &position);
        FTBL.EV_DisableCharacterDisplay(BELL_RYO);
        FTBL.GetCharaPosition(BELL_ANCHOR, -1, &position, 0x40000000U);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(BELL_REN), &position, 1);
        FTBL.SetScenarioReflectorBoardValues(BELL_REN, reflector);
        FTBL.SetRefBoardParamsForCharaById(BELL_REN, 1);
        FTBL.EV_PlayDialogue("0202C031");
        EVT_AT_SetMot(BELL_REN, 0x2183, 0.0f, 1.0f, 0x40000000U, 0);
        work->phase = 4;
        FTBL.EV_stepSetFlags(412, 0);
        FTBL.EVS_LoopCamera(110, BELL_ANCHOR, BELL_ANCHOR);
        EVT_AT_SetSample(30, 1, 120);
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 120);
        FTBL.HLib_SetCurrentTaskTo(BellRingCallback);
        FTBL.HLib_SetupTaskDelay(150);
        break;
    case 1:
        FTBL.EV_stepSetFlags(411, 0);
        FTBL.SetPlayerControlEnabled(0);
        work->phase = 0;
        work->busy = 1;
        FTBL.HLib_SetCurrentTaskTo(BellRingCallback);
        break;
    case 2:
        FTBL.SetPlayerControlEnabled(0);
        FTBL.srchReset();
        position.x = 208.2f;
        position.y = 103.6f;
        position.z = 196.0f;
        rotation.x = 0;
        rotation.y = 0x6000;
        rotation.z = 0;
        FTBL.EV_AT_SetAngY_0(BELL_RYO, BELL_ROTATION_FLAGS, &rotation, 0);
        FTBL.EV_AT_SetPos(BELL_RYO, BELL_POSITION_FLAGS, &position);
        switch (work->escapeTalks) {
        case 0:
            EVT_AT_SetVoice("0202C028", 0);
            EVT_AT_SetVoice("0202C036", 60);
            FTBL.EVS_LoopCamera(120, BELL_ANCHOR, BELL_ANCHOR);
            break;
        case 1:
            EVT_AT_SetVoice("0202C037", 0);
            EVT_AT_SetVoice("0202C038", 70);
            FTBL.EVS_LoopCamera(121, BELL_ANCHOR, BELL_ANCHOR);
            break;
        case 2:
            EVT_AT_SetVoice("0202C039", 0);
            EVT_AT_SetVoice("0202C025", 70);
            FTBL.EVS_LoopCamera(122, BELL_ANCHOR, BELL_ANCHOR);
            break;
        default:
            EVT_AT_SetVoice("0202C039", 0);
            EVT_AT_SetVoice("0202C025", 70);
            switch ((unsigned char)FTBL.GetRandomNumberInRange(3.0f)) {
            case 0:
                FTBL.EVS_LoopCamera(120, BELL_ANCHOR, BELL_ANCHOR);
                break;
            case 1:
                FTBL.EVS_LoopCamera(121, BELL_ANCHOR, BELL_ANCHOR);
                break;
            default:
                FTBL.EVS_LoopCamera(122, BELL_ANCHOR, BELL_ANCHOR);
                break;
            }
            break;
        }
        work->escapeTalks++;
        work->command = 0;
        work->busy = 1;
        EVT_RepeatCall(BellEyeTowardBell, 0, 30, 1);
        EVT_RepeatCall(BellResetEye, 0, 120, 1);
        EVT_AT_SetPoint(BELL_RYO, BELL_REN, 5, 1, 0, 30, 0, 0x20000000U);
        EVT_AT_SetPoint(BELL_RYO, 0, 0, 1, 0, 120, 0, 0x10000000U);
        EVT_AT_SetPoint(BELL_REN, BELL_RYO, 5, 1, 0, 1, 0, 0x20000000U);
        EVT_AT_SetPoint(BELL_REN, 0, 0, 1, 0, 120, 0, 0x10000000U);
        EVT_AT_SetMot(BELL_RYO, 0xf003, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_RepeatCall(BellRestoreControl, work, 120, 1);
        break;
    case 3:
        if (work->busy || !FTBL.ryoNotBusy())
            break;
        FTBL.SetPlayerControlEnabled(0);
        FTBL.srchReset();
        switch (work->conversations) {
        case 0:
            EVT_AT_SetCam(210, BELL_RYO, BELL_RYO, 0, 0, 0);
            EVT_AT_SetVoice("0202A018", 0);
            EVT_AT_SetCam(220, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 80);
            EVT_AT_SetVoice("0202C013", 80);
            duration = 170;
            break;
        case 1:
            EVT_AT_SetCam(210, BELL_RYO, BELL_RYO, 0, 0, 0);
            EVT_AT_SetVoice("0202A019", 0);
            EVT_AT_SetVoice("0202A020", 20);
            EVT_AT_SetCam(250, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 100);
            EVT_AT_SetVoice("0202C014", 100);
            EVT_AT_SetCam(230, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 180);
            EVT_AT_SetVoice("0202C015", 180);
            duration = 280;
            break;
        case 2:
            EVT_AT_SetCam(200, BELL_RYO, BELL_RYO, 0, 0, 0);
            EVT_AT_SetVoice("0202A019", 0);
            EVT_AT_SetVoice("0202A022", 20);
            EVT_AT_SetCam(220, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 90);
            EVT_AT_SetVoice("0202C017", 90);
            EVT_AT_SetCam(210, BELL_RYO, BELL_RYO, 0, 0, 160);
            EVT_AT_SetVoice("0202A023", 160);
            EVT_AT_SetCam(230, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 220);
            EVT_AT_SetVoice("0202C018", 220);
            duration = 310;
            break;
        case 3:
            EVT_AT_SetCam(200, BELL_RYO, BELL_RYO, 0, 0, 0);
            EVT_AT_SetVoice("0202A019", 0);
            EVT_AT_SetVoice("0202A024", 20);
            EVT_AT_SetCam(250, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 80);
            EVT_AT_SetVoice("0202C019", 80);
            EVT_AT_SetVoice("0202A025", 210);
            EVT_AT_SetCam(240, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 250);
            EVT_AT_SetVoice("0202C020", 250);
            EVT_AT_SetVoice("0202C021", 350);
            EVT_AT_SetCam(210, BELL_RYO, BELL_RYO, 0, 0, 480);
            EVT_AT_SetVoice("0202A026", 480);
            EVT_AT_SetVoice("0202A027", 540);
            EVT_AT_SetCam(230, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 610);
            EVT_AT_SetVoice("0202C022", 610);
            EVT_AT_SetVoice("0202C023", 680);
            duration = 850;
            break;
        case 4:
            EVT_AT_SetCam(250, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 0);
            EVT_AT_SetVoice("0202A019", 0);
            EVT_AT_SetVoice("0202C024", 20);
            EVT_AT_SetCam(220, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 130);
            EVT_AT_SetVoice("0202C025", 130);
            EVT_AT_SetVoice("0202A009", 200);
            duration = 240;
            break;
        default:
            if ((unsigned char)FTBL.GetRandomNumberInRange(2.0f) == 0)
                EVT_AT_SetVoice("0202A019", 0);
            else
                EVT_AT_SetVoice("0202A028", 0);
            switch ((unsigned char)FTBL.GetRandomNumberInRange(3.0f)) {
            case 0:
                EVT_AT_SetCam(240, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 20);
                EVT_AT_SetVoice("0202C024", 20);
                duration = 140;
                break;
            case 1:
                EVT_AT_SetCam(230, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 20);
                EVT_AT_SetVoice("0202C026", 20);
                duration = 110;
                break;
            default:
                EVT_AT_SetCam(220, BELL_ANCHOR, BELL_ANCHOR, 0, 0, 20);
                EVT_AT_SetVoice("0202C025", 20);
                duration = 100;
                break;
            }
            break;
        }
        work->conversations++;
        work->command = 0;
        work->busy = 1;
        EVT_RepeatCall(BellEyeTowardBell, 0, 30, 1);
        EVT_RepeatCall(BellResetEye, 0, 100, 1);
        EVT_AT_SetPoint(BELL_RYO, BELL_REN, 5, 1, 0, 30, 0, 0x20000000U);
        EVT_AT_SetPoint(BELL_RYO, BELL_REN, 5, 1, 0, duration, 0, 0x10000000U);
        EVT_AT_SetMot(BELL_RYO, 0xf003, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_RepeatCall(BellRestoreControl, work, duration, 1);
        FTBL.HLib_SetupTaskDelay(duration);
        break;
    }
}

void BellRingCallback(void *data)
{
    BellWork *work = (BellWork *)data;
    Vector3 position;
    Vector3i rotation, handRotation;
    switch (work->phase) {
    case 0:
        FTBL.EVS_LoopCamera(100, BELL_ANCHOR, BELL_ANCHOR);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(BELL_RYO));
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(BELL_REN));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(BELL_RYO), 1, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(BELL_REN), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(BELL_REN), 1, 1, 0);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(BELL_REN), 0, (void *)areaLoopHandPoses[BELL_RINGING_HAND_POSE], 0);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(BELL_REN), 1, (void *)areaLoopHandPoses[BELL_RINGING_HAND_POSE], 0);
        handRotation.x = 0;
        handRotation.y = 4732;
        handRotation.z = 1820;
        FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(BELL_RYO), 1, 21, &handRotation);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(BELL_RYO), 1, (void *)areaLoopHandPoses[BELL_RINGING_HAND_POSE], 10);
        position.x = 208.5f;
        position.y = 104.5f;
        position.z = 194.5f;
        FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(BELL_RYO), &position, 20, 2);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(BELL_RYO), &position, 1);
        EVT_RepeatCall(BellResetEye, 0, 170, 1);
        BELL_SET_GRAVITY(BELL_RYO, 0);
        BELL_SET_GRAVITY(BELL_REN, 0);
        position.x = 209.1f;
        position.y = 103.57f;
        position.z = 194.75f;
        rotation.x = 0;
        rotation.y = 6189;
        rotation.z = 0;
        FTBL.EV_AT_SetAngY_0(BELL_RYO, BELL_ROTATION_FLAGS, &rotation, 0);
        FTBL.EV_AT_SetPos(BELL_RYO, BELL_POSITION_FLAGS, &position);
        EVT_AT_SetMot(BELL_RYO, 0x2185, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_MenCtrl(10, 0, 0, 0, 0, 255, 0, 0, 0, 100);
        EVT_AT_SetSample(30, 1, 80);
        FTBL.HLib_SetupTaskDelay(110);
        work->phase = 4;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        if (!FTBL.ForceLoadNpc(BELL_YANG))
            return;
        FTBL.EV_SetCharacterAlphaEnabled(BELL_YANG, 0);
        FTBL.EV_SetCharacterAlphaEnabled(BELL_REN, 0);
        FTBL.EV_SetCharacterAlphaEnabled(BELL_RYO, 0);
        FTBL.EV_stepSetFlags(305, 0);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(BELL_RYO), 0, 0);
        FTBL.EV_EnableCharacterDisplay(BELL_REN);
        FTBL.EV_EnableCharacterDisplay(BELL_RYO);
        FTBL.EV_DisableCharacterDisplay(BELL_SHUTTER);
        FTBL.EV_DisableCharacterDisplay(BELL_SIGN);
        FTBL.SetDoorSubjectEnabled(BELL_DOOR, 0);
        FTBL.TelCode_Enqueue();
        if (work->renRouteCompletion)
            FTBL.npcUpdateFlags(BELL_REN);
        else
            FTBL.UnknownCharaFunc(BELL_REN);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EVT_AT_SetSample(30, 0, 0);
        FTBL.EV_LoadAudio("0202A000", 0);
        FTBL.EV_LoadCutsceneFromCurrentScene("0202A000");
        QueueSkAuthorStart(work->task, BELL_AUTHOR_RING, 0, 0, 0);
        work->phase++;
        break;
    case 5:
        FTBL.EV_LoadAudio("0203A000", 0);
        FTBL.EV_LoadCutsceneFromCurrentScene("0203A000");
        QueueSkAuthorStart(work->task, BELL_AUTHOR_YANG, 0, 0, 0);
        work->phase++;
        break;
    case 6:
        FTBL.HLib_SetCurrentTaskTo(BellExitCallback);
        work->phase++;
        break;
    }
}

void BellRestoreControl(void *data)
{
    ((BellWork *)data)->busy = 0;
    FTBL.SetPlayerControlEnabled(1);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(BELL_RYO), 0, 0);
    FTBL.EVS_LoopCamera(0, 0, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
}

void BellEyeTowardBell(void *data)
{
    Vector3 point;
    point.x = 208.1f;
    point.y = 105.0f;
    point.z = 194.9f;
    FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(BELL_RYO), &point, 10, 2);
}

void BellResetEye(void *data)
{
    FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(BELL_RYO), 0, 16, 0);
}

void BellExitCallback(void *data)
{
    BellWork *work = (BellWork *)data;
    FTBL.DebugLogToFileRemoved("=============== YAN_KANPOUYA_BELL_END ==============\n");
    FTBL.EV_setHoldStatesFlags(1);
    FTBL.SetDoorSubjectEnabled(BELL_DOOR, 1);
    FTBL.EV_FreeAuthorPak(0, bellAuthorPak);
    FTBL.EV_FreeAuthorPak(0, bellYangAuthorPak);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.EV_setHoldStatesFlags(1);
    FTBL.srchClearTarget(2);
    FTBL.EV_SetCanDoAreaJumpTo(1);
    BELL_SET_GRAVITY(BELL_RYO, 1);
    BELL_SET_GRAVITY(BELL_REN, 1);
    FTBL.EV_SetCharacterAlphaEnabled(BELL_REN, 1);
    FTBL.EV_SetCharacterAlphaEnabled(BELL_YANG, 1);
    FTBL.EV_SetCharacterAlphaEnabled(BELL_RYO, 1);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(BELL_REN));
    FTBL.npcLockUnlock(BELL_REN);
    FTBL.npcLockUnlock(BELL_YANG);
    FTBL.npcFree(BELL_REN);
    FTBL.npcFree(BELL_YANG);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.DeleteMotion(work->motion);
    FTBL.EVS_LoopCamera(0, 0, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    FTBL.EV_ControlPackageResources(0, 6, bellPackage, 0, 0);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.EV_scenQueueLoad(g_actNumber, BELL_RETURN_SCENE, 51);
}
#endif
#else
#include "s2/areas/area_bell.hpp"
#include "s2/areas/area_bell_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/event_call.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sk_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/telephone_cord.hpp"
#include "s2/ui.hpp"
#include <array>
#include <bit>

namespace shenmue::s2 {
namespace bell_event {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name ren{SHEN_NAME(REN_)};
constexpr shen_name yang{SHEN_NAME(YAN_)};
constexpr shen_name anchor{SHEN_NAME(a212)};
constexpr shen_name bellDoor{SHEN_NAME(R107)};
BellEventWork *work{};
std::array<SearchTarget, 2> targets{};
void Gravity(shen_name character, bool enabled) {
    const auto state = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    EV_SetCharaControllerFlags(character, 2, state);
    EV_SetCharaControllerFlags(character, 3, state);
}
void Hand(shen_name character, int side, std::array<std::int32_t, 58> &command, int ticks) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(character), side, reinterpret_cast<std::byte *>(command.data()), static_cast<std::int16_t>(ticks));
}
void Pose(shen_name character, const Vector3 &position, std::int32_t yaw) {
    EV_AT_SetAngY(character, EventTransformFlags::AllAxes, {0, yaw, 0});
    EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}
void Motion(shen_name character, std::uint32_t animation, std::uint32_t flags) {
    EventSetMotion request{0, character, animation, 0, 1, flags, 0};
    EVT_AT_SetMot_Callback(&request);
}
void Voice(const char *voice, int delay = 0) {
    EVT_AT_SetVoice(voice, delay);
}
void Camera(int id, shen_name character, int delay = 0) {
    EVT_AT_SetCam(id, character, character, 0, 0, delay);
}
void EyeTowardBell(void *) {
    const Vector3 point{208.10001f, 105, 194.89999f};
    SetEyeToChara(ctrlGetCharaByName(ryo), &point, 10, 2);
}
void ResetEye(void *) {
    SetEyeToChara(ctrlGetCharaByName(ryo), nullptr, 16, 0);
}
void RestoreControl(void *parameter) {
    using enum CameraState;
    auto *state = static_cast<BellEventWork *>(parameter);
    state->busy = 0;
    SetPlayerControlEnabled(1);
    mtwkUpdateWithPosition(scenFindMotionwork(ryo), nullptr, 0);
    EVS_LoopCamera(0, {}, {});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
}
void QueuePoint(shen_name character, shen_name target, int bone, std::uint32_t flags, int delay) {
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetPoint_Callback, 6, 0x0b, sizeof(EventSetPoint), AREA_EVENT_TASK);
    RegisterEventTask(task);
    auto *point = static_cast<EventSetPoint *>(HLib::GetTaskParameter(task));
    *point = {character, target, bone, 1, 0, 0, flags, 0, 1, static_cast<float>(delay)};
}
int Conversation(std::uint8_t index) {
    switch (index) {
    case 0:
        Camera(210, ryo);
        camCtrlDisableFilmBars();
        Voice("0202A018"); // @text EN: "This place sure doesn't look like\na snitch's dwelling." | JA: "これで本当に情報屋なのか？"
        Camera(220, anchor, 80);
        Voice("0202C013", 80); // @text EN: "Fools you, doesn't it?" | JA: "この店からじゃ\n想像できねえだろ。"
        return 170;
    case 1:
        Camera(210, ryo);
        camCtrlDisableFilmBars();
        Voice("0202A019"); // @text EN: "Ren." | JA: "レン。"
        Voice("0202A020", 20); // @text EN: "How does Yang get his information?" | JA: "いったいどうやって\nヤンは情報を？"
        Camera(250, anchor, 100);
        Voice("0202C014", 100); // @text EN: "His sources are the best in Hong Kong." | JA: "ヤツの仕入れルートは\n香港一だ。"
        Camera(230, anchor, 180);
        Voice("0202C015", 180); // @text EN: "He gets the prohibited stuff\nthrough the black market." | JA: "禁制品だろうと\n闇ルートで仕込んでくる。"
        return 280;
    case 2:
        Camera(200, ryo);
        camCtrlDisableFilmBars();
        Voice("0202A019"); // @text EN: "Ren." | JA: "レン。"
        Voice("0202A022", 20); // @text EN: "I've never seen these things before." | JA: "みんな見かけない物ばかりだな。"
        Camera(220, anchor, 90);
        Voice("0202C017", 90); // @text EN: "Aren't there any medicine shops in Japan?" | JA: "日本に漢方薬屋はねえのか？"
        Camera(210, ryo, 160);
        Voice("0202A023", 160); // @text EN: "There are.\nBut I've never been inside one." | JA: "あるけど入ったことはない。"
        Camera(230, anchor, 220);
        Voice("0202C018", 220); // @text EN: "Well, you don't look like you'd\never get sick." | JA: "てめえは病気に\n縁がなさそうだしな。"
        return 310;
    case 3:
        Camera(200, ryo);
        camCtrlDisableFilmBars();
        Voice("0202A019"); // @text EN: "Ren." | JA: "レン。"
        Voice("0202A024", 20); // @text EN: "Do the medicines here work?" | JA: "ここの薬は効くのか？"
        Camera(250, anchor, 80);
        Voice("0202C019", 80); // @text EN: "They're quite well known.\nThere's even Dongzhongxiacao here." | JA: "その筋じゃ有名だ。\n冬虫夏草とかもあるぜ。"
        Voice("0202A025", 210); // @text EN: "What is that?" | JA: "なんだ、それは？"
        Camera(240, anchor, 250);
        Voice("0202C020", 250); // @text EN: "It's a plant that grows out\nof an insect in the ground." | JA: "土ん中にいる虫とかに\n寄生して育つ植物だ。"
        Voice("0202C021", 350); // @text EN: "An insect in the winter that\nturns into a plant by the summer." | JA: "冬には虫だったものが\n夏には草になっている。"
        Camera(210, ryo, 480);
        Voice("0202A026", 480); // @text EN: "Dongzhongxiacao=@" | JA: "だから冬虫夏草か。"
        Voice("0202A027", 540); // @text EN: "And that becomes a medicine?" | JA: "そんなものも薬になるのか？"
        Camera(230, anchor, 610);
        Voice("0202C022", 610); // @text EN: "I heard it's highly nutritious." | JA: "滋養強壮に効くって話だ。"
        Voice("0202C023", 680); // @text EN: "But Yang here sells fake stuff too,\ndepending on the customer." | JA: "まあ、ヤンの野郎は客によっちゃ、\nまがい物をつかませるからな。"
        return 850;
    case 4:
        Camera(250, anchor);
        camCtrlDisableFilmBars();
        Voice("0202A019"); // @text EN: "Ren." | JA: "レン。"
        Voice("0202C024", 20); // @text EN: "Enough talking now." | JA: "もう、くっちゃべるのは\nいいだろ。"
        Camera(220, anchor, 130);
        Voice("0202C025", 130); // @text EN: "Just ring the bell." | JA: "さっさと呼び鈴を押せ。"
        Voice("0202A009", 200); // @text EN: "Yeah." | JA: "ああ。"
        return 240;
    default: {
        Voice(static_cast<std::uint8_t>(static_cast<std::int32_t>(GetRandomNumberInRange(2))) == 0 ? "0202A019" : "0202A028"); // @text 0202A019 EN: "Ren." | JA: "レン。" ; 0202A028 EN: "Hey." | JA: "おい。"
        const std::uint8_t choice = static_cast<std::uint8_t>(static_cast<std::int32_t>(GetRandomNumberInRange(3)));
        if (choice == 0) {
            Camera(240, anchor, 20);
            Voice("0202C024", 20); // @text EN: "Enough talking now." | JA: "もう、くっちゃべるのは\nいいだろ。"
            return 140;
        }
        if (choice == 1) {
            Camera(230, anchor, 20);
            Voice("0202C026", 20); // @text EN: "Ring the damn bell." | JA: "呼び鈴を押しゃあ\nいいんだよ。"
            return 110;
        }
        Camera(220, anchor, 20);
        Voice("0202C025", 20); // @text EN: "Just ring the bell." | JA: "さっさと呼び鈴を押せ。"
        return 100;
    }
    }
}
} // namespace bell_event

void BELL_init_func() {
    bell_event::targets = {};
    bell_event::work = nullptr;
}
void BELL_main_func(std::uintptr_t argument) {
    using namespace bell_event;
    const auto command = *reinterpret_cast<const std::uint32_t *>(argument);
    if (!command) {
        SetPlayerControlEnabled(0);
        ReleaseNpcScriptRouteOwner();
        SetScenarioFollowCharacter({});
        EV_SetNewSceneIndex();
        EV_setHoldStatesFlags(0);
        auto *task = HLib::EnqueueTaskWithParameter(BellLoadNpcCallback, 5, 0x0b, sizeof(BellEventWork), AREA_EVENT_TASK);
        work = static_cast<BellEventWork *>(HLib::GetTaskParameter(task));
        work->task = task;
        work->elapsed = 0;
        work->busy = 0;
        work->escapeTalks = 0;
        work->conversations = 0;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 679, 1);
        EV_NPC_NewLoadSw(0);
        EV_CheckWarmStartNpcRoute(ren);
        EV_CheckWarmStartNpcRoute(yang);
        Gravity(ryo, false);
        EV_LoadCutsceneFromCurrentScene("0202A000.smp");
        EV_FreeAuthorPak(1, "0202");
        EV_FreeAuthorPak(1, "0203");
        EV_SetCanDoAreaJumpTo(0);
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("BELL"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("BELL"), 0);
        EV_setHoldStatesFlags(0);
    }
    work->command = command;
}
void BellLoadNpcCallback(void *parameter) {
    using namespace bell_event;
    if (!EV_ForceLoadNpc(ren))
        return;
    SetLowQualityHands(ctrlGetCharaByName(ren));
    EV_SetCharacterAlphaEnabled(ren, false);
    HLib::SetCurrentTaskTo(BellInteractionInitCallback);
    QueueSkAuthorStart(static_cast<BellEventWork *>(parameter)->task, SHEN_NAME(BEL0), nullptr, nullptr, 0);
}
void BellInteractionInitCallback(void *parameter) {
    using enum CameraState;
    using namespace bell_event;
    auto *state = static_cast<BellEventWork *>(parameter);
    Pose(ren, {207.64999f, 103.6f, 195.10001f}, 58982);
    Pose(ryo, {208.73f, 103.6f, 195.91f}, 57344);
    Motion(ren, 0x2182, 0x41000000U);
    Motion(ryo, 0xf003, 0x40000000U);
    SetLowQualityHands(ctrlGetCharaByName(ren));
    SetHighModelXYZ(ctrlGetCharaByName(ren), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ren), 1, 1, 0);
    Hand(ren, 0, restingHand, 0);
    Hand(ren, 1, restingHand, 0);
    targets = {SearchTarget{SHEN_NAME(E000), 0, 0, {std::bit_cast<float>(0x4350a3d7U), 104.5f, 194.73f}, 0.3f, -32768, 32768}, SearchTarget{}};
    srchSetTarget(2, targets.data());
    Pose(anchor, {std::bit_cast<float>(0x4350a3d7U), 104.5f, 194.73f}, 0);
    DisableScenarioCharacterDisplay(SHEN_NAME(a047));
    EVS_LoopCamera(0, {}, {});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    state->renRouteCompletion = static_cast<std::uint32_t>(GetNpcScriptRouteCompletion(ren));
    EV_npcUnknownFunc(ren);
    Gravity(ryo, true);
    EventFilmControl film{0, 10, 255, 0, 0, 0, 0, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&film);
    SetPlayerControlEnabled(1);
    HLib::SetCurrentTaskTo(BellInteractionCallback);
}
void BellInteractionCallback(void *parameter) {
    using namespace bell_event;
    auto *state = static_cast<BellEventWork *>(parameter);
    if (state->command) {
        if (state->command == 1) {
            EV_stepSetFlags(0, 0x19b);
            SetPlayerControlEnabled(0);
            state->busy = 1;
            state->phase = 0;
            HLib::SetCurrentTaskTo(BellRingCallback);
            return;
        }
        if (state->command == 3 && !state->busy && ryoNotBusy()) {
            SetPlayerControlEnabled(0);
            srchReset();
            const auto duration = Conversation(state->conversations);
            ++state->conversations;
            state->command = 0;
            state->busy = 1;
            QueueEventCallFunction(EyeTowardBell, nullptr, 30);
            QueueEventCallFunction(ResetEye, nullptr, 100);
            QueuePoint(ryo, ren, 5, 0x20000000U, 30);
            EVT_AT_SetPoint(ryo, ren, 5, 1, 0, duration, 0, 0x10000000U);
            EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0x40000000U, 0);
            QueueEventCallFunction(RestoreControl, state, duration);
            HLib::SetupTaskDelay(duration);
            return;
        }
        if (state->command != 2)
            return;
        SetPlayerControlEnabled(0);
        srchReset();
        Pose(ryo, {208.2f, 103.6f, 196}, 24576);
        int camera = 120;
        if (!state->escapeTalks) {
            Voice("0202C028"); // @text EN: "Where do you think you're going?" | JA: "おい、どこ行くんだ。"
            Voice("0202C036", 60); // @text EN: "Hurry up and ring the bell." | JA: "とっとと呼び鈴を押せよ。"
        } else if (state->escapeTalks == 1) {
            Voice("0202C037"); // @text EN: "What are you doing, stupid?" | JA: "何してんだ、バカ。"
            Voice("0202C038", 70); // @text EN: "Ring the bell and get Yang!" | JA: "呼び鈴押して\nヤンを呼べってんだよ。"
            camera = 121;
        } else {
            Voice("0202C039"); // @text EN: "Where are you going?" | JA: "どこ行くつもりだ。"
            Voice("0202C025", 70); // @text EN: "Just ring the bell." | JA: "さっさと呼び鈴を押せ。"
            if (state->escapeTalks == 2)
                camera = 122;
            else {
                const std::uint8_t choice = static_cast<std::uint8_t>(static_cast<std::int32_t>(GetRandomNumberInRange(3)));
                camera = choice == 0 ? 120 : choice == 1 ? 121 : 122;
            }
        }
        EVS_LoopCamera(camera, anchor, anchor);
        ++state->escapeTalks;
        state->command = 0;
        state->busy = 1;
        QueueEventCallFunction(EyeTowardBell, nullptr, 30);
        QueueEventCallFunction(ResetEye, nullptr, 120);
        QueuePoint(ryo, ren, 5, 0x20000000U, 30);
        QueuePoint(ryo, {}, 0, 0x10000000U, 120);
        QueuePoint(ren, ryo, 5, 0x20000000U, 1);
        QueuePoint(ren, {}, 0, 0x10000000U, 120);
        Motion(ryo, 0xf003, 0x40000000U);
        QueueEventCallFunction(RestoreControl, state, 120);
        return;
    }
    if (!state->busy && !GetShouldRenderIcons(RenderIconSlot::Any))
        ++state->elapsed;
    if (state->elapsed == 1030 || state->elapsed == 1920 || state->elapsed == 2840)
        SetPlayerControlEnabled(1);
    else if (state->elapsed == 900 || state->elapsed == 1800 || state->elapsed == 2700) {
        SetPlayerControlEnabled(0);
        if (IsItemSelectable())
            RequestMemoClose();
        EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0x80000000U, 0);
        if (state->elapsed == 900) {
            Voice("0202C032"); // @text EN: "What are you waiting for?" | JA: "何、ぐずぐずしてんだ。"
            Voice("0202C026", 70); // @text EN: "Ring the damn bell." | JA: "呼び鈴を押しゃあ\nいいんだよ。"
        } else if (state->elapsed == 1800) {
            Voice("0202C033"); // @text EN: "You're wasting time." | JA: "時間がもったいねえだろ。"
            Voice("0202C025", 60); // @text EN: "Just ring the bell." | JA: "さっさと呼び鈴を押せ。"
        } else {
            Voice("0202C034"); // @text EN: "Give me a break." | JA: "おい、いいかげんにしろ。"
            Voice("0202C035", 70); // @text EN: "Ring the bell and get Yang." | JA: "呼び鈴押して\nヤンを呼べ。"
        }
    }
    if (state->elapsed < 3600)
        return;
    if (IsItemSelectable())
        RequestMemoClose();
    srchReset();
    RefreshPadTask(0, 1);
    SetPlayerControlEnabled(0);
    Pose(ren, {207.7f, 103.6f, 194.96001f}, 58982);
    DisableScenarioCharacterDisplay(ryo);
    Vector3 position{};
    EV_GetCharaPosition(anchor, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    mtwkUpdateWithPosition(scenFindMotionwork(ren), &position, 1);
    Vector4 reflector{1, 0.3f, 0.3f, 0.3f};
    SetScenarioReflectorBoardValues(ren, &reflector);
    EV_SetRefBoardParamsFor(ren, 1);
    EV_PlayDialogue("0202C031");
    EVT_AT_SetMot(ren, 0x2183, 0, 1, 0x40000000U, 0);
    state->phase = 4;
    EV_stepSetFlags(0, 0x19c);
    EVS_LoopCamera(110, anchor, anchor);
    EVT_AT_FadeBGM(30, 1, 120);
    EVT_AT_MenCtrl({1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 120});
    HLib::SetCurrentTaskTo(BellRingCallback);
    HLib::SetupTaskDelay(150);
}
void BellRingCallback(void *parameter) {
    using namespace bell_event;
    auto *state = static_cast<BellEventWork *>(parameter);
    if (!state->phase) {
        EVS_LoopCamera(100, anchor, anchor);
        SetLowQualityHands(ctrlGetCharaByName(ryo));
        SetLowQualityHands(ctrlGetCharaByName(ren));
        SetHighModelXYZ(ctrlGetCharaByName(ryo), 1, 1, 0);
        SetHighModelXYZ(ctrlGetCharaByName(ren), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(ren), 1, 1, 0);
        Hand(ren, 0, ringingHand, 0);
        Hand(ren, 1, ringingHand, 0);
        const Vector3i rotation{0, 4732, 1820};
        ApplyCharacterHandRotation(ctrlGetCharaByName(ryo), 1, 21, &rotation);
        Hand(ryo, 1, ringingHand, 10);
        const Vector3 look{208.5f, 104.5f, 194.5f};
        SetEyeToChara(ctrlGetCharaByName(ryo), &look, 20, 2);
        mtwkUpdateWithPosition(scenFindMotionwork(ryo), &look, 1);
        QueueEventCallFunction(ResetEye, nullptr, 170);
        Gravity(ryo, false);
        Gravity(ren, false);
        Pose(ryo, {209.10001f, 103.57f, 194.75f}, 6189);
        Motion(ryo, 0x2185, 0x40000000U);
        EVT_AT_MenCtrl({1, 10, 0, 0, 0, 0, 255, 0, 0, 0, 100});
        EVT_AT_FadeBGM(30, 1, 80);
        HLib::SetupTaskDelay(110);
        state->phase = 4;
        return;
    }
    if (state->phase <= 4) {
        if (!EV_ForceLoadNpc(yang))
            return;
        EV_SetCharacterAlphaEnabled(yang, false);
        EV_SetCharacterAlphaEnabled(ren, false);
        EV_SetCharacterAlphaEnabled(ryo, false);
        EV_stepSetFlags(0, 0x131);
        mtwkUpdateWithPosition(scenFindMotionwork(ryo), nullptr, 0);
        EnableScenarioCharacterDisplay(ren);
        EnableScenarioCharacterDisplay(ryo);
        DisableScenarioCharacterDisplay(SHEN_NAME(a047));
        DisableScenarioCharacterDisplay(SHEN_NAME(a257));
        SetDoorSubjectEnabled(bellDoor, 0);
        EnqueueTelephoneCord();
        if (state->renRouteCompletion)
            EV_npcUpdateFlags(ren);
        else
            EV_npcUnknownFunc(ren);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
        EV_LoadAudio("0202A000", 0);
        EV_LoadCutsceneFromCurrentScene("0202A000");
        QueueSkAuthorStart(state->task, SHEN_NAME(BEL1), nullptr, nullptr, 0);
        snapAssignFrmCntFlags(1, 100);
        ++state->phase;
        return;
    }
    if (state->phase == 5) {
        EV_LoadAudio("0203A000", 0);
        EV_LoadCutsceneFromCurrentScene("0203A000");
        QueueSkAuthorStart(state->task, SHEN_NAME(BEL2), nullptr, nullptr, 0);
        ++state->phase;
    } else if (state->phase == 6) {
        HLib::SetCurrentTaskTo(BellExitCallback);
        ++state->phase;
    }
}
void BellExitCallback(void *parameter) {
    using enum CameraState;
    using namespace bell_event;
    auto *state = static_cast<BellEventWork *>(parameter);
    EV_setHoldStatesFlags(1);
    SetDoorSubjectEnabled(bellDoor, 1);
    EV_FreeAuthorPak(0, "0202");
    EV_FreeAuthorPak(0, "0203");
    sbtLoadProgram();
    EV_setHoldStatesFlags(1);
    srchClearTarget(2);
    EV_SetCanDoAreaJumpTo(1);
    Gravity(ryo, true);
    Gravity(ren, true);
    EV_SetCharacterAlphaEnabled(ren, true);
    EV_SetCharacterAlphaEnabled(yang, true);
    EV_SetCharacterAlphaEnabled(ryo, true);
    hndlSetFlags(ctrlGetCharaByName(ren));
    EV_npcLockUnlock(ren);
    EV_npcLockUnlock(yang);
    EV_npcFree(ren);
    EV_npcFree(yang);
    EV_NPC_NewLoadSw(1);
    DeleteMotion(state->motion);
    EVS_LoopCamera(0, {}, {});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BELL"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(state->task);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(QB00), 51);
}
const ScriptAreaDefinition areaBELLScriptDefinition{.areaId = SHEN_NAME(BELL), .field04 = 0, .init = BELL_init_func, .main = BELL_main_func};
} // namespace shenmue::s2
#endif
