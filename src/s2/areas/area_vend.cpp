#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_vend.hpp"
#if defined(SHENMUE_EVENT_CONSTANTS)
const Vector3 vendHandOffset = {-.186f,
#if SHENMUE_EVENT_VEND_SCENE
    .01f,
#else
    .03f,
#endif
    .427f};
const Vector3i vendHandRawRotation = {4580, 0, 0};
const Vector3 vendHandRotation = {25.16f, 0, 0};
const float vendPrizeProbability = .5f;

#elif defined(SHENMUE_EVENT_ENTRY)
void RunVendingMachineEvent(void *argument) {
    QueueClearNpcs(eVEND_EVT_InitVend_Enqueue, argument);
}

#else
#if !defined(SHENMUE_EVENT_VEND_ACTIONS)
#if SHENMUE_EVENT_VEND_SCENE
static const shen_name vendCharacters[] = {ID_RYO, 0xffffffffU};
int vendCanRecords[9][2] = {{0, 1}, {0, 1}, {0, 1}, {4, 5}, {2, 3}, {8, 9}, {6, 7}, {6, 7}, {10, 11}};
int vendArchive = -1;
int vendFade = 0;
Vector3 vendBasePosition = {0, 0, 0};
Vector3i vendBaseRotation = {0, 0, 0}, vendHandAngle = {38229, 32768, 0};
int vendCanKind = 1;
Vector3 vendCanAttachment = {0.112f, 0.029f, 0.002f};
Vector3i vendCanRotation = {6917, 11923, 3640};
Vector3 vendWinningAttachment = {0.112f, 0.029f, 0.002f};
Vector3i vendWinningRotation = {6917, 11923, 3640};
VendLamps vendLamps = {0, {172, 173, 174, 175, 176, 177, 178, 179}};
int vendInitialHandCommand[57] = {0, 726, 59712, 0, 0, 65354, 0, 62624, 3822, 0, 2002, 54070, 0, 364, 53160, 0, 0, 62442, 0, 546, 1456, 0, 0, 54798, 0, 0,
    50612, 0, 0, 56436, 0, 546, 1456, 0, 0, 52796, 0, 0, 51340, 0, 0, 56800, 0, 546, 909, 0, 0, 50248, 0, 0, 55708, 0, 0, 57528, 0, 546, 182};
int vendSelectionHandCommand[57] = {0, 726, 63534, 0, 0, 65354, 0, 62624, 3822, 0, 0, 64444, 0, 0, 62806, 0, 0, 65354, 0, 546, 1820, 0, 0, 58802, 0, 0, 54252,
    0, 0, 59894, 0, 546, 1456, 0, 0, 56436, 0, 0, 56072, 0, 0, 59166, 0, 546, 909, 0, 0, 52978, 0, 0, 56800, 0, 0, 59348, 0, 546, 1092};
int vendWaitHandCommand[57] = {0, 726, 63534, 0, 0, 65354, 0, 62624, 3822, 0, 0, 60258, 0, 0, 61168, 0, 0, 64080, 0, 546, 1820, 0, 0, 58802, 0, 0, 54252, 0, 0,
    59894, 0, 546, 1456, 0, 0, 56436, 0, 0, 56072, 0, 0, 59166, 0, 546, 909, 0, 0, 52978, 0, 0, 56800, 0, 0, 59348, 0, 546, 1092};
int vendDrinkHandCommand[57] = {0, 726, 63534, 0, 0, 65354, 0, 62624, 3822, 0, 0, 60258, 0, 0, 61168, 0, 0, 62806, 0, 546, 1820, 0, 0, 58802, 0, 0, 54252, 0, 0,
    59894, 0, 546, 1456, 0, 0, 56436, 0, 0, 56072, 0, 0, 59166, 0, 546, 909, 0, 0, 52978, 0, 0, 56800, 0, 0, 59348, 0, 546, 1092};
int vendCanHandCommand[57] = {0, 63578, 64262, 0, 455, 65354, 0, 63534, 3822, 0, 0, 58223, 0, 0, 54847, 0, 0, 62806, 0, 546, 3822, 0, 0, 60724, 0, 0, 53436, 0,
    0, 61240, 0, 546, 2908, 0, 0, 59041, 0, 0, 56031, 0, 0, 61686, 0, 546, 2539, 0, 0, 58679, 0, 0, 55796, 0, 0, 63690, 0, 546, 1814};
int vendAltHandCommand[57] = {0, 63895, 64262, 0, 63989, 64626, 0, 63534, 3822, 0, 1274, 58413, 0, 1092, 54076, 0, 727, 59166, 0, 546, 3094, 0, 909, 60810, 0,
    1638, 51888, 0, 727, 58874, 0, 546, 2544, 0, 1092, 58487, 0, 546, 52526, 0, 0, 61868, 0, 364, 1993, 0, 0, 58810, 0, 0, 54839, 0, 0, 60778, 0, 546, 1268};
int vendReturnRightCommand[57] = {0, 726, 58438, 0, 0, 65354, 0, 62624, 3822, 0, 0, 50794, 0, 0, 53160, 0, 909, 60440, 0, 546, 1820, 0, 0, 54980, 0, 0, 48974,
    0, 1092, 59166, 0, 546, 1456, 0, 0, 54252, 0, 0, 50248, 0, 0, 58438, 0, 546, 909, 0, 0, 52978, 0, 0, 54070, 0, 0, 56072, 0, 546, 1092};
int vendDrinkRightCommand[57] = {0, 726, 63534, 0, 0, 65354, 0, 62624, 3822, 0, 0, 50794, 0, 0, 59530, 0, 0, 64080, 0, 546, 1820, 0, 0, 54980, 0, 0, 48974, 0,
    1092, 59166, 0, 546, 1456, 0, 0, 54252, 0, 0, 50248, 0, 0, 58438, 0, 546, 909, 0, 0, 52978, 0, 0, 54070, 0, 0, 56072, 0, 546, 1092};
Vector3i vendSelectionHandRotation = {55526, 0, 3276}, vendCanHandRotation = {64366, 65219, 450}, vendAltHandRotation = {64366, 65219, 64712};
VendColour vendColours[] = {
    {SHEN_NAME4('A', 'R', '0', '1'), 3},
    {SHEN_NAME4('A', 'R', '0', '2'), 3},
    {SHEN_NAME4('A', 'R', '0', '2'), 3},
    {SHEN_NAME4('A', 'R', '0', '3'), 3},
    {SHEN_NAME4('A', 'K', '0', '0'), 1},
    {SHEN_NAME4('A', 'K', 'S', '0'), 1},
    {SHEN_NAME4('A', 'B', '0', '0'), 2},
    {SHEN_NAME4('W', 'S', '0', '0'), 3},
    {SHEN_NAME4('W', 'K', '0', '0'), 3},
    {SHEN_NAME4('W', 'T', '0', '0'), 2},
    {SHEN_NAME4('W', 'N', '0', '0'), 2},
    {SHEN_NAME4('W', 'R', '0', '0'), 2},
    {SHEN_NAME4('W', 'E', '0', '0'), 3},
    {SHEN_NAME4('W', 'E', 'G', '0'), 3},
    {SHEN_NAME4('W', 'E', 'S', '1'), 3},
    {SHEN_NAME4('Q', '1', '0', '0'), 3},
    {SHEN_NAME4('Q', '2', '0', '0'), 1},
    {SHEN_NAME4('Q', '3', '0', '0'), 2},
    {SHEN_NAME4('Q', 'E', '0', '1'), 2},
    {SHEN_NAME4('Q', 'E', '0', '0'), 2},
};
shen_name vendMachine;

#else
static const shen_name vendCharacters[] = {ID_RYO, 0xffffffffU};
int vendSelectionHandCommand[57] = {0, 726, 63534, 0, 0, 65354, 0, 62624, 3822, 0, 0, 64444, 0, 0, 62806, 0, 0, 65354, 0, 546, 1820, 0, 0, 58802, 0, 0, 54252,
    0, 0, 59894, 0, 546, 1456, 0, 0, 56436, 0, 0, 56072, 0, 0, 59166, 0, 546, 909, 0, 0, 52978, 0, 0, 56800, 0, 0, 59348, 0, 546, 1092};
int vendWaitHandCommand[57] = {0, 726, 63534, 0, 0, 65354, 0, 62624, 3822, 0, 0, 60258, 0, 0, 61168, 0, 0, 64080, 0, 546, 1820, 0, 0, 58802, 0, 0, 54252, 0, 0,
    59894, 0, 546, 1456, 0, 0, 56436, 0, 0, 56072, 0, 0, 59166, 0, 546, 909, 0, 0, 52978, 0, 0, 56800, 0, 0, 59348, 0, 546, 1092};
int vendDrinkHandCommand[57] = {0, 726, 63534, 0, 0, 65354, 0, 62624, 3822, 0, 0, 60258, 0, 0, 61168, 0, 0, 62806, 0, 546, 1820, 0, 0, 58802, 0, 0, 54252, 0, 0,
    59894, 0, 546, 1456, 0, 0, 56436, 0, 0, 56072, 0, 0, 59166, 0, 546, 909, 0, 0, 52978, 0, 0, 56800, 0, 0, 59348, 0, 546, 1092};
Vector3i vendHandRotations[2] = {{52793, 64626, 3640}, {52793, 64626, 5461}};
Vector3i vendSelectionHandRotation = {55526, 0, 3276};
int vendCanRecords[8][2] = {{0, 1}, {0, 1}, {0, 1}, {4, 5}, {2, 3}, {8, 9}, {6, 7}, {6, 7}};
int vendArchive = -1;
Vector3 vendBasePosition = {0, 0, 0};
Vector3i vendBaseRotation = {0, 0, 0}, vendHandAngle = {38229, 32768, 0};
Vector3 vendCanAttachment = {0.114f, 0.002f, -0.036f};
Vector3i vendCanRotation = {8192, 11377, 3640};
VendLamps vendLamps = {0, {172, 173, 174, 175, 176, 177, 178, 179}};

#endif
void eVEND_EVT_InitVend_Enqueue(void *argument) {
#if SHENMUE_EVENT_VEND_SCENE

    HLibTask *proc, *task;
    VendWork **procWork, *work;
    const char *directory;
    int archive;
    FTBL.DebugLogToFileRemoved("@@@         EV_InitVend( ) \n");
    proc = FTBL.HLib_EnqueueTaskWithParameter(VendingProcCallback, 5, 11, sizeof(VendWork *), SHEN_NAME4('V', 'N', 'D', '9'));
    task = FTBL.HLib_EnqueueTaskWithParameter(eVEND_EVT_InitVend, 4, 12, sizeof(VendWork), SHEN_NAME4('V', 'N', 'D', '1'));
    procWork = (VendWork **)FTBL.HLib_GetTaskParameter(proc);
    work = (VendWork *)FTBL.HLib_GetTaskParameter(task);
    *procWork = work;
    work->procTask = proc;
    work->task = task;
    if (argument)
        work->originalMachine = *(shen_name *)argument;
    else
        work->originalMachine = 0xffffffffU;
    FTBL.DebugLogToFileRemoved("EV_MOUNT_AFS_PARTITION in ET_InitVend\n");
    directory = FTBL.Filepath_Gen(7);
    archive = (int)FTBL.EV_ControlPackageResources(1, 0, (void *)directory, (int)"VEND.afs", 0);
    vendArchive = archive;
    FTBL.DebugLogToFileRemoved("EV_MOUNT_AFS_PARTITION ret_code = %08x\n", archive);
    work->state = 0;
    InitializeVendingResources(work);
    EventDebugRegister(3, eVEND_ET_AbortVend, work, 30000);

#else

    HLibTask *proc, *task;
    VendWork **procWork, *work;
    const char *directory;
    int archive;
    FTBL.DebugLogToFileRemoved("@@@              EV_InitVend( ) \n");
    FTBL.EV_EventSetUpHelpIcon_0(25, 0);
    proc = FTBL.HLib_EnqueueTaskWithParameter(VendingProcCallback, 3, 11, sizeof(VendWork *), TASK_VEND);
    task = FTBL.HLib_EnqueueTaskWithParameter(eVEND_EVT_InitVend, 4, 11, sizeof(VendWork), TASK_VEND);
    procWork = (VendWork **)FTBL.HLib_GetTaskParameter(proc);
    work = (VendWork *)FTBL.HLib_GetTaskParameter(task);
    *procWork = work;
    work->procTask = proc;
    work->task = task;
    if (argument)
        work->originalMachine = *(shen_name *)argument;
    else
        work->originalMachine = 0xffffffffU;
    InitializeVendingResources(work);
    FTBL.DebugLogToFileRemoved("EV_MOUNT_AFS_PARTITION in ET_InitVend\n");
    directory = FTBL.Filepath_Gen(7);
    archive = (int)FTBL.EV_ControlPackageResources(1, 0, (void *)directory, (int)"VEND.afs", 0);
    vendArchive = archive;
    work->state = 0;
    EventDebugRegister(3, eVEND_ET_AbortVend, work, 30000);

#endif
}
#if !SHENMUE_EVENT_VEND_SCENE
void eVEND_ET_AbortVend(void *data) {
    FTBL.DebugLogToFileRemoved("@@@    EV_AbortVend( ) @\n");
    eVEND_ET_KillVend(data);
}
void eVEND_ET_KillVend(VendWork *work) {
    VendButtons *buttons = (VendButtons *)&work->buttons;
    FTBL.DebugLogToFileRemoved("@@@     EV_KillVend( ) \n");
    if (vendArchive != -1) {
        if (FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0)) {
            FTBL.DebugLogToFileRemoved(
                "Cancel(EV_DESTROY_PAKFILE_AFS) read in ET_KillVend no:%d\n", vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
            FTBL.EV_ControlPackageResources(0, 13, (void *)vendArchive, vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
        } else if (work->state == 4) {
            FTBL.DebugLogToFileRemoved("Do EV_FREE_PAKFILE_AFS\n");
            FTBL.EV_ControlPackageResources(0, 12, (void *)vendArchive, vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
        }
        work->state = 5;
        FTBL.DebugLogToFileRemoved("EV_UNMOUNT_AFS_PARTITION in ET_KillVend\n");
        FTBL.EV_ControlPackageResources(1, 1, (void *)vendArchive, 0, 0);
        vendArchive = -1;
    }
    FTBL.DebugLogToFileRemoved("EV_DispUpHandModel False in ET_KillVend\n");
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ID_HAND), 0, 0, 0);
    FTBL.DebugLogToFileRemoved("EV_DisableCharDisplay ID_HAND in ET_KillVend\n");
    FTBL.EV_DisableCharacterDisplay(ID_HAND);
    FTBL.DebugLogToFileRemoved("EV_FreeData Real Hand in ET_KillVend\n");
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ID_HAND));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ID_RYO));
    FTBL.DebugLogToFileRemoved("Do ET_KillVend\n");
    ReleaseVendingResources(work);
    FTBL.DebugLogToFileRemoved("Do EV_HLKillOtherHierarchyTask\n");
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.DebugLogToFileRemoved("Do EV_HLKillOtherHierarchyTask\n");
    FTBL.EV_HLKillOtherHierarchyTask(work->procTask);
    FTBL.EV_SetPlayerFlag(12, 551, 0);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.DebugLogToFileRemoved("Do EV_NPC_NewLoadSw\n");
    FTBL.EV_NPC_NewLoadSw_0(1);
    EVT_AT_MenCtrl(8, 255, 0, 0, 0, 0, 0, 0, 0, 15);
    FTBL.DebugLogToFileRemoved("Do ET_AB_EndEvent\n");
    EventDebugUnregister(3);
}
void VendingProcCallback(void *data) {
}
#endif
void eVEND_EVT_InitVend(void *data) {
#if SHENMUE_EVENT_VEND_SCENE

    VendWork *work = (VendWork *)data;
    Vector3 position;
    Vector3i rotation;
    int unused = 0;
    int colour;
    FTBL.DebugLogToFileRemoved("@@@      EVT_InitVend( )  \n");
    switch (vendFade) {
    case 0:
        vendFade = 1;
        FTBL.DebugLogToFileRemoved(" Now state -->VM_FADE_OUTIN \n");
        colour = GetVendingMachineColour(&g_currentMap);
        if (colour == 1) {
            FTBL.DebugLogToFileRemoved(" VM_COLOL ==> RED \n");
            ID_MACHINE = SHEN_NAME4('J', 'H', 'N', 'R');
        } else if (colour == 2) {
            FTBL.DebugLogToFileRemoved(" VM_COLOL ==> WHITE \n");
            ID_MACHINE = SHEN_NAME4('J', 'H', 'N', 'W');
        } else if (colour == 3) {
            FTBL.DebugLogToFileRemoved(" VM_COLOL ==> GREEN \n");
            ID_MACHINE = SHEN_NAME4('J', 'H', 'N', 'G');
        } else {
            FTBL.DebugLogToFileRemoved(" ****** WARNING ********* \n");
            FTBL.DebugLogToFileRemoved(" VEND.EV3 can't get <AREA-ID>\n");
        }
        FTBL.EV_EventSetUpHelpIcon_0(25, 0);
        EVT_AT_MenCtrl(6, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        EVT_AT_MenCtrl(40, 255, 0, 0, 0, 0, 0, 0, 0, 10);
        FTBL.HLib_SetupTaskDelay(6);
        break;
    case 1:
        vendFade = 0;
        FTBL.DebugLogToFileRemoved(" Now state -->VM_VM_TRANS_CHARA \n");
        EventDebugCheck(3, 4);
        if (FTBL.DoesScenarioCharacterExist(work->originalMachine)) {
            FTBL.EV_EnableCharacterDisplay(ID_MACHINE);
            FTBL.EV_DisableCharacterDisplay(work->originalMachine);
            FTBL.GetCharaPosition(work->originalMachine, 0xffffffffU, &position, 0x40000000);
            FTBL.GetScenarioCharacterEulerAngles(work->originalMachine, &rotation, 0x40000000);
            FTBL.EV_AT_SetPos(ID_MACHINE, 0x78000000, &position);
            FTBL.EV_AT_SetAngY_0(ID_MACHINE, 0x78000000, &rotation, 0);
        }
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ID_HAND));
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ID_RYO));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ID_RYO), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ID_RYO), 1, 1, 0);
        FTBL.AT_SetLocalBaseChara(ID_RYO, ID_MACHINE);
        FTBL.AT_SetLocalBaseChara(ID_BASE, ID_MACHINE);
        FTBL.AT_SetLocalBaseChara(ID_HAND, ID_BASE);
        FTBL.EV_AT_SetAngY_0(ID_HAND, 0x38000000, &vendHandAngle, 0);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(ID_RYO), 0, vendInitialHandCommand, 0);
        FTBL.EV_SetPlayerFlag(12, 551, 1);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.EV_AT_SetPos(ID_BASE, 0x38000000, &vendBasePosition);
        FTBL.EV_AT_SetAngY_0(ID_BASE, 0x38000000, &vendBaseRotation, 0);
        EVT_AT_SetMot(ID_RYO, 0xf003, 0, 1, 0x40000000, 0);
        FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_CheckVend);
        break;
    }

#else

    FTBL.DebugLogToFileRemoved("@@@      EVT_InitVend( )  \n");
    EventDebugCheck(3, 4);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ID_HAND));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ID_RYO));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ID_RYO), 1, 2, 0);
    FTBL.AT_SetLocalBaseChara(ID_RYO, ID_MACHINE);
    FTBL.AT_SetLocalBaseChara(ID_BASE, ID_MACHINE);
    FTBL.AT_SetLocalBaseChara(ID_HAND, ID_BASE);
    FTBL.EV_AT_SetAngY_0(ID_HAND, 0x38000000, &vendHandAngle, 0);
    FTBL.EV_SetPlayerFlag(12, 551, 1);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_AT_SetPos(ID_BASE, 0x38000000, &vendBasePosition);
    FTBL.EV_AT_SetAngY_0(ID_BASE, 0x38000000, &vendBaseRotation, 0);
    EVT_AT_SetMot(ID_RYO, 0xf003, 0.0f, 1.0f, 0x40000000, 0);
    FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_CheckVend);

#endif
}
void eVEND_EVT_CheckVend(void *data) {
    VendWork *work = (VendWork *)data;
    FTBL.DebugLogToFileRemoved("@@@    EVT_CheckVend ( ) \n");
    if (FTBL.GetCurrentControlInput(1) == 0x400 || work->originalMachine != 0xffffffffU) {
        if (FTBL.GetPlayerStat(2) >= 5) {
#if SHENMUE_EVENT_VEND_SCENE
            int frames = VendingInsertCoin(-5);
#else
            int frames = VendingInsertCoin();
#endif
            FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_InitSelect);
            FTBL.HLib_SetupTaskDelay(frames);
        } else {
            int frames = VendingNoMoney();
            FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_VendEnd);
            FTBL.HLib_SetupTaskDelay(frames);
        }
    }
}
void eVEND_EVT_InitSelect(void *data) {
    VendWork *work = (VendWork *)data;
    VendButtons *buttons = (VendButtons *)&work->buttons;
    const Vector3 *button;
    FTBL.DebugLogToFileRemoved("@@@  EVT_InitSelect( ) \n");
#if !SHENMUE_EVENT_VEND_SCENE
    HideVendingButtons(buttons);
#endif
    button = &buttons->positions[buttons->selected];
    FTBL.EV_DisableCharacterDisplay(ID_RYO);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ID_HAND), 1, 0, 0);
    FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(ID_HAND), 0, 21, &vendSelectionHandRotation);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(ID_HAND), 0, vendSelectionHandCommand, 0);
    FTBL.EV_EnableCharacterDisplay(ID_HAND);
    FTBL.EV_EnsureCharacterDraw(ID_HAND);
    EVT_AT_SetPos(ID_HAND, ID_BASE, vendHandOffset.x, vendHandOffset.y, vendHandOffset.z, 0);
    EVT_AT_SetAng(ID_HAND, ID_BASE, vendHandRotation.x, vendHandRotation.y, vendHandRotation.z, 0);
    EVT_AT_SetPos(ID_BASE, ID_MACHINE, button->x, button->y, button->z, 0);
    EVT_AT_SetAng(ID_BASE, ID_MACHINE, 0.0f, 0.0f, 0.0f, 0);
    work->state = 1;
    work->timer = 0;
    EVT_AT_SetCam(1101, ID_BASE, ID_BASE, 0, 0, 0);
    HideVendingLamps();
    FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_SelectJuice);
}
void eVEND_EVT_SelectJuice(void *data) {
#if SHENMUE_EVENT_VEND_SCENE

    VendWork *work = (VendWork *)data;
    VendButtons *buttons = (VendButtons *)&work->buttons;
    int minutes = FTBL.shenGetMinutesUntilNextFrameMinute();
    unsigned int input;
    work->elapsed++;
    if (minutes < 1) {
        QueueEventMessage("（仮）寝るぞ！！", 50, 0);
        if (work->prizeState == 3 || work->prizeState == 0)
            EVT_AT_StartMoneyCount(5, 70);
        FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_VendEnd);
        FTBL.HLib_SetupTaskDelay(50);
    } else if (work->elapsed > 5400) {
        QueueEventMessage("（仮）やめとくか…", 50, 0);
        if (work->prizeState == 3 || work->prizeState == 0)
            EVT_AT_StartMoneyCount(5, 70);
        FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_VendEnd);
        FTBL.HLib_SetupTaskDelay(100);
    }
    FTBL.GetBattleDataConfig(0, SHEN_NAME4('B', 'T', 'C', 'C'));
    FTBL.GetBattleDataConfig(0, SHEN_NAME4('B', 'T', 'E', 'V'));
    input = FTBL.GetCurrentControlInput(1);
    if (FTBL.GetBattleDataConfig(1, SHEN_NAME4('B', 'T', 'E', 'V')) && FTBL.GetBattleDataConfig(2, SHEN_NAME4('B', 'T', 'E', 'V'))) {
        int frames;
        work->state = 2;
        work->timer = 0;
        if (buttons->selected > 5)
            work->sound = 1;
        else
            work->sound = 0;
        frames = PressVendingButton(&buttons->positions[buttons->selected]);
        vendLamps.selected = buttons->selected;
        QueueVendingAutoWait(RestoreVendingLamps, &vendLamps, frames - 5, frames - 7);
        if (work->prizeState == 0) {
            work->prizeState = 1;
            DetermineVendingPrize(work, FTBL.GetRandomNumberInRange(1));
        }
        if (work->prizeState == 1) {
            work->prizeState = 2;
            FTBL.EV_ControlPackageResources(0, 8, (void *)vendArchive, vendCanRecords[8][0], vendCanRecords[8][1]);
            FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_HitCan);
            FTBL.HLib_SetupTaskDelay(30);
        } else {
            FTBL.EV_ControlPackageResources(0, 8, (void *)vendArchive, vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
            FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_DrinkJuice);
        }
        FTBL.HLib_SetupTaskDelay(frames);
    } else if (FTBL.GetBattleDataConfig(1, SHEN_NAME4('B', 'T', 'C', 'C')) && FTBL.GetBattleDataConfig(2, SHEN_NAME4('B', 'T', 'C', 'C'))) {
        if (work->prizeState == 3 || work->prizeState == 0)
            EVT_AT_StartMoneyCount(5, 0);
        FTBL.DebugLogToFileRemoved("Put B button at selecting Can\n");
        FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_VendEnd);
    } else {
        int previous = buttons->selected;
        if (input == 0x8000)
            SelectNextVendingButton(buttons);
        else if (input == 0x4000)
            SelectPreviousVendingButton(buttons);
        else if (input == 0x1000)
            SelectUpperVendingButton(buttons);
        else if (input == 0x2000)
            SelectLowerVendingButton(buttons);
        if (input == 0x8000 || input == 0x4000 || input == 0x1000 || input == 0x2000)
            FTBL.DebugLogToFileRemoved("Can No:mw->cbtn=%d\n", buttons->selected);
        if (previous != buttons->selected) {
            InterpolateVendingHand(&buttons->positions[buttons->selected], &buttons->positions[previous], 1, 0);
        }
    }

#else

    VendWork *work = (VendWork *)data;
    VendButtons *buttons = (VendButtons *)&work->buttons;
    int frames;
    unsigned int input = FTBL.GetCurrentControlInput(1);
    if (input == 0x400) {
        work->state = 2;
        work->timer = 0;
        frames = PressVendingButton((VendButtons *)&buttons->positions[buttons->selected], -5);
        FTBL.DebugLogToFileRemoved("Load Can data in EVT_SelectJuice no:%d/%d\n", vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
        FTBL.EV_ControlPackageResources(0, 8, (void *)vendArchive, vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
        vendLamps.selected = buttons->selected;
        QueueVendingAutoWait(RestoreVendingLamps, &vendLamps, frames - 5, frames - 7);
        FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_DrinkJuice);
        FTBL.HLib_SetupTaskDelay(frames);
    } else if (input == 0x200) {
        FTBL.DebugLogToFileRemoved("Put B button at selecting Can\n");
        FTBL.DebugLogToFileRemoved("ET_AbortVend at B-button cancel.\n");
        eVEND_ET_AbortVend(work);
    } else {
        int previous = buttons->selected;
        if (input == 0x8000)
            SelectNextVendingButton(buttons);
        else if (input == 0x4000)
            SelectPreviousVendingButton(buttons);
        else if (input == 0x1000)
            SelectUpperVendingButton(buttons);
        else if (input == 0x2000)
            SelectLowerVendingButton(buttons);
        if (input == 0x8000 || input == 0x4000 || input == 0x1000 || input == 0x2000)
            FTBL.DebugLogToFileRemoved("Can No:mw->cbtn=%d\n", buttons->selected);
        if (previous != buttons->selected) {
            frames = InterpolateVendingHand((VendButtons *)&buttons->positions[buttons->selected], (VendButtons *)&buttons->positions[previous], 1, 0);
            FTBL.HLib_SetupTaskDelay(frames);
        }
    }

#endif
}
#if SHENMUE_EVENT_VEND_SCENE
void DetermineVendingPrize(VendWork *work, float random) {
    if (random < 0.08f) {
        FTBL.DebugLogToFileRemoved("Dual-S val=%f\n", random);
        work->prizeItem = 4016;
    } else if (random >= 0.08f && random < 0.16f) {
        FTBL.DebugLogToFileRemoved("Dual-G val=%f\n", random);
        work->prizeItem = 4017;
    } else if (random >= 0.16f && random < 0.24f) {
        FTBL.DebugLogToFileRemoved("PROB_AKIRA val=%f\n", random);
        work->prizeItem = 4000;
    } else if (random >= 0.24f && random < 0.32f) {
        FTBL.DebugLogToFileRemoved("JUKY  val=%f\n", random);
        work->prizeItem = 4002;
    } else if (random >= 0.32f && random < 0.4f) {
        FTBL.DebugLogToFileRemoved("SARA  val=%f\n", random);
        work->prizeItem = 4004;
    } else if (random >= 0.4f && random < 0.48f) {
        FTBL.DebugLogToFileRemoved("RAU  val=%f\n", random);
        work->prizeItem = 4006;
    } else if (random >= 0.48f && random < 0.56f) {
        FTBL.DebugLogToFileRemoved("PAI  val=%f\n", random);
        work->prizeItem = 4008;
    } else if (random >= 0.56f && random < 0.64f) {
        FTBL.DebugLogToFileRemoved("URFU  val=%f\n", random);
        work->prizeItem = 4010;
    } else if (random >= 0.64f && random < 0.72f) {
        FTBL.DebugLogToFileRemoved("JEFLY  val=%f\n", random);
        work->prizeItem = 4012;
    } else if (random >= 0.72f && random < 0.8f) {
        FTBL.DebugLogToFileRemoved("KAGE  val=%f\n", random);
        work->prizeItem = 4014;
    } else
        work->prizeState = 3;
}
#endif
void eVEND_EVT_DrinkJuice(void *data) {
#if SHENMUE_EVENT_VEND_SCENE

    VendWork *work = (VendWork *)data;
    VendButtons *buttons = (VendButtons *)&work->buttons;
    int frames;
    FTBL.DebugLogToFileRemoved("@@@     EVT_DrinkJuice( ) \n");
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    work->state = 4;
    FTBL.DebugLogToFileRemoved("Loading can done in EVT_SelectJuice no:%d\n", vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
    if (work->sound != 0) {
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(ID_RYO), 0, vendAltHandCommand, 0);
        FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(ID_RYO), 0, 21, &vendAltHandRotation);
    } else {
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(ID_RYO), 0, vendCanHandCommand, 0);
        FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(ID_RYO), 0, 21, &vendCanHandRotation);
    }
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(ID_RYO), 1, vendDrinkRightCommand, 190);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(ID_RYO), 1, vendReturnRightCommand, 410);
    frames = VendingLiftCan(work->sound);
    frames = VendingDrink(frames);
    FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_VendEnd);
    FTBL.HLib_SetupTaskDelay(frames);

#else

    VendWork *work = (VendWork *)data;
    VendButtons *buttons = (VendButtons *)&work->buttons;
    int frames;
    FTBL.DebugLogToFileRemoved("@@@     EVT_DrinkJuice( ) \n");
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    work->state = 4;
    FTBL.DebugLogToFileRemoved("Loading can done in EVT_SelectJuice no:%d\n", vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
    frames = VendingLiftCan();
    frames = VendingDrink(frames);
    EVT_AT_MenCtrl(16, 0, 0, 0, 0, 255, 0, 0, 0, frames - 16);
    FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_VendEnd);
    FTBL.HLib_SetupTaskDelay(frames);

#endif
}
#if SHENMUE_EVENT_VEND_SCENE
void eVEND_EVT_HitCan(void *data) {
    VendWork *work = (VendWork *)data;
    VendButtons *buttons = (VendButtons *)&work->buttons;
    int frames;
    int unused = 0;
    switch (vendFade) {
    case 0:
        vendFade = 1;
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        work->state = 4;
        FTBL.DebugLogToFileRemoved("Loading can done in EVT_SelectJuice no:%d\n", vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
        FTBL.DebugLogToFileRemoved("HIT_prosesing_01\n");
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(ID_RYO), 0, vendCanHandCommand, 0);
        FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(ID_RYO), 0, 21, &vendCanHandRotation);
        frames = VendingLiftWinningCan(work->sound);
        frames += 90;
        QueueVendingItem(work->prizeItem, frames - 120);
        EVT_AT_MenCtrl(6, 0, 0, 0, 0, 255, 0, 0, 0, frames - 10);
        EVT_AT_MenCtrl(40, 255, 0, 0, 0, 0, 0, 0, 0, frames);
        FTBL.HLib_SetupTaskDelay(frames);
        break;
    case 1:
        vendFade = 0;
        FTBL.EV_DisableCharacterDisplay(ID_CAN);
        if (vendArchive != -1) {
            if (FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0)) {
                FTBL.DebugLogToFileRemoved("Hit Hit 1 Cancel(EV_DESTROY_PAKFILE_AFS) read in ET_KillVend no:%d\n", vendCanRecords[8][0], vendCanRecords[8][1]);
                FTBL.EV_ControlPackageResources(0, 13, (void *)vendArchive, vendCanRecords[8][0], vendCanRecords[8][1]);
            } else if (work->state == 4) {
                FTBL.DebugLogToFileRemoved("Hit Hit 2 Do EV_FREE_PAKFILE_AFS\n");
                FTBL.EV_ControlPackageResources(0, 12, (void *)vendArchive, vendCanRecords[8][0], vendCanRecords[8][1]);
            }
        }
        work->state = 1;
        FTBL.DebugLogToFileRemoved("HIT_prosesing_02\n");
        FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_InitSelect);
        break;
    }
}
#endif
void eVEND_EVT_VendEnd(void *data) {
#if SHENMUE_EVENT_VEND_SCENE

    FTBL.DebugLogToFileRemoved("@@@             EVT_VendEnd( ) \n");
    FTBL.DebugLogToFileRemoved("Do EV_EventSetUpHelpIcon\n");
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.DebugLogToFileRemoved("EntryNumber is %d\n", g_entryPoint);
    if (g_entryPoint == 99)
        FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_InitVend);
    else {
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)eVEND_ET_KillVend);
        FTBL.HLib_SetupTaskDelay(8);
    }

#else

    FTBL.DebugLogToFileRemoved("@@@             EVT_VendEnd( ) \n");
    FTBL.DebugLogToFileRemoved("Do EV_EventSetUpHelpIcon\n");
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.DebugLogToFileRemoved("EntryNumber is %d\n", g_entryPoint);
    if (g_entryPoint == 99)
        FTBL.HLib_SetCurrentTaskTo(eVEND_EVT_InitVend);
    else
        eVEND_ET_KillVend(data);

#endif
}
#if SHENMUE_EVENT_VEND_SCENE
void eVEND_ET_AbortVend(void *data) {
    FTBL.DebugLogToFileRemoved("@@@    EV_AbortVend( ) @\n");
    eVEND_ET_KillVend(data);
}
void eVEND_ET_KillVend(VendWork *work) {
    VendButtons *buttons = (VendButtons *)&work->buttons;
    FTBL.DebugLogToFileRemoved("@@@     EV_KillVend( ) \n");
    if (vendArchive != -1) {
        if (FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0)) {
            FTBL.DebugLogToFileRemoved(
                "Cancel(EV_DESTROY_PAKFILE_AFS) read in ET_KillVend no:%d\n", vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
            FTBL.EV_ControlPackageResources(0, 13, (void *)vendArchive, vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
        } else if (work->state == 4) {
            FTBL.DebugLogToFileRemoved("Do EV_FREE_PAKFILE_AFS\n");
            FTBL.EV_ControlPackageResources(0, 12, (void *)vendArchive, vendCanRecords[buttons->selected][0], vendCanRecords[buttons->selected][1]);
        }
        work->state = 5;
        FTBL.DebugLogToFileRemoved("EV_UNMOUNT_AFS_PARTITION in ET_KillVend\n");
        FTBL.EV_ControlPackageResources(1, 1, (void *)vendArchive, 0, 0);
        vendArchive = -1;
    }
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    FTBL.DebugLogToFileRemoved("EV_DispUpHandModel False in ET_KillVend\n");
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ID_HAND), 0, 0, 0);
    FTBL.DebugLogToFileRemoved("EV_DisableCharDisplay ID_HAND in ET_KillVend\n");
    FTBL.EV_DisableCharacterDisplay(ID_HAND);
    FTBL.DebugLogToFileRemoved("EV_FreeData Real Hand in ET_KillVend\n");
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ID_HAND));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ID_RYO));
    FTBL.DebugLogToFileRemoved("Do EV_HLKillOtherHierarchyTask\n");
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.DebugLogToFileRemoved("Do EV_HLKillOtherHierarchyTask\n");
    FTBL.EV_HLKillOtherHierarchyTask(work->procTask);
    FTBL.EV_SetPlayerFlag(12, 551, 0);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.DebugLogToFileRemoved("Do ET_KillVend\n");
    ReleaseVendingResources(work);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.DebugLogToFileRemoved("Do EV_NPC_NewLoadSw\n");
    FTBL.EV_NPC_NewLoadSw_0(1);
    EVT_AT_MenCtrl(10, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.DebugLogToFileRemoved("Do ET_AB_EndEvent\n");
    EventDebugUnregister(3);
}
void VendingProcCallback(void *data) {
}
#endif
void InitializeVendingButtons(VendButtons *buttons) {
    int i;
    buttons->count = 8;
    buttons->upper = 6;
    buttons->selected = 3;
    {
        Vector3 origin;
        origin.x = 0.0 - (buttons->upper / 2 - 1) * 0.086 - 0.015;
        origin.y = 1.25;
        origin.z = 0.42;
        for (i = 0; i < buttons->count; i++) {
            if (i < buttons->upper) {
                buttons->positions[i].x = origin.x + i * 0.086;
                buttons->positions[i].y = origin.y + 0.0;
                buttons->positions[i].z = origin.z + 0.0;
            } else {
                buttons->positions[i].x = origin.x + (i - buttons->count + buttons->upper) * 0.086;
                buttons->positions[i].y = origin.y - 0.3;
                buttons->positions[i].z = origin.z + 0.0;
            }
        }
    }
}
#if !SHENMUE_EVENT_VEND_SCENE
void HideVendingButtons(VendButtons *buttons) {
}
#endif
#if !SHENMUE_EVENT_VEND_SCENE
void ShowVendingButtons(VendButtons *buttons) {
}
#endif
void SelectNextVendingButton(VendButtons *buttons) {
    if (buttons->selected != buttons->upper - 1 && buttons->selected < buttons->count - 1)
        buttons->selected++;
}
void SelectPreviousVendingButton(VendButtons *buttons) {
    if (buttons->selected > 0 && buttons->selected != buttons->upper)
        buttons->selected--;
}
void SelectUpperVendingButton(VendButtons *buttons) {
    if (buttons->selected >= buttons->upper)
        buttons->selected = buttons->selected - buttons->count + buttons->upper;
}
void SelectLowerVendingButton(VendButtons *buttons) {
    if (buttons->selected < buttons->upper) {
        buttons->selected = buttons->selected - buttons->upper + buttons->count;
        if (buttons->selected < buttons->upper)
            buttons->selected = buttons->upper;
    }
}
#if !SHENMUE_EVENT_VEND_SCENE
void InitializeVendingCan(void) {
}
#endif
#if !SHENMUE_EVENT_VEND_SCENE
void ReleaseVendingCan(void) {
}
#endif
void InitializeVendingResources(VendWork *work) {
#if !SHENMUE_EVENT_VEND_SCENE
    Vector3 position;
    Vector3i rotation;
#endif
    const char *directory;
    work->characters = Queue_EVT_AT_ReadChr(work->task, vendCharacters);
    InitializeVendingButtons((VendButtons *)&work->buttons);
    directory = FTBL.Filepath_Gen(7);
    FTBL.EV_ControlPackageResources(0, 0, (void *)directory, (int)"VEND", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "VEND", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "VEND", 0, 0);
#if SHENMUE_EVENT_VEND_SCENE
    FTBL.EV_SetNewSceneIndex();
#endif
    FTBL.EV_LoadCutsceneFromCurrentScene("0109E000");
#if SHENMUE_EVENT_VEND_SCENE
    work->prizeState = 0;
    work->elapsed = 0;
#else
    FTBL.EV_EnableCharacterDisplay(ID_MACHINE);
    if (FTBL.DoesScenarioCharacterExist(work->originalMachine)) {
        FTBL.EV_DisableCharacterDisplay(work->originalMachine);
        FTBL.GetCharaPosition(work->originalMachine, 0xffffffffU, &position, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(work->originalMachine, &rotation, 0x40000000);
        FTBL.EV_AT_SetPos(ID_MACHINE, 0x78000000, &position);
        FTBL.EV_AT_SetAngY_0(ID_MACHINE, 0x78000000, &rotation, 0);
    }
#endif
}
void ReleaseVendingResources(VendWork *work) {
    FTBL.DebugLogToFileRemoved("Do EV_EnableCharDisplay\n");
    FTBL.EV_EnableCharacterDisplay(ID_RYO);
    FTBL.DebugLogToFileRemoved("Do EV_DisableCharDisplay\n");
    FTBL.EV_DisableCharacterDisplay(ID_MACHINE);
    FTBL.DebugLogToFileRemoved("Do EV_IsCharacterExist\n");
    if (FTBL.DoesScenarioCharacterExist(work->originalMachine)) {
        FTBL.DebugLogToFileRemoved("Do EV_EnableCharDisplay\n");
        FTBL.EV_EnableCharacterDisplay(work->originalMachine);
    }
    FTBL.EV_ControlPackageResources(0, 6, "VEND", 0, 0);
    FTBL.DebugLogToFileRemoved("Do ET_AT_KillWork\n");
    ET_AT_KillWork(work->characters);
}
void eVEND_ET_SetHandUpYubiWait(void *data) {
    VendHandWait *work = (VendHandWait *)data;
    if (work->state == 2) {
        work->state = 3;
        FTBL.DebugLogToFileRemoved("EVVendHand_END in ET_SetHandUpYubiWait\n");
        FTBL.EV_HLKillOtherHierarchyTask(work->task);
    }
    if (work->state == 4) {
        FTBL.DebugLogToFileRemoved("EVVendHand_ERROR in ET_SetHandUpYubiWait\n");
        FTBL.EV_HLKillOtherHierarchyTask(work->task);
    }
    if (work->timer > work->threshold) {
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(work->character), work->side, work->commands, work->duration);
        work->state = 2;
        FTBL.HLib_SetupTaskDelay(work->duration);
    }
    work->timer++;
}
int QueueVendingHandWait(shen_name character, int side, const int *commands, int duration, int threshold) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(eVEND_ET_SetHandUpYubiWait, 3, 11, sizeof(VendHandWait), TASK_EVNT);
    VendHandWait *work = (VendHandWait *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    memcpy(work->commands, commands, sizeof(work->commands));
    work->character = character;
    work->side = side;
    work->duration = duration;
    work->threshold = threshold;
    work->state = 0;
    work->timer = 0;
    return threshold + duration;
}
#endif
#if defined(SHENMUE_EVENT_VEND_ACTIONS)
#if SHENMUE_EVENT_VEND_SCENE
int VendingInsertCoin(int cash) {
    EVT_AT_SetPosAng(ID_RYO, ID_MACHINE, 0.0f, 1.0f, 0.0f, 0);
    EVT_AT_SetMot(ID_RYO, 5060, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetCam(1103, ID_MACHINE, ID_MACHINE, 0, 0, 0);
    EVT_AT_SetSound(1481, 30);
    EVT_AT_SetSound(191, 125);
    EVT_AT_StartMoneyCount(cash, 70);
    return 170;
}
#else
int VendingInsertCoin(void) {
    EVT_AT_SetPosAng(ID_RYO, ID_MACHINE, 0.039135001599788666f, 0.8900179862976074f, 0.0f, 0);
    EVT_AT_SetMot(ID_RYO, 5057, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetCam(1103, ID_MACHINE, ID_MACHINE, 0, 0, 0);
    EVT_AT_SetSound(191, 125);
    return 170;
}
#endif
#if SHENMUE_EVENT_VEND_SCENE
int VendingLiftCan(int sound) {
    Queue_EVT_AT_Cling(ID_CAN, ID_RYO, 14, &vendCanAttachment, &vendCanRotation, 82, 0);
    EVT_AT_CharaDisp(ID_HAND, 0, 1);
    EVT_AT_CharaDisp(ID_RYO, 0, 0);
    EVT_AT_CharaDisp(ID_CAN, 82, 0);
    EVT_AT_SetPosAng(ID_RYO, ID_MACHINE, 0.0f, 1.0f, 0.0f, 0);
    EVT_AT_SetMot(ID_RYO, 5061, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetCam(1104, ID_MACHINE, ID_MACHINE, 0, 0, 0);
    EVT_AT_SetSound(198, 80);
    if (sound)
        EVT_AT_SetSound(195, 290);
    else
        EVT_AT_SetSound(196, 290);
    return 170;
}
#else
int VendingLiftCan(void) {
    Queue_EVT_AT_Cling(ID_CAN, ID_RYO, 14, &vendCanAttachment, &vendCanRotation, 82, 0);
    EVT_AT_CharaDisp(ID_HAND, 0, 1);
    EVT_AT_CharaDisp(ID_RYO, 0, 0);
    EVT_AT_CharaDisp(ID_CAN, 82, 0);
    EVT_AT_SetPosAng(ID_RYO, ID_MACHINE, 0.039135001599788666f, 0.8900179862976074f, 0.0f, 0);
    EVT_AT_SetMot(ID_RYO, 5060, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetCam(1104, ID_MACHINE, ID_MACHINE, 0, 0, 0);
    EVT_AT_SetSound(198, 80);
    EVT_AT_SetSound(196, 290);
    return 170;
}
#endif
#if SHENMUE_EVENT_VEND_SCENE
int VendingLiftWinningCan(int sound) {
    Queue_EVT_AT_Cling(ID_CAN, ID_RYO, 14, &vendWinningAttachment, &vendWinningRotation, 82, 0);
    EVT_AT_CharaDisp(ID_HAND, 0, 1);
    EVT_AT_CharaDisp(ID_RYO, 0, 0);
    EVT_AT_CharaDisp(ID_CAN, 82, 0);
    EVT_AT_SetPosAng(ID_RYO, ID_MACHINE, 0.0f, 1.0f, 0.0f, 0);
    EVT_AT_SetMot(ID_RYO, 5061, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetCam(1104, ID_MACHINE, ID_MACHINE, 0, 0, 0);
    EVT_AT_SetSound(198, 80);
    QueueEventMessage("んっ？ 当たり缶だ。", 140, 20);
    return 180;
}
#endif
int VendingDrink(int delay) {
    int detach;
    Queue_EVT_AT_Cling(ID_CAN, ID_RYO, 14, 0, 0, (detach = delay + 690), 1);
    EVT_AT_CharaDisp(ID_CAN, detach, 1);
#if SHENMUE_EVENT_VEND_SCENE
    EVT_AT_SetMot(ID_RYO, 5059, 0.0f, 1.0f, 0, delay);
#else
    EVT_AT_SetMot(ID_RYO, 5058, 0.0f, 1.0f, 0, delay);
#endif
    EVT_AT_SetCam(1105, ID_MACHINE, ID_MACHINE, 0, 0, delay);
    EVT_AT_SetCam(1106, ID_MACHINE, ID_MACHINE, 0, 0, delay + 575);
    EVT_AT_SetSound(192, delay + 210);
#if SHENMUE_EVENT_VEND_SCENE
    EVT_AT_SetSound(192, delay + 285);
#else
    EVT_AT_SetSound(192, delay + 290);
#endif
    EVT_AT_SetSound(192, delay + 470);
    EVT_AT_SetSound(197, detach);
    return delay + 765;
}
static const float vendInterpolation[8] = {0, .022727f, .113636f, .318182f, .681818f, .886364f, .977273f, 1};
int InterpolateVendingHand(const Vector3 *target, const Vector3 *start, int step, int delay) {
#if SHENMUE_EVENT_VEND_SCENE

    int i;
    for (i = 0; i < 8; i += step) {
        shen_name machine = ID_MACHINE;
        const float *p = (const float *)target;
        float x, y, z;
        x = (*p++ - start->x) * vendInterpolation[i] + start->x;
        y = (*p - start->y) * vendInterpolation[i] + start->y;
        p = (const float *)target + 2;
        z = (*p - start->z) * vendInterpolation[i] + start->z;
        EVT_AT_SetPos(ID_BASE, machine, x, y, z, i + delay);
    }
    return 8 / step + delay;

#else

    int i;
    for (i = 0; i < 8; i += step) {
        const float *p = (const float *)target;
        float x, y, z;
        x = (*p++ - start->x) * vendInterpolation[i] + start->x;
        y = (*p - start->y) * vendInterpolation[i] + start->y;
        p = (const float *)target + 2;
        z = (*p - start->z) * vendInterpolation[i] + start->z;
        EVT_AT_SetPos(ID_BASE, ID_MACHINE, x, y, z, i + delay);
    }
    return 8 / step + delay;

#endif
}
#if SHENMUE_EVENT_VEND_SCENE
int PressVendingButton(const Vector3 *button) {
    Vector3 pressed, target;
    int begin, end;
    target.x = button->x;
    target.y = button->y - 0.01f;
    target.z = button->z;
    *(int *)&pressed.x = *(int *)&target.x;
    *(int *)&pressed.y = *(int *)&target.y;
    *(int *)&pressed.z = *(int *)&target.z;
    target.x = pressed.x;
    target.y = pressed.y;
    target.z = pressed.z - 0.008f;
    begin = InterpolateVendingHand(&pressed, button, 1, 7);
    QueueVendingHandWait(ID_HAND, 0, vendWaitHandCommand, 5, 5);
    end = InterpolateVendingHand(&target, &pressed, 4, begin);
    end = InterpolateVendingHand(&pressed, &target, 4, end);
    EVT_AT_CharaDisp(ID_RYO, 0, 1);
    EVT_AT_SetCam(1102, ID_MACHINE, ID_MACHINE, 0, 0, 0);
    EVT_AT_SetSound(190, end - 2);
    EVT_AT_SetSound(193, end);
    return end;
}
#else
int PressVendingButton(const Vector3 *button, int cash) {
    Vector3 pressed, target;
    int begin, end;
    SHENMUE_EVENT_CLOBBER("r9");
    target.x = button->x;
    target.y = button->y - 0.03f;
    target.z = button->z;
    ((int *)&pressed)[0] = ((int *)&target)[0];
    ((int *)&pressed)[1] = ((int *)&target)[1];
    ((int *)&pressed)[2] = ((int *)&target)[2];
    target.x = pressed.x;
    target.y = pressed.y;
    target.z = pressed.z - 0.008f;
    EVT_AT_StartMoneyCount(cash, 10);
    begin = InterpolateVendingHand(&pressed, button, 1, 7);
    QueueVendingHandWait(ID_HAND, 0, vendWaitHandCommand, 5, 5);
    end = InterpolateVendingHand(&target, &pressed, 4, begin);
    end = InterpolateVendingHand(&pressed, &target, 4, end);
    EVT_AT_CharaDisp(ID_RYO, 0, 1);
    EVT_AT_SetCam(1102, ID_MACHINE, ID_MACHINE, 0, 0, 0);
    EVT_AT_SetSound(190, end - 2);
    EVT_AT_SetSound(193, end);
    return end;
}
#endif
int VendingNoMoney(void) {
#if SHENMUE_EVENT_VEND_SCENE

    EVT_AT_StartMoneyCount(0, 10);
    QueueEventMessage("金がないな…", 30, 60);
    EVT_AT_SetPosAng(ID_RYO, ID_MACHINE, 0, 1, 0, 0);
    return 70;

#else

    EVT_AT_StartMoneyCount(0, 10);
    QueueEventMessage("金がないな…", 30, 60);
    EVT_AT_SetPosAng(ID_RYO, ID_MACHINE, 0.039135001599788666f, 0.8900179862976074f, 0.0f, 0);
    EVT_AT_SetMot(ID_RYO, 5057, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetCam(1103, ID_MACHINE, ID_MACHINE, 0, 0, 0);
    return 90;

#endif
}
#if SHENMUE_EVENT_VEND_SCENE
void VendingItemCallback(void *data) {
    int *work = (int *)data;
    FTBL.AddInventoryItem(work[1], 1, 1);
    if (work[0])
        FTBL.HLib_DestroyCurrentTask();
}
#endif
#if SHENMUE_EVENT_VEND_SCENE
void QueueVendingItem(int item, int delay) {
    if (delay != 0) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(VendingItemCallback, 6, 11, 8, TASK_EVNT);
        int *work;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        work = FTBL.HLib_GetTaskParameter(task);
        work[0] = 1;
        work[1] = item;
    } else
        FTBL.AddInventoryItem(item, 1, 1);
}
#endif
int HideVendingLamps(void) {
    Vector3 position;
    Vector3i rotation;
    int i;
    for (i = 0; i < 8; i++) {
        FTBL.scenApplyPosOrRotToChildId(ID_MACHINE, vendLamps.parts[i], &position, &rotation, 2);
        vendLamps.positions[i].x = position.x;
        vendLamps.positions[i].y = position.y;
        vendLamps.positions[i].z = position.z;
        position.z += 0.1f;
        FTBL.scenApplyPosOrRotToChildId(ID_MACHINE, vendLamps.parts[i], &position, 0, 0);
    }
    return 0;
}
void RestoreVendingLamps(void *data) {
    VendLamps *lamps = (VendLamps *)data;
    int i;
    for (i = 0; i < 8; i++) {
        if (lamps->selected != lamps->parts[i])
            FTBL.scenApplyPosOrRotToChildId(ID_MACHINE, lamps->parts[i], &lamps->positions[i], 0, 0);
        else if (lamps->selected == lamps->parts[i])
            FTBL.DebugLogToFileRemoved("@@@   part == lamp[%d]\n", i);
    }
}
int ShowVendingLamps(VendLamps *lamps) {
    int i;
    for (i = 0; i < 8; i++)
        FTBL.scenApplyPosOrRotToChildId(ID_MACHINE, lamps->parts[i], &lamps->positions[i], 0, 0);
    return 0;
}
int GetVendingAutoWaitState(void) {
    HLibTask *task = FTBL.HLib_GetCurrentTask();
    return ((VendAutoWait *)FTBL.HLib_GetTaskParameter(task))->state;
}
int FinishVendingAutoWait(void) {
    HLibTask *task = FTBL.HLib_GetCurrentTask();
    return ((VendAutoWait *)FTBL.HLib_GetTaskParameter(task))->state = 5;
}
void eVEND_ET_SetVendAutoWait(void *data) {
    VendAutoWait *work = (VendAutoWait *)data;
    switch (work->state) {
    case 0:
    case 1:
        if (work->timer >= work->begin) {
            work->callback(work->argument);
            work->state = 2;
        } else
            work->state = 1;
        break;
    case 2:
        if (work->timer < work->end)
            work->callback(work->argument);
        else
            work->state = 3;
        break;
    case 3:
    case 4:
    case 5:
        FTBL.EV_HLKillOtherHierarchyTask(work->task);
        break;
    }
    work->timer++;
}
int QueueVendingAutoWait(HLibTaskCallback callback, void *argument, int begin, int end) {
    if (begin >= end)
        end = begin + 1;
    if (!callback || ((unsigned int)callback & 1))
        return 0;
    {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(eVEND_ET_SetVendAutoWait, 3, 11, sizeof(VendAutoWait), TASK_EVNT);
        VendAutoWait *work = (VendAutoWait *)FTBL.HLib_GetTaskParameter(task);
        work->task = task;
        work->begin = begin;
        work->end = end;
        work->timer = work->state = 0;
        work->callback = callback;
        work->argument = argument;
    }
    return end;
}
#if SHENMUE_EVENT_VEND_SCENE
int GetVendingMachineColour(const shen_name *scene) {
    int i;
    for (i = 0; vendColours; i++) {
        if (vendColours[i].map == *scene)
            return vendColours[i].colour;
    }
    return 0;
}
#endif
#endif
#endif

#else
#include "s2/areas/area_vend.hpp"
#include "s2/areas/vending_hand_commands.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/debug.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cstring>

namespace shenmue::s2 {
namespace {
VendMachState vendingState;
std::int32_t vendingElapsed, vendingValidSelection;
std::uint16_t vendingPrizeVoice = 1;
std::int64_t vendingArchive = -1;
shen_name vendingMachine;
VendingLampBank vendingLamps;
const Vector3 vendingInitialBasePosition{
    .x = std::bit_cast<float>(0xffffffffU), .y = std::bit_cast<float>(0xffffffffU), .z = std::bit_cast<float>(0xffffffffU)};
const Vector3i vendingInitialBaseRotation{.x = -1, .y = -1, .z = -1};

void eVEND_EVT_InitVend(void *);
void eVEND_EVT_CheckVend(void *);
void eVEND_EVT_InitSelect(void *);
void eVEND_EVT_VoiceInSelect(void *);
void eVEND_EVT_SelectJuice(void *);
void eVEND_EVT_DrinkJuice(void *);
void eVEND_EVT_HitCan(void *);
void eVEND_EVT_VendEnd(void *);
void eVEND_ET_KillVend(void *);

// @verified - p
void VendingProcCallback(void *) {
}

void SelectVendingCanRecords(VendingMachineWork &work) {
    const VendingCanRecord &record = (work.colour == VM_COLOR_RED ? vendRedCans : vendOtherCans)[work.previousSelected];
    work.archiveRecord = record.record;
    work.alternateRecord = record.alternate;
    work.sound = record.sound;
    work.canKind = record.kind;
}

void PlayVendingVoice(VendingMachineWork &work, std::int32_t delay) {
    const float random = GetRandomNumberInRange(1.0f);
    const char *voice = nullptr;
    switch (work.voiceState) {
    case 1:
        if (!(random >= .2))
            voice = "0109A001"; // @text EN: "A drink." | JA: "ジュースか。"
        else if (random < .4)
            voice = "0109A002"; // @text EN: "A vending machine." | JA: "自動販売機があるな。"
        else if (random < .6)
            voice = "0109A003"; // @text EN: "$5 a can." | JA: "１本５ドルか…"
        else if (random < .8)
            voice = "0109A004"; // @text EN: "I think I'll get a drink." | JA: "１本買ってみるか。"
        else
            voice = "0109A005"; // @text EN: "I'll buy a can." | JA: "ちょっと買ってみるか。"
        break;
    case 2:
        voice = random >= .5 ? "0109A009" : "0109A008"; // @text 0109A009 EN: "This isn't enough to buy me a can." | JA: "これじゃ、買えないな…" ; 0109A008 EN: "I don't have enough money." | JA: "足りないな…"
        break;
    case 3:
        voice = "0109A010"; // @text EN: "Which one should I get?" | JA: "どれにするか…"
        break;
    case 4:
        voice = !(random >= .3) ? "0109A036" : (random < .1 || random >= .6 ? "0109A038" : "0109A037"); // @text 0109A036 EN: "I'll get another can." | JA: "もう１本か…" ; 0109A038 EN: "I'll get an extra can." | JA: "もう１本だな。" ; 0109A037 EN: "Another can." | JA: "もう１本…"
        break;
    case 5:
        voice = "0109A013"; // @text EN: "This one." | JA: "これか。"
        break;
    case 7:
        voice = "0109A029"; // @text EN: "Maybe next time." | JA: "またにするか…"
        break;
    case 8:
        switch (vendingPrizeVoice) {
        case 1:
            voice = random < .3 ? "0109A026" : (random >= .6 ? "0109A028" : "0109A027"); // @text 0109A026 EN: "Oh, a winning can." | JA: "んっ…当たり缶だ。" ; 0109A028 EN: "Oh, it's a winner." | JA: "おっ…当たったな。" ; 0109A027 EN: "Oh, I got a winning can." | JA: "おっ…当たった。"
            break;
        case 2:
            voice = random < .3 ? "0109A030" : (random >= .6 ? "0109A032" : "0109A031"); // @text 0109A030 EN: "Inside=@" | JA: "中には…" ; 0109A032 EN: "Let's see what's inside." | JA: "中にあるのは…" ; 0109A031 EN: "What's inside=@?" | JA: "入ってるのは…"
            break;
        case 3:
            voice = random < .3 ? "0109A033" : (random >= .6 ? "0109A035" : "0109A034"); // @text 0109A033 EN: "This." | JA: "…これか。" ; 0109A035 EN: "This is what I got." | JA: "こんなものが…" ; 0109A034 EN: "This was inside." | JA: "これが入ってたのか。"
            break;
        default:
            break;
        }
        if (voice)
            EVT_AT_SetVoice(voice, 1, delay);
        ++vendingPrizeVoice;
        return;
    case 9:
        if (!(random >= .1))
            voice = "0109A016";
        else if (random < .2)
            voice = "0109A017";
        else if (random < .3)
            voice = "0109A018";
        else if (random < .4)
            voice = "0109A019";
        else if (random < .5)
            voice = "0109A020";
        else if (random < .6)
            voice = "0109A021";
        else if (random < .7)
            voice = "0109A022";
        else if (random < .8)
            voice = "0109A023";
        else if (random < .9)
            voice = "0109A024";
        else if (random < 1.0)
            voice = "0109A017";
        break;
    case 10:
        voice = random >= .5 ? "0109A007" : "0109A006"; // @text 0109A007 EN: "I guess not." | JA: "やっぱりやめよう。" ; 0109A006 EN: "No, I won't." | JA: "やめておこう…"
        break;
    default:
        break;
    }
    if (voice)
        EVT_AT_SetVoice(voice, 1, delay);
}

void InitializeVendingButtons(VendingMachineWork &work) {
    work.buttonCount = 8;
    work.upperButtons = 6;
    work.selected = 3;
    for (std::int32_t index = 0; index < work.buttonCount; ++index) {
        const std::int32_t column = index < work.upperButtons ? index : index + work.upperButtons - work.buttonCount;
        work.buttons[index] = {.x = static_cast<float>(column) * .086f - .187f, .y = index < work.upperButtons ? 1.25f : .95f, .z = .42f};
    }
}

void InitializeVendingResources(VendingMachineWork &work) {
    work.characters = Queue_EVT_AT_ReadChr(work.task, vendCharacterNames);
    InitializeVendingButtons(work);
    const char *directory = Filepath_Gen(FilepathGenMode::PackDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("VEND"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("VEND"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("VEND"), 0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0109E000");
    work.prizeState = 0;
}

void eVEND_EVT_InitVend_Enqueue(void *argument) {
    HLib::Task *proc = HLib::EnqueueTaskWithParameter(VendingProcCallback, 5, 0x0b, 8, VEND_PROC_TASK);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(eVEND_EVT_InitVend, 4, 0x0c, sizeof(VendingMachineWork), VEND_MAIN_TASK);
    VendingMachineWork *work = static_cast<VendingMachineWork *>(HLib::GetTaskParameter(task));
    *static_cast<VendingMachineWork **>(HLib::GetTaskParameter(proc)) = work;
    work->procTask = proc;
    work->task = task;
    work->originalMachine = argument ? *static_cast<shen_name *>(argument) : EVENT_CHARACTER_INVALID;
    InitializeVendingResources(*work);
    work->voiceState = 1;
    work->contentState = 0;
    work->firstSelection = 1;
    EV_FreeAuthorPak(1, "0109");
    EV_EventSetUpHelpIcon_0(25, 0);
    work->colour = VM_COLOR_UNKNOWN;
    for (const VendingMapColour &row : vendMapColours) {
        if (row.map.name == g_currentMap.name) {
            work->colour = row.colour;
            break;
        }
    }
    if (work->colour == VM_COLOR_RED) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 600, " VM_COLOR ==> RED \n");
        vendingMachine = VEND_RED;
    } else if (work->colour == VM_COLOR_WHITE) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 605, " VM_COLOR ==> WHITE \n");
        vendingMachine = VEND_WHITE;
    } else if (work->colour == VM_COLOR_GREEN) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 610, " VM_COLOR ==> GREEN \n");
        vendingMachine = VEND_GREEN;
    } else {
        EV_debug_log("VEND\\eVEND_et_vend.c", 614, " ****** WARNING ********* \n");
        EV_debug_log("VEND\\eVEND_et_vend.c", 615, " VEND.EV3 can't get <AREA-ID>\n");
    }
    if (work->colour >= VM_COLOR_RED && work->colour <= VM_COLOR_GREEN) {
        const char *directory = Filepath_Gen(FilepathGenMode::PackDirectory);
        vendingArchive = EV_pakUtils(PakCategory::Partition, PakPartitionOp::Mount, reinterpret_cast<std::uintptr_t>(directory),
            reinterpret_cast<std::uintptr_t>(work->colour == VM_COLOR_RED ? "VEND.afs" : "SUNT.afs"));
    }
    work->wait = 0;
}

void eVEND_EVT_InitVend(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    switch (vendingState) {
    case VEND_CHECK_CASH:
        vendingState = VEND_FADE;
        if (std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash))) > 5)
            PlayVendingVoice(work, 25);
        [[fallthrough]];
    case VEND_FADE: {
        vendingState = VM_VEND_TRANS_CHARA;
        EV_debug_log("VEND\\eVEND_et_vend.c", 660, " Now state -->VEND_FADE \n");
        EVT_AT_MenCtrl({.cleanup = 1,
            .frames = 40,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 15.0f});
        HLib::SetupTaskDelay(6);
        return;
    }
    case VM_VEND_TRANS_CHARA: {
        vendingState = VEND_FADE;
        EV_debug_log("VEND\\eVEND_et_vend.c", 671, " Now state -->VM_VEND_TRANS_CHARA \n");
        if (DoesScenarioCharacterExist(work.originalMachine)) {
            EV_EnableCharacterDisplay(vendingMachine);
            EV_DisableCharacterDisplay(work.originalMachine);
            Vector3 position;
            Vector3i rotation;
            EV_GetCharaPosition(work.originalMachine, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            GetScenarioCharacterEulerAngles(work.originalMachine, &rotation, EventTransformFlags::World);
            EV_AT_SetPos(vendingMachine, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(vendingMachine, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        }
        SetLowQualityHands(ctrlGetCharaByName(VEND_HAND));
        SetLowQualityHands(ctrlGetCharaByName(VEND_RYO));
        SetHighModelXYZ(ctrlGetCharaByName(VEND_RYO), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(VEND_RYO), 1, 1, 0);
        AT_SetLocalBaseChara(VEND_RYO, vendingMachine);
        AT_SetLocalBaseChara(VEND_BASE, vendingMachine);
        AT_SetLocalBaseChara(VEND_HAND, VEND_BASE);
        EV_AT_SetAngY(VEND_HAND, EventTransformFlags::AllAxes, vendHandAngle);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_RYO), 0, reinterpret_cast<std::byte *>(vendIdleHandCommand), 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 551, 1);
        SetPlayerControlEnabled(0);
        EV_AT_SetPos(VEND_BASE, EventTransformFlags::AllAxes, vendingInitialBasePosition);
        EV_AT_SetAngY(VEND_BASE, EventTransformFlags::AllAxes, vendingInitialBaseRotation);
        EventSetMotion motion{.cleanup = 0, .character = VEND_RYO, .animation = 0xf003, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf003);
        HLib::SetCurrentTaskTo(eVEND_EVT_CheckVend);
        break;
    }
    default:
        break;
    }
}

std::int32_t VendingNoMoney() {
    PATI_StartMoneyCount(0, 10);
    EventSetPositionAngle position{
        .cleanup = 0, .character = VEND_RYO, .parent = vendingMachine, .position = {.x = 0, .y = 0, .z = 1}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1111, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    return 70;
}

std::int32_t VendingInsertCoin() {
    EventSetPositionAngle position{
        .cleanup = 0, .character = VEND_RYO, .parent = vendingMachine, .position = {.x = 0, .y = 0, .z = 1}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{.cleanup = 0, .character = VEND_RYO, .animation = 5060, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(5060);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1103, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetSound(1481, 30);
    EVT_AT_SetSound(191, 125);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-5), 70);
    return 170;
}

void eVEND_EVT_CheckVend(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    EV_debug_log("VEND\\eVEND_et_vend.c", 736, "@@@    eVEND_EVT_CheckVend ( ) \n");
    if (GetCurrentControlInput(INPUT_PRESSED) == PAD_BUTTON_X || work.originalMachine.name != 0xffffffffU) {
        std::int32_t delay;
        if (std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash))) < 5) {
            work.voiceState = 2;
            delay = VendingNoMoney();
            PlayVendingVoice(work, 45);
            HLib::SetCurrentTaskTo(eVEND_EVT_VendEnd);
            HLib::SetupTaskDelay(delay + 50);
        } else {
            delay = VendingInsertCoin();
            HLib::SetCurrentTaskTo(eVEND_EVT_InitSelect);
            HLib::SetupTaskDelay(delay);
        }
    }
}

void HideVendingLamps() {
    using enum EventCharacterPartTransformMode;
    for (std::int32_t index = 0; index < 8; ++index) {
        Vector3 position;
        Vector3i rotation;
        EV_scenApplyPosOrRotToPartOf(vendingMachine, static_cast<std::uint16_t>(vendingLamps.parts[index]), &position, &rotation, Get);
        vendingLamps.positions[index] = position;
        position.z += .1f;
        EV_scenApplyPosOrRotToPartOf(vendingMachine, static_cast<std::uint16_t>(vendingLamps.parts[index]), &position, nullptr, Absolute);
    }
}

void RestoreVendingLamps(void *data) {
    using enum EventCharacterPartTransformMode;
    VendingLampBank &lamps = *static_cast<VendingLampBank *>(data);
    for (std::int32_t index = 0; index < 8; ++index) {
        if (lamps.selected == lamps.parts[index])
            EV_debug_log("VEND\\eVEND_et_vend_auto.c", 468, "@@@   part == lamp[%d]\n", index);
        else
            EV_scenApplyPosOrRotToPartOf(vendingMachine, static_cast<std::uint16_t>(lamps.parts[index]), &lamps.positions[index], nullptr, Absolute);
    }
}

void eVEND_EVT_InitSelect(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    EV_debug_log("VEND\\eVEND_et_vend.c", 772, "@@@  eVEND_EVT_InitSelect( ) \n");
    EV_DisableCharacterDisplay(VEND_RYO);
    SetHighModelXYZ(ctrlGetCharaByName(VEND_HAND), 1, 0, 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(VEND_HAND), 0, 21, &vendSelectionHandRotation);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_HAND), 0, reinterpret_cast<std::byte *>(vendSelectionHandCommand), 0);
    EV_EnableCharacterDisplay(VEND_HAND);
    EV_EnsureCharacterDraw(VEND_HAND);
    EventSetPosition position{.cleanup = 0, .character = VEND_HAND, .parent = VEND_BASE, .position = vendHandOffset, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{.cleanup = 0, .character = VEND_HAND, .parent = VEND_BASE, .rotation = {.x = 4580, .y = 0, .z = 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    position = {.cleanup = 0, .character = VEND_BASE, .parent = vendingMachine, .position = work.buttons[work.selected], .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = VEND_BASE, .parent = vendingMachine, .rotation = {}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    work.contentState = 1;
    EventSetCamera camera{.cleanup = 0, .camera = 1101, .eye = VEND_BASE, .target = VEND_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    HideVendingLamps();
    HLib::SetCurrentTaskTo(eVEND_EVT_VoiceInSelect);
    HLib::SetupTaskDelay(5);
}

void eVEND_EVT_VoiceInSelect(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    if (work.firstSelection) {
        work.voiceState = 3;
        work.firstSelection = 0;
    } else
        work.voiceState = 4;
    PlayVendingVoice(work, 0);
    HLib::SetCurrentTaskTo(eVEND_EVT_SelectJuice);
    HLib::SetupTaskDelay(50);
}

std::int32_t InterpolateVendingHand(const Vector3 &target, const Vector3 &previous, std::int32_t step, std::int32_t delay) {
    std::int32_t index = 0;
    do {
        const float factor = vendInterpolation[index];
        const Vector3 position{.x = (target.x - previous.x) * factor + previous.x,
            .y = (target.y - previous.y) * factor + previous.y,
            .z = (target.z - previous.z) * factor + previous.z};
        EVT_AT_SetPos(VEND_BASE, vendingMachine, position, index + delay);
        index += step;
    } while (index < 8);
    return delay + 8 / step;
}

void eVEND_ET_SetHandUpYubiWait(void *data) {
    VendingHandWaitWork &work = *static_cast<VendingHandWaitWork *>(data);
    if (work.state == EVVendHand_END) {
        work.state = EVVendHand_RETIRED;
        EV_debug_log("VEND\\eVEND_et_vend.c", 1466, "EVVendHand_END in eVEND_ET_SetHandUpYubiWait\n");
        HLib::EV_HLKillOtherHierarchyTask(work.task);
    }
    if (work.state == EVVendHand_ERROR) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 1470, "EVVendHand_ERROR in eVEND_ET_SetHandUpYubiWait\n");
        HLib::EV_HLKillOtherHierarchyTask(work.task);
    }
    if (work.timer > work.threshold) {
        ApplyCharacterHandPoseCommand(
            ctrlGetCharaByName(work.character), work.side, reinterpret_cast<std::byte *>(work.command), static_cast<std::int16_t>(work.duration));
        work.state = EVVendHand_END;
        HLib::SetupTaskDelay(work.duration);
    }
    ++work.timer;
}

std::int32_t Queue_eVEND_ET_SetHandUpYubiWait() {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(eVEND_ET_SetHandUpYubiWait, 3, 0x0b, sizeof(VendingHandWaitWork), AREA_EVENT_TASK);
    VendingHandWaitWork *work = static_cast<VendingHandWaitWork *>(HLib::GetTaskParameter(task));
    work->task = task;


    std::memcpy(work->command, vendWaitHandCommand, sizeof(work->command));
    work->character = VEND_HAND;
    work->side = 0;
    work->duration = 5;
    work->threshold = 5;
    work->timer = 0;
    work->state = EVVendHand_INIT;
    return 10;
}

std::int32_t PressVendingButton(const Vector3 &button) {
    const float x = button.x, y = button.y - .01f, z = button.z, pressedZ = z - .008f;
    const Vector3 target{.x = x, .y = y, .z = z};
    const std::int32_t begin = InterpolateVendingHand(target, button, 1, 7);
    Queue_eVEND_ET_SetHandUpYubiWait();
    const float deltaX = x - x, deltaY = y - y;
    for (std::int32_t index = 0; index < 8; index += 4) {
        const float factor = vendInterpolation[index];
        EVT_AT_SetPos(VEND_BASE, vendingMachine, {.x = factor * deltaX + x, .y = factor * deltaY + y, .z = factor * (pressedZ - z) + z}, index + begin);
    }
    const std::int32_t end = begin + 2;
    for (std::int32_t index = 0; index < 8; index += 4) {
        const float factor = vendInterpolation[index];
        EVT_AT_SetPos(VEND_BASE, vendingMachine, {.x = factor * deltaX + x, .y = factor * deltaY + y, .z = factor * (z - pressedZ) + pressedZ}, index + end);
    }
    EV_DisableCharacterDisplay(VEND_RYO);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1102, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetSound(190, end);
    EVT_AT_SetSound(193, end + 2);
    return end + 2;
}

void eVEND_ET_SetVendAutoWait(void *data) {
    VendingAutoWaitWork &work = *static_cast<VendingAutoWaitWork *>(data);
    if (work.state < 0) {
        ++work.timer;
        return;
    }
    if (work.state <= 1) {
        if (work.timer >= work.begin) {
            work.callback(work.argument);
            ++work.timer;
            work.state = 2;
            return;
        }
        work.state = 1;
        ++work.timer;
        return;
    }
    if (work.state == 2) {
        if (work.timer >= work.end)
            work.state = 3;
        else
            work.callback(work.argument);
        ++work.timer;
        return;
    }
    if (work.state >= 3 && work.state <= 5)
        HLib::EV_HLKillOtherHierarchyTask(work.task);
    ++work.timer;
}

void DetermineVendingPrize(VendingMachineWork &work, float random) {
    const std::int32_t previous = work.previousSelected;
    work.previousSelected = 8;
    if (!(random >= .01)) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 967, "Dual-S val=%f\n", random);
        work.prizeItem = ITEM_DURAL_SILVER;
    } else if (random < .02) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 970, "Dual-G val=%f\n", random);
        work.prizeItem = ITEM_DURAL_GOLD;
    } else if (random < .0425) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 973, "PROB_AKIRA val=%f\n", random);
        work.prizeItem = ITEM_AKIRA1;
    } else if (random < .065) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 976, "JUKY  val=%f\n", random);
        work.prizeItem = ITEM_JACKY1;
    } else if (random < .0875) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 979, "SARA  val=%f\n", random);
        work.prizeItem = ITEM_SARAH1;
    } else if (random < .11) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 982, "RAU  val=%f\n", random);
        work.prizeItem = ITEM_LAU1;
    } else if (random < .1325) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 985, "PAI  val=%f\n", random);
        work.prizeItem = ITEM_PAI1;
    } else if (random < .155) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 988, "URFU  val=%f\n", random);
        work.prizeItem = ITEM_WOLF1;
    } else if (random < .1775) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 991, "JEFLY  val=%f\n", random);
        work.prizeItem = ITEM_JEFFRY1;
    } else if (random < .2) {
        EV_debug_log("VEND\\eVEND_et_vend.c", 994, "KAGE  val=%f\n", random);
        work.prizeItem = ITEM_KAGE1;
    } else {
        work.prizeState = 3;
        work.previousSelected = previous;
    }
}

void eVEND_EVT_SelectJuice(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    const std::int32_t minutes = shenGetMinutesUntilNextFrameMinute();
    if (++vendingElapsed > 3000 || minutes < 1) {
        work.voiceState = 10;
        PlayVendingVoice(work, 0);
        PATI_StartMoneyCount(5, 50);
        HLib::SetCurrentTaskTo(eVEND_EVT_VendEnd);
        HLib::SetupTaskDelay(70);
    }
    GetBattleDataConfig(0, VEND_CANCEL);
    GetBattleDataConfig(0, VEND_ACCEPT);
    const std::uint16_t input = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_PRESSED));
    if (work.wait > 0) {
        --work.wait;
        return;
    }
    if (!GetBattleDataConfig(1, VEND_ACCEPT) || !GetBattleDataConfig(2, VEND_ACCEPT) || vendingValidSelection) {
        if (GetBattleDataConfig(1, VEND_CANCEL) && GetBattleDataConfig(2, VEND_CANCEL)) {
            work.voiceState = 7;
            PlayVendingVoice(work, 0);
            AddThisToCashOrSegaCoins(5, ECashOrCoins::Cash2);
            EV_debug_log("VEND\\eVEND_et_vend.c", 925, "Put B button at selecting Can\n");
            HLib::SetCurrentTaskTo(eVEND_EVT_VendEnd);
            return;
        }
        vendingValidSelection = 0;
        const std::int32_t previous = work.selected;
        switch (input) {
        case PAD_RIGHT:
            if (previous != work.upperButtons - 1 && previous < work.buttonCount - 1)
                ++work.selected;
            break;
        case PAD_LEFT:
            if (previous > 0 && previous != work.upperButtons)
                --work.selected;
            break;
        case PAD_UP:
            if (previous >= work.upperButtons)
                work.selected = previous + work.upperButtons - work.buttonCount;
            break;
        case PAD_DOWN:
            if (previous < work.upperButtons) {
                work.selected = previous + work.buttonCount - work.upperButtons;
                if (work.selected < work.upperButtons)
                    work.selected = work.upperButtons;
            }
            break;
        default:
            break;
        }
        if (previous == work.selected)
            return;
        vendingValidSelection = 1;
        work.wait = InterpolateVendingHand(work.buttons[work.selected], work.buttons[previous], 1, 0);
        return;
    }
    work.voiceState = 5;
    work.contentState = 2;
    work.previousSelected = work.selected;
    PlayVendingVoice(work, 0);
    const std::int32_t delay = PressVendingButton(work.buttons[work.selected]);
    vendingLamps.selected = work.selected;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(eVEND_ET_SetVendAutoWait, 3, 0x0b, sizeof(VendingAutoWaitWork), AREA_EVENT_TASK);
    VendingAutoWaitWork *wait = static_cast<VendingAutoWaitWork *>(HLib::GetTaskParameter(task));
    wait->task = task;
    wait->begin = delay - 5;
    wait->end = delay - 5 < delay - 7 ? delay - 7 : delay - 4;
    wait->timer = 0;
    wait->state = 0;
    wait->callback = RestoreVendingLamps;
    wait->argument = &vendingLamps;
    if (work.prizeState == 0) {
        work.prizeState = 1;
        DetermineVendingPrize(work, GetRandomNumberInRange(1.0f));
    }
    if (work.prizeState == 1) {
        work.prizeState = 2;
        vendingElapsed = 0;
        SelectVendingCanRecords(work);
        EV_pakUtils(PakCategory::Package, PakPackageOp::LoadAfs, static_cast<std::uintptr_t>(vendingArchive),
            static_cast<std::uintptr_t>(work.archiveRecord), work.alternateRecord);
        HLib::SetCurrentTaskTo(eVEND_EVT_HitCan);
        HLib::SetupTaskDelay(30);
    } else {
        SelectVendingCanRecords(work);
        EV_pakUtils(PakCategory::Package, PakPackageOp::LoadAfs, static_cast<std::uintptr_t>(vendingArchive),
            static_cast<std::uintptr_t>(work.archiveRecord), work.alternateRecord);
        HLib::SetCurrentTaskTo(eVEND_EVT_DrinkJuice);
    }
    HLib::SetupTaskDelay(delay);
}

void eVEND_EVT_CHAR_PARTS_GET(void *data) {
    using enum EventCharacterPartTransformMode;
    VendingPartWork &work = *static_cast<VendingPartWork *>(data);
    EV_scenApplyPosOrRotToPartOf(work.character, static_cast<std::uint16_t>(work.part), &work.position, &work.rotation, Absolute);
    if (work.cleanup)
        HLib::DestroyCurrentTask();
}

std::int32_t PickUpVendingCan(std::uint32_t sound) {
    using enum EventCharacterPartTransformMode;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = {.cleanup = 1,
        .target = VEND_CAN,
        .character = VEND_RYO,
        .bone = 14,
        .position = vendCanAttachment,
        .rotation = vendCanAttachmentRotation,
        .mode = 0,
        .delay = 82};
    EV_DisableCharacterDisplay(VEND_HAND);
    EV_EnableCharacterDisplay(VEND_RYO);
    EVT_AT_ObjectDisp_1(VEND_CAN, 82, 0);
    EventSetPositionAngle position{
        .cleanup = 0, .character = VEND_RYO, .parent = vendingMachine, .position = {.x = 0, .y = 0, .z = 1}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{.cleanup = 0, .character = VEND_RYO, .animation = 5062, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(5062);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1104, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetSound(198, 80);
    EVT_AT_SetSound(sound ? 196 : 195, 290);
    Vector3 first, second;
    Vector3i rotation;
    EV_scenApplyPosOrRotToPartOf(VEND_CAN, 152, &first, &rotation, Get);
    EV_scenApplyPosOrRotToPartOf(VEND_CAN, 153, &second, &rotation, Get);
    second.y -= 100.0f;
    task = HLib::EnqueueTaskWithParameter(eVEND_EVT_CHAR_PARTS_GET, 6, 0x0b, sizeof(VendingPartWork), AREA_EVENT_TASK);
    HLib::SetupTimedTask(task, 290);
    *static_cast<VendingPartWork *>(HLib::GetTaskParameter(task)) = {.character = VEND_CAN, .part = 153, .position = first, .rotation = rotation, .cleanup = 1};
    task = HLib::EnqueueTaskWithParameter(eVEND_EVT_CHAR_PARTS_GET, 6, 0x0b, sizeof(VendingPartWork), AREA_EVENT_TASK);
    HLib::SetupTimedTask(task, 290);
    *static_cast<VendingPartWork *>(HLib::GetTaskParameter(task)) = {
        .character = VEND_CAN, .part = 152, .position = second, .rotation = rotation, .cleanup = 1};
    return 170;
}

std::int32_t AnimateVendingDrink(std::int32_t delay) {
    const std::int32_t detach = delay + 690;
    HLib::Task *task;
    if (detach == 0) {
        EventCling work;
        work.cleanup = 0;
        work.target = VEND_CAN;
        work.character = VEND_RYO;
        work.mode = 1;
        work.delay = 0;
        EVT_AT_Cling(&work);
    } else {
        task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
        RegisterEventTask(task);
        EventCling *work = static_cast<EventCling *>(HLib::GetTaskParameter(task));
        work->cleanup = 1;
        work->target = VEND_CAN;
        work->character = VEND_RYO;
        work->mode = 1;
        work->delay = static_cast<float>(detach);
    }
    EVT_AT_ObjectDisp_1(VEND_CAN, detach, 1);
    EVT_AT_SetMot(VEND_RYO, 5059, 0, 1, 0, delay);
    EVT_AT_SetCam(1105, vendingMachine, vendingMachine, 0, 0, delay);
    EVT_AT_SetCam(1106, vendingMachine, vendingMachine, 0, 0, delay + 575);
    EVT_AT_SetSound(192, delay + 210);
    EVT_AT_SetSound(192, delay + 285);
    EVT_AT_SetSound(192, delay + 470);
    EVT_AT_SetSound(197, detach);
    EVT_AT_SetFaceLooks(VEND_RYO, FacialExpressionId::Smile, 30, .5f, delay + 200);
    EVT_AT_SetFaceLooks(VEND_RYO, FacialExpressionId::Normal, 30, 1, delay + 300);
    return delay + 765;
}

std::int32_t PickUpVendingPrize() {
    GetMotionFrameCount(5062);
    GetMotionFrameCount(5061);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = {.cleanup = 1,
        .target = VEND_CAN,
        .character = VEND_RYO,
        .bone = 14,
        .position = vendCanAttachment,
        .rotation = vendCanAttachmentRotation,
        .mode = 0,
        .delay = 82};
    EV_DisableCharacterDisplay(VEND_HAND);
    EV_EnableCharacterDisplay(VEND_RYO);
    EVT_AT_ObjectDisp_1(VEND_CAN, 82, 0);
    EventSetPositionAngle position{
        .cleanup = 0, .character = VEND_RYO, .parent = vendingMachine, .position = {.x = 0, .y = 0, .z = 1}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{.cleanup = 0, .character = VEND_RYO, .animation = 5062, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(5062);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1104, .eye = vendingMachine, .target = vendingMachine, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(1109, vendingMachine, vendingMachine, 0, 0, 110);
    EVT_AT_SetSound(198, 80);
    EVT_AT_ObjectDisp_1(vendingMachine, 230, 1);
    EVT_AT_ObjectDisp_1(VEND_CAN, 230, 1);
    EVT_AT_SetMot(VEND_RYO, 5061, 0, 1, 0x40000000U, 200);
    EVT_AT_SetCam(1110, vendingMachine, vendingMachine, 0, 0, 200);
    Queue_EVT_AT_CtrlHandUp_1(VEND_RYO, 0, reinterpret_cast<std::byte *>(vendPrizeHandCommand), 5, 230);
    EVT_AT_ObjectDisp_1(vendingMachine, 390, 0);
    EVT_AT_SetFaceLooks(VEND_RYO, FacialExpressionId::Surprise, 30, .5f, 230);
    EVT_AT_SetFaceLooks(VEND_RYO, FacialExpressionId::Smile, 30, .3f, 280);
    return 390;
}

void eVEND_EVT_DrinkJuice(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    EV_debug_log("VEND\\eVEND_et_vend.c", 1014, "@@@     eVEND_EVT_DrinkJuice( ) \n");
    work.voiceState = 9;
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    work.contentState = 4;
    ApplyCharacterHandPoseCommand(
        ctrlGetCharaByName(VEND_RYO), 0, reinterpret_cast<std::byte *>(work.canKind == VEND_CAN_STANDARD ? vendCanHandCommand : vendAlternateCanCommand), 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(VEND_RYO), 0, 21, work.canKind == VEND_CAN_STANDARD ? &vendCanHandRotation : &vendAlternateHandRotation);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_RYO), 1, reinterpret_cast<std::byte *>(vendLeftDrinkCommand), 190);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_RYO), 1, reinterpret_cast<std::byte *>(vendLeftExitCommand), 410);
    const std::int32_t delay = AnimateVendingDrink(PickUpVendingCan(work.sound));
    PlayVendingVoice(work, delay - 240);
    HLib::SetCurrentTaskTo(eVEND_EVT_VendEnd);
    HLib::SetupTaskDelay(delay);
}

void eVEND_EVT_REGIST_ITEM_EFFECT(void *data) {
    VendingItemWork &work = *static_cast<VendingItemWork *>(data);
    AddInventoryItem(work.item, 1, 1);
    if (work.cleanup)
        HLib::DestroyCurrentTask();
}

void Queue_eVEND_EVT_REGIST_ITEM_EFFECT(std::uint32_t item, std::int32_t delay) {
    if (!delay) {
        AddInventoryItem(item, 1, 1);
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(eVEND_EVT_REGIST_ITEM_EFFECT, 6, 0x0b, sizeof(VendingItemWork), AREA_EVENT_TASK);
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    *static_cast<VendingItemWork *>(HLib::GetTaskParameter(task)) = {.cleanup = 1, .item = item};
}

void eVEND_EVT_HitCan(void *data) {
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    work.voiceState = 8;
    if (vendingState != VEND_FADE) {
        if (vendingState != VM_VEND_TRANS_CHARA)
            return;
        vendingState = VEND_FADE;
        if (vendingArchive != -1) {
            const bool pending = EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0) != 0;
            if (pending || work.contentState == 4)
                EV_pakUtils(PakCategory::Package, pending ? 13 : 12, static_cast<std::uintptr_t>(vendingArchive),
                    static_cast<std::uintptr_t>(work.archiveRecord), work.alternateRecord);
        }
        work.contentState = 1;
        EV_debug_log("VEND\\eVEND_et_vend.c", 1105, "HIT_prosesing_02\n");
        HLib::SetCurrentTaskTo(eVEND_EVT_InitSelect);
        return;
    }
    vendingState = VM_VEND_TRANS_CHARA;
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    work.contentState = 4;
    EV_debug_log("VEND\\eVEND_et_vend.c", 1062, "HIT_prosesing_01\n");
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(VEND_RYO), 0, reinterpret_cast<std::byte *>(vendCanHandCommand), 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(VEND_RYO), 0, 21, &vendCanHandRotation);
    const std::int32_t delay = PickUpVendingPrize();
    PlayVendingVoice(work, delay - 270);
    PlayVendingVoice(work, delay - 190);
    PlayVendingVoice(work, delay - 130);
    EVT_AT_SetPoint(VEND_RYO, VEND_CAN, 0, 1, 0, delay - 190, 190, 0);
    Queue_eVEND_EVT_REGIST_ITEM_EFFECT(work.prizeItem, delay - 120);
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 6,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(delay - 10)});
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 40,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(delay)});
    HLib::SetupTaskDelay(delay);
}

void eVEND_EVT_VendEnd(void *) {
    EV_debug_log("VEND\\eVEND_et_vend.c", 1123, "@@@             eVEND_EVT_VendEnd( ) \n");
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    HLib::SetCurrentTaskTo(eVEND_ET_KillVend);
    EventFilmControl film{.cleanup = 0,
        .frames = 10,
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
    HLib::SetupTaskDelay(10);
}

void ReleaseVendingResources(VendingMachineWork &work) {
    EV_EnableCharacterDisplay(VEND_RYO);
    EV_DisableCharacterDisplay(vendingMachine);
    if (DoesScenarioCharacterExist(work.originalMachine))
        EV_EnableCharacterDisplay(work.originalMachine);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("VEND"), 0);
    ET_AT_KillWork(work.characters);
}

void eVEND_ET_KillVend(void *data) {
    using enum CameraState;
    VendingMachineWork &work = *static_cast<VendingMachineWork *>(data);
    EV_debug_log("VEND\\eVEND_et_vend.c", 1162, "@@@     EV_KillVend( ) \n");
    if (vendingState != VEND_FADE) {
        if (vendingState != VM_VEND_TRANS_CHARA)
            return;
        work.voiceState = 11;
        if (vendingArchive != -1) {
            const bool pending = EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0) != 0;
            if (pending || work.contentState == 4)
                EV_pakUtils(PakCategory::Package, pending ? 13 : 12, static_cast<std::uintptr_t>(vendingArchive),
                    static_cast<std::uintptr_t>(work.archiveRecord), work.alternateRecord);
            work.contentState = 5;
            EV_pakUtils(PakCategory::Partition, PakPartitionOp::Unmount, static_cast<std::uintptr_t>(vendingArchive), 0);
            vendingArchive = -1;
        }
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetHighModelXYZ(ctrlGetCharaByName(VEND_HAND), 0, 0, 0);
        EV_DisableCharacterDisplay(VEND_HAND);
        hndlSetFlags(ctrlGetCharaByName(VEND_HAND));
        hndlSetFlags(ctrlGetCharaByName(VEND_RYO));
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(VEND_RYO), 0, 0, 1);
        EV_CleanupMapControlTask();
        HLib::EV_HLKillOtherHierarchyTask(work.task);
        HLib::EV_HLKillOtherHierarchyTask(work.procTask);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 551, 0);
        SetPlayerControlEnabled(1);
        ReleaseVendingResources(work);
        sbtLoadProgram();
        EV_NPC_NewLoadSw(1);
        return;
    }
    vendingState = VM_VEND_TRANS_CHARA;
    if (work.voiceState == 7 || work.voiceState == 10) {
        EVT_AT_MenCtrl({.cleanup = 1,
            .frames = 10,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 100});
        HLib::SetupTaskDelay(100);
    } else {
        EventFilmControl film{.cleanup = 0,
            .frames = 10,
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
}
} // namespace

void InitializeVendingMachineEvent() {
    vendingLamps = {.selected = 0, .parts = {172, 173, 174, 175, 176, 177, 178, 179}, .positions = {}};
    vendingPrizeVoice = 1;
    vendingState = VEND_CHECK_CASH;
    vendingElapsed = 0;
    vendingValidSelection = 0;
    vendingArchive = -1;
}

void RunVendingMachineEvent(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), AREA_EVENT_TASK);
    RegisterEventTask(task);
    StepNpcClearWork *work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = eVEND_EVT_InitVend_Enqueue;
    work->callbackData = reinterpret_cast<void *>(argument);
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3;
}
} // namespace shenmue::s2

#endif
