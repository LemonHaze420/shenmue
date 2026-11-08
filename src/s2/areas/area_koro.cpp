#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_koro.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void AreaKOROMain(void *argument)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.DebugLogToFileRemoved("==== TanSaKu ====\n");
    FTBL.EV_FreeAuthorPak(1, "0224");
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(KORO_RYO));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(KORO_RYO), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(KORO_RYO), 1, 0, 0);
    {
        Vector3 position = {283.5f, 32.399998f, 216.0f};
        Vector3i rotation = {0, 0x8000, 0};
        FTBL.EV_EnableCharacterDisplay(KORO_RYO);
    }
    koroWork.phase = 0;
    koroWork.age = 0;
    koroWork.entry = (shen_name)argument;
    FTBL.EV_SetCanDoAreaJumpTo(0);
    if (koroWork.entry == KORO_ENTRY_INIT)
        FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(KoroInitCallback, 4, 13, KORO_TASK), 2);
    else if (koroWork.entry == 1)
        FTBL.HLib_EnqueueTaskWithoutParameter(KoroOnLightCallback, 4, 13, KORO_TASK);
    else if (koroWork.entry == KORO_ENTRY_DOOR)
        FTBL.HLib_EnqueueTaskWithoutParameter(KoroDoorCallback, 4, 13, KORO_TASK);
    else if (koroWork.entry == KORO_ENTRY_LIGHT)
        KoroPushSwitch();
    else {
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(KORO_RYO), 1, koroRightHand, 10);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(KORO_RYO), 0, koroLeftHand, 10);
        FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(KORO_RYO), 1, 4, &koroRightHandPosition);
        FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(KORO_RYO), 0, 4, &koroLeftHandPosition);
        FTBL.EVS_LoopCamera(1030, koroBaseId, koroBaseId);
        FTBL.SetDisplayFlagForProp(KORO_PROP, 0);
        FTBL.HLib_EnqueueTaskWithoutParameter(KoroDustBoxCallback, 4, 13, KORO_TASK);
    }
}
#else
#define KORO_SET_GRAVITY(name, mode) do { FTBL.EV_SetCharaControllerFlags(name, 2, mode); FTBL.EV_SetCharaControllerFlags(name, 3, mode); } while (0)
#define KORO_RESTORE_CONTROLS() do { FTBL.SetPlayerControlEnabled(1); KORO_SET_GRAVITY(KORO_RYO, 1); } while (0)
KoroFlags koroFlags = {0, 0, 0};
Vector3 koroScrapPositions[KORO_SCRAP_COUNT] = {{0.072f, 0.051f, 0.061f}, {-0.024f, 0.056f, 0.068f}, {-0.072f, 0.079f, 0.036f}, {-0.066f, 0.082f, -0.031f},
    {0.0f, 0.069f, -0.077f}, {-0.064f, 0.043f, -0.062f}, {0.008f, 0.025f, 0.079f}, {-0.089f, 0.067f, -0.037f}, {0.0f, 0.097f, 0.0f}, {0.013f, 0.122f, -0.062f}};
Vector3i koroScrapRotations[KORO_SCRAP_COUNT] = {{0, 9102, 2730}, {0, 41870, 0}, {0, 20753, 0}, {0, 44418, 0}, {0, 12561, 0}, {910, 10922, 0}, {0, 36772, 0},
    {0, 11832, 0}, {0, 22209, 0}, {63715, 3458, 1820}};
char koroLightLabel[] = "LIGHT";
Vector3i koroLeftHandPosition = {0, 63898, 0};
int koroLeftHand[57] = {1999, 1456, 60441, 1638, 64444, 64628, 546, 61988, 4093, 0, 64810, 60985, 0, 64081, 60983, 0, 726, 64626, 0, 0, 64628, 0, 0, 61348, 0,
    0, 59893, 0, 65172, 63717, 0, 0, 65354, 0, 729, 62439, 0, 65355, 59165, 0, 64810, 64992, 0, 0, 0, 0, 909, 58617, 0, 1456, 60619, 0, 63354, 183, 64080, 182,
    0};
Vector3i koroRightHandPosition = {0, 2730, 0};
int koroRightHand[57] = {1999, 1456, 60441, 1638, 65353, 64810, 546, 61442, 4093, 0, 64810, 60257, 0, 65355, 60801, 0, 726, 64626, 0, 0, 64628, 0, 0, 61348,
    0, 0, 59347, 0, 65172, 63717, 0, 0, 65354, 0, 729, 62439, 0, 65355, 59165, 0, 64810, 64992, 0, 0, 0, 0, 909, 58617, 0, 1456, 60619, 0, 63354, 183, 64080,
    182, 0};
float koroLights[2][9][4] = {{{1.0f, 0.03f, 0.03f, 0.12f}, {0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 0.3f, 0.3f, 0.7f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}},
    {{1.0f, 1.0f, 0.87f, 0.78f}, {1.0f, 1.0f, 1.0f, 0.72f}, {1.0f, 1.0f, 0.87f, 0.78f}, {1.0f, 0.83f, 0.62f, 1.0f}, {1.0f, 0.83f, 0.62f, 1.0f},
        {1.0f, 0.6f, 0.85f, 1.0f}, {1.0f, 0.6f, 0.85f, 1.0f}, {1.0f, 0.6f, 0.85f, 1.0f}, {1.0f, 1.0f, 0.87f, 0.78f}}};
float koroLightSequence[] = {0.1f, 0.15f, 0.2f, 0.25f, 0.3f, 0.35f, 0.4f, 0.4f, 0.5f, 0.4f, 0.4f, 0.5f, 0.4f, 0.4f, 0.5f, 0.7f, 0.8f, 0.85f, 0.9f, 0.93f, 0.96f,
    0.97f, 0.98f, 0.99f, 1.0f, 1.0f, 1.0f, 0.9f, 1.0f, 1.0f, 1.0f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.9f,
    0.8f, 0.9f, 1.0f, -1.0f};
static int koroLightFrame;
KoroWork koroWork;
shen_name koroBaseId;
shen_name koroTrashScraps[KORO_SCRAP_COUNT];

void KoroPlaceCharacter(shen_name name, const Vector3 *position, int yaw)
{
    Vector3i rotation = {0, yaw, 0};
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 0, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 0);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(name), 0, 0);
    FTBL.EV_AT_SetPos(name, 0x38000000U, position);
    FTBL.EV_AT_SetAngY_0(name, 0x38000000U, &rotation, 0);
}

void KoroPlayMotion(shen_name name, unsigned int motion)
{
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 0, 0);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(name), 0, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(name), motion, 0);
}

void KoroInitCallback(void *data)
{
    int index;
    koroWork.age++;
    switch (koroWork.phase) {
    case 0: {
        Vector3 position;
        Vector3i rotation;
        int camera[2];
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        camera[0] = 1;
        camera[1] = 0;
        FTBL.camCtrlUtil(17, camera);
        SHENMUE_EVENT_BOUNDARY();
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"KORO", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        koroFlags.lightOn = 0;
        koroFlags.controlsRestored = 1;
        FTBL.ptycCharaUnlock(KORO_REN);
        FTBL.DeactivatePartyCompanion(KORO_REN);
        KORO_SET_GRAVITY(KORO_TRASH, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KORO_RYO), 71, 0, 0);
        SHENMUE_EVENT_BOUNDARY();
        FTBL.EV_ControlPackageResources(2, 2, "KORO", 0, 0);
        for (index = 0; index < KORO_SCRAP_COUNT; index++) {
            switch (FTBL.shenGetRandomInt32(4)) {
            case 0:
                koroTrashScraps[index] = FTBL.CreateEventCharacter(3, "T1G", 0);
                break;
            case 1:
                koroTrashScraps[index] = FTBL.CreateEventCharacter(3, "T2G", 0);
                break;
            case 2:
                koroTrashScraps[index] = FTBL.CreateEventCharacter(3, "T3G", 0);
                break;
            case 3:
                koroTrashScraps[index] = FTBL.CreateEventCharacter(3, "T4G", 0);
                break;
            }
            FTBL.AT_SetLocalBaseChara(koroTrashScraps[index], KORO_TRASH);
            FTBL.EV_EnableCharacterDisplay(koroTrashScraps[index]);
            FTBL.EV_EnsureCharacterDraw(koroTrashScraps[index]);
            FTBL.EV_AT_SetPos(koroTrashScraps[index], 0x38000000U, &koroScrapPositions[index]);
            FTBL.EV_AT_SetAngY_0(koroTrashScraps[index], 0x38000000U, &koroScrapRotations[index], 0);
        }
        koroBaseId = FTBL.CreateEventCharacter(3, 0, 0);
        FTBL.DebugLogToFileRemoved("baseid:[%c%c%c%c]\n", koroBaseId >> 24, koroBaseId >> 16, koroBaseId >> 8, koroBaseId);
        position.x = 0.0f;
        position.y = 0.0f;
        position.z = 0.0f;
        rotation.x = 0;
        rotation.y = 0;
        rotation.z = 0;
        FTBL.EV_AT_SetPos(koroBaseId, 0x38000000U, &position);
        FTBL.EV_AT_SetAngY_0(koroBaseId, 0x38000000U, &rotation, 0);
        KORO_SetLightState(0);
        koroWork.phase++;
        break;
    }
    case 1:
        if (FTBL.EV_GetPlayerFlag(11, 1116)) {
            FTBL.SetPlayerControlEnabled(1);
            FTBL.HLib_DestroyCurrentTask();
            break;
        }
        KoroPlayMotion(KORO_RYO, 8324);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KORO_RYO), 0xf002, 4);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KORO_RYO), 0xf002, 3);
        FTBL.HLib_SetupTaskDelay(KORO_PlayDialogue(0));
        koroWork.phase++;
        break;
    case 2:
        koroWork.phase++;
        break;
    case 3:
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KORO_RYO), 73, 0, 4);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KORO_RYO), 73, 0, 3);
        KoroPlayMotion(KORO_RYO, 0xf002);
        KORO_RESTORE_CONTROLS();
        FTBL.HLib_SetCurrentTaskTo(KoroRoamCallback);
        break;
    }
}

void KoroRoamCallback(void *data)
{
    Vector3 center = {279.07f, 132.4f, 209.47f};
    Vector3 position;
    if (!koroFlags.controlsRestored) {
        KORO_RESTORE_CONTROLS();
        koroFlags.controlsRestored = 1;
    }
    FTBL.GetCharaPosition(KORO_RYO, 0xffffffffU, &position, 0);
    if (koroFlags.approach == 2 && !FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(KORO_RYO), 58, 1)) {
        FTBL.DebugLogToFileRemoved("汎用移動終了\n");
        koroFlags.approach = 0;
        FTBL.SetPlayerControlEnabled(1);
        KORO_SET_GRAVITY(KORO_RYO, 1);
    }
    if (koroFlags.approach == 1) {
        FTBL.DebugLogToFileRemoved("汎用移動開始\n");
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(KORO_RYO), &center, 0, 0, 0);
        koroFlags.approach = 2;
    }
    if (!FTBL.ComputeAngle(&center, 1.5f, 16384, 0x8000, &position) && FTBL.ryoNotBusy()) {
        FTBL.SetPlayerControlEnabled(0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KORO_RYO), 0xf002, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(KORO_RYO), 1);
        FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(KORO_RYO), 15, 0);
        FTBL.HLib_SetupTaskDelay(KORO_PlayDialogue(2));
        koroFlags.approach = 1;
    }
    koroFlags.lightOn = FTBL.EV_GetPlayerFlag(11, 1116);
    if (koroFlags.lightOn == 1) {
        FTBL.DebugLogToFileRemoved("Light_on\n");
        FTBL.HLib_DestroyCurrentTask();
    }
}

void KoroOnLightCallback(void *data)
{
    koroWork.age++;
    if (koroWork.phase)
        return;
    koroFlags.lightOn = 1;
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(KORO_LIGHT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
    FTBL.CheckIsArgEquZero(1, 0);
    KORO_RESTORE_CONTROLS();
    FTBL.HLib_DestroyCurrentTask();
}

void KoroDoorCallback(void *data)
{
    FTBL.srchReset();
    FTBL.SetPlayerControlEnabled(0);
    FTBL.camCtrlSetFovTransitionTo(0);
    FTBL.camCtrlSetState(0);
    FTBL.HLib_SetCurrentTaskTo(KoroDoorDoneCallback);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(KORO_RYO), 1);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(KORO_RYO), 15, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KORO_RYO), 0xf002, 0);
    FTBL.HLib_SetupTaskDelay(KORO_PlayDialogue(3));
}

void KoroDoorDoneCallback(void *data)
{
    KORO_RESTORE_CONTROLS();
    FTBL.HLib_DestroyCurrentTask();
}

void KoroDustBoxCallback(void *data)
{
    koroWork.age++;
    FTBL.EV_ControlPackageResources(2, 2, "KORO", 0, 0);
    if (koroWork.age == 50)
        FTBL.EVS_LoopCamera(1031, KORO_RYO, KORO_RYO);
    if (koroWork.age == 110)
        KORO_PlayDialogue(1);
    switch (koroWork.phase) {
    case 0: {
        Vector3 position;
        Vector3i rotation;
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KORO_RYO), 71, 0, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(KORO_RYO), 0);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(KORO_RYO), 17, 0);
        KORO_SET_GRAVITY(KORO_TRASH, 0);
        position.x = 279.8f;
        position.y = 132.40001f;
        position.z = 211.45f;
        rotation.x = 0;
        rotation.y = 0xc000;
        rotation.z = 0;
        FTBL.EV_AT_SetPos(KORO_RYO, 0x38000000U, &position);
        FTBL.EV_AT_SetAngY_0(KORO_RYO, 0x38000000U, &rotation, 0);
        FTBL.EV_ControlPackageResources(2, 2, "KORO", 0, 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KORO_RYO), 8323, 0);
        FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(KORO_RYO), 120.0f, 0);
        FTBL.srchSetLookMode();
        FTBL.SetDisplayFlagForProp(KORO_PROP, 0);
        koroWork.phase++;
        break;
    }
    case 1:
        FTBL.HLib_SetupTaskDelay(50);
        koroWork.phase++;
        break;
    case 2: {
        Vector3 offset = {0.12287f, -0.260312f, -0.175563f};
        Vector3i rotation = {-7260, -17515, 7113};
        int bone = 14;
        FTBL.EV_SetCharaControllerFlags(KORO_RYO, 3, 0);
        FTBL.EV_SetCharaControllerFlags(KORO_TRASH, 3, 0);
        FTBL.AttachScenarioCharacter(KORO_TRASH, KORO_RYO, bone, &offset, &rotation);
        koroWork.phase++;
        break;
    }
    case 3:
        if (koroWork.age == 310)
            koroWork.phase++;
        break;
    case 4:
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        koroWork.phase++;
        FTBL.HLib_SetupTaskDelay(30);
        break;
    case 5: {
        EventDateTime dateTime;
        int minutes;
        FTBL.EV_ControlPackageResources(0, 6, "KORO", 0, 0);
        FTBL.EV_stepSetFlags(267, 0);
        FTBL.HLib_DestroyCurrentTask();
        FTBL.GetCurrentDateTime((DateTime *)&dateTime);
        minutes = dateTime.minute + dateTime.hour * 60 + 30;
        dateTime.minute = minutes % 60;
        dateTime.hour = minutes / 60;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&dateTime);
        FTBL.EV_scenQueueLoad(g_actNumber, KORO_NEXT_AREA, 0);
        break;
    }
    }
}

void KoroSwitchRevealCallback(void *data)
{
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.HLib_DestroyCurrentTask();
}

void KoroPushSwitch(void)
{
    Vector3 origin = {279.654f, 133.5875f, 208.8037f};
    Vector3 offset = {-0.35218f, -1.20512f, 0.65925f};
    Vector3i rotation = {0, 0, 0};
    Vector3 position = offset;
    FTBL.DebugLogToFileRemoved("PUSH LIGHT\n");
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(KoroSwitchRevealCallback, 4, 13, KORO_TASK), 1);
    FTBL.EV_SetCharaControllerFlags(KORO_RYO, 1, 0);
    KORO_SET_GRAVITY(KORO_RYO, 0);
    FTBL.EV_ControlPackageResources(2, 2, "KORO", 0, 0);
    FTBL.EVS_LoopCamera(2000, KORO_RYO, KORO_RYO);
    FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
    KoroPlayMotion(KORO_RYO, 8326);
    FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(KORO_RYO), 60.0f, 0);
    KoroPlaceCharacter(KORO_RYO, &position, rotation.y);
    FTBL.EV_EnableCharacterDisplay(KORO_RYO);
    FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(KoroSwitchLightCallback, 4, 13, KORO_TASK), 30);
}

void KoroSwitchLightCallback(void *data)
{
    KORO_SetLightState(1);
    FTBL.Loop3DSoundEffect(2698);
    FTBL.HLib_SetCurrentTaskTo(KoroSwitchDoneCallback);
    FTBL.HLib_SetupTaskDelay(60);
}

void KoroSwitchDoneCallback(void *data)
{
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    FTBL.EV_SetPlayerFlag(12, 962, 1);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KORO_RYO), 71, 0, 0);
    KORO_RESTORE_CONTROLS();
    FTBL.HLib_DestroyCurrentTask();
}

void KoroSetTaskWithDelay(HLibTaskCallback callback, int delay)
{
    koroWork.phase = 0;
    koroWork.age = 0;
    FTBL.HLib_SetCurrentTaskTo(callback);
    if (delay)
        FTBL.HLib_SetupTaskDelay(delay);
}

void KoroSetTask(HLibTaskCallback callback)
{
    koroWork.phase = 0;
    koroWork.age = 0;
    FTBL.HLib_SetCurrentTaskTo(callback);
}

int KORO_PlayDialogue(int selection)
{
    int delay = 0;
    switch (selection) {
    case 0:
        EVT_AT_SetCam(2003, KORO_RYO, KORO_RYO, 6, 0, delay);
        EVT_AT_SetVoice("0224A165", delay);
        delay += 126;
        EVT_AT_SetVoice("0224A166", delay);
        delay += 117;
        EVT_AT_SetCam(2004, KORO_RYO, KORO_RYO, 6, 0, delay);
        EVT_AT_SetVoice("0224A175", delay);
        delay += 90;
        break;
    case 1:
        EVT_AT_SetVoice("0224A286", delay);
        delay += 130;
        EVT_AT_SetVoice("0224A218", delay);
        delay += 60;
        break;
    case 2:
        switch (FTBL.shenGetRandomInt32(6)) {
        case 0:
            EVT_AT_SetVoice("0224A168", delay);
            delay += 134;
            break;
        case 1:
            EVT_AT_SetVoice("0224A169", delay);
            delay += 117;
            break;
        case 2:
            EVT_AT_SetVoice("0224A171", delay);
            delay += 99;
            break;
        case 3:
            EVT_AT_SetVoice("0224A172", delay);
            delay += 55;
            break;
        case 4:
            EVT_AT_SetVoice("0224A173", delay);
            delay += 108;
            break;
        default:
            EVT_AT_SetVoice("0224A174", delay);
            delay += 64;
            break;
        }
        break;
    case 3:
        EVT_AT_SetVoice("0224A210", delay);
        delay += 73;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        break;
    case 10:
        EVT_AT_SetVoice("0224A202", delay);
        delay += 117;
        EVT_AT_SetVoice("0224A203", delay);
        delay += 134;
        break;
    case 11:
        EVT_AT_SetVoice("0224A201", delay);
        delay += 90;
        break;
    }
    return delay;
}

void QueueKoroLightLoop(void)
{
    koroLightFrame = 0;
    FTBL.HLib_EnqueueTaskWithoutParameter((HLibTaskCallback)KoroLightLoopCallback, 4, 13, KORO_TASK);
    KoroLightLoopCallback();
}

void KoroLightLoopCallback(void)
{
    float alpha = koroLightSequence[koroLightFrame];
    if (alpha < -0.5f) {
        FTBL.HLib_DestroyCurrentTask();
        FTBL.SetupLightConfig();
    } else {
        InterpolateKoroLights(alpha);
        koroLightFrame++;
    }
}

void InterpolateKoroLights(float alpha)
{
    float colour[4];
    float inverse = 1.0f - alpha;
    int index;
    for (index = 0; index < 9; index++) {
        colour[0] = inverse * koroLights[0][index][0] + alpha * koroLights[1][index][0];
        colour[1] = inverse * koroLights[0][index][1] + alpha * koroLights[1][index][1];
        colour[2] = inverse * koroLights[0][index][2] + alpha * koroLights[1][index][2];
        colour[3] = inverse * koroLights[0][index][3] + alpha * koroLights[1][index][3];
        FTBL.SetLightRuntimeColour(index + 1, colour);
    }
    FTBL.SetDefaultVS();
    if (alpha > 0.5f) {
        FTBL.SetObjectFlagByIndex(1, 1);
        FTBL.SetScenarioCharacterDrawControl(KORO_RYO, 2, 1, 0.0f);
    } else {
        FTBL.SetObjectFlagByIndex(1, 0);
        FTBL.SetScenarioCharacterDrawControl(KORO_RYO, 2, 0, 0.0f);
    }
}

void KORO_SetLightState(int animate)
{
    Vector3 position;
    if (FTBL.EV_GetPlayerFlag(11, 1116)) {
        if (animate)
            QueueKoroLightLoop();
    } else {
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.SetDefaultVS();
        FTBL.EV_lightLoad(KORO_LIGHT_DEFINITION);
        position.x = 279.27f;
        position.y = 133.83f;
        position.z = 210.5f;
        FTBL.SetLightRuntimePosition(3, &position);
        InterpolateKoroLights(0.0f);
        FTBL.CheckIsArgEquZero(1, 0);
        FTBL.SetObjectFlagByIndex(1, 0);
    }
}
#endif
#else
#include "s2/areas/area_koro.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/scene_data.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include <array>

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, trash{SHEN_NAME(ZGBG)}, token{SHEN_NAME(kous)};
std::int32_t age{}, phase{}, state{}, lightFrame{};
shen_name cameraOwner{};
std::array<shen_name, 10> scraps{};
std::uint8_t lightOn{}, controlsRestored{}, darkPhase{}, spareAge[4]{};
constexpr Vector3 scrapPositions[]{{0.072f, 0.051f, 0.061f}, {-0.024f, 0.056f, 0.068f}, {-0.072f, 0.079f, 0.036f}, {-0.066f, 0.082f, -0.031f},
    {0.0f, 0.069f, -0.077f}, {-0.064f, 0.043f, -0.062f}, {0.008f, 0.025f, 0.079f}, {-0.089f, 0.067f, -0.037f}, {0.0f, 0.097f, 0.0f}, {0.013f, 0.122f, -0.062f}};
constexpr Vector3i scrapRotations[]{{0, 9102, 2730}, {0, 41870, 0}, {0, 20753, 0}, {0, 44418, 0}, {0, 12561, 0}, {910, 10922, 0}, {0, 36772, 0}, {0, 11832, 0},
    {0, 22209, 0}, {63715, 3458, 1820}};
std::array<std::int32_t, 57> leftHand{1999, 1456, 60441, 1638, 65354, 64811, 546, 61442, 4093, 0, 64811, 60258, 0, 65355, 60801, 0, 725, 64626, 0, 0, 64628, 0,
    0, 61349, 0, 0, 59348, 0, 65172, 63718, 0, 0, 65354, 0, 729, 62440, 0, 65355, 59166, 0, 64811, 64992, 0, 0, 0, 0, 908, 58618, 0, 1456, 60619, 0, 63354, 182,
    64080, 182, 0};
std::array<std::int32_t, 57> rightHand{1999, 1456, 60441, 1638, 64444, 64628, 546, 61989, 4093, 0, 64811, 60985, 0, 64081, 60983, 0, 725, 64626, 0, 0, 64628, 0,
    0, 61349, 0, 0, 59893, 0, 65172, 63718, 0, 0, 65354, 0, 729, 62440, 0, 65355, 59166, 0, 64811, 64992, 0, 0, 0, 0, 908, 58618, 0, 1456, 60619, 0, 63354, 182,
    64080, 182, 0};
constexpr float lightSequence[]{0.1f, 0.15f, 0.2f, 0.25f, 0.3f, 0.35f, 0.4f, 0.4f, 0.5f, 0.4f, 0.4f, 0.5f, 0.4f, 0.4f, 0.5f, 0.7f, 0.8f, 0.85f, 0.9f, 0.93f,
    0.96f, 0.97f, 0.98f, 0.99f, 1, 1, 1, 0.9f, 1, 1, 1, 0.9f, 1, 1, 1, 1, 1, 1, 1, 0.9f, 1, 1, 1, 1, 1, 0.9f, 0.8f, 0.9f, 1, -1};
constexpr Vector4 darkLights[]{
    {1, 0.03f, 0.03f, 0.12f}, {0, 0, 0, 0}, {1, 0.3f, 0.3f, 0.7f}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
constexpr Vector4 brightLights[]{{1, 1, 0.87f, 0.78f}, {1, 1, 1, 0.72f}, {1, 1, 0.87f, 0.78f}, {1, 0.83f, 0.62f, 1}, {1, 0.83f, 0.62f, 1}, {1, 0.6f, 0.85f, 1},
    {1, 0.6f, 0.85f, 1}, {1, 0.6f, 0.85f, 1}, {1, 1, 0.87f, 0.78f}};
MotionWork *RyoMotion() {
    return scenFindMotionwork(ryo);
}
void Gravity(shen_name name, bool enabled) {
    const auto mode = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    EV_SetCharaControllerFlags(name, CCMF_ApplyGravity, mode);
    EV_SetCharaControllerFlags(name, CCMF_ApplyGravity | CCMF_UpdateHeight, mode);
}
void RestoreControls() {
    SetPlayerControlEnabled(1);
    Gravity(ryo, true);
}
void PlayRyo(std::uint16_t motion) {
    mtwkSetRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
    mtwkSetFrameTime(RyoMotion(), 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(RyoMotion(), 0);
    mtwkRequestMotion(RyoMotion(), motion, MotionRegion::LowerBody);
}
void LoopCamera(std::int32_t camera, shen_name eye, shen_name target) {
    EVS_LoopCamera(camera, eye, target);
}
void WaitPackage() {
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KORO"), 0);
}
void InterpolateLights(float alpha) {
    const float inverse = 1.0f - alpha;
    for (std::uint32_t i = 0; i < 9; ++i) {
        const auto &dark = darkLights[i];
        const auto &bright = brightLights[i];
        const Vector4 colour{
            alpha * bright.x + inverse * dark.x, alpha * bright.y + inverse * dark.y, alpha * bright.z + inverse * dark.z, alpha * bright.w + inverse * dark.w};
        shenSetLightColour(i + 1, colour);
    }
    SetDefaultVS();
    const bool lit = alpha > 0.5f;
    EV_SetObjectFlagByIndex(1, lit);
    SetScenarioCharacterDrawControl(ryo, 2, lit);
}
void LightLoop_Callback(void *) {
    const float alpha = lightSequence[lightFrame];
    if (alpha >= -0.5f) {
        InterpolateLights(alpha);
        ++lightFrame;
    } else {
        HLib::DestroyCurrentTask();
        SetupLightConfig();
    }
}
void QueueLightLoop() {
    lightFrame = 0;
    HLib::EnqueueTaskWithoutParameter(LightLoop_Callback, 4, 0x0d, token.name);
    LightLoop_Callback(nullptr);
}
void DoorDone_Callback(void *) {
    RestoreControls();
    HLib::DestroyCurrentTask();
}
void Door_Callback(void *) {
    using enum CameraState;
    srchReset();
    SetPlayerControlEnabled(0);
    camCtrlSetFovTransitionTo(false);
    camCtrlSetState(Init);
    HLib::SetCurrentTaskTo(DoorDone_Callback);
    mtwkSetRequestedRootMode(RyoMotion(), 1);
    mtwkSetFrameTime(RyoMotion(), 15, MotionRegion::LowerBody);
    mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::LowerBody);
    EVT_AT_SetVoice("0224A210", 1, 0); // @text EN: "I've got to find a clue=@" | JA: "手がかりを探さないとな…"
    HLib::SetupTaskDelay(73);
}
void OnLight_Callback(void *) {
    ++age;
    if (phase)
        return;
    lightOn = 1;
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(SHEN_NAME(0000));
    PushGlobalLightingProgram();
    RestoreControls();
    HLib::DestroyCurrentTask();
}
void SwitchReveal_Callback(void *) {
    SetScreenFilmFade(0, 0, 0, 0, 0, 0, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
void SwitchDone_Callback(void *) {
    using enum CameraState;
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 962, 1);
    mtwkSetRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
    RestoreControls();
    HLib::DestroyCurrentTask();
}
void SwitchLight_Callback(void *) {
    KORO_SetLightState(1);
    Loop3DSoundEffect(2698);
    HLib::SetCurrentTaskTo(SwitchDone_Callback);
    HLib::SetupTaskDelay(60);
}
void PushSwitch() {
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    auto *task = HLib::EnqueueTaskWithoutParameter(SwitchReveal_Callback, 4, 0x0d, token.name);
    HLib::SetupTimedTask(task, 1);
    EV_SetCharaControllerFlags(ryo, CCMF_UpdateHeight, NpcCharacterControllerState::Disabled);
    Gravity(ryo, false);
    WaitPackage();
    LoopCamera(2000, ryo, ryo);
    Vector3 position{-0.35218f, -1.20512f, 0.65925002f};
    const Vector3 origin{279.65399f, 133.58749f, 208.8037f};
    const Vector3i rotation{};
    scenApplyTransRot(&origin, &rotation, &position);
    PlayRyo(0x2086);
    mtwkSetRegionEvalTime(RyoMotion(), 60.0f, MotionRegion::LowerBody);
    mtwkSetRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(RyoMotion(), 0);
    mtwkSetFrameTime(RyoMotion(), 0, MotionRegion::LowerBody);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, rotation);
    EV_EnableCharacterDisplay(ryo);
    task = HLib::EnqueueTaskWithoutParameter(SwitchLight_Callback, 4, 0x0d, token.name);
    HLib::SetupTimedTask(task, 30);
}
void DustBox_Callback(void *) {
    ++age;
    WaitPackage();
    if (age == 50)
        LoopCamera(1031, ryo, ryo);
    if (age == 110) {
        EVT_AT_SetVoice("0224A286", 1, 0); // @text EN: "Could be a clue leading to\nYuanda Zhu on these tapes." | JA: "このテープの中に、\n朱元達の情報があるかもしれない…"
        EVT_AT_SetVoice("0224A218", 1, 130); // @text EN: "I should go back to Ren's hideout." | JA: "レンのアジトに戻るか。"
    }
    switch (phase) {
    case 0: {
        mtwkSetRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(RyoMotion(), 0);
        mtwkSetExtraFlag(RyoMotion(), 17, 0);
        Gravity(trash, false);
        const Vector3 position{279.79999f, 132.40001f, 211.45f};
        const Vector3i rotation{0, 49152, 0};
        EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, rotation);
        WaitPackage();
        mtwkRequestMotion(RyoMotion(), 0x2083, MotionRegion::LowerBody);
        mtwkSetRegionEvalTime(RyoMotion(), 120.0f, MotionRegion::LowerBody);
        srchSetLookMode();
        SetDisplayFlagForProp(SHEN_NAME(M736), 0);
        ++phase;
        break;
    }
    case 1:
        HLib::SetupTaskDelay(50);
        ++phase;
        break;
    case 2: {
        const Vector3 position{0.12287f, -0.26031199f, -0.17556299f};
        const std::array<std::int32_t, 3> rotation{-7260, -17515, 7113};
        EV_SetCharaControllerFlags(ryo, CCMF_ApplyGravity | CCMF_UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(trash, CCMF_ApplyGravity | CCMF_UpdateHeight, NpcCharacterControllerState::Disabled);
        SetFixoDataForMotionWork(ctrlGetScenarioChara(trash), ctrlGetScenarioChara(ryo), 14, position, rotation);
        ++phase;
        break;
    }
    case 3:
        if (age == 310)
            phase = 4;
        break;
    case 4:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        ++phase;
        HLib::SetupTaskDelay(30);
        break;
    case 5: {
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KORO"), 0);
        EV_stepSetFlags(0, 267);
        HLib::DestroyCurrentTask();
        DateTime time{};
        shenGetCurrentDateTime(time);
        const int minutes = 60 * time.hour + time.minute + 30;
        time.hour = static_cast<std::int8_t>(minutes / 60);
        time.minute = static_cast<std::int8_t>(minutes % 60);
        SetCurrentDateTimeTo(time);
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(QRR0), 0);
        break;
    }
    default:
        break;
    }
}
void DarkFree_Callback(void *) {
    const Vector3 position{279.07001f, 132.39999f, 209.47f};
    if (!controlsRestored) {
        RestoreControls();
        controlsRestored = 1;
    }
    Vector3 current;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &current, EventTransformFlags::Local);
    if (darkPhase == 2 && !mtwkSetExtraFlag(RyoMotion(), 58, 1)) {
        darkPhase = 0;
        RestoreControls();
    }
    if (darkPhase == 1) {
        mtwkCopyTransition(RyoMotion(), &position, 0, 0, 0);
        darkPhase = 2;
    }
    if (!ComputeAngle(position, 1.5f, 0x4000, 0x8000, current) && ryoNotBusy()) {
        SetPlayerControlEnabled(0);
        mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(RyoMotion(), 1);
        mtwkSetFrameTime(RyoMotion(), 15, MotionRegion::LowerBody);
        HLib::SetupTaskDelay(KORO_PlayDialogue(2));
        darkPhase = 1;
    }
    lightOn = static_cast<std::uint8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1116));
    if (lightOn == 1 && controlsRestored == 0)
        HLib::DestroyCurrentTask();
}
void SearchStart_Callback(void *) {
    using enum CameraState;
    ++age;
    switch (phase) {
    case 0: {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        CameraUtilityParameters camera{1, 0};
        camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&camera));
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("KORO"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        controlsRestored = 1;
        lightOn = 0;
        ptycCharaUnlock(SHEN_NAME(REN_));
        DeactivatePartyCompanion(SHEN_NAME(REN_));
        Gravity(trash, false);
        mtwkSetRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
        WaitPackage();
        constexpr const char *models[]{"T1G", "T2G", "T3G", "T4G"};
        for (std::size_t i = 0; i < scraps.size(); ++i) {
            scraps[i] = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>(models[shenGetRandomInt32(4)]), shen_name{});
            AT_SetLocalBaseChara(scraps[i], trash);
            EV_EnableCharacterDisplay(scraps[i]);
            EV_EnsureCharacterDraw(scraps[i]);
            EV_AT_SetPos(scraps[i], EventTransformFlags::AllAxes, scrapPositions[i]);
            EV_AT_SetAngY(scraps[i], EventTransformFlags::AllAxes, scrapRotations[i]);
        }
        cameraOwner = CreateEventCharacter(CharacterCreationHighModel, nullptr, shen_name{});
        EV_AT_SetPos(cameraOwner, EventTransformFlags::AllAxes, Vector3{});
        EV_AT_SetAngY(cameraOwner, EventTransformFlags::AllAxes, Vector3i{});
        KORO_SetLightState(0);
        ++phase;
        break;
    }
    case 1:
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1116)) {
            PlayRyo(0x2084);
            mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::LeftArm);
            mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::RightArm);
            HLib::SetupTaskDelay(KORO_PlayDialogue(0));
            ++phase;
        } else {
            SetPlayerControlEnabled(1);
            HLib::DestroyCurrentTask();
        }
        break;
    case 2:
        phase = 3;
        break;
    case 3:
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        mtwkSetRegionFlag(RyoMotion(), 73, 0, MotionRegion::LeftArm);
        mtwkSetRegionFlag(RyoMotion(), 73, 0, MotionRegion::RightArm);
        PlayRyo(0xf002);
        RestoreControls();
        HLib::SetCurrentTaskTo(DarkFree_Callback);
        break;
    default:
        break;
    }
}
} // namespace

std::int32_t KORO_PlayDialogue(std::int32_t selection) {
    switch (selection) {
    case 0:
        EVT_AT_SetCam(2003, ryo, ryo, 0, 6, 0);
        EVT_AT_SetVoice("0224A165", 1, 0); // @text EN: "It's pitch black.\nNo one's here?" | JA: "まっ暗だな…\n誰もいないのか？"
        EVT_AT_SetVoice("0224A166", 1, 126); // @text EN: "There's no light.\nI can't see a thing." | JA: "明かりがないと、\n何も見えないな。"
        EVT_AT_SetCam(2004, ryo, ryo, 0, 6, 243);
        EVT_AT_SetVoice("0224A175", 1, 243); // @text EN: "Where's the light switch?" | JA: "電灯のスイッチはどこだ？"
        return 333;
    case 1:
        EVT_AT_SetVoice("0224A286", 1, 0); // @text EN: "Could be a clue leading to\nYuanda Zhu on these tapes." | JA: "このテープの中に、\n朱元達の情報があるかもしれない…"
        EVT_AT_SetVoice("0224A218", 1, 130); // @text EN: "I should go back to Ren's hideout." | JA: "レンのアジトに戻るか。"
        return 190;
    case 2: {
        constexpr const char *voices[]{"0224A168", "0224A169", "0224A171", "0224A172", "0224A173", "0224A174"}; // @text 0224A168 EN: "It's so dark.\nWhere's the light in here?" | JA: "まっ暗だな…\nどこかに明かりはないのか？" ; 0224A169 EN: "I can't find anything in the dark." | JA: "このままじゃ、何も探せないな…" ; 0224A171 EN: "Can't find anything when it's so dark." | JA: "暗くて何もわからないな…" ; 0224A172 EN: "It's pitch black." | JA: "まっ暗だ…" ; 0224A173 EN: "Can't find anything like this." | JA: "このままじゃ、何も探せないな。" ; 0224A174 EN: "I've got to find a light." | JA: "明かりをつけないとな…"
        constexpr int delays[]{134, 117, 99, 55, 108, 64};
        const int choice = shenGetRandomInt32(6);
        EVT_AT_SetVoice(voices[choice], 1, 0);
        return delays[choice];
    }
    case 3:
        EVT_AT_SetVoice("0224A210", 1, 0); // @text EN: "I've got to find a clue=@" | JA: "手がかりを探さないとな…"
        return 73;
    case 10:
        EVT_AT_SetVoice("0224A202", 1, 0); // @text EN: "Cassette tapes in the trash can?" | JA: "ん…？　カセットテープが\n捨ててあるな…"
        EVT_AT_SetVoice("0224A203", 1, 117); // @text EN: "Maybe=@ in here?" | JA: "何かを記録したのか…？\nもしかすると…"
        return 251;
    case 11:
        EVT_AT_SetVoice("0224A201", 1, 0); // @text EN: "These are=@\ncassette tapes!" | JA: "これは…カセットテープだ！"
        return 90;
    default:
        return 0;
    }
}

void KORO_SetLightState(std::int32_t animate) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1116)) {
        if (animate)
            QueueLightLoop();
    } else {
        EV_SetModelDataDefaultVertexShader();
        SetDefaultVS();
        EV_lightLoad(SHEN_NAME(0000));
        SetSelectedLightDirection(3, Vector3{279.26999f, 133.83f, 210.5f});
        InterpolateLights(0);
        EV_SetObjectFlagByIndex(1, 0);
    }
}

void AreaKOROInit() {
    age = 0;
    phase = 0;
    state = 0;
    lightFrame = 0;
    cameraOwner = {};
    scraps = {};
    lightOn = 0;
    controlsRestored = 0;
    darkPhase = 0;
    for (auto &value : spareAge)
        value = 0;
}

void AreaKOROMain(std::uintptr_t parameter) {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0224");
    SetLowQualityHands(ctrlGetScenarioChara(ryo));
    SetHighModelXYZ(ctrlGetScenarioChara(ryo), 1, 1, 0);
    SetHighModelXYZ(ctrlGetScenarioChara(ryo), 1, 0, 0);
    EV_EnableCharacterDisplay(ryo);
    state = static_cast<std::int32_t>(parameter);
    phase = 0;
    age = 0;
    EV_SetCanDoAreaJumpTo(0);
    HLib::TaskCallback callback;
    if (static_cast<std::uint32_t>(state) == SHEN_NAME(INIT)) {
        auto *task = HLib::EnqueueTaskWithoutParameter(SearchStart_Callback, 4, 0x0d, token.name);
        HLib::SetupTimedTask(task, 2);
        return;
    }
    if (state == 1)
        callback = OnLight_Callback;
    else if (static_cast<std::uint32_t>(state) == SHEN_NAME(DOOR))
        callback = Door_Callback;
    else if (static_cast<std::uint32_t>(state) == SHEN_NAME(LIGT)) {
        PushSwitch();
        return;
    } else {
        ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(ryo), 1, reinterpret_cast<std::byte *>(leftHand.data()), 10);
        ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(ryo), 0, reinterpret_cast<std::byte *>(rightHand.data()), 10);
        const Vector3i left{0, 2730, 0}, right{0, 63898, 0};
        ApplyCharacterHandRotation(ctrlGetScenarioChara(ryo), 1, 4, &left);
        ApplyCharacterHandRotation(ctrlGetScenarioChara(ryo), 0, 4, &right);
        LoopCamera(1030, cameraOwner, cameraOwner);
        SetDisplayFlagForProp(SHEN_NAME(M736), 0);
        callback = DustBox_Callback;
    }
    HLib::EnqueueTaskWithoutParameter(callback, 4, 0x0d, token.name);
}
const ScriptAreaDefinition areaKOROScriptDefinition{.areaId = SHEN_NAME(KORO), .field04 = 0, .init = AreaKOROInit, .main = AreaKOROMain};
} // namespace shenmue::s2::areas
#endif
